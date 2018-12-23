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
#include "opcshutdownfar.h"
#include "com/comstring.h"

namespace NkOPC
{

	COPCShutdownFar::COPCShutdownFar(NkCom::CServer* pSrv, IUnknown* p, ULONG32* pid)
		: CComFarUnknown(pSrv, p, pid)
	{

	}

	HRESULT COPCShutdownFar::query_interface(REFIID iid)
	{
		if (iid == IID_IUnknown
			|| iid == IID_IOPCShutdown) {
			return internal_query_interface(iid);
		}

#ifdef NK_TRACE_QUERY_INTERFACE

		NkTrace::CTrace& Trace = NkTrace::CTrace::Instance();
		if (Trace.IsTraceLevel3()) {
			NkCom::CComString str;
			StringFromIID(iid, &str);
			Trace.Trace(NkTrace::CTrace::TraceDetailed
				, L"IOPCGroup::QueryInterface(%s) E_NOINTERFACE"
				, str.data());
		}

#endif
		return E_NOINTERFACE;
	}

	void COPCShutdownFar::call(ULONG32 index, NkStream::CStream& stream)
	{
		switch (index) {
		case 1: ShutdownRequest(stream); break;
		}

		if (NkTrace::CTrace::IsTraceLevel3()) {
			static const WCHAR* pszFctNames[] = {
				L"dummy",
				L"IOPCShutdown::ShutdownRequest()"
			};
			if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
				_ASSERT(0);
				return;
			}
			NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
		}
	}

	void COPCShutdownFar::ShutdownRequest(NkStream::CStream& stream)
	{
		NkCom::CComString Reason;
		stream >> Reason;

		NkCom::CComPtr<IOPCShutdown> sp;
		internal_get_interface(sp);

		stream << sp->ShutdownRequest(Reason);
		stream.flush();
	}


}


