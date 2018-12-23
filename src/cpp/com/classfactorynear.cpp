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
#include "classfactorynear.h"
#include "bstr.h"
#include "types/conversion.h"
#include "com/interfaces.h"

namespace NkCom
{
	CClassFactoryNear::CClassFactoryNear(CServer* p, ULONG32 id)
		: CNearUnknown(p, id)
		, m_iid(IID_NULL)
	{
	}

	STDMETHODIMP CClassFactoryNear::CreateInstance( /* [unique][in] */ IUnknown *pUnkOuter
		, /* [in] */ REFIID riid
		, /* [iid_is][out] */ void **ppvObject)
	{
		if (pUnkOuter != 0) {
			return CLASS_E_NOAGGREGATION;
		}

		try {
			CNearUnknown::CCallFctGuard srv(this, 1, L"IClassFactory::CreateInstance()");
			srv.stream << riid;
			srv.stream << m_iid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				ULONG32 id;
				srv.stream.check_type(VT_UNKNOWN);
				srv.stream >> id;
				m_pSrv->create_near_proxy(m_iid, riid, ppvObject, id);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP CClassFactoryNear::LockServer( /* [in] */ BOOL fLock)
	{
		if (fLock) {
			m_pSrv->AddRef();
		}
		else {
			m_pSrv->Release();
		}
		return S_OK;
	}

	STDMETHODIMP CClassFactoryNear::GetLicInfo(/* [out][in] */LICINFO *pLicInfo)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 3, L"IClassFactory2::GetLicInfo()");
			srv.stream << pLicInfo->cbLicInfo;
			srv.stream << pLicInfo->fLicVerified;
			srv.stream << pLicInfo->fRuntimeKeyAvail;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				srv.stream >> pLicInfo->cbLicInfo;
				srv.stream >> pLicInfo->fLicVerified;
				srv.stream >> pLicInfo->fRuntimeKeyAvail;
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP CClassFactoryNear::RequestLicKey(/* [in] */ DWORD dwReserved
		,/* [out] */BSTR *pBstrKey)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 4, L"IClassFactory2::RequestLicKey()");
			srv.stream << dwReserved;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				CBSTR Key;
				srv.stream >> Key;
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP CClassFactoryNear::CreateInstanceLic( /* [in] */ IUnknown *pUnkOuter,
		/* [in] */ IUnknown *pUnkReserved
		,/* [in] */ REFIID riid,
		/* [in] */ BSTR bstrKey
		,/* [iid_is][out] */ PVOID *ppvObj)
	{
		if (pUnkOuter != 0 || pUnkReserved != 0) {
			return CLASS_E_NOAGGREGATION;
		}

		try {
			CNearUnknown::CCallFctGuard srv(this, 5, L"IClassFactory2::CreateInstanceLic()");
			srv.stream << riid;
			CBSTR::read(srv.stream, bstrKey);
			srv.stream << m_iid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				ULONG32 id;
				srv.stream.check_type(VT_UNKNOWN);
				srv.stream >> id;
				m_pSrv->create_near_proxy(m_iid, riid, ppvObj, id);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	CLSID CClassFactoryNear::creator_iid() const
	{
		return m_iid;
	}

	void CClassFactoryNear::creator_iid(CLSID val)
	{
		m_iid = val;
	}
}