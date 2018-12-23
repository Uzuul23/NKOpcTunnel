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
#include "opc/opcda.h"

namespace NkOPC
{
	class COPCDataCallbackFar : public NkCom::CComFarUnknown
	{
	public:
		COPCDataCallbackFar(NkCom::CServer* pSrv, IUnknown* p, ULONG32* pid)
			: CComFarUnknown(pSrv, p, pid) {}

		HRESULT query_interface(REFIID iid);
		void call(ULONG32 index, NkStream::CStream& stream);

		//IOPCDataCallback

		void OnDataChange(NkStream::CStream& stream);
		void OnReadComplete(NkStream::CStream& stream);
		void OnWriteComplete(NkStream::CStream& stream);
		void OnCancelComplete(NkStream::CStream& stream);

	private:
		COPCDataCallbackFar();
		COPCDataCallbackFar &operator =(const COPCDataCallbackFar&);
		COPCDataCallbackFar(const COPCDataCallbackFar&);
	};
}