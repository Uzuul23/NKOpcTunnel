#include "stdafx.h"
#include "comfarsrv.h"
#include "com/comfarunknown.h"
#include "trace/trace.h"
#include "error/exception.h"
#include "error/baseexception.h"
#include "stream/interfaces.h"
#include "stream/stream.h"
#include "stream/mtiodevice.h"
#include "threading/lock.h"
#include "threading/workerthread.h"
#include "com/comptr.h"
#include "com/interfaces.h"
#include "com/unknownimpl.h"
#include "com/vector.h"
#include "3rd-party/microsoft/threading/threadpool.h"
#include <vector>
#include <list>

namespace NkCom
{
	// CComFarSrv

	const size_t const_buffer_size = 8192;

	
	class CComFarSrv::CImpl : public NkStream::CStreamIODeviceEvent
	{
	public:
		CComFarSrv *m_pThis;
		NkStream::CStreamIODevice *m_p_iodevice = 0;
		NkStream::CMTIODevice *m_p_threading_iodevice = 0;
		NkStream::CStream m_StreamSnd;
		NkStream::CStream m_StreamRcv;
		bool m_auto_delete_provider;
		bool m_bApartmentThreaded;
		CLSID m_clsid = CLSID_NULL;
		ULONG32 m_server_id = 0;

		ULONG m_cRef;
		NkThreading::CCriticalSection m_Lock;
		CServerEvent* p_release_event;

		CImpl(CComFarSrv *pThis, NkStream::CStreamIODevice* p_new_iodevice
			, CServerEvent* p_event, bool auto_delete_provider)
			: m_pThis(pThis)
			, m_p_iodevice(p_new_iodevice)
			, p_release_event(p_event)
			, m_auto_delete_provider(auto_delete_provider)
			, m_cRef(0)
			, m_StreamSnd(0, const_buffer_size, 2)
			, m_StreamRcv(0, const_buffer_size, 1)
		{
			NkError::CBaseException::check_arg(p_new_iodevice, __FILE__, __LINE__);

			m_p_threading_iodevice = new NkStream::CMTIODevice(p_new_iodevice, false, const_buffer_size);
			m_StreamSnd.stream_device(m_p_threading_iodevice);
			m_StreamRcv.stream_device(m_p_threading_iodevice);
			m_p_threading_iodevice->start_async_callback(this, 1, true);

			NkTrace::CTrace::TraceDet(L"CComFarSrv::CComFarSrv()");
		}

		~CImpl()
		{
			delete m_p_threading_iodevice;

			if (m_auto_delete_provider) {
				delete m_p_iodevice;
			}

			NkTrace::CTrace::TraceDet(L"CComFarSrv::~CComFarSrv()");
		}

		bool m_b_shutdown = false;
		void async_shutdown()
		{
			if (!m_b_shutdown) {
				CThreadPool::QueueUserWorkItem(&CImpl::shutdown, this);
			}
		}

		void shutdown()
		{
			if (m_b_shutdown) {
				return;
			}

			m_b_shutdown = true;
			m_p_threading_iodevice->close();

			try
			{
				HRESULT hr = S_OK;

				for (auto i : m_advise_infos) {
					NkCom::CComPtr<IConnectionPoint> sp_cp;
					hr = i->m_sp_cp->Unadvise(i->m_cookie);
					NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
					delete i;
				}
			}
			catch (...) {}

			for (auto i : m_Instances) {
				if (i) {
					i->Release();
				}
			}

			if (p_release_event) {
				p_release_event->on_shutdown(m_pThis);
			}
		}

		class CAdviseInfo
		{
		public:
			CAdviseInfo(IConnectionPoint* pcp, DWORD cookie)
				: m_sp_cp(pcp)
				, m_cookie(cookie)
			{}
			NkCom::CComPtr<IConnectionPoint> m_sp_cp;
			DWORD m_cookie = 0;
		};

		std::list<CAdviseInfo*> m_advise_infos;

		void store_cp_advise(IConnectionPoint* pcp, DWORD cookie)
		{
			_ASSERT(pcp != 0 && cookie != 0);
			if (pcp == 0) {
				return;
			}
			m_advise_infos.push_back(new CAdviseInfo(pcp, cookie));
		}

		void store_cp_unadvise(IConnectionPoint* pcp, DWORD cookie)
		{
			_ASSERT(pcp != 0 && cookie != 0);
			if (pcp == 0) {
				return;
			}
			try
			{
				IID iid1;
				HRESULT hr = pcp->GetConnectionInterface(&iid1);
				NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

				for (auto i : m_advise_infos) {
					IID iid2;
					hr = i->m_sp_cp->GetConnectionInterface(&iid2);
					NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
					if (cookie == i->m_cookie && iid1 == iid2) {
						m_advise_infos.remove(i);
						delete i;
						return;
					}
				}
				_ASSERT(0);
			}
			catch (...) {}
		}

		virtual bool on_time_out()
		{
#ifdef _DEBUG
			return true;
#endif // _DEBUG

			async_shutdown();
			return false;
		}

		virtual bool on_receive(ULONG32 stream_id)
		{
			_ASSERT(stream_id == 1);

			try
			{
				short FarCmd;
				m_StreamRcv >> FarCmd;

				switch (FarCmd) {
				case FarCmdLogon: {
					NkCom::CVector<BYTE, size_t> credentials;
					m_StreamRcv >> credentials;
					//TODO:
					m_StreamRcv << static_cast<HRESULT>(S_OK);//m_pThis->logon(credentials.data(), credentials.size());
					m_StreamRcv.flush();
					break;
					}
				case FarCmdCoCreate: {
					CLSID clsid;
					DWORD dwClsContext;
					IID iid;

					m_StreamRcv >> clsid;
					m_StreamRcv >> dwClsContext;
					m_StreamRcv >> iid;

					ULONG32 id = 0;
					HRESULT hr = m_pThis->create(clsid, dwClsContext, iid, id);
					m_StreamRcv << hr;
					m_StreamRcv << id;
					m_StreamRcv.flush();
					break;
				}
				case FarCmdFctCall: {
					ULONG32 id;
					ULONG index;
					m_StreamRcv >> id;
					m_StreamRcv >> index;
					get_instance(id)->call(index, m_StreamRcv);
					break;
				}
				case FarCmdDelete: {
					ULONG32 id;
					m_StreamRcv >> id;
					delete get_instance(id);
					m_StreamRcv << static_cast<HRESULT>(S_OK);
					m_StreamRcv.flush();
					break;
				}
				case FarCmdQueryInterface: {
					ULONG32 id;
					IID iid;
					m_StreamRcv >> id;
					m_StreamRcv >> iid;

					CComFarUnknown *p = get_instance(id);
					HRESULT hr = p->query_interface(iid);
					m_StreamRcv << hr;
					m_StreamRcv.flush();
					break;
				}
				case FarCmdShutdown: {
					async_shutdown();
					return false;
				}
				default: NkError::CBaseException::check_result(E_INVALIDARG, __FILE__, __LINE__);
				}
				return true;
			}
			catch (NkError::CException& e)
			{
				//if (e.base_error() != NkError::CBaseException::connection_closed) {
					e.report();
				//}
				async_shutdown();
			}
			return false;
		}

		std::vector<CComFarUnknown*> m_Instances;

		ULONG32 register_instance(CComFarUnknown* p)
		{
			for (size_t loop = 0; loop < m_Instances.size(); ++loop) {
				if (m_Instances[loop] == 0) {
					m_Instances[loop] = p;
					return static_cast<ULONG32>(loop + 1);
				}
			}
			m_Instances.push_back(p);
			return static_cast<ULONG32>(m_Instances.size());
		}
		void unregister_instance(ULONG32 id)
		{
			if (id > 0 && id <= m_Instances.size()) {
				m_Instances[id - 1] = 0;
				return;
			}
			NkError::CBaseException::check_result(E_INVALIDARG, __FILE__, __LINE__);
		}
		CComFarUnknown* get_instance(ULONG32 id)
		{
			CComFarUnknown* p = 0;

			if (id > 0 && id <= m_Instances.size()) {
				p = m_Instances[id - 1];
			}

			if (p == 0) {
				NkError::CBaseException::check_result(E_INVALIDARG, __FILE__, __LINE__);
			}
			return p;
		}

		NkStream::CStream& begin_fct_call(ULONG32 index, ULONG32 id)
		{
			if (m_b_shutdown) {
				throw NkError::CBaseException
					(NkError::CBaseException::BaseErrors::server_down, __FILE__, __LINE__);
			}

			m_Lock.lock();

			m_StreamSnd << static_cast<short>(CServer::FarCmdFctCall);
			m_StreamSnd << id;
			m_StreamSnd << index;
			return m_StreamSnd;
		}

		void end_fct_call()
		{
			m_Lock.unlock();
		}

		HRESULT handle_error(NkError::CException& e)
		{
			e.report();
			async_shutdown();
			return E_FAIL;
		}

		HRESULT remote_create_instance(REFCLSID rclsid, DWORD dwClsContext
			, REFIID proxy_iid, REFIID riid, LPVOID *ppv) throw()
		{
			/*if (m_b_shutdown) {
				return E_FAIL;
			}*/

			//try
			//{
			//	NkThreading::CLockGuard lock(m_Lock);

			//	m_StreamSnd << static_cast<short>(FarCmdCoCreate);
			//	m_StreamSnd << rclsid;
			//	m_StreamSnd << dwClsContext;
			//	m_StreamSnd << riid;
			//	m_StreamSnd.flush();

			//	HRESULT hr;
			//	ULONG32 id;
			//	m_StreamSnd >> hr;
			//	m_StreamSnd >> id;

			//	NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			//	//return create(rclsid, riid, ppv, id);
			//	return E_FAIL;
			//}
			//catch (NkError::CException& e) {
			//	return handle_error(e);
			//}

			return E_NOTIMPL;
		}

		HRESULT remote_query_interface(REFIID riid, ULONG32 id)
		{
			if (m_b_shutdown) {
				return E_NOINTERFACE;
			}

			try {
				NkThreading::CLockGuard lock(m_Lock);

				m_StreamSnd << static_cast<short>(FarCmdQueryInterface);
				m_StreamSnd << id;
				m_StreamSnd << riid;
				m_StreamSnd.flush();

				HRESULT hr;
				m_StreamSnd >> hr;
				return hr;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}

		void remote_delete_instance(ULONG32 id)
		{
			if (m_b_shutdown) {
				return;
			}

			try
			{
				NkThreading::CLockGuard lock(m_Lock);

				m_StreamSnd << static_cast<short>(FarCmdDelete);
				m_StreamSnd << id;
				m_StreamSnd.flush();

				HRESULT hr;
				m_StreamSnd >> hr;
			}
			catch (NkError::CException& e) {
				handle_error(e);
			}
		}
	};

	CComFarSrv::CComFarSrv(NkStream::CStreamIODevice* pNewProvider
		, CServerEvent* p_event/* = 0*/
		, bool auto_delete_provider/* = true*/)
		: _Impl(new CImpl(this, pNewProvider, p_event, auto_delete_provider))
	{
	}

	CComFarSrv::~CComFarSrv()
	{
		delete _Impl;
	}

	STDMETHODIMP CComFarSrv::QueryInterface(REFGUID iid, void **pp)
	{
		if (iid == IID_IUnknown) {
			(*pp) = this;
			AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	ULONG CComFarSrv::Release()
	{
		if (--_Impl->m_cRef != 0) {
			return _Impl->m_cRef;
		}
		delete this;
		return 0;
	}

	ULONG CComFarSrv::AddRef()
	{
		return ++_Impl->m_cRef;
	}

	HRESULT CComFarSrv::remote_logon(const BYTE credentials[], size_t cb)
	{
		return E_NOTIMPL;
	}

	HRESULT CComFarSrv::remote_create_instance(REFCLSID rclsid, DWORD dwClsContext
		, REFIID proxy_iid, REFIID riid, LPVOID *ppv) throw()
	{
		return _Impl->remote_create_instance(rclsid, dwClsContext, proxy_iid, riid, ppv);
	}

	HRESULT CComFarSrv::remote_create_instance(REFCLSID rclsid, DWORD dwClsContext, REFIID riid, ULONG32& id)
	{
		return E_NOTIMPL;
	}

	HRESULT CComFarSrv::remote_query_interface(REFIID riid, ULONG32 id)
	{
		return _Impl->remote_query_interface(riid, id);
	}

	void CComFarSrv::remote_delete_instance(ULONG32 id)
	{
		return _Impl->remote_delete_instance(id);
	}

	NkStream::CStream& CComFarSrv::begin_fct_call(ULONG32 index, ULONG32 id)
	{
		return _Impl->begin_fct_call(index, id);
	}

	void CComFarSrv::end_fct_call()
	{
		_Impl->end_fct_call();
	}

	ULONG32 CComFarSrv::server_id()
	{
		return _Impl->m_server_id;
	}

	void CComFarSrv::server_id(ULONG32 id)
	{
		_Impl->m_server_id = id;
	}

	REFCLSID CComFarSrv::clsid()
	{
		return _Impl->m_clsid;
	}

	void CComFarSrv::clsid(REFCLSID clsid)
	{
		_Impl->m_clsid = clsid;
	}

	ULONG32 CComFarSrv::register_instance(CComFarUnknown* p)
	{
		return _Impl->register_instance(p);
	}

	void CComFarSrv::unregister_instance(ULONG32 id)
	{
		_Impl->unregister_instance(id);
	}

	CComFarUnknown* CComFarSrv::get_instance(ULONG32 id)
	{
		return _Impl->get_instance(id);
	}

	HRESULT CComFarSrv::handle_error(NkError::CException& e)
	{
		return _Impl->handle_error(e);
	}

	IGlobalInterfaceTable* CComFarSrv::get_git()
	{
		return 0;
	}

	void CComFarSrv::store_cp_advise(IConnectionPoint* p_cp, DWORD cookie)
	{
		_Impl->store_cp_advise(p_cp, cookie);
	}

	void CComFarSrv::store_cp_unadvise(IConnectionPoint* p_cp, DWORD cookie)
	{
		_Impl->store_cp_unadvise(p_cp, cookie);
	}

	void CComFarSrv::shutdown()
	{
		_Impl->shutdown();
	}

	void CComFarSrv::lock_server(bool lock)
	{
	}

	void CComFarSrv::lock_object(bool lock)
	{
	}

	HRESULT CComFarSrv::remote_create_class_factory(REFCLSID rclsid, DWORD dwClsContext, REFIID factory_iid, REFIID riid, LPVOID *ppv)
	{
		return E_NOTIMPL;
	}
}