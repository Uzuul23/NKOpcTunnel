#pragma once
#include "com/comnearsrv.h"

namespace NkSSL
{
	class COpenSSLCtx;
}

namespace NkOPC
{
	class COPCNearSrv : public NkCom::CComNearSrv
	{
	public:
		COPCNearSrv(NkStream::CStreamIODevice* pNewProvider, NkCom::CModule *pModule
			, bool auto_delete_provider = true);

		static COPCNearSrv* create_new_server(const char* addr
			, NkCom::CModule *pModule = 0, NkCom::CLogonEvent *pLogon = 0);

		static COPCNearSrv* create_new_server_ssl(const char* addr
			, NkSSL::COpenSSLCtx& ctx, NkCom::CModule *pModule = 0
			, NkCom::CLogonEvent *pLogon = 0);

		virtual void create_near_proxy(REFIID proxy_iid, REFIID riid, void** ppv, ULONG32 id);
		virtual NkCom::CComFarUnknown* create_far_proxy(IUnknown* p, REFIID riid, ULONG32& id);
	};
}