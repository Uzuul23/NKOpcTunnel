#include "stdafx.h"
#include "opcnearsrv.h"
#include "opc/opcadvisesinkfar.h"
#include "opc/OPCServerList2near.h"
#include "opc/opcservernear.h"
#include "opc/opcgroupnear.h"
#include "opc/opcshutdownfar.h"
#include "opc/opcdatacallbackfar.h"
#include "error/exception.h"
#include "socket/socket.h"
#include "ssl/sslsocket.h"
#include "ssl/opensslctx.h"


namespace NkOPC
{
	COPCNearSrv::COPCNearSrv(NkStream::CStreamIODevice* pNewProvider, NkCom::CModule *pModule
		, bool auto_delete_provider /*= true*/) 
		: CComNearSrv(pNewProvider, pModule, auto_delete_provider)
	{

	}

	NkOPC::COPCNearSrv* COPCNearSrv::create_new_server(const char* addr
		, NkCom::CModule *pModule /*= 0*/, NkCom::CLogonEvent *pLogon/* = 0*/)
	{

		NkOPC::COPCNearSrv* pServer = 0;
		try {
			NkSocket::CSocket* pSocket = new NkSocket::CSocket;
			try {
				pSocket->socket();
				pSocket->connect(addr);
			}
			catch (...) {
				delete pSocket;
				throw;
			}
			
			pServer = new NkOPC::COPCNearSrv(pSocket, pModule);
			if (pLogon) {
				pLogon->on_logon(pServer);
			}
			return pServer;
		}
		catch (NkError::CException& e) {
			e.report();
			delete pServer;
			throw;
		}
		return 0;
	}

	NkOPC::COPCNearSrv* COPCNearSrv::create_new_server_ssl(const char* addr
		, NkSSL::COpenSSLCtx& ctx, NkCom::CModule *pModule /*= 0*/
		, NkCom::CLogonEvent *pLogon/* = 0*/)
	{
		NkOPC::COPCNearSrv* pServer = 0;
		try {
			NkSSL::CSocket *pSocket = new NkSSL::CSocket;
			try {
				pSocket->socket();
				pSocket->connect(addr);
				pSocket->ssl_connect(ctx);
			}
			catch (...) {
				delete pSocket;
				throw;
			}

			pServer = new NkOPC::COPCNearSrv(pSocket, pModule);

			if (pLogon) {
				pLogon->on_logon(pServer);
			}
			return pServer;
		}
		catch (NkError::CException& e) {
			e.report();
			delete pServer;
			throw;
		}
		return 0;
	}

	void COPCNearSrv::create_near_proxy(REFIID proxy_iid, REFIID riid, void** ppv, ULONG32 id)
	{
		if (proxy_iid == IID_IOPCServer)
		{
			COPCServerNear* pNew = new COPCServerNear(this, id);
			NkError::CBaseException::check_result(pNew->QueryInterface(riid, ppv), __FILE__, __LINE__);
		}
		else if (proxy_iid == IID_IOPCServerList)
		{
			COPCServerList2Near* pNew = new COPCServerList2Near(this, id);
			NkError::CBaseException::check_result(pNew->QueryInterface(riid, ppv), __FILE__, __LINE__);
		}
		else if (proxy_iid == IID_IOPCServerList2)
		{
			COPCServerList2Near* pNew = new COPCServerList2Near(this, id);
			NkError::CBaseException::check_result(pNew->QueryInterface(riid, ppv), __FILE__, __LINE__);
		}
		else
		{
			NkError::CBaseException::check(E_INVALIDARG, __FILE__, __LINE__);
		}
	}

	NkCom::CComFarUnknown* COPCNearSrv::create_far_proxy(IUnknown* p, REFIID riid, ULONG32& id)
	{
		if (riid == IID_IOPCShutdown)
		{
			NkCom::CComFarUnknown* pNew = new COPCShutdownFar(this, p, &id);
			return pNew;
		}
		if (riid == IID_IOPCDataCallback)
		{
			NkCom::CComFarUnknown* pNew = new COPCDataCallbackFar(this, p, &id);
			return pNew;
		}
		if (riid == IID_IAdviseSink)
		{
			NkCom::CComFarUnknown* pNew = new COPCAdviseSinkFar(this, p, &id);
			return pNew;
		}
		NkError::CBaseException::check(E_INVALIDARG, __FILE__, __LINE__);
		return 0;
	}

	

}
