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
#include "opcshutdownnear.h"
#include "types/String.h"

namespace NkOPC
{
	COPCShutdownNear::COPCShutdownNear(NkCom::CServer* p, ULONG32 id) : CNearUnknown(p, id)
	{
	}

	NkOPC::COPCShutdownNear::~COPCShutdownNear()
	{
	}

	STDMETHODIMP COPCShutdownNear::ShutdownRequest(/* [string][in] */ LPCWSTR szReason)
	{
		try {
			NkType::CString str;
			if (NkTrace::CTrace::IsTraceLevel3()) {
				str.printf(L"IOPCShutdown::ShutdownRequest(%s)", szReason);
			}
			CNearUnknown::CCallFctGuard srv(this, 1, str);
			srv.stream << szReason;
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e) {
			handle_error(e);
			return S_OK;
		}
	}
}