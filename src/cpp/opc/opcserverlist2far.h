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
#include "com/comfarunknown.h"
#include "stream/stream.h"
#include "opc/opccomn.h"

namespace NkOPC
{
	class COPCServerList2Far : public NkCom::CComFarUnknown
	{
	public:
		COPCServerList2Far(NkCom::CServer* pSrv, REFCLSID rclsid
			, DWORD dwClsContext, REFIID riid);

		NK_UNKNOWN_IMP;

		HRESULT query_interface(REFIID iid);
		void call(ULONG32 index, NkStream::CStream& stream);
		void EnumClassesOfCategories(NkStream::CStream& stream);
		void GetClassDetails(NkStream::CStream& stream);
		void CLSIDFromProgID(NkStream::CStream& stream);

	private:
		COPCServerList2Far();
		COPCServerList2Far &operator =(const COPCServerList2Far&);
		COPCServerList2Far(const COPCServerList2Far&);
		NkCom::CComPtr<IOPCServerList2> m_spOPCServerList2;
	};
}