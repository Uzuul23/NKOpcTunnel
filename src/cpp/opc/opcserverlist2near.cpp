#include "stdafx.h"
#include "opcserverlist2near.h"
#include "types/conversion.h"
#include "error/baseexception.h"
#include "com/comptr.h"
#include "com/vector.h"
#include "com/enum.h"

namespace NkOPC
{
	COPCServerList2Near::COPCServerList2Near(NkCom::CServer* p, ULONG32 id)
		: CNearUnknown(p, id)
	{
	}

	STDMETHODIMP COPCServerList2Near::EnumClassesOfCategories(/* [in] */ ULONG cImplemented
		, /* [size_is][in] */ CATID rgcatidImpl[]
		, /* [in] */ ULONG cRequired
		, /* [size_is][in] */ CATID rgcatidReq[]
		, /* [out] */ IOPCEnumGUID **ppenumClsid)
	{
		try
		{
			NkType::zero(ppenumClsid);
			NkError::CBaseException::check_arg(ppenumClsid, __FILE__, __LINE__);
			CNearUnknown::CCallFctGuard srv(this, 1, L"IOPCServerList2Near::EnumClassesOfCategories()");

			srv.stream << NkCom::CVector<CATID>(rgcatidImpl, cImplemented);
			srv.stream << NkCom::CVector<CATID>(rgcatidReq, cRequired);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CVector<GUID> guids;
				srv.stream >> guids;

				NkCom::CComEnum<IOPCEnumGUID, GUID>* p_enum_guids = 0;
				NkCom::CComEnum<IOPCEnumGUID, GUID>::create(&p_enum_guids);
				p_enum_guids->take_ownership(guids);
				p_enum_guids->AddRef();
				
				(*ppenumClsid) = p_enum_guids;
			}
			return srv.result;
		}
		catch (NkError::CException& e)
		{
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerList2Near::GetClassDetails(/* [in] */ REFCLSID clsid
		, /* [out] */ LPOLESTR *ppszProgID
		, /* [out] */ LPOLESTR *ppszUserType
		, /* [out] */ LPOLESTR *ppszVerIndProgID)
	{
		try {
			NkType::zero(ppszProgID);
			NkType::zero(ppszUserType);
			NkType::zero(ppszVerIndProgID);
			NkError::CBaseException::check_arg(ppszProgID, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppszUserType, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppszVerIndProgID, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 2, L"IOPCServerList2Near::GetClassDetails()");

			srv.stream << clsid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CComString strProgID;
				NkCom::CComString strUserType;
				NkCom::CComString strVerIndProgID;

				srv.stream >> strProgID;
				srv.stream >> strUserType;
				srv.stream >> strVerIndProgID;

				strProgID.detach(ppszProgID);
				strUserType.detach(ppszUserType);
				strVerIndProgID.detach(ppszVerIndProgID);
			}
			return srv.result;
		}
		catch (NkError::CException& e)
		{
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerList2Near::CLSIDFromProgID(/* [in] */ LPCOLESTR szProgId
		, /* [out] */ LPCLSID clsid)
	{
		try {
			NkError::CBaseException::check_arg(clsid, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 3, L"IOPCServerList2Near::CLSIDFromProgID()");
			srv.stream << NkCom::CComString(szProgId);
			srv.stream.flush();
			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				srv.stream >> (*clsid);
			}
			return srv.result;
		}
		catch (NkError::CException& e)
		{
			return handle_error(e);
		}
	}
}