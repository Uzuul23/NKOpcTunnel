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
#include "opc/opccomn.h"
#include "com/nearunknown.h"
#include "opc/opcenum.h"

namespace NkOPC
{
	class COPCServerList2Near : public NkCom::CNearUnknown<COPCServerList2Near>
		, public IOPCServerList2
	{
	public:
		COPCServerList2Near(NkCom::CServer* p, ULONG32 id);

		IMPL_INTERFACE1(IOPCServerList2);

		STDMETHOD(EnumClassesOfCategories)(
			/* [in] */ ULONG cImplemented,
			/* [size_is][in] */ CATID rgcatidImpl[],
			/* [in] */ ULONG cRequired,
			/* [size_is][in] */ CATID rgcatidReq[],
			/* [out] */ IOPCEnumGUID **ppenumClsid);
		STDMETHOD(GetClassDetails)(
			/* [in] */ REFCLSID clsid,
			/* [out] */ LPOLESTR *ppszProgID,
			/* [out] */ LPOLESTR *ppszUserType,
			/* [out] */ LPOLESTR *ppszVerIndProgID);
		STDMETHOD(CLSIDFromProgID)(
			/* [in] */ LPCOLESTR szProgId,
			/* [out] */ LPCLSID clsid);

	private:
		COPCServerList2Near();
		COPCServerList2Near &operator =(const COPCServerList2Near&);
		COPCServerList2Near(const COPCServerList2Near&);
	};
}