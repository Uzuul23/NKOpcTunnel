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
#include "stream/stream.h"
#include "com/comfarsrv.h"
#include "com/enumfar.h"
#include <ocidl.h>

namespace NkCom
{
	class CConnectionPointFar : public CComFarUnknown
	{
	public:
		CConnectionPointFar(CServer* pSrv, IConnectionPoint* p, ULONG32* pid = 0);
		CConnectionPointFar(NkCom::CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid);

		HRESULT query_interface(REFIID iid);
		void call(ULONG32 index, NkStream::CStream& stream);
		void GetConnectionInterface(NkStream::CStream& stream);
		void GetConnectionPointContainer(NkStream::CStream& stream);
		void Advise(NkStream::CStream& stream);
		void Unadvise(NkStream::CStream& stream);
		void EnumConnections(NkStream::CStream& stream);

	private:
		CConnectionPointFar();
		CConnectionPointFar &operator =(const CConnectionPointFar&);
		CConnectionPointFar(const CConnectionPointFar&);

		NkCom::CComPtr<IConnectionPoint> m_spConnectionPoint;
	};

	typedef NkCom::CComPtrFar < IEnumConnectionPoints
		, CComEnumUnkFar < IEnumConnectionPoints, IConnectionPoint
		, CConnectionPointFar >> CComPtrFarEnumCP;
}