#include "stdafx.h"
#include "opcservernear.h"
#include "types/uniquepointer.h"
#include "trace/trace.h"
#include "com/unknownimpl.h"
#include "com/comstring.h"
#include "opc/opctypes.h"
#include "com/enumnear.h"
#include "opc/opcgroupnear.h"
#include "com/connectionpointnear.h"
#include "com/comptr.h"

namespace NkOPC
{
	STDMETHODIMP COPCServerNear::QueryInterface(REFIID riid, void **ppvObject)
	{
		if (ppvObject == 0) {
			return E_POINTER;
		}

		if (FAILED(remote_query_interface(riid))) {
			return E_NOINTERFACE;
		}

		if (riid == IID_IUnknown) {
			(*ppvObject) = static_cast<IOPCCommon*>(this);
		}
		else if (riid == IID_IOPCCommon) {
			(*ppvObject) = static_cast<IOPCCommon*>(this);
		}
		else if (riid == IID_IOPCServer) {
			(*ppvObject) = static_cast<IOPCServer*>(this);
		}
		else if (riid == IID_IConnectionPointContainer) {
			(*ppvObject) = static_cast<IConnectionPointContainer*>(this);
		}
		else if (riid == IID_IOPCBrowse) {
			(*ppvObject) = static_cast<IOPCBrowse*>(this);
		}
		else if (riid == IID_IOPCItemIO) {
			(*ppvObject) = static_cast<IOPCItemIO*>(this);
		}
		else if (riid == IID_IOPCItemProperties) {
			(*ppvObject) = static_cast<IOPCItemProperties*>(this);
		}
		else if (riid == IID_IOPCServerPublicGroups) {
			(*ppvObject) = static_cast<IOPCServerPublicGroups*>(this);
		}
		else if (riid == IID_IOPCBrowseServerAddressSpace) {
			(*ppvObject) = static_cast<IOPCBrowseServerAddressSpace*>(this);
		}
		else {
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	//IOPCCommon

	STDMETHODIMP COPCServerNear::SetLocaleID( /* [in] */ LCID dwLcid)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 1, L"IOPCCommon::SetLocaleID()");
			srv.stream << dwLcid;
			srv.stream.flush();
			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::GetLocaleID( /* [out] */ LCID *pdwLcid)
	{
		try {
			NkError::CBaseException::check_arg(pdwLcid, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 2, L"IOPCCommon::GetLocaleID()");

			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				srv.stream >> *pdwLcid;
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::QueryAvailableLocaleIDs( /* [out] */ DWORD *pdwCount
		, /* [size_is][size_is][out] */ LCID **pdwLcid)
	{
		try {
			NkError::CBaseException::check_arg(pdwCount, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwLcid, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 3, L"IOPCCommon::QueryAvailableLocaleIDs()");

			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CVector<LCID> dwLcids;
				srv.stream >> dwLcids;
				dwLcids.detach(pdwLcid, pdwCount);
			}
			NkTrace::CTrace::TraceDet(L"IOPCCommon::QueryAvailableLocaleIDs(%d)", *pdwCount);
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::GetErrorString( /* [in] */ HRESULT dwError
		, /* [string][out] */ LPWSTR *ppString)
	{
		try {
			NkError::CBaseException::check_arg(ppString, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 4, L"IOPCCommon::GetErrorString()");

			srv.stream << dwError;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CComString strString;
				srv.stream >> strString;
				strString.detach(ppString);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::SetClientName( /* [string][in] */ LPCWSTR szName)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 5, L"IOPCCommon::SetClientName()");

			srv.stream << NkCom::CComString(szName);
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	//IOPCServer

	STDMETHODIMP COPCServerNear::GetErrorString( /* [in] */ HRESULT dwError
		, /* [in] */ LCID dwLocale
		, /* [string][out] */ LPWSTR *ppString)
	{
		try {
			NkError::CBaseException::check_arg(ppString, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 7, L"IOPCServer::GetErrorString()");

			srv.stream << dwError;
			srv.stream << dwLocale;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CComString strString;
				srv.stream >> strString;
				strString.detach(ppString);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::AddGroup( /* [string][in] */ LPCWSTR szName
		, /* [in] */ BOOL bActive
		, /* [in] */ DWORD dwRequestedUpdateRate
		, /* [in] */ OPCHANDLE hClientGroup
		, /* [in][unique] */ LONG *pTimeBias
		, /* [in][unique] */ FLOAT *pPercentDeadband
		, /* [in] */ DWORD dwLCID
		, /* [out] */ OPCHANDLE *phServerGroup
		, /* [out] */ DWORD *pRevisedUpdateRate
		, /* [in] */ REFIID riid
		, /* [iid_is][out] */ LPUNKNOWN *ppUnk)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 6, L"IOPCServer::AddGroup()");

			srv.stream << szName;
			srv.stream << bActive;
			srv.stream << dwRequestedUpdateRate;
			srv.stream << hClientGroup;
			srv.stream << NkType::CUniquePointer<LONG>(pTimeBias);
			srv.stream << NkType::CUniquePointer<FLOAT>(pPercentDeadband);;
			srv.stream << dwLCID;
			srv.stream << riid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				srv.stream >> *phServerGroup;
				srv.stream >> *pRevisedUpdateRate;

				NkCom::CComPtrNear<IUnknown, COPCGroupNear> spGroup(m_pSrv);
				srv.stream >> spGroup;
				spGroup.Detach(ppUnk);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::GetGroupByName(
		/* [string][in] */ LPCWSTR szName
		, /* [in] */ REFIID riid
		, /* [iid_is][out] */ LPUNKNOWN *ppUnk)
	{
		try {
			NkError::CBaseException::check_arg(ppUnk, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 8, L"IOPCServer::AddGroup()");
			srv.stream << szName;
			srv.stream << riid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CComPtrNear<IUnknown, COPCGroupNear> spGroup(m_pSrv);
				srv.stream >> spGroup;
				spGroup.Detach(ppUnk);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::GetStatus( /* [out] */ OPCSERVERSTATUS **ppServerStatus)
	{
		try {
			NkError::CBaseException::check_arg(ppServerStatus, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 9, L"IOPCServer::GetStatus()");

			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkOPC::COPCServerStatus spServerStatus;
				srv.stream >> spServerStatus;
				spServerStatus.Detach(ppServerStatus);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::RemoveGroup( /* [in] */ OPCHANDLE hServerGroup
		, /* [in] */ BOOL bForce)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 10, L"IOPCServer::RemoveGroup()");

			srv.stream << hServerGroup;
			srv.stream << bForce;
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::CreateGroupEnumerator( /* [in] */ OPCENUMSCOPE dwScope
		, /* [in] */ REFIID riid
		, /* [iid_is][out] */ LPUNKNOWN *ppUnk)
	{
		try {
			NkError::CBaseException::check_arg(ppUnk, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 11, L"IOPCServer::CreateGroupEnumerator()");

			srv.stream << dwScope;
			srv.stream << riid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				if (riid == IID_IEnumUnknown) {
					CComPtrNearEnumOPCGroups spEnum(m_pSrv);
					srv.stream >> spEnum;
					(*ppUnk) = spEnum.Detach();
				}
				else if (riid == IID_IEnumString) {
					NkCom::CComPtrNearEnumString spEnum(m_pSrv);
					srv.stream >> spEnum;
					(*ppUnk) = spEnum.Detach();
				}
				else {
					return E_NOINTERFACE;
				}
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	//IConnectionPointContainer

	STDMETHODIMP COPCServerNear::EnumConnectionPoints( /* [out] */ IEnumConnectionPoints **ppEnum)
	{
		try {
			NkError::CBaseException::check_arg(ppEnum, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 12, L"IConnectionPointContainer::EnumConnectionPoints()");

			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CComPtrNearEnumCP spEnum(m_pSrv);
				srv.stream >> spEnum;
				spEnum.Detach(ppEnum);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::FindConnectionPoint( /* [in] */ REFIID riid
		, /* [out] */ IConnectionPoint **ppCP)
	{
		try {
			NkError::CBaseException::check_arg(ppCP, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 13, L"IConnectionPointContainer::FindConnectionPoint()");
			srv.stream << riid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CComPtrNear<IConnectionPoint, NkCom::CConnectionPointNear> spCP(m_pSrv);
				srv.stream >> spCP;
				spCP.Detach(ppCP);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	//IOPCBrowse

	STDMETHODIMP COPCServerNear::GetProperties(/* [in] */ DWORD dwItemCount,
		/* [size_is][string][in] */ LPWSTR *pszItemIDs,
		/* [in] */ BOOL bReturnPropertyValues,
		/* [in] */ DWORD dwPropertyCount,
		/* [size_is][in] */ DWORD *pdwPropertyIDs,
		/* [size_is][size_is][out] */ OPCITEMPROPERTIES **ppItemProperties)
	{
		try {
			NkError::CBaseException::check_arg(pszItemIDs, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwPropertyIDs, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppItemProperties, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 14, L"IOPCBrowse::GetProperties()");

			srv.stream << NkCom::CStringVector<DWORD>(pszItemIDs, dwItemCount);
			srv.stream << bReturnPropertyValues;
			srv.stream << NkCom::CVector<DWORD, DWORD>(pdwPropertyIDs, dwPropertyCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				CVectorOPCITEMPROPERTIES ItemProperties;
				srv.stream >> ItemProperties;
				ItemProperties.detach(ppItemProperties, pdwPropertyIDs);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}

		return E_NOTIMPL;
	}

	STDMETHODIMP COPCServerNear::Browse(
		/* [string][in] */ LPWSTR szItemID,
		/* [string][out][in] */ LPWSTR *pszContinuationPoint,
		/* [in] */ DWORD dwMaxElementsReturned,
		/* [in] */ OPCBROWSEFILTER dwBrowseFilter,
		/* [string][in] */ LPWSTR szElementNameFilter,
		/* [string][in] */ LPWSTR szVendorFilter,
		/* [in] */ BOOL bReturnAllProperties,
		/* [in] */ BOOL bReturnPropertyValues,
		/* [in] */ DWORD dwPropertyCount,
		/* [size_is][in] */ DWORD *pdwPropertyIDs,
		/* [out] */ BOOL *pbMoreElements,
		/* [out] */ DWORD *pdwCount,
		/* [size_is][size_is][out] */ OPCBROWSEELEMENT **ppBrowseElements)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 15, L"IOPCBrowse::Browse()");

			NkError::CBaseException::check_arg(szItemID, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pszContinuationPoint, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(szElementNameFilter, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(szVendorFilter, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwPropertyIDs, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pbMoreElements, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwCount, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppBrowseElements, __FILE__, __LINE__);
			
			srv.stream << szItemID;
			srv.stream << *pszContinuationPoint;
			srv.stream << dwMaxElementsReturned;
			srv.stream << dwBrowseFilter;
			srv.stream << szElementNameFilter;
			srv.stream << szVendorFilter;
			srv.stream << bReturnAllProperties;
			srv.stream << bReturnPropertyValues;
			srv.stream << NkCom::CVector<DWORD>(pdwPropertyIDs, dwPropertyCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result))
			{
				NkCom::CComString ContinuationPoint;
				CVectorOPCBROWSEELEMENT BrowseElements;
				srv.stream >> ContinuationPoint;
				srv.stream >> (*pbMoreElements);
				srv.stream >> BrowseElements;
				BrowseElements.detach(ppBrowseElements, pdwCount);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	//IOPCItemIO

	STDMETHODIMP COPCServerNear::Read( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ LPCWSTR *pszItemIDs
		, /* [size_is][in] */ DWORD *pdwMaxAge
		, /* [size_is][size_is][out] */ VARIANT **ppvValues
		, /* [size_is][size_is][out] */ WORD **ppwQualities
		, /* [size_is][size_is][out] */ FILETIME **ppftTimeStamps
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try {
			NkError::CBaseException::check_arg(pszItemIDs, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwMaxAge, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppvValues, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppwQualities, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppftTimeStamps, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 16, L"IOPCItemIO::Read()");
			srv.stream << NkCom::CStringVector<DWORD>(pszItemIDs, dwCount);
			srv.stream << NkCom::CVector<DWORD>(pdwMaxAge, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CVariantVector<DWORD> Values;
				NkCom::CVector<WORD> Qualities;
				NkCom::CVector<FILETIME> TimeStamps;
				NkCom::CVector<HRESULT> Errors;
				srv.stream >> Values;
				srv.stream >> Qualities;
				srv.stream >> TimeStamps;
				srv.stream >> Errors;
				Values.detach(ppvValues);
				Qualities.detach(ppwQualities);
				TimeStamps.detach(ppftTimeStamps);
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::WriteVQT( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ LPCWSTR *pszItemIDs
		, /* [size_is][in] */ OPCITEMVQT *pItemVQT
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try {
			NkError::CBaseException::check_arg(pszItemIDs, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pItemVQT, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 17, L"IOPCItemIO::WriteVQT()");
			srv.stream << NkCom::CStringVector<DWORD>(pszItemIDs, dwCount);
			srv.stream << NkOPC::CVectorOPCITEMVQT(pItemVQT, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	//IOPCItemProperties

	STDMETHODIMP COPCServerNear::QueryAvailableProperties( /* [in] */ LPWSTR szItemID
		, /* [out] */ DWORD *pdwCount
		, /* [size_is][size_is][out] */ DWORD **ppPropertyIDs
		, /* [size_is][size_is][out] */ LPWSTR **ppDescriptions
		, /* [size_is][size_is][out] */ VARTYPE **ppvtDataTypes)
	{
		try {
			NkError::CBaseException::check_arg(ppPropertyIDs, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppDescriptions, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppvtDataTypes, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 18, L"IOPCItemProperties::QueryAvailableProperties()");
			srv.stream << szItemID;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CVector<DWORD, DWORD> PropertyIDs;
				NkCom::CStringVector<DWORD> Descriptions;
				NkCom::CVector<VARTYPE, DWORD> vtDataTypes;
				srv.stream >> PropertyIDs;
				srv.stream >> Descriptions;
				srv.stream >> vtDataTypes;
				PropertyIDs.detach(ppPropertyIDs, pdwCount);
				Descriptions.detach(ppDescriptions);
				vtDataTypes.detach(ppvtDataTypes);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::GetItemProperties( /* [in] */ LPWSTR szItemID
		, /* [in] */ DWORD dwCount, /* [size_is][in] */ DWORD *pdwPropertyIDs
		, /* [size_is][size_is][out] */ VARIANT **ppvData
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try {
			NkError::CBaseException::check_arg(pdwPropertyIDs, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppvData, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 19, L"IOPCItemProperties::QueryAvailableProperties()");
			srv.stream << szItemID;
			srv.stream << NkCom::CVector<DWORD, DWORD>(pdwPropertyIDs, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CVariantVector<DWORD> vData;
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> vData;
				srv.stream >> Errors;
				vData.detach(ppvData);
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::LookupItemIDs( /* [in] */ LPWSTR szItemID
		, /* [in] */ DWORD dwCount
		, /* [size_is][in] */ DWORD *pdwPropertyIDs
		, /* [size_is][size_is][string][out] */ LPWSTR **ppszNewItemIDs
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try {
			NkError::CBaseException::check_arg(pdwPropertyIDs, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppszNewItemIDs, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 20, L"IOPCItemProperties::QueryAvailableProperties()");
			srv.stream << szItemID;
			srv.stream << NkCom::CVector<DWORD, DWORD>(pdwPropertyIDs, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CStringVector<DWORD> szNewItemIDs;
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> szNewItemIDs;
				srv.stream >> Errors;
				szNewItemIDs.detach(ppszNewItemIDs);
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	//IOPCServerPublicGroups

	STDMETHODIMP COPCServerNear::GetPublicGroupByName( /* [string][in] */ LPCWSTR szName
		, /* [in] */ REFIID riid
		, /* [iid_is][out] */ LPUNKNOWN *ppUnk)
	{
		try {
			NkError::CBaseException::check_arg(ppUnk, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 21, L"IOPCServerPublicGroups::GetPublicGroupByName()");
			srv.stream << riid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CComPtrNear<IUnknown, COPCGroupNear> sp(m_pSrv);
				srv.stream >> sp;
				sp.Detach(ppUnk);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::RemovePublicGroup( /* [in] */ OPCHANDLE hServerGroup
		, /* [in] */ BOOL bForce)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 22, L"IOPCServerPublicGroups::RemovePublicGroup()");
			srv.stream << hServerGroup;
			srv.stream << bForce;
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	//IOPCBrowseServerAddressSpace

	STDMETHODIMP COPCServerNear::QueryOrganization( /* [out] */ OPCNAMESPACETYPE *pNameSpaceType)
	{
		try {
			NkError::CBaseException::check_arg(pNameSpaceType, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 23, L"IOPCBrowseServerAddressSpace::QueryOrganization()");
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				srv.stream >> *pNameSpaceType;
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::ChangeBrowsePosition( /* [in] */ OPCBROWSEDIRECTION dwBrowseDirection
		, /* [string][in] */ LPCWSTR szString)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 24, L"IOPCBrowseServerAddressSpace::ChangeBrowsePosition()");
			srv.stream << dwBrowseDirection;
			srv.stream << szString;
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::BrowseOPCItemIDs( /* [in] */ OPCBROWSETYPE dwBrowseFilterType
		, /* [string][in] */ LPCWSTR szFilterCriteria
		, /* [in] */ VARTYPE vtDataTypeFilter
		, /* [in] */ DWORD dwAccessRightsFilter
		, /* [out] */ LPENUMSTRING *ppIEnumString)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 25, L"IOPCBrowseServerAddressSpace::BrowseOPCItemIDs()");
			srv.stream << dwBrowseFilterType;
			srv.stream << szFilterCriteria;
			srv.stream << vtDataTypeFilter;
			srv.stream << dwAccessRightsFilter;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CComPtrNearEnumString spEnum(m_pSrv);
				srv.stream >> spEnum;
				spEnum.Detach(ppIEnumString);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::GetItemID( /* [in] */ LPWSTR szItemDataID
		, /* [string][out] */ LPWSTR *szItemID)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 26, L"IOPCBrowseServerAddressSpace::GetItemID()");
			srv.stream << szItemDataID;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CComPtrNearEnumString spEnum(m_pSrv);
				NkCom::CComString ItemId;
				srv.stream >> ItemId;
				ItemId.detach(szItemID);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCServerNear::BrowseAccessPaths( /* [string][in] */ LPCWSTR szItemID
		, /* [out] */ LPENUMSTRING *ppIEnumString)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 27, L"IOPCBrowseServerAddressSpace::BrowseAccessPaths()");
			srv.stream << szItemID;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CComPtrNearEnumString spEnum(m_pSrv);
				srv.stream >> spEnum;
				spEnum.Detach(ppIEnumString);
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			return handle_error(e);
		}
	}
}