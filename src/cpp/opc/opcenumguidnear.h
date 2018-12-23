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
#include "opc/opcenum.h"
#include "com/comptr.h"

namespace NkOPC
{
	class COPCEnumGUIDNear : public NkCom::CNearUnknown<COPCEnumGUIDNear>, public IOPCEnumGUID
	{
	private:
		COPCEnumGUIDNear();
		COPCEnumGUIDNear &operator =(const COPCEnumGUIDNear&);
		COPCEnumGUIDNear(const COPCEnumGUIDNear&);

	public:
		COPCEnumGUIDNear(NkCom::CServer* p, ULONG32 id);;

		IMPL_INTERFACE1(IOPCEnumGUID);

		STDMETHOD(Next)(/* [in] */ ULONG celt,
			/* [length_is][size_is][out] */ GUID *rgelt,
			/* [out] */ ULONG *pceltFetched);

		STDMETHOD(Skip)(/* [in] */ ULONG celt);

		STDMETHOD(Reset)(void);

		STDMETHOD(Clone)(/* [out] */ IOPCEnumGUID **ppenum);
	};

	inline NkStream::CStream& operator<<(NkStream::CStream& l, const NkCom::CComPtr<IOPCEnumGUID>& r)
	{
		//r.read(l);
		return l;
	}

	inline NkStream::CStream& operator>>(NkStream::CStream& l, NkCom::CComPtr<IOPCEnumGUID>& r)
	{
		//r.write(l);
		return l;
	}
}
