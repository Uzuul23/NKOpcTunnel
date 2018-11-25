#pragma once
#include "com/comnearsrv.h"
#include "trace/trace.h"
#include <vector>

namespace NkCom
{
	template <class BaseClass>
	class CNearUnknown
	{
	private:
		CNearUnknown();
		CNearUnknown &operator =(const CNearUnknown&);
		CNearUnknown(const CNearUnknown&);

	public:
		CNearUnknown(CServer* pSrv, ULONG32 id) : m_pSrv(pSrv), m_id(id)
		{
			NkError::CBaseException::check_arg(pSrv, __FILE__, __LINE__);
			m_pSrv->AddRef();
		}
		virtual ~CNearUnknown()
		{
			m_pSrv->remote_delete_instance(m_id);
			m_pSrv->Release();
		}

		class CInterfaceInfo
		{
		public:
			CInterfaceInfo() : m_iid(IID_NULL), m_implemented(false) {}
			CInterfaceInfo(REFIID riid, bool implemented) : m_iid(riid)
				, m_implemented(implemented) {}
			CInterfaceInfo &operator =(const CInterfaceInfo& d)
			{
				m_iid = d.m_iid;
				m_implemented = d.m_implemented;
				return *this;
			}
			CInterfaceInfo(const CInterfaceInfo& d)
			{
				(*this) = d;
			}

			IID m_iid;
			bool m_implemented;
		};

		HRESULT remote_query_interface(REFIID riid)
		{
			if (riid == IID_IUnknown) {
				return S_OK;
			}
			for (auto i : m_iids) {
				if (i.m_iid == riid) {
					return i.m_implemented ? S_OK : E_NOINTERFACE;
				}
			}
			if (SUCCEEDED(m_pSrv->remote_query_interface(riid, m_id))) {
				m_iids.push_back(CInterfaceInfo(riid, true));
				return S_OK;
			}
			else {
				m_iids.push_back(CInterfaceInfo(riid, false));
				return E_NOINTERFACE;
			}
		}

		static const char* class_name()
		{
			return typeid(BaseClass).name();
		}

		static BaseClass* create_instance(CServer* p, ULONG32 id)
		{
			BaseClass* pNew = new BaseClass(p, id);
			return pNew;
		}

		class CCallFctGuard
		{
		public:
			CCallFctGuard(CNearUnknown* p, ULONG index) : _p(p)
				, stream(p->begin_fct_call(index))
				, _FctName(0)
			{
			}
			CCallFctGuard(CNearUnknown* p, ULONG index, wchar_t* FctName) : _p(p)
				, stream(p->begin_fct_call(index))
				, _FctName(FctName)
			{
			}
			~CCallFctGuard()
			{
				_p->end_fct_call();

				if (_FctName && NkTrace::CTrace::IsTraceLevel3()) {
					NkTrace::CTrace::TraceDet(L"local call: %s result(0x%x)", _FctName, result);
				}
			}

			HRESULT result = S_OK;
			CNearUnknown* _p;
			NkStream::CStream& stream;
			wchar_t* _FctName;
		};

	protected:

		NkStream::CStream& begin_fct_call(ULONG32 index)
		{
			return m_pSrv->begin_fct_call(index, m_id);
		}
		void end_fct_call()
		{
			m_pSrv->end_fct_call();
		}
		HRESULT handle_error(NkError::CException& e)
		{
			return m_pSrv->handle_error(e);
		}

		std::vector<CInterfaceInfo> m_iids;
		CServer* m_pSrv;
		ULONG32 m_id;
		ULONG _cRef = 0;
	};
}

#define IMPL_ADDREF_RELEASE	\
	STDMETHOD_(ULONG, AddRef)(){ return ++_cRef; }	\
	STDMETHOD_(ULONG, Release)() { if (--_cRef != 0) return _cRef; delete this; return 0; }

#define IMPL_INTERFACE1(I1)	\
	IMPL_ADDREF_RELEASE	\
	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject)	{	\
	if (FAILED(m_pSrv->remote_query_interface(riid, m_id))) {return E_NOINTERFACE;} \
	if (riid == IID_IUnknown || riid == __uuidof(I1)) {	\
	(*ppvObject) = static_cast<I1*>(this); AddRef(); return S_OK; }	\
	return E_NOINTERFACE; }