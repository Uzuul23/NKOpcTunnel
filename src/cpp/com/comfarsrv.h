#pragma once
#include "stream/interfaces.h"
#include "com/interfaces.h"

namespace NkCom
{
	class CComFarSrv : public CServer
	{
	public:
		CComFarSrv(NkStream::CStreamIODevice* provider
			, NkCom::CServerEvent* pevent = 0
			, bool auto_delete_provider = true);

		virtual ~CComFarSrv();

		STDMETHOD_(ULONG, AddRef)();
		STDMETHOD_(ULONG, Release)();
		STDMETHOD(QueryInterface) (REFGUID iid, void **pp);

		ULONG32 server_id();
		void server_id(ULONG32 id);

		// CServer
		virtual REFCLSID clsid();
		virtual void clsid(REFCLSID clsid);
		ULONG32 register_instance(CComFarUnknown* p);
		void unregister_instance(ULONG32 id);
		CComFarUnknown* get_instance(ULONG32 id);
		virtual void lock_server(bool lock);
		virtual void lock_object(bool lock);
		IGlobalInterfaceTable* get_git();

		virtual void store_cp_advise(IConnectionPoint* p_cp, DWORD cookie);
		virtual void store_cp_unadvise(IConnectionPoint* p_cp, DWORD cookie);
		virtual void shutdown();

		void remote_close_connection() {};

		HRESULT remote_logon(const BYTE credentials[], size_t cb);
		HRESULT remote_create_instance(REFCLSID rclsid, DWORD dwClsContext, REFIID riid, ULONG32& id);;
		HRESULT remote_create_class_factory(REFCLSID rclsid, DWORD dwClsContext, REFIID factory_iid, REFIID riid, LPVOID *ppv);
		HRESULT remote_create_instance(REFCLSID rclsid, DWORD dwClsContext, REFIID proxy_iid, REFIID riid, LPVOID *ppv) throw();
		HRESULT remote_query_interface(REFIID riid, ULONG32 id) throw();
		void remote_delete_instance(ULONG32 id);

		NkStream::CStream& begin_fct_call(ULONG32 index, ULONG32 id);
		void end_fct_call();
		HRESULT handle_error(NkError::CException& e);

		virtual HRESULT logon(const BYTE* credentials, size_t cb) = 0;
		virtual HRESULT create(REFCLSID rclsid, DWORD dwClsContext, REFIID riid, ULONG32& id) = 0;

	private:
		CComFarSrv();
		CComFarSrv(const CComFarSrv&);
		CComFarSrv& operator=(const CComFarSrv&);

		class CImpl;
		CImpl* _Impl;
	};
}