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

#pragma once
#include "com/nearunknown.h"
#include "opc/opccomn.h"
#include "opc/opcenum.h"
#include "opc/opcda.h"

namespace NkOPC
{
	class COPCServerNear : public NkCom::CNearUnknown < COPCServerNear >
		, public IOPCCommon
		, public IOPCServer
		, public IConnectionPointContainer
		, public IOPCBrowse
		, public IOPCItemIO
		, public IOPCItemProperties
		, public IOPCServerPublicGroups
		, public IOPCBrowseServerAddressSpace
	{
	public:
		COPCServerNear(NkCom::CServer* p, ULONG32 id) : CNearUnknown(p, id){};

		IMPL_ADDREF_RELEASE;
		STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject);

		//IOPCCommon

		STDMETHOD(SetLocaleID)(LCID dwLcid);
		STDMETHOD(GetLocaleID)(LCID *pdwLcid);
		STDMETHOD(QueryAvailableLocaleIDs)(DWORD *pdwCount, LCID **pdwLcid);
		STDMETHOD(GetErrorString)(HRESULT dwError, LPWSTR *ppString);
		STDMETHOD(SetClientName)(LPCWSTR szName);

		//IOPCServer

		STDMETHOD(AddGroup)(LPCWSTR szName, BOOL bActive
			, DWORD dwRequestedUpdateRate, OPCHANDLE hClientGroup
			, LONG *pTimeBias, FLOAT *pPercentDeadband, DWORD dwLCID
			, OPCHANDLE *phServerGroup, DWORD *pRevisedUpdateRate
			, REFIID riid, LPUNKNOWN *ppUnk);
		STDMETHOD(GetErrorString)(HRESULT dwError, LCID dwLocale, LPWSTR *ppString);
		STDMETHOD(GetGroupByName)(LPCWSTR szName, REFIID riid, LPUNKNOWN *ppUnk);
		STDMETHOD(GetStatus)(OPCSERVERSTATUS **ppServerStatus);
		STDMETHOD(RemoveGroup)(OPCHANDLE hServerGroup, BOOL bForce);
		STDMETHOD(CreateGroupEnumerator)(OPCENUMSCOPE dwScope, REFIID riid, LPUNKNOWN *ppUnk);

		//IConnectionPointContainer

		STDMETHOD(EnumConnectionPoints)(IEnumConnectionPoints **ppEnum);
		STDMETHOD(FindConnectionPoint)(REFIID riid, IConnectionPoint **ppCP);

		//IOPCBrowse

		STDMETHOD(GetProperties)(DWORD dwItemCount, LPWSTR *pszItemIDs
			, BOOL bReturnPropertyValues, DWORD dwPropertyCount, DWORD *pdwPropertyIDs
			, OPCITEMPROPERTIES **ppItemProperties);
		STDMETHOD(Browse)(LPWSTR szItemID, LPWSTR *pszContinuationPoint
			, DWORD dwMaxElementsReturned, OPCBROWSEFILTER dwBrowseFilter
			, LPWSTR szElementNameFilter, LPWSTR szVendorFilter
			, BOOL bReturnAllProperties, BOOL bReturnPropertyValues
			, DWORD dwPropertyCount, DWORD *pdwPropertyIDs
			, BOOL *pbMoreElements, DWORD *pdwCount, OPCBROWSEELEMENT **ppBrowseElements);

		//IOPCItemIO

		STDMETHOD(Read)(DWORD dwCount, LPCWSTR *pszItemIDs, DWORD *pdwMaxAge
			, VARIANT **ppvValues, WORD **ppwQualities, FILETIME **ppftTimeStamps
			, HRESULT **ppErrors);
		STDMETHOD(WriteVQT)(DWORD dwCount, LPCWSTR *pszItemIDs, OPCITEMVQT *pItemVQT
			, HRESULT **ppErrors);

		//IOPCItemProperties

		STDMETHOD(QueryAvailableProperties)(LPWSTR szItemID, DWORD *pdwCount
			, DWORD **ppPropertyIDs, LPWSTR **ppDescriptions, VARTYPE **ppvtDataTypes);
		STDMETHOD(GetItemProperties)(LPWSTR szItemID, DWORD dwCount
			, DWORD *pdwPropertyIDs, VARIANT **ppvData, HRESULT **ppErrors);
		STDMETHOD(LookupItemIDs)(LPWSTR szItemID, DWORD dwCount, DWORD *pdwPropertyIDs
			, LPWSTR **ppszNewItemIDs, HRESULT **ppErrors);

		//IOPCServerPublicGroups

		STDMETHOD(GetPublicGroupByName)(LPCWSTR szName, REFIID riid, LPUNKNOWN *ppUnk);
		STDMETHOD(RemovePublicGroup)(OPCHANDLE hServerGroup, BOOL bForce);

		//IOPCBrowseServerAddressSpace

		STDMETHOD(QueryOrganization)(OPCNAMESPACETYPE *pNameSpaceType);
		STDMETHOD(ChangeBrowsePosition)(OPCBROWSEDIRECTION dwBrowseDirection
			, LPCWSTR szString);
		STDMETHOD(BrowseOPCItemIDs)(OPCBROWSETYPE dwBrowseFilterType
			, LPCWSTR szFilterCriteria, VARTYPE vtDataTypeFilter
			, DWORD dwAccessRightsFilter, LPENUMSTRING *ppIEnumString);
		STDMETHOD(GetItemID)(LPWSTR szItemDataID, LPWSTR *szItemID);
		STDMETHOD(BrowseAccessPaths)(LPCWSTR szItemID, LPENUMSTRING *ppIEnumString);
	};
}