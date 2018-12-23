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
#include "com/interfaces.h"
#include "com/comptr.h"
#include "com/comfarunknown.h"
#include "com/connectionpointnear.h"
#include "com/connectionpointcontainernear.h"

namespace NkCom
{
	STDMETHODIMP CConnectionPointNear::GetConnectionInterface( /* [out] */ IID *pIID)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 1, L"IConnectionPoint::GetConnectionInterface()");
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				srv.stream >> *pIID;
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP CConnectionPointNear::GetConnectionPointContainer( /* [out] */ IConnectionPointContainer **ppCPC)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 2, L"IConnectionPoint::GetConnectionPointContainer()");
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				CComPtrNearCPC spCPC(m_pSrv);
				srv.stream >> spCPC;
				spCPC.Detach(ppCPC);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP CConnectionPointNear::Advise( /* [in] */ IUnknown *pUnkSink, /* [out] */ DWORD *pdwCookie)
	{
		try{
			ULONG32 id;
			IID iid;
			HRESULT hr = GetConnectionInterface(&iid);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 3, L"IConnectionPoint::Advise()");
			CComPtr<IUnknown> sp(m_pSrv->create_far_proxy(pUnkSink, iid, id));

			srv.stream << id;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				srv.stream >> *pdwCookie;
				sp.Detach();
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP CConnectionPointNear::Unadvise( /* [in] */ DWORD dwCookie)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 4, L"IConnectionPoint::Unadvise()");
			srv.stream << dwCookie;
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP CConnectionPointNear::EnumConnections( /* [out] */ IEnumConnections **ppEnum)
	{
		NkTrace::CTrace::trace_info("client call: IConnectionPoint::EnumConnections() but not implemented");
		return E_NOTIMPL;
	}
}