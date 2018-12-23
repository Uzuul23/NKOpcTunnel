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
#include "opcdatacallbacknear.h"
#include "com/vector.h"

namespace NkOPC
{
	COPCDataCallbackNear::COPCDataCallbackNear(NkCom::CServer* p, ULONG32 id) : CNearUnknown(p, id)
	{
	}


	COPCDataCallbackNear::~COPCDataCallbackNear()
	{
	}

	STDMETHODIMP COPCDataCallbackNear::QueryInterface(REFIID riid, void **ppvObject)
	{
		if (FAILED(remote_query_interface(riid))) {
			return E_NOINTERFACE;
		}

		if (riid == IID_IUnknown) {
			(*ppvObject) = static_cast<IOPCDataCallback*>(this);
		}
		else if (riid == IID_IOPCDataCallback) {
			(*ppvObject) = static_cast<IOPCDataCallback*>(this);
		}
		else {
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	STDMETHODIMP COPCDataCallbackNear::OnDataChange(
		  /* [in] */ DWORD dwTransid
		, /* [in] */ OPCHANDLE hGroup
		, /* [in] */ HRESULT hrMasterquality
		, /* [in] */ HRESULT hrMastererror
		, /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phClientItems
		, /* [size_is][in] */ VARIANT *pvValues
		, /* [size_is][in] */ WORD *pwQualities
		, /* [size_is][in] */ FILETIME *pftTimeStamps
		, /* [size_is][in] */ HRESULT *pErrors)
	{
		try {
			NkType::CString str;
			if (NkTrace::CTrace::IsTraceLevel3()) {
				str.printf(L"IOPCDataCallback::OnDataChange(MQ: %d, ME: %d, CNT: %d)"
					, hrMasterquality, hrMastererror, dwCount);
			}
			CNearUnknown::CCallFctGuard srv(this, 1, str);

			NkError::CBaseException::check_arg(phClientItems, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pvValues, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pwQualities, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pftTimeStamps, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pErrors, __FILE__, __LINE__);

			srv.stream << dwTransid;
			srv.stream << hGroup;
			srv.stream << hrMasterquality;
			srv.stream << hrMastererror;
			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phClientItems, dwCount);
			srv.stream << NkCom::CVariantVector<DWORD>(pvValues, dwCount);
			srv.stream << NkCom::CVector<WORD, DWORD>(pwQualities, dwCount);
			srv.stream << NkCom::CVector<FILETIME, DWORD>(pftTimeStamps, dwCount);
			srv.stream << NkCom::CVector<HRESULT, DWORD>(pErrors, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e) {
			handle_error(e);
			return S_OK;
		}
	}

	STDMETHODIMP COPCDataCallbackNear::OnReadComplete(
		/* [in] */ DWORD dwTransid
		, /* [in] */ OPCHANDLE hGroup
		, /* [in] */ HRESULT hrMasterquality
		, /* [in] */ HRESULT hrMastererror
		, /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phClientItems
		, /* [size_is][in] */ VARIANT *pvValues
		, /* [size_is][in] */ WORD *pwQualities
		, /* [size_is][in] */ FILETIME *pftTimeStamps
		, /* [size_is][in] */ HRESULT *pErrors)
	{
		try {
			NkType::CString str;
			if (NkTrace::CTrace::IsTraceLevel3()) {
				str.printf(L"IOPCDataCallback::OnReadComplete(MQ: %d, ME: %d, CNT: %d)"
					, hrMasterquality, hrMastererror, dwCount);
			}
			CNearUnknown::CCallFctGuard srv(this, 2, str);

			NkError::CBaseException::check_arg(phClientItems, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pvValues, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pwQualities, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pftTimeStamps, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pErrors, __FILE__, __LINE__);

			srv.stream << dwTransid;
			srv.stream << hGroup;
			srv.stream << hrMasterquality;
			srv.stream << hrMastererror;
			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phClientItems, dwCount);
			srv.stream << NkCom::CVariantVector<DWORD>(pvValues, dwCount);
			srv.stream << NkCom::CVector<WORD, DWORD>(pwQualities, dwCount);
			srv.stream << NkCom::CVector<FILETIME, DWORD>(pftTimeStamps, dwCount);
			srv.stream << NkCom::CVector<HRESULT, DWORD>(pErrors, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e) {
			handle_error(e);
			return S_OK;
		}
	}

	STDMETHODIMP COPCDataCallbackNear::OnWriteComplete(
		DWORD dwTransid
		, /* [in] */ OPCHANDLE hGroup
		, /* [in] */ HRESULT hrMastererr
		, /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *pClienthandles
		, /* [size_is][in] */ HRESULT *pErrors)
	{
		try {
			NkType::CString str;
			if (NkTrace::CTrace::IsTraceLevel3()) {
				str.printf(L"IOPCDataCallback::OnWriteComplete(ME: %d, CNT: %d)"
					, hrMastererr, dwCount);
			}
			CNearUnknown::CCallFctGuard srv(this, 3, str);

			NkError::CBaseException::check_arg(pClienthandles, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pErrors, __FILE__, __LINE__);

			srv.stream << dwTransid;
			srv.stream << hGroup;
			srv.stream << hrMastererr;
			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(pClienthandles, dwCount);
			srv.stream << NkCom::CVector<HRESULT, DWORD>(pErrors, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e) {
			handle_error(e);
			return S_OK;
		}
	}

	STDMETHODIMP COPCDataCallbackNear::OnCancelComplete( /* [in] */ DWORD dwTransid
		, /* [in] */ OPCHANDLE hGroup)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 4, L"IOPCDataCallback::OnCancelComplete()");

			srv.stream << dwTransid;
			srv.stream << hGroup;
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