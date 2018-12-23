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
#include "com/nearunknown.h"
#include "OCIdl.h"

namespace NkCom
{
	class CClassFactoryNear : public CNearUnknown < CClassFactoryNear >
		, public IClassFactory2
	{
	public:
		CClassFactoryNear(CServer* p, ULONG32 id);;

		IMPL_INTERFACE1(IClassFactory);

		STDMETHOD(CreateInstance)(IUnknown *pUnkOuter, REFIID riid, void **ppvObject);
		STDMETHOD(LockServer)(BOOL fLock);
		STDMETHOD(GetLicInfo)(LICINFO *pLicInfo);
		STDMETHOD(RequestLicKey)(DWORD dwReserved, BSTR *pBstrKey);
		STDMETHOD(CreateInstanceLic)(IUnknown *pUnkOuter, IUnknown *pUnkReserved
			, REFIID riid, BSTR bstrKey, PVOID *ppvObj);

		CLSID creator_iid() const;
		void creator_iid(CLSID val);

	private:
		CClassFactoryNear();
		CClassFactoryNear &operator =(const CClassFactoryNear&);
		CClassFactoryNear(const CClassFactoryNear&);

		CLSID m_iid;
	};
}