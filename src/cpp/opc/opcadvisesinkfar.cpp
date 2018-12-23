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
#include "opcadvisesinkfar.h"
#include "error/baseexception.h"
#include "stream/stream.h"
#include "trace/trace.h"
#include "com/comstring.h"
#include "opc/opcadvisesinknear.h"
#include "opc/opcda.h"
#include <objidl.h>

namespace NkOPC
{
	UINT COPCAdviseSinkFar::m_cfOPCSTMFORMATDATA = 0;
	UINT COPCAdviseSinkFar::m_cfOPCSTMFORMATDATATIME = 0;
	UINT COPCAdviseSinkFar::m_cfOPCSTMFORMATWRITECOMPLETE = 0;

	COPCAdviseSinkFar::COPCAdviseSinkFar(NkCom::CServer* pSrv, IUnknown* p, ULONG32* pid)
		: CComFarUnknown(pSrv, p, pid)
	{
		RegisterClipboardFormats();
	}

	void COPCAdviseSinkFar::RegisterClipboardFormats()
	{
		if (m_cfOPCSTMFORMATDATA == 0)
		{
			m_cfOPCSTMFORMATDATA = RegisterClipboardFormat(L"OPCSTMFORMATDATA");
			m_cfOPCSTMFORMATDATATIME = RegisterClipboardFormat(L"OPCSTMFORMATDATATIME");
			m_cfOPCSTMFORMATWRITECOMPLETE = RegisterClipboardFormat(L"OPCSTMFORMATWRITECOMPLETE");
		}
	}

	HRESULT COPCAdviseSinkFar::query_interface(REFIID iid)
	{
		if (iid == IID_IUnknown
			|| iid == IID_IAdviseSink){
			return internal_query_interface(iid);
		}

#ifdef NK_TRACE_QUERY_INTERFACE

		NkTrace::CTrace& Trace = NkTrace::CTrace::Instance();
		if (Trace.TraceLevel() >= NkTrace::CTrace::TraceLevel_3)
		{
			NkCom::CComString str;
			StringFromIID(iid, &str);
			Trace.Trace(NkTrace::CTrace::TraceDetailed
				, L"IAdviseSink::QueryInterface(%s) E_NOINTERFACE"
				, str.data());
		}

#endif

		return E_NOINTERFACE;
	}

	void COPCAdviseSinkFar::call(ULONG32 index, NkStream::CStream& stream)
	{
		switch (index){
		case 1: OnDataChange(stream); break;
		}
		if (NkTrace::CTrace::IsTraceLevel3()) {
			static const WCHAR* pszFctNames[] = {
				L"dummy",
				L"IAdviseSink::OnDataChange()"
			};
			if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
				_ASSERT(0);
				return;
			}
			NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
		}
	}

	void COPCAdviseSinkFar::OnDataChange(NkStream::CStream& stream)
	{
		int cfFormat;
		FORMATETC Formatetc;
		Formatetc.ptd = 0;
		Formatetc.dwAspect = DVASPECT_CONTENT;
		Formatetc.lindex = -1;
		Formatetc.tymed = TYMED_HGLOBAL;

		STGMEDIUM Stgmed;
		Stgmed.tymed = TYMED_HGLOBAL;
		Stgmed.pUnkForRelease = 0;

		stream >> cfFormat;
		if (cfFormat == COPCAdviseSinkNear::OPCSTMFORMATDATA){
			Formatetc.cfFormat = m_cfOPCSTMFORMATDATA;
		}
		else if (cfFormat == COPCAdviseSinkNear::OPCSTMFORMATDATATIME){
			Formatetc.cfFormat = m_cfOPCSTMFORMATDATATIME;
		}
		else if (cfFormat == COPCAdviseSinkNear::OPCSTMFORMATWRITECOMPLETE){
			Formatetc.cfFormat = m_cfOPCSTMFORMATWRITECOMPLETE;
		}
		else{
			NkError::CBaseException::check(E_INVALIDARG, __FILE__, __LINE__);
		}

		DWORD dwSize = stream.read_count();

		NkWin::CGlobalMem gmem;
		gmem.alloc(dwSize);
		Stgmed.hGlobal = gmem;

		byte* pby;
		gmem.lock(&pby);
		stream.read(pby, dwSize);

		NkCom::CComPtr<IAdviseSink> sp;
		internal_get_interface(sp);

		sp->OnDataChange(&Formatetc, &Stgmed);

		stream << static_cast<HRESULT>(S_OK);
		stream.flush();
	}
}