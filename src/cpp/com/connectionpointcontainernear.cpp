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
#include "com/connectionpointnear.h"
#include "com/connectionpointcontainernear.h"

namespace NkCom
{
	STDMETHODIMP CConnectionPointContainerNear::EnumConnectionPoints(
		/* [out] */ IEnumConnectionPoints **ppEnum)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 1, L"CConnectionPointContainer::EnumConnectionPoints()");

			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				CComPtrNearEnumCP spEnum(m_pSrv);
				srv.stream >> spEnum;
				spEnum.Detach(ppEnum);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP CConnectionPointContainerNear::FindConnectionPoint(
		/* [in] */ REFIID riid, /* [out] */ IConnectionPoint **ppCP)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 2, L"CConnectionPointContainer::FindConnectionPoint()");
			srv.stream << riid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				CComPtrNear<IConnectionPoint, NkCom::CConnectionPointNear> spCP(m_pSrv);
				srv.stream >> spCP;
				spCP.Detach(ppCP);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}
}