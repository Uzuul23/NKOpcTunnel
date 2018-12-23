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
#include "com/comptr.h"
#include "com/comfarunknown.h"
#include <OCIdl.h>

namespace NkCom
{
	class CClassFactoryFar : public NkCom::CComFarUnknown
	{
	public:

		CClassFactoryFar(CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid);

		HRESULT query_interface(REFIID iid);
		void call(ULONG32 index, NkStream::CStream& stream);
		void CreateInstance(NkStream::CStream& stream);
		void LockServer(NkStream::CStream& stream);
		void GetLicInfo(NkStream::CStream& stream);
		void RequestLicKey(NkStream::CStream& stream);
		void CreateInstanceLic(NkStream::CStream& stream);

	private:
		CClassFactoryFar();
		CClassFactoryFar &operator =(const CClassFactoryFar&);
		CClassFactoryFar(const CClassFactoryFar&);

		NkCom::CComPtr<IClassFactory> m_spClassFactory;
		NkCom::CComPtr<IClassFactory2> m_spClassFactory2;
	};
}