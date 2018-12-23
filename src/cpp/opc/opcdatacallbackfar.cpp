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
#include "opcdatacallbackfar.h"
#include "error/baseexception.h"
#include "trace/trace.h"
#include "com/comstring.h"
#include "com/vector.h"
#include "com/comptr.h"
#include "stream/stream.h"
#include "opc/opcda.h"

namespace NkOPC
{
	HRESULT COPCDataCallbackFar::query_interface(REFIID iid)
	{
		if (iid == IID_IUnknown
			|| iid == IID_IOPCDataCallback){
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

	void COPCDataCallbackFar::call(ULONG32 index, NkStream::CStream& stream)
	{
		switch (index) {
		case 1: OnDataChange(stream); break;
		case 2: OnReadComplete(stream); break;
		case 3: OnWriteComplete(stream); break;
		case 4: OnCancelComplete(stream); break;
		}

		if (NkTrace::CTrace::IsTraceLevel3()) {
			static const WCHAR* pszFctNames[] = {
				L"dummy",
				L"IOPCDataCallback::OnDataChange()",
				L"IOPCDataCallback::OnReadComplete()",
				L"IOPCDataCallback::OnWriteComplete()",
				L"IOPCDataCallback::OnCancelComplete()"
			};

			if (index > (sizeof(pszFctNames)/sizeof(WCHAR*))) {
				_ASSERT(0);
				return;
			}
			NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
		}
	}

	void COPCDataCallbackFar::OnDataChange(NkStream::CStream& stream)
	{
		DWORD dwTransid;
		OPCHANDLE hGroup;
		HRESULT hrMasterquality;
		HRESULT hrMastererror;
		NkCom::CVector<OPCHANDLE, DWORD> hClientItems;
		NkCom::CVariantVector<DWORD> vValues;
		NkCom::CVector<WORD, DWORD> wQualities;
		NkCom::CVector<FILETIME, DWORD> ftTimeStamps;
		NkCom::CVector<HRESULT, DWORD> pErrors;

		stream >> dwTransid;
		stream >> hGroup;
		stream >> hrMasterquality;
		stream >> hrMastererror;
		stream >> hClientItems;
		stream >> vValues;
		stream >> wQualities;
		stream >> ftTimeStamps;
		stream >> pErrors;

		NkCom::CComPtr<IOPCDataCallback> sp;
		internal_get_interface(sp);

		stream << sp->OnDataChange(dwTransid, hGroup, hrMasterquality, hrMasterquality
			, hClientItems.size(), hClientItems, vValues, wQualities, ftTimeStamps, pErrors);

		stream.flush();
	}

	void COPCDataCallbackFar::OnReadComplete(NkStream::CStream& stream)
	{
		DWORD dwTransid;
		OPCHANDLE hGroup;
		HRESULT hrMasterquality;
		HRESULT hrMastererror;
		NkCom::CVector<OPCHANDLE, DWORD> hClientItems;
		NkCom::CVariantVector<DWORD> vValues;
		NkCom::CVector<WORD, DWORD> wQualities;
		NkCom::CVector<FILETIME, DWORD> ftTimeStamps;
		NkCom::CVector<HRESULT, DWORD> pErrors;

		stream >> dwTransid;
		stream >> hGroup;
		stream >> hrMasterquality;
		stream >> hrMastererror;
		stream >> hClientItems;
		stream >> vValues;
		stream >> wQualities;
		stream >> ftTimeStamps;
		stream >> pErrors;

		NkCom::CComPtr<IOPCDataCallback> sp;
		internal_get_interface(sp);

		stream << sp->OnReadComplete(dwTransid, hGroup, hrMasterquality, hrMasterquality
			, hClientItems.size(), hClientItems, vValues, wQualities, ftTimeStamps, pErrors);

		stream.flush();
	}

	void COPCDataCallbackFar::OnWriteComplete(NkStream::CStream& stream)
	{
		DWORD dwTransid;
		OPCHANDLE hGroup;
		HRESULT hrMastererr;
		NkCom::CVector<OPCHANDLE, DWORD> Clienthandles;
		NkCom::CVector<HRESULT, DWORD> pErrors;

		stream >> dwTransid;
		stream >> hGroup;
		stream >> hrMastererr;
		stream >> Clienthandles;
		stream >> pErrors;

		NkCom::CComPtr<IOPCDataCallback> sp;
		internal_get_interface(sp);

		stream << sp->OnWriteComplete(dwTransid, hGroup, hrMastererr
			, Clienthandles.size(), Clienthandles, pErrors);

		stream.flush();
	}

	void COPCDataCallbackFar::OnCancelComplete(NkStream::CStream& stream)
	{
		DWORD dwTransid;
		OPCHANDLE hGroup;

		stream >> dwTransid;
		stream >> hGroup;

		NkCom::CComPtr<IOPCDataCallback> sp;
		internal_get_interface(sp);

		stream << sp->OnCancelComplete(dwTransid, hGroup);
		stream.flush();
	}
}