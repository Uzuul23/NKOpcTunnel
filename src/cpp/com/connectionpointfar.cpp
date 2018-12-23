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
#include "connectionpointfar.h"
#include "com/connectionpointcontainerfar.h"

namespace NkCom
{
	CConnectionPointFar::CConnectionPointFar(CServer* pSrv, IConnectionPoint* p, ULONG32* pid)
		: CComFarUnknown(pSrv, pid)
		, m_spConnectionPoint(p)
	{
	}

	CConnectionPointFar::CConnectionPointFar(NkCom::CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid) : CComFarUnknown(pSrv, rclsid, dwClsContext, riid)
	{
		NkError::CBaseException::check_result(query_interface(riid), __FILE__, __LINE__);
	}

	HRESULT CConnectionPointFar::query_interface(REFIID iid)
	{
		if (iid == IID_IUnknown) {
			return S_OK;
		}
		if (iid == IID_IConnectionPoint) {
			if (!m_spConnectionPoint) {
				return m_spUnknown->QueryInterface(&m_spConnectionPoint);
			}
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	void CConnectionPointFar::call(ULONG32 index, NkStream::CStream& stream)
	{
		switch (index) {
		case 1: GetConnectionInterface(stream); break;
		case 2: GetConnectionPointContainer(stream); break;
		case 3: Advise(stream); break;
		case 4: Unadvise(stream); break;
		case 5: EnumConnections(stream); break;
		}

		if (NkTrace::CTrace::IsTraceLevel3()) {
			static const WCHAR* pszFctNames[] = {
				L"dummy",
				L"IConnectionPoint::GetConnectionInterface()",
				L"IConnectionPoint::GetConnectionPointContainer()",
				L"IConnectionPoint::Advise()",
				L"IConnectionPoint::Unadvise()",
				L"IConnectionPoint::EnumConnections()"
			};
			if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
				_ASSERT(0);
				return;
			}
			NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
		}
	}

	void CConnectionPointFar::GetConnectionInterface(NkStream::CStream& stream)
	{
		IID iid;
		HRESULT hr = m_spConnectionPoint->GetConnectionInterface(&iid);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << iid;
		}
		stream.flush();
	}

	void CConnectionPointFar::GetConnectionPointContainer(NkStream::CStream& stream)
	{
		NkCom::CComPtrFar<IConnectionPointContainer, CConnectionPointContainerFar> spCPC(m_pSrv);

		HRESULT hr = m_spConnectionPoint->GetConnectionPointContainer(&spCPC);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << spCPC;
		}
		stream.flush();
	}

	void CConnectionPointFar::Advise(NkStream::CStream& stream)
	{
		ULONG32 id;
		IID iid;
		DWORD cookie;

		stream >> id;

		HRESULT hr = m_spConnectionPoint->GetConnectionInterface(&iid);

		if (FAILED(hr)) {
			stream << hr;
			stream.flush();
			return;
		}

		CComPtr<IUnknown> sp;

		try {
			m_pSrv->create_near_proxy(iid, iid, (void**)&sp, id);
		}
		catch (NkError::CException& e) {
			e.report();
			stream << static_cast<HRESULT>(E_UNEXPECTED);
			stream.flush();
			return;
		}

		hr = m_spConnectionPoint->Advise(sp, &cookie);

		stream << hr;
		if (SUCCEEDED(hr)) {
			//TODO:
			//sp->AddRef();
			m_pSrv->store_cp_advise(m_spConnectionPoint, cookie);
			stream << cookie;
		}
		stream.flush();
	}

	void CConnectionPointFar::Unadvise(NkStream::CStream& stream)
	{
		DWORD cookie;
		stream >> cookie;

		HRESULT hr = m_spConnectionPoint->Unadvise(cookie);

		if (SUCCEEDED(hr)) {
			m_pSrv->store_cp_unadvise(m_spConnectionPoint, cookie);
		}

		stream << hr;
		stream.flush();
	}

	void CConnectionPointFar::EnumConnections(NkStream::CStream& stream)
	{
		_ASSERT(0);
	}
}