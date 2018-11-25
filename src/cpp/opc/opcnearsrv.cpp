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
#include "socket/streamsocketssl.h"


namespace NkOPC
{
	COPCNearSrv::COPCNearSrv(NkStream::CStreamIODevice* pNewProvider, NkCom::CModule *pModule
		, bool auto_delete_provider /*= true*/) 
		: CComNearSrv(pNewProvider, pModule, auto_delete_provider)
	{

	}

	//NkCom::CServer* COPCNearSrv::create_new_server(NkSocket::CBind* pAddressInfo
	//	, NkCom::CModule *pModule /*= 0*/, NkCom::CLogonEvent *pLogon/* = 0*/)
	//{
	//	NkSocket::CStreamSocket* pSocket = 0;
	//	NkOPC::COPCNearSrv* pServer = 0;
	//	try {
	//		pSocket = new NkSocket::CStreamSocket;
	//		pSocket->connect(pAddressInfo);
	//		pServer = new NkOPC::COPCNearSrv(pSocket, pModule);
	//		if (pLogon) {
	//			pLogon->on_logon(pServer);
	//		}
	//		return pServer;
	//	}
	//	catch (NkError::CException& e) {
	//		e.report();
	//		delete pSocket;
	//		delete pServer;
	//		throw;
	//	}
	//	return 0;
	//}

	COPCNearSrv* COPCNearSrv::create_new_server(const char* addr
		, NkCom::CModule *pModule /*= 0*/, NkCom::CLogonEvent *pLogon/* = 0*/)
	{
		NkSocket::CSocket* pSocket = 0;
		NkOPC::COPCNearSrv* pServer = 0;
		try {
			pSocket = new NkSocket::CSocket;
			pSocket->socket();
			pSocket->connect(addr);
			pServer = new NkOPC::COPCNearSrv(pSocket, pModule);
			if (pLogon) {
				pLogon->on_logon(pServer);
			}
			return pServer;
		}
		catch (NkError::CException& e) {
			e.report();
			delete pSocket;
			delete pServer;
			throw;
		}
		return 0;
	}

#if defined NK_USE_SSL
	NkCom::CServer* COPCNearSrv::create_new_server_ssl(const NkSocket::CBind& bind, NkSSL::CNKOpenSSLCtx& ssl_ctx, NkCom::CModule *pModule /*= 0*/)
	{
		NkSocket::CStreamSocketSSl* pSocket = 0;
		NkOPC::COPCNearSrv* pServer = 0;
		try {
			pSocket = new NkSocket::CStreamSocketSSl;
			pSocket->connect(bind, ssl_ctx);
			pServer = new NkOPC::COPCNearSrv(pSocket, pModule);
			return pServer;
		}
		catch (NkError::CException& e) {
			e.report();
			delete pSocket;
			delete pServer;
			throw;
		}
		return 0;
	}
#endif

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
