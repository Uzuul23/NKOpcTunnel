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

#pragma once
#include "com/comfarsrv.h"
#include "com/enumfar.h"
#include <ocidl.h>

namespace NkCom
{
	class CConnectionPointContainerFar : public CComFarUnknown
	{
	public:
		CConnectionPointContainerFar(CServer* pSrv, IConnectionPointContainer* p, ULONG32* pid = 0)
			: CComFarUnknown(pSrv, pid)
			, _spCPC(p)
		{
		}

		CConnectionPointContainerFar(CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid)
			: CComFarUnknown(pSrv, rclsid, dwClsContext, riid)
		{
			NkError::CBaseException::check_result(query_interface(riid), __FILE__, __LINE__);
		}

		HRESULT query_interface(REFIID iid)
		{
			if (iid == IID_IUnknown) {
				return S_OK;
			}
			if (iid == IID_IConnectionPointContainer) {
				if (!_spCPC) {
					return m_spUnknown->QueryInterface(&_spCPC);
				}
				return S_OK;
			}
			return E_NOINTERFACE;
		}

		void call(ULONG32 index, NkStream::CStream& stream)
		{
			switch (index) {
			case 1: EnumConnectionPoints(stream); break;
			case 2: FindConnectionPoint(stream); break;
			}

			if (NkTrace::CTrace::IsTraceLevel3()) {
				static const WCHAR* pszFctNames[] = {
					L"dummy",
					L"IConnectionPointContainer::EnumConnectionPoints()",
					L"IConnectionPointContainer::FindConnectionPoint()"
				};
				if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
					_ASSERT(0);
					return;
				}
				NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
			}
		}

		void EnumConnectionPoints(NkStream::CStream& stream)
		{
			CComPtrFarEnumCP spEnum(m_pSrv);

			HRESULT hr = _spCPC->EnumConnectionPoints(&spEnum);

			stream << hr;
			if (SUCCEEDED(hr)) {
				stream << spEnum;
			}
			stream.flush();
		}

		void FindConnectionPoint(NkStream::CStream& stream)
		{
			IID iid;
			NkCom::CComPtrFar<IConnectionPoint, NkCom::CConnectionPointFar> spCP(m_pSrv);

			stream >> iid;

			HRESULT hr = _spCPC->FindConnectionPoint(iid, &spCP);

			stream << hr;
			if (SUCCEEDED(hr)) {
				stream << spCP;
			}
			stream.flush();
		}

	private:
		CConnectionPointContainerFar();
		CConnectionPointContainerFar &operator =(const CConnectionPointContainerFar&);
		CConnectionPointContainerFar(const CConnectionPointContainerFar&);

		NkCom::CComPtr<IConnectionPointContainer> _spCPC;
	};
}