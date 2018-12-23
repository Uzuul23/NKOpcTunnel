/*	This file is part of NKOpcTunnel.
*
*	Copyright (c) Henryk Anschuetz 
*	Berlin, Germany
*
*	mailto:uzuul23@online.de
*
*	NKOpcTunnel is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NKOpcTunnel is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with NKOpcTunnel.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "stdafx.h"
#include "comnearsrv.h"
#include "error/exception.h"
#include "types/conversion.h"
#include "com/comfarunknown.h"
#include "com/classfactorynear.h"
#include "com/comstring.h"
#include "trace/trace.h"
#include "com/module.h"
#include "stream/stream.h"
#include "stream/mtiodevice.h"
#include "threading/lock.h"
#include "threading/workerthread.h"
#include "com/unknownimpl.h"
#include "com/interfaces.h"
#include "com/comptr.h"
#include "com/vector.h"
#include "3rd-party/microsoft/threading/threadpool.h"
#include <unknwn.h>
#include <guiddef.h>

namespace NkCom
{

	const size_t const_buffer_size = 8192;

	class CComNearSrv::CImpl : public NkStream::CStreamIODeviceEvent
	{
	public:
		CComNearSrv *m_pThis;
		NkStream::CStreamIODevice *m_p_iodevice;
		NkStream::CMTIODevice *m_p_threading_iodevice;
		CModule *m_pModule;
		bool m_auto_delete_provider;
		//bool m_bApartmentThreaded;
		CLSID m_clsid = CLSID_NULL;
		ULONG32	m_server_id = 0;
		ULONG m_cRef;

		NkStream::CStream m_StreamSnd;
		NkStream::CStream m_StreamRcv;
		//NkThreading::CWorkerThread<CImpl> m_Worker;
		//NkCom::CComPtr<IGlobalInterfaceTable> m_sp_git;

		NkThreading::CCriticalSection m_snd_lock;

		CImpl(CComNearSrv *pThis, NkStream::CStreamIODevice* pNewProvider
			, CModule *pModule
			, bool auto_delete_provider)
			: m_pThis(pThis)
			, m_p_iodevice(pNewProvider)
			, m_pModule(pModule)
			, m_auto_delete_provider(auto_delete_provider)
			, m_cRef(0)
			, m_StreamSnd(0, const_buffer_size, 1)
			, m_StreamRcv(0, const_buffer_size, 2)
		{
			NkError::CBaseException::check_arg(pNewProvider, __FILE__, __LINE__);

			m_pModule ? m_pModule->lock_server(true) : 0;

			//check com threading model
			NkError::CBaseException::check_result(::CoInitializeEx(0, COINIT_MULTITHREADED), __FILE__, __LINE__);
			::CoUninitialize();

			//m_bApartmentThreaded = FAILED(::CoInitializeEx(0, COINIT_MULTITHREADED));


			m_p_threading_iodevice = new NkStream::CMTIODevice(pNewProvider, false, const_buffer_size);
			

			m_StreamSnd.stream_device(m_p_threading_iodevice);
			m_StreamRcv.stream_device(m_p_threading_iodevice);
			m_p_threading_iodevice->start_async_callback(this, 2);
		}

		~CImpl()
		{
			try
			{
				if (!m_p_threading_iodevice->is_closed()) {
					NkThreading::CLockGuard lock(m_snd_lock);
					m_StreamSnd << static_cast<short>(CServer::FarCmdShutdown);
					m_StreamSnd.flush();
				}
			}
			catch (...) {}

			shutdown();

			delete m_p_threading_iodevice;

			if (m_auto_delete_provider) {
				delete m_p_iodevice;
			}

			if (m_pModule) {
				m_pModule->lock_server(false);
			}

			if (NkTrace::CTrace::IsTraceLevel3()) {
				NkTrace::CTrace::TraceDet(L"CComNearSrv::~CComNearSrv()");
			}
		}

		HRESULT handle_error(NkError::CException& e)
		{
			e.report();
			async_shutdown();
			return E_FAIL;
		}

		virtual bool on_time_out()
		{
#ifdef _DEBUG
			return true;
#endif // _DEBUG

			async_shutdown();
			return false;
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
		}

		virtual bool on_receive(ULONG32 stream_id)
		{
			_ASSERT(stream_id == 2);

			try
			{
				short FarCmd;
				m_StreamRcv >> FarCmd;

				switch (FarCmd) {
				case CServer::FarCmdFctCall: {
					ULONG32 id;
					ULONG index;
					m_StreamRcv >> id;
					m_StreamRcv >> index;
					get_instance(id)->call(index, m_StreamRcv);
					break;
				}
				case CServer::FarCmdDelete: {
					ULONG32 id;
					m_StreamRcv >> id;
					delete get_instance(id);
					m_StreamRcv << static_cast<HRESULT>(S_OK);
					m_StreamRcv.flush();
					break;
				}
				case CServer::FarCmdQueryInterface: {
					ULONG32 id;
					IID iid;
					m_StreamRcv >> id;
					m_StreamRcv >> iid;
					m_StreamRcv << get_instance(id)->query_interface(iid);
					m_StreamRcv.flush();
					break;
				}
				case CServer::FarCmdShutdown: {
					async_shutdown();
					return false;
				}
				default: NkError::CBaseException::check_result(E_INVALIDARG, __FILE__, __LINE__);
				}
				return true;
			}
			catch (NkError::CException& e) {
				if (e.base_error() != NkError::CBaseException::connection_closed) {
					e.report();
				}
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
			m_snd_lock.lock();

			if (m_b_shutdown) {
				m_snd_lock.unlock();
				throw NkError::CBaseException
					(NkError::CBaseException::BaseErrors::server_down, __FILE__, __LINE__);
			}


			m_StreamSnd << static_cast<short>(CServer::FarCmdFctCall);
			m_StreamSnd << id;
			m_StreamSnd << index;
			return m_StreamSnd;
		}

		void end_fct_call()
		{
			m_snd_lock.unlock();
		}

		HRESULT remote_logon(const BYTE credentials[], size_t cb)
		{
			try {
				NkThreading::CLockGuard lock(m_snd_lock);

				if (m_b_shutdown) {
					throw NkError::CBaseException
						(NkError::CBaseException::BaseErrors::server_down, __FILE__, __LINE__);
				}

				NkCom::CVector<BYTE, size_t> credentials(credentials, cb);

				m_StreamSnd << static_cast<short>(CServer::FarCmdLogon);
				m_StreamSnd << credentials;
				m_StreamSnd.flush();

				HRESULT hr;
				m_StreamSnd >> hr;

				if (FAILED(hr)) {
					return hr;
				}
			}
			catch (NkError::CException& e) {
				e.report();
				return e.error();
			}
			return S_OK;
		}

		HRESULT remote_create_instance(REFCLSID rclsid, DWORD dwClsContext, REFIID riid, ULONG32& id)
		{
			try {
				NkThreading::CLockGuard lock(m_snd_lock);

				if (m_b_shutdown) {
					throw NkError::CBaseException
						(NkError::CBaseException::BaseErrors::server_down, __FILE__, __LINE__);
				}

				m_StreamSnd << static_cast<short>(CServer::FarCmdCoCreate);
				m_StreamSnd << rclsid;
				m_StreamSnd << dwClsContext;
				m_StreamSnd << riid;
				m_StreamSnd.flush();

				HRESULT hr;
				m_StreamSnd >> hr;
				m_StreamSnd >> id;

				if (FAILED(hr)) {
					return hr;
				}
			}
			catch (NkError::CException& e) {
				e.report();
				return e.error();
			}
			return S_OK;
		}

		HRESULT remote_query_interface(REFIID riid, ULONG32 id)
		{
			try {
				NkThreading::CLockGuard lock(m_snd_lock);

				if (m_b_shutdown) {
					throw NkError::CBaseException
						(NkError::CBaseException::BaseErrors::server_down, __FILE__, __LINE__);
				}

				/*if (NkTrace::CTrace::IsTraceLevel3()) {
					NkCom::CComString str;
					NkError::CBaseException::check_result(StringFromIID(riid, &str), __FILE__, __LINE__);
					NkTrace::CTrace::TraceDet(L"local call: CNearUnknown::QueryInterface(%s)"
						, str.data());
				}*/

				m_StreamSnd << static_cast<short>(CServer::FarCmdQueryInterface);
				m_StreamSnd << id;
				m_StreamSnd << riid;
				m_StreamSnd.flush();

				HRESULT hr;
				m_StreamSnd >> hr;
				return hr;
			}
			catch (NkError::CException& e) {
				return e.error();
			}
		}

		void remote_delete_instance(ULONG32 id)
		{
			try
			{
				NkThreading::CLockGuard lock(m_snd_lock);

				if (m_b_shutdown) {
					throw NkError::CBaseException
						(NkError::CBaseException::BaseErrors::server_down, __FILE__, __LINE__);
				}

				m_StreamSnd << static_cast<short>(CServer::FarCmdDelete);
				m_StreamSnd << id;
				m_StreamSnd.flush();

				HRESULT hr;
				m_StreamSnd >> hr;
			}
			catch (NkError::CException& e) {
				handle_error(e);
			}
		}

		HRESULT remote_create_class_factory(REFCLSID rclsid, DWORD dwClsContext, REFIID factory_iid
			, REFIID riid, LPVOID *ppv) throw()
		{
			try {
				NkThreading::CLockGuard lock(m_snd_lock);

				m_StreamSnd << static_cast<short>(CServer::FarCmdCoCreate);
				m_StreamSnd << rclsid;
				m_StreamSnd << dwClsContext;
				m_StreamSnd << riid;
				m_StreamSnd.flush();

				HRESULT hr;
				ULONG32 id;
				m_StreamSnd >> hr;
				m_StreamSnd >> id;

				if (FAILED(hr)) {
					return hr;
				}

				CClassFactoryNear* pNew = new CClassFactoryNear(m_pThis, id);
				pNew->creator_iid(factory_iid);
				pNew->QueryInterface(riid, ppv);
			}
			catch (NkError::CException& e)
			{
				e.report();
				return e.error();
			}
			return S_OK;
		}
	};

	
	CComNearSrv::CComNearSrv(NkStream::CStreamIODevice* pNewProvider, CModule *pModule
		, bool auto_delete_provider/* = true*/)
		: _Impl(new CImpl(this, pNewProvider, pModule, auto_delete_provider))
	{
	}

	CComNearSrv::~CComNearSrv()
	{
		delete _Impl;
	}

	STDMETHODIMP CComNearSrv::QueryInterface(REFGUID iid, void **pp)
	{
		if (iid == IID_IUnknown) {
			(*pp) = this;
			AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	ULONG CComNearSrv::Release()
	{
		if (--_Impl->m_cRef != 0) {
			return _Impl->m_cRef;
		}
		delete this;
		return 0;
	}

	ULONG CComNearSrv::AddRef()
	{
		return ++_Impl->m_cRef;
	}

	HRESULT CComNearSrv::remote_create_instance(REFCLSID rclsid
		, DWORD dwClsContext
		, REFIID riid, ULONG32& id)
	{
		return _Impl->remote_create_instance(rclsid, dwClsContext, riid, id);
	}

	HRESULT CComNearSrv::remote_create_class_factory(REFCLSID rclsid, DWORD dwClsContext, REFIID factory_iid
		, REFIID riid, LPVOID *ppv) throw()
	{
		return _Impl->remote_create_class_factory(rclsid, dwClsContext, factory_iid, riid, ppv);
	}

	HRESULT CComNearSrv::remote_create_instance(REFCLSID rclsid, DWORD dwClsContext
		, REFIID proxy_iid, REFIID riid, LPVOID *ppv) throw()
	{
		/*try
		{
			NkThreading::CLockGuard lock(m_Lock);

			m_StreamSnd << static_cast<short>(FarCmdCoCreate);
			m_StreamSnd << rclsid;
			m_StreamSnd << dwClsContext;
			m_StreamSnd << riid;
			m_StreamSnd.flush();

			HRESULT hr;
			ULONG32 id;
			m_StreamSnd >> hr;
			m_StreamSnd >> id;

			if (FAILED(hr))
			{
				return hr;
			}

			create_near_proxy(proxy_iid, riid, ppv, id);
		}
		catch (NkError::CException& e)
		{
			e.report();
			return e.error();
		}*/
		return S_OK;
	}

	HRESULT CComNearSrv::remote_query_interface(REFIID riid, ULONG32 id)
	{
		return _Impl->remote_query_interface(riid, id);
	}

	void CComNearSrv::remote_delete_instance(ULONG32 id)
	{
		_Impl->remote_delete_instance(id);
	}

	void CComNearSrv::shutdown()
	{

	}

	NkStream::CStream& CComNearSrv::begin_fct_call(ULONG32 index, ULONG32 id)
	{
		return _Impl->begin_fct_call(index, id);
	}

	void CComNearSrv::end_fct_call()
	{
		_Impl->end_fct_call();
	}

	REFCLSID CComNearSrv::clsid()
	{
		return _Impl->m_clsid;
	}

	void CComNearSrv::clsid(REFCLSID clsid)
	{
		_Impl->m_clsid = clsid;
	}

	ULONG32 CComNearSrv::register_instance(CComFarUnknown* p)
	{
		return _Impl->register_instance(p);
	}

	void CComNearSrv::unregister_instance(ULONG32 id)
	{
		_Impl->unregister_instance(id);
	}

	CComFarUnknown* CComNearSrv::get_instance(ULONG32 id)
	{
		return _Impl->get_instance(id);
	}

	HRESULT CComNearSrv::remote_logon(const BYTE credentials[], size_t cb)
	{
		return _Impl->remote_logon(credentials, cb);
	}

	HRESULT CComNearSrv::handle_error(NkError::CException& e)
	{
		return _Impl->handle_error(e);
	}

	ULONG32 CComNearSrv::server_id()
	{
		return _Impl->m_server_id;
	}

	void CComNearSrv::server_id(ULONG32 id)
	{
		_Impl->m_server_id = id;
	}

	IGlobalInterfaceTable* CComNearSrv::get_git()
	{
		return 0;// _Impl->m_sp_git;
	}
}