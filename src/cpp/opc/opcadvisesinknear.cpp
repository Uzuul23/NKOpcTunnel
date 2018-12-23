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
#include "opcadvisesinknear.h"
#include "opc/opcda.h"
#include "windows/globalmem.h"
#include "error/baseexception.h"
#include "stream/stream.h"

namespace NkOPC
{
	UINT COPCAdviseSinkNear::m_cfOPCSTMFORMATDATA = 0;
	UINT COPCAdviseSinkNear::m_cfOPCSTMFORMATDATATIME = 0;
	UINT COPCAdviseSinkNear::m_cfOPCSTMFORMATWRITECOMPLETE = 0;

	COPCAdviseSinkNear::COPCAdviseSinkNear(NkCom::CServer* p, ULONG32 id) : CNearUnknown(p, id)
	{
		RegisterClipboardFormats();
	}

	void COPCAdviseSinkNear::RegisterClipboardFormats()
	{
		if (m_cfOPCSTMFORMATDATA == 0)
		{
			m_cfOPCSTMFORMATDATA = RegisterClipboardFormat(L"OPCSTMFORMATDATA");
			m_cfOPCSTMFORMATDATATIME = RegisterClipboardFormat(L"OPCSTMFORMATDATATIME");
			m_cfOPCSTMFORMATWRITECOMPLETE = RegisterClipboardFormat(L"OPCSTMFORMATWRITECOMPLETE");
		}
	}

	void COPCAdviseSinkNear::OnDataChange(
		/* [unique][in] */ FORMATETC *pFormatetc
		, /* [unique][in] */ STGMEDIUM *pStgmed)
	{
		try{
			NkError::CBaseException::check_arg(pFormatetc, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pStgmed, __FILE__, __LINE__);

			if (pStgmed->tymed != TYMED_HGLOBAL
				|| pStgmed->pUnkForRelease != 0
				|| pFormatetc->ptd != 0
				|| pFormatetc->dwAspect != DVASPECT_CONTENT
				|| pFormatetc->lindex != -1
				|| pFormatetc->tymed != TYMED_HGLOBAL)
			{
				NkError::CBaseException::check(E_INVALIDARG, __FILE__, __LINE__);
			}

			int cfFormat = 0;
			DWORD dwSize = 0;
			DWORD dwCount = 0;
			HRESULT hrStatus = S_OK;
			NkWin::CGlobalMem data(pStgmed->hGlobal);

			BYTE* pby = 0;
			data.lock(&pby);			

			if (pFormatetc->cfFormat == m_cfOPCSTMFORMATDATA){
				cfFormat = OPCSTMFORMATDATA;
				OPCGROUPHEADER header;
				memcpy(&header, pby, sizeof(header));
				dwSize = header.dwSize;
				dwCount = header.dwItemCount;
				hrStatus = header.hrStatus;
			}
			else if (pFormatetc->cfFormat == m_cfOPCSTMFORMATDATATIME){
				cfFormat = OPCSTMFORMATDATATIME;
				OPCGROUPHEADER header;
				memcpy(&header, pby, sizeof(header));
				dwSize = header.dwSize;
				dwCount = header.dwItemCount;
				hrStatus = header.hrStatus;
			}
			else if (pFormatetc->cfFormat == m_cfOPCSTMFORMATWRITECOMPLETE){
				cfFormat = OPCSTMFORMATWRITECOMPLETE;
				OPCGROUPHEADERWRITE header;
				memcpy(&header, pby, sizeof(header));
				dwSize = header.dwItemCount * sizeof(OPCITEMHEADERWRITE) 
					+ sizeof(OPCGROUPHEADERWRITE);
				dwCount = header.dwItemCount;
				hrStatus = header.hrStatus;
			}
			else{
				NkError::CBaseException::check(E_INVALIDARG, __FILE__, __LINE__);
			}

			NkType::CString str;
			if (NkTrace::CTrace::IsTraceLevel3()) {
				str.printf(L"IAdviseSink::OnDataChange(ME: %d, CNT: %d)", hrStatus, dwCount);
			}

			CNearUnknown::CCallFctGuard srv(this, 1, str);

			srv.stream << cfFormat;
			srv.stream.write_count(dwSize);
			srv.stream.write(pby, dwSize);
			srv.stream.flush();

			srv.stream >> srv.result;
		}
		catch (NkError::CException& e){
			handle_error(e);
		}
	}

	void COPCAdviseSinkNear::OnViewChange( /* [in] */ DWORD dwAspect
		, /* [in] */ LONG lindex)
	{
	}

	void COPCAdviseSinkNear::OnRename( /* [in] */ IMoniker *pmk)
	{
	}

	void COPCAdviseSinkNear::OnSave(void)
	{
	}

	void COPCAdviseSinkNear::OnClose(void)
	{
	}
}