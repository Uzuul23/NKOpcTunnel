#pragma once
#include <unknwn.h>

namespace NkError
{
	class CException;
}

namespace NkStream
{
	class CStream;
}

namespace NkType
{
	class CConnectionAddressInfo;
}

interface IConnectionPoint;

namespace NkCom
{
	class CComFarUnknown;
	class CServer : public IUnknown
	{
	public:
		CServer() {}
		virtual ~CServer() {}
		virtual REFCLSID clsid() = 0;
		virtual void clsid(REFCLSID clsid) = 0;
 		virtual ULONG32 register_instance(CComFarUnknown* p) = 0;
		virtual void unregister_instance(ULONG32 id) = 0;
		virtual CComFarUnknown* get_instance(ULONG32 id) = 0;
		virtual IGlobalInterfaceTable* get_git() = 0;
		virtual void create_near_proxy(REFIID proxy_iid, REFIID riid, void** ppv, ULONG32 id) = 0;
		virtual CComFarUnknown* create_far_proxy(IUnknown* p, REFIID riid, ULONG32& id) = 0;
		virtual HRESULT remote_logon(const BYTE credentials[], size_t cb) = 0;
		virtual HRESULT remote_create_instance(REFCLSID rclsid, DWORD dwClsContext, REFIID riid, ULONG32& id) = 0;
		virtual HRESULT remote_create_class_factory(REFCLSID rclsid, DWORD dwClsContext, REFIID factory_iid, REFIID riid, LPVOID *ppv) = 0;
		virtual HRESULT remote_create_instance(REFCLSID rclsid, DWORD dwClsContext, REFIID proxy_iid, REFIID riid, LPVOID *ppv) = 0;
		virtual void remote_delete_instance(ULONG32 id) = 0;
		virtual HRESULT remote_query_interface(REFIID riid, ULONG32 id) = 0;
		virtual NkStream::CStream& begin_fct_call(ULONG32 index, ULONG32 id) = 0;
		virtual void end_fct_call() = 0;
		virtual HRESULT handle_error(NkError::CException& e) = 0;
		virtual void store_cp_advise(IConnectionPoint* p_cp, DWORD cookie) = 0;
		virtual void store_cp_unadvise(IConnectionPoint* p_cp, DWORD cookie) = 0;
		virtual void shutdown() = 0;
		virtual ULONG32 server_id() = 0;
		virtual void server_id(ULONG32 id) = 0;

		enum FarCmds
		{
			FarCmdLogon = 1,
			FarCmdCoCreate,
			FarCmdFctCall,
			FarCmdDelete,
			FarCmdQueryInterface,
			FarCmdShutdown
		};
	};

	class CServerEvent
	{
	public:
		virtual void on_shutdown(CServer* p) = 0;
	};

	class CLogonEvent
	{
	public:
		virtual HRESULT on_logon(CServer* p) = 0;
	};
}
