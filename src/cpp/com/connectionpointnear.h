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
#include "com/comnearsrv.h"
#include "com/enumnear.h"
#include "com/comptr.h"
#include <OCIdl.h>

namespace NkCom
{
	class CConnectionPointNear : public CNearUnknown < CConnectionPointNear >
		, public IConnectionPoint
	{
	public:
		CConnectionPointNear(CServer* p, ULONG32 id) : CNearUnknown(p, id){};

		IMPL_INTERFACE1(IConnectionPoint);

		STDMETHOD(GetConnectionInterface)(IID *pIID);
		STDMETHOD(GetConnectionPointContainer)(IConnectionPointContainer **ppCPC);
		STDMETHOD(Advise)(IUnknown *pUnkSink, DWORD *pdwCookie);
		STDMETHOD(Unadvise)(DWORD dwCookie);
		STDMETHOD(EnumConnections)(IEnumConnections **ppEnum);

	private:
		CConnectionPointNear();
		CConnectionPointNear &operator =(const CConnectionPointNear&);
		CConnectionPointNear(const CConnectionPointNear&);
	};

	typedef CComPtrNear < IEnumConnectionPoints, CComEnumUnkNear < IEnumConnectionPoints
		, IConnectionPoint, CConnectionPointNear >> CComPtrNearEnumCP;
}