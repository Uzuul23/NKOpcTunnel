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