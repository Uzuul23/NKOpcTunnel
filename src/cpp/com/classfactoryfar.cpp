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
#include "classfactoryfar.h"
#include "com/comstring.h"
#include "com/bstr.h"
#include "stream/stream.h"
#include "error/baseexception.h"

namespace NkCom
{
	CClassFactoryFar::CClassFactoryFar(CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid)
		: CComFarUnknown(pSrv)
	{
		HRESULT hr = CoGetClassObject(rclsid, dwClsContext, 0, riid, (void**)&m_spUnknown);
		NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
		NkError::CBaseException::check_result(query_interface(riid), __FILE__, __LINE__);
	}

	HRESULT CClassFactoryFar::query_interface(REFIID iid)
	{
		if (iid == IID_IUnknown){
			return S_OK;
		}
		if (iid == IID_IClassFactory){
			if (!m_spClassFactory) {
				return m_spUnknown.QueryInterface(&m_spClassFactory);
			}
			return S_OK;
		}
		if (iid == IID_IClassFactory2){
			if (!m_spClassFactory2){
				return m_spUnknown.QueryInterface(&m_spClassFactory2);
			}
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	void CClassFactoryFar::call(ULONG32 index, NkStream::CStream& stream)
	{
		switch (index)
		{
		case 1: CreateInstance(stream); break;
		case 2: LockServer(stream); break;
		case 3: GetLicInfo(stream); break;
		case 4: RequestLicKey(stream); break;
		case 5: CreateInstanceLic(stream); break;
		}

		if (NkTrace::CTrace::IsTraceLevel3()) {
			static const WCHAR* pszFctNames[] = {
				L"dummy",
				L"IClassFactory::CreateInstance()",
				L"IClassFactory::LockServer()",
				L"IClassFactory2::GetLicInfo()",
				L"IClassFactory2::RequestLicKey()",
				L"IClassFactory2::CreateInstanceLic()"
			};

			if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
				_ASSERT(0);
				return;
			}
			NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
		}
	}

	void CClassFactoryFar::CreateInstance(NkStream::CStream& stream)
	{
		IID iid;
		IID proxy_iid;
		stream >> iid;
		stream >> proxy_iid;

		CComPtr<IUnknown> sp;
		HRESULT hr = S_OK;
		if (m_spClassFactory2){
			m_spClassFactory2->CreateInstance(0, iid, (void**)&sp);
		}
		else{
			m_spClassFactory->CreateInstance(0, iid, (void**)&sp);
		}

		if (SUCCEEDED(hr)){
			ULONG32 id;
			CComPtr<IUnknown> spProxy;
			try{
				spProxy = m_pSrv->create_far_proxy(sp, proxy_iid, id);
			}
			catch (NkError::CException& e){
				stream << static_cast<HRESULT>(E_UNEXPECTED);
				e.report();
			}
			stream << hr;
			stream.write_type(VT_UNKNOWN);
			stream << id;
			spProxy.Detach();
		}
		else{
			stream << hr;
		}
		stream.flush();
	}

	void CClassFactoryFar::LockServer(NkStream::CStream& stream)
	{
		BOOL fLock;
		stream >> fLock;

		if (m_spClassFactory2){
			stream << m_spClassFactory2->LockServer(fLock);
		}
		else{
			stream << m_spClassFactory->LockServer(fLock);
		}

		stream.flush();
	}

	void CClassFactoryFar::GetLicInfo(NkStream::CStream& stream)
	{
		LICINFO LicInfo;
		stream >> LicInfo.cbLicInfo;
		stream >> LicInfo.fLicVerified;
		stream >> LicInfo.fRuntimeKeyAvail;

		HRESULT hr = m_spClassFactory2->GetLicInfo(&LicInfo);

		stream << hr;
		if (SUCCEEDED(hr)){
			stream << LicInfo.cbLicInfo;
			stream << LicInfo.fLicVerified;
			stream << LicInfo.fRuntimeKeyAvail;
		}
		stream.flush();
	}

	void CClassFactoryFar::RequestLicKey(NkStream::CStream& stream)
	{
		DWORD dwReserved;
		CBSTR Key;
		stream >> dwReserved;

		HRESULT hr = m_spClassFactory2->RequestLicKey(dwReserved, &Key);

		stream << hr;
		if (SUCCEEDED(hr)){
			stream << Key;
		}
		stream.flush();
	}

	void CClassFactoryFar::CreateInstanceLic(NkStream::CStream& stream)
	{
		IID iid;
		CBSTR Key;
		IID proxy_iid;
		stream >> iid;
		stream >> Key;
		stream >> proxy_iid;

		CComPtr<IUnknown> sp;
		HRESULT hr = m_spClassFactory2->CreateInstanceLic(0, 0, iid, Key, (void**)&sp);

		if (SUCCEEDED(hr)){
			ULONG32 id;
			CComPtr<IUnknown> spProxy;
			try{
				spProxy = m_pSrv->create_far_proxy(sp, proxy_iid, id);
			}
			catch (NkError::CException& e){
				stream << static_cast<HRESULT>(E_UNEXPECTED);
				e.report();
			}
			stream << hr;
			stream.write_type(VT_UNKNOWN);
			stream << id;
			spProxy.Detach();
		}
		else{
			stream << hr;
		}
		stream.flush();
	}
}