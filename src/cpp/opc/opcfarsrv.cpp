#include "stdafx.h"
#include "opcfarsrv.h"
#include "opc/opcregistry.h"
#include "util/passvault.h"
#include "ssl/sha.h"

namespace NkOPC
{
	class COPCFarSrv::CImpl
	{
	public:
		bool m_logon = false;
	};

	COPCFarSrv::COPCFarSrv(NkStream::CStreamIODevice* provider
		, NkCom::CServerEvent* p_event /*= 0 */
		, bool auto_delete_provider /*= true*/)
		: NkCom::CComFarSrv(provider, p_event, auto_delete_provider)
		, _Impl(new CImpl)
	{
	}

	COPCFarSrv::~COPCFarSrv()
	{
		delete _Impl;
	}

	STDMETHODIMP_(ULONG) COPCFarSrv::AddRef()
	{
		return __super::AddRef();
	}

	STDMETHODIMP_(ULONG) COPCFarSrv::Release()
	{
		return __super::Release();
	}

	STDMETHODIMP COPCFarSrv::QueryInterface(REFGUID iid, void **pp)
	{
		return __super::QueryInterface(iid, pp);
	}


	HRESULT COPCFarSrv::create(REFCLSID rclsid, DWORD dwClsContext, REFIID riid, ULONG32& id)
	{
		try
		{
			if (rclsid == CLSID_OpcServerList) {
				NkCom::CComFarUnknown* pNew = new COPCServerList2Far(this, rclsid, dwClsContext, riid);
				id = pNew->id();
				return S_OK;
			}
			if (riid == IID_IOPCServer) {
				NkCom::CComFarUnknown* pNew = new COPCServerFar(this, rclsid, dwClsContext, riid);
				id = pNew->id();
				return S_OK;
			}
			if (riid == IID_IClassFactory) {
				NkCom::CComFarUnknown* pNew = new NkCom::CClassFactoryFar(this, rclsid, dwClsContext, riid);
				id = pNew->id();
				return S_OK;
			}
		}
		catch (NkError::CBaseException& e) {
			e.report();
			return e.error();
		}
		return E_NOINTERFACE;
	}

	void COPCFarSrv::create_near_proxy(REFIID proxy_iid, REFIID riid, void** ppv, ULONG32 id)
	{
		if (proxy_iid == IID_IOPCShutdown)
		{
			COPCShutdownNear* pNew = new COPCShutdownNear(this, id);
			NkError::CBaseException::check_result(pNew->QueryInterface(riid, ppv), __FILE__, __LINE__);
		}
		else if (proxy_iid == IID_IOPCDataCallback)
		{
			COPCDataCallbackNear* pNew = new COPCDataCallbackNear(this, id);
			NkError::CBaseException::check_result(pNew->QueryInterface(riid, ppv), __FILE__, __LINE__);
		}
		else if (proxy_iid == IID_IAdviseSink)
		{
			COPCAdviseSinkNear* pNew = new COPCAdviseSinkNear(this, id);
			NkError::CBaseException::check_result(pNew->QueryInterface(riid, ppv), __FILE__, __LINE__);
		}
		else
		{
			NkError::CBaseException::check(E_INVALIDARG, __FILE__, __LINE__);
		}
	}

	NkCom::CComFarUnknown* COPCFarSrv::create_far_proxy(IUnknown* p, REFIID riid, ULONG32& id)
	{
		if (riid == IID_IOPCServer)
		{
			NkCom::CComFarUnknown* pNew = new COPCServerFar(this, p);
			id = pNew->id();
			/*_ASSERT(0);
		   if (m_shutdown_cookie == 0)
		   {
		   try
		   {
		   pNew->internal_get_interface(m_spCPC);

		   NkCom::CComPtr<IConnectionPoint> spCP;

		   HRESULT hr = spCPC->FindConnectionPoint(IID_IOPCShutdown, &spCP);
		   NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

		   hr = spCP->Advise(static_cast<CServer*>(this), &m_shutdown_cookie);
		   NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
		   }
		   catch (NkError::CException& e)
		   {
		   e.report();
		   }
		   }*/

			return pNew;
		}
		if (riid == IID_IOPCGroupStateMgt)
		{
			NkCom::CComFarUnknown* pNew = new COPCGroupFar(this, p);
			id = pNew->id();
			return pNew;
		}
		NkError::CBaseException::check(E_INVALIDARG, __FILE__, __LINE__);
		return 0;
	}

	HRESULT COPCFarSrv::logon(const BYTE* credentials, size_t cb)
	{

		//NkUtil::CPassVault vault;
		//vault.setAuthenticationBlob(credentials, cb);

		/*_Impl->m_logon = true;
		return S_OK;

		_Impl->m_logon = false;
		BYTE digest[SHA256_DIGEST_LENGTH] = { 0 };

		try {
			NkOPC::CTunnelRegEntry::GetPassHash(digest, sizeof(digest));
		}
		catch (...) {
			return E_ACCESSDENIED;
		}

		if (memcmp(credentials, digest, SHA256_DIGEST_LENGTH) == 0) {
			_Impl->m_logon = true;
			return S_OK;
		}*/
		return S_OK;
	}

	bool COPCFarSrv::has_access(COPCFarSrvAccess level)
	{
		return true;
	}

	bool COPCFarSrv::filter_access(COPCFarSrvAccess level
		, const std::vector<GUID>& guids
		, std::vector<bool>& accesses
		, size_t* allowed_count)
	{
		if (!has_access(level) || level != COPCFarSrvAccess::BrowseOPCServer) {
			return false;
		}

		accesses.clear();
		accesses.reserve(guids.size());

		std::vector<GUID> tunnel_guids;
		NkOPC::CTunnelRegEntry::EnumEntries(tunnel_guids);

		if (allowed_count) {
			(*allowed_count) = 0;
		}

		for (auto i: guids) {
			auto f = std::find(tunnel_guids.begin(), tunnel_guids.end(), i);
			if (f != tunnel_guids.end()) {
				accesses.push_back(false);
			}
			else {
				accesses.push_back(true);
				if (allowed_count) {
					(*allowed_count)++;
				}
			}
		}
		return true;
	}
}