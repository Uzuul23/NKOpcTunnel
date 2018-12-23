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
#include "opcserverfar.h"
#include "com/connectionpointfar.h"
#include "com/enumfar.h"
#include "opc/opctypes.h"
#include "opc/opcgroupfar.h"
#include "types/uniquepointer.h"
#include <list>

namespace NkOPC
{
	class COPCServerFar::CImpl
	{
	public:
		std::list<OPCHANDLE> m_server_group_handles;
	};

	COPCServerFar::COPCServerFar(NkCom::CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid)
		: CComFarUnknown(pSrv, rclsid, dwClsContext, riid)
		, _Impl(new CImpl)
	{
		NkError::CBaseException::check_result(query_interface(riid), __FILE__, __LINE__);
	}

	COPCServerFar::COPCServerFar(NkCom::CServer* pSrv, IUnknown* p)
		: CComFarUnknown(pSrv, p)
		, _Impl(new CImpl)
	{
	}

	COPCServerFar::~COPCServerFar()
	{
		NkCom::CComPtr<IOPCServer> sp;
		internal_get_interface(sp);

		for (auto i : _Impl->m_server_group_handles) {
			HRESULT hr = sp->RemoveGroup(i, TRUE);
			_ASSERT(SUCCEEDED(hr));
		}
		delete _Impl;
	}

	HRESULT COPCServerFar::query_interface(REFIID iid)
	{
		if (iid == IID_IUnknown
			|| iid == IID_IOPCCommon
			|| iid == IID_IOPCServer
			|| iid == IID_IConnectionPointContainer
			|| iid == IID_IOPCBrowse
			|| iid == IID_IOPCItemIO
			|| iid == IID_IOPCItemProperties
			|| iid == IID_IOPCServerPublicGroups
			|| iid == IID_IOPCBrowseServerAddressSpace) {
			return internal_query_interface(iid);
		}

#ifdef NK_TRACE_QUERY_INTERFACE

		NkTrace::CTrace& Trace = NkTrace::CTrace::Instance();
		if (Trace.IsTraceLevel3()) {
			NkCom::CComString str;
			StringFromIID(iid, &str);
			Trace.Trace(NkTrace::CTrace::TraceDetailed
				, L"IOPCServer::QueryInterface(%s) E_NOINTERFACE"
				, str.data());
		}

#endif

		return E_NOINTERFACE;
	}

	void COPCServerFar::call(ULONG32 index, NkStream::CStream& stream)
	{
		HRESULT hr = S_OK;

		switch (index) {
		case 1: SetLocaleID(stream); break;
		case 2: GetLocaleID(stream); break;
		case 3: QueryAvailableLocaleIDs(stream); break;
		case 4: GetErrorString(stream); break;
		case 5: SetClientName(stream); break;
		case 6: AddGroup(stream); break;
		case 7: GetErrorString2(stream); break;
		case 8: GetGroupByName(stream); break;
		case 9: GetStatus(stream); break;
		case 10: RemoveGroup(stream); break;
		case 11: CreateGroupEnumerator(stream); break;
		case 12: EnumConnectionPoints(stream); break;
		case 13: FindConnectionPoint(stream); break;
		case 14: GetProperties(stream); break;
		case 15: Browse(stream); break;
		case 16: Read(stream); break;
		case 17: WriteVQT(stream); break;
		case 18: QueryAvailableProperties(stream); break;
		case 19: GetItemProperties(stream); break;
		case 20: LookupItemIDs(stream); break;
		case 21: GetPublicGroupByName(stream); break;
		case 22: RemovePublicGroup(stream); break;
		case 23: QueryOrganization(stream); break;
		case 24: ChangeBrowsePosition(stream); break;
		case 25: BrowseOPCItemIDs(stream); break;
		case 26: GetItemID(stream); break;
		case 27: BrowseAccessPaths(stream); break;
		}

		if (NkTrace::CTrace::IsTraceLevel3()) {
			static const WCHAR* pszFctNames[] = {
				L"dummy",
				L"IOPCCommon::SetLocaleID()",
				L"IOPCCommon::GetLocaleID()",
				L"IOPCCommon::QueryAvailableLocaleIDs()",
				L"IOPCCommon::GetErrorString()",
				L"IOPCCommon::SetClientName()",
				L"IOPCServer::AddGroup()",
				L"IOPCServer::GetErrorString()",
				L"IOPCServer::GetGroupByName()",
				L"IOPCServer::GetStatus()",
				L"IOPCServer::RemoveGroup()",
				L"IOPCServer::CreateGroupEnumerator()",
				L"IConnectionPointContainer::EnumConnectionPoints()",
				L"IConnectionPointContainer::FindConnectionPoint()",
				L"IOPCBrowse::GetProperties()",
				L"IOPCBrowse::Browse()",
				L"IOPCItemIO::Read()",
				L"IOPCItemIO::WriteVQT()",
				L"IOPCItemProperties::QueryAvailableProperties()",
				L"IOPCItemProperties::GetItemProperties()",
				L"IOPCItemProperties::LookupItemIDs()",
				L"IOPCServerPublicGroups::GetPublicGroupByName()",
				L"IOPCServerPublicGroups::RemovePublicGroup()",
				L"IOPCBrowseServerAddressSpace::QueryOrganization()",
				L"IOPCBrowseServerAddressSpace::ChangeBrowsePosition()",
				L"IOPCBrowseServerAddressSpace::BrowseOPCItemIDs()",
				L"IOPCBrowseServerAddressSpace::GetItemID()",
				L"IOPCBrowseServerAddressSpace::BrowseAccessPaths()"
			};
			if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
				_ASSERT(0);
				return;
			}
			NkTrace::CTrace::TraceDet(L"remote call: %s result(0x%x)"
				, pszFctNames[index], hr);
		}
	}

	// IOPCCommon

	void COPCServerFar::SetLocaleID(NkStream::CStream& stream)
	{
		LCID dwLcid;
		stream >> dwLcid;

		NkCom::CComPtr<IOPCCommon> sp;
		internal_get_interface(sp);

		stream << sp->SetLocaleID(dwLcid);
		stream.flush();
	}

	void COPCServerFar::GetLocaleID(NkStream::CStream& stream)
	{
		LCID dwLcid;

		NkCom::CComPtr<IOPCCommon> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetLocaleID(&dwLcid);
		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << dwLcid;
		}
		stream.flush();
	}

	void COPCServerFar::QueryAvailableLocaleIDs(NkStream::CStream& stream)
	{
		NkCom::CVector<LCID> dwLcids;

		NkCom::CComPtr<IOPCCommon> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->QueryAvailableLocaleIDs(dwLcids.size_ptr(), &dwLcids);
		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << dwLcids;
		}
		stream.flush();
	}

	void COPCServerFar::GetErrorString(NkStream::CStream& stream)
	{
		HRESULT dwError;
		NkCom::CComString strString;
		stream >> dwError;

		NkCom::CComPtr<IOPCCommon> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetErrorString(dwError, &strString);
		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << strString;
		}
		stream.flush();
	}

	void COPCServerFar::SetClientName(NkStream::CStream& stream)
	{
		NkCom::CComString strName;
		stream >> strName;

		NkCom::CComPtr<IOPCCommon> sp;
		internal_get_interface(sp);

		stream << sp->SetClientName(strName);
		stream.flush();
	}

	//IOPCServer

	void COPCServerFar::AddGroup(NkStream::CStream& stream)
	{
		NkCom::CComString strName;
		BOOL bActive;
		DWORD dwRequestedUpdateRate;
		OPCHANDLE hClientGroup;
		NkType::CUniquePointer<LONG> pTimeBias;
		NkType::CUniquePointer<FLOAT> pPercentDeadband;
		DWORD dwLCID;
		OPCHANDLE hServerGroup;
		DWORD dwRevisedUpdateRate;
		IID iid;
		NkCom::CComPtr<IUnknown> spUnk;

		stream >> strName;
		stream >> bActive;
		stream >> dwRequestedUpdateRate;
		stream >> hClientGroup;
		stream >> pTimeBias;
		stream >> pPercentDeadband;
		stream >> dwLCID;
		stream >> iid;

		NkCom::CComPtr<IOPCServer> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->AddGroup(strName, bActive, dwRequestedUpdateRate
			, hClientGroup, pTimeBias, pPercentDeadband, dwLCID, &hServerGroup
			, &dwRevisedUpdateRate, iid, &spUnk);

		if (SUCCEEDED(hr)) {
			ULONG32 id;
			NkCom::CComPtr<IUnknown> spProxy;
			_Impl->m_server_group_handles.push_back(hServerGroup);
			try {
				spProxy = m_pSrv->create_far_proxy(spUnk, IID_IOPCGroupStateMgt, id);
			}
			catch (NkError::CException& e) {
				stream << static_cast<HRESULT>(E_UNEXPECTED);
				e.report();
			}
			stream << hr;
			stream << hServerGroup;
			stream << dwRevisedUpdateRate;
			stream.write_type(VT_UNKNOWN);
			stream << id;
			spProxy.Detach();
		}
		else {
			stream << hr;
		}
		stream.flush();
	}

	void COPCServerFar::GetErrorString2(NkStream::CStream& stream)
	{
		HRESULT dwError;
		LCID dwLocale;
		NkCom::CComString strString;
		stream >> dwError;
		stream >> dwLocale;

		NkCom::CComPtr<IOPCServer> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetErrorString(dwError, dwLocale, &strString);
		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << strString;
		}
		stream.flush();
	}

	void COPCServerFar::GetGroupByName(NkStream::CStream& stream)
	{
		NkCom::CComString strName;
		IID iid;
		NkCom::CComPtr<IUnknown> spUnk;

		stream >> strName;
		stream >> iid;

		NkCom::CComPtr<IOPCServer> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetGroupByName(strName, iid, &spUnk);

		if (SUCCEEDED(hr)) {
			ULONG32 id;
			NkCom::CComPtr<IUnknown> spProxy;
			try {
				spProxy = m_pSrv->create_far_proxy(spUnk, IID_IOPCGroupStateMgt, id);
			}
			catch (NkError::CException& e) {
				stream << static_cast<HRESULT>(E_UNEXPECTED);
				e.report();
			}
			stream << hr;
			stream.write_type(VT_UNKNOWN);
			stream << id;
			spProxy.Detach();
		}
		else {
			stream << hr;
		}
		stream.flush();
	}

	void COPCServerFar::GetStatus(NkStream::CStream& stream)
	{
		COPCServerStatus spServerStatus;

		NkCom::CComPtr<IOPCServer> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetStatus(&spServerStatus);
		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << spServerStatus;
		}
		stream.flush();
	}

	void COPCServerFar::RemoveGroup(NkStream::CStream& stream)
	{
		OPCHANDLE hServerGroup;
		BOOL bForce;

		stream >> hServerGroup;
		stream >> bForce;

		NkCom::CComPtr<IOPCServer> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->RemoveGroup(hServerGroup, bForce);
		_Impl->m_server_group_handles.remove(hServerGroup);

		stream << hr;
		stream.flush();
	}

	void COPCServerFar::CreateGroupEnumerator(NkStream::CStream& stream)
	{
		OPCENUMSCOPE dwScope;
		IID iid;

		stream >> dwScope;
		stream >> iid;

		NkCom::CComPtr<IOPCServer> sp;
		internal_get_interface(sp);

		if (iid == IID_IEnumUnknown) {
			CComPtrFarEnumOPCGroups spEnum(m_pSrv);
			HRESULT hr = sp->CreateGroupEnumerator(dwScope, iid, reinterpret_cast<IUnknown**>(&spEnum));

			stream << hr;
			if (SUCCEEDED(hr)) {
				stream << spEnum;
			}
		}
		else if (iid == IID_IEnumString) {
			NkCom::CComPtrFarEnumString spEnum(m_pSrv);
			HRESULT hr = sp->CreateGroupEnumerator(dwScope, iid, reinterpret_cast<IUnknown**>(&spEnum));

			stream << hr;
			if (SUCCEEDED(hr)) {
				stream << spEnum;
			}
		}
		else {
			stream << static_cast<HRESULT>(E_NOINTERFACE);
		}
		stream.flush();
	}

	//IConnectionPointContainer

	void COPCServerFar::EnumConnectionPoints(NkStream::CStream& stream)
	{
		NkCom::CComPtrFarEnumCP spEnum(m_pSrv);

		NkCom::CComPtr<IConnectionPointContainer> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->EnumConnectionPoints(&spEnum);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << spEnum;
		}
		stream.flush();
	}

	void COPCServerFar::FindConnectionPoint(NkStream::CStream& stream)
	{
		IID iid;
		NkCom::CComPtrFar<IConnectionPoint, NkCom::CConnectionPointFar> spCP(m_pSrv);

		stream >> iid;

		NkCom::CComPtr<IConnectionPointContainer> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->FindConnectionPoint(iid, &spCP);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << spCP;
		}
		stream.flush();
	}

	//IOPCBrowse

	void COPCServerFar::GetProperties(NkStream::CStream& stream)
	{
		NkCom::CStringVector<DWORD> szItemIDs;
		BOOL bReturnPropertyValues;
		NkCom::CVector<DWORD, DWORD> dwPropertyIDs;
		CVectorOPCITEMPROPERTIES ItemProperties;

		stream >> szItemIDs;
		stream >> bReturnPropertyValues;
		stream >> dwPropertyIDs;

		NkCom::CComPtr<IOPCBrowse> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetProperties(szItemIDs.size(), szItemIDs, bReturnPropertyValues
			, dwPropertyIDs.size(), dwPropertyIDs, &ItemProperties);

		stream << hr;
		if (SUCCEEDED(hr)) {
			ItemProperties.size(szItemIDs.size());
			stream << ItemProperties;
		}
		stream.flush();
	}

	void COPCServerFar::Browse(NkStream::CStream& stream)
	{
		NkCom::CComString ItemID;
		NkCom::CComString ContinuationPoint;
		DWORD dwMaxElementsReturned;
		OPCBROWSEFILTER dwBrowseFilter;
		NkCom::CComString ElementNameFilter;
		NkCom::CComString VendorFilter;
		BOOL bReturnAllProperties;
		BOOL bReturnPropertyValues;
		NkCom::CVector<DWORD> PropertyIDs;
		BOOL bMoreElements;
		CVectorOPCBROWSEELEMENT BrowseElements;

		stream >> ItemID;
		stream >> ContinuationPoint;
		stream >> dwMaxElementsReturned;
		stream >> dwBrowseFilter;
		stream >> ElementNameFilter;
		stream >> VendorFilter;
		stream >> bReturnAllProperties;
		stream >> bReturnPropertyValues;
		stream >> PropertyIDs;

		NkCom::CComPtr<IOPCBrowse> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Browse(ItemID, ContinuationPoint.data_ptr()
			, dwMaxElementsReturned, dwBrowseFilter
			, ElementNameFilter, VendorFilter, bReturnAllProperties
			, bReturnPropertyValues, PropertyIDs.size(), PropertyIDs
			, &bMoreElements, BrowseElements.size_ptr(), &BrowseElements);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << ContinuationPoint;
			stream << bMoreElements;
			stream << BrowseElements;
		}
		stream.flush();
	}

	//IOPCItemIO

	void COPCServerFar::Read(NkStream::CStream& stream)
	{
		NkCom::CStringVector<DWORD> ItemIDs;
		NkCom::CVector<DWORD> MaxAge;
		NkCom::CVariantVector<DWORD> Values;
		NkCom::CVector<WORD> Qualities;
		NkCom::CVector<FILETIME> TimeStamps;
		NkCom::CVector<HRESULT> Errors;

		stream >> ItemIDs;
		stream >> MaxAge;

		NkCom::CComPtr<IOPCItemIO> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Read(ItemIDs.size(), ItemIDs, MaxAge
			, &Values, &Qualities, &TimeStamps, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Values.size(ItemIDs.size());
			Qualities.size(ItemIDs.size());
			TimeStamps.size(ItemIDs.size());
			Errors.size(ItemIDs.size());
			stream << Values;
			stream << Qualities;
			stream << TimeStamps;
			stream << Errors;
		}
		stream.flush();
	}

	void COPCServerFar::WriteVQT(NkStream::CStream& stream)
	{
		NkCom::CStringVector<DWORD> ItemIDs;
		NkOPC::CVectorOPCITEMVQT ItemVQT;
		NkCom::CVector<HRESULT> Errors;

		stream >> ItemIDs;
		stream >> ItemVQT;

		NkCom::CComPtr<IOPCItemIO> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->WriteVQT(ItemIDs.size(), ItemIDs
			, ItemVQT, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(ItemIDs.size());
			stream << Errors;
		}
		stream.flush();
	}

	//IOPCItemProperties

	void COPCServerFar::QueryAvailableProperties(NkStream::CStream& stream)
	{
		NkCom::CComString szItemID;
		NkCom::CVector<DWORD, DWORD> PropertyIDs;
		NkCom::CStringVector<DWORD> Descriptions;
		NkCom::CVector<VARTYPE, DWORD> vtDataTypes;

		stream >> szItemID;

		NkCom::CComPtr<IOPCItemProperties> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->QueryAvailableProperties(szItemID, PropertyIDs.size_ptr()
			, &PropertyIDs, &Descriptions, &vtDataTypes);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Descriptions.size(PropertyIDs.size());
			vtDataTypes.size(PropertyIDs.size());
			stream << PropertyIDs;
			stream << Descriptions;
			stream << vtDataTypes;
		}
		stream.flush();
	}

	void COPCServerFar::GetItemProperties(NkStream::CStream& stream)
	{
		NkCom::CComString szItemID;
		NkCom::CVector<DWORD, DWORD> PropertyIDs;
		NkCom::CVariantVector<DWORD> vData;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> szItemID;
		stream >> PropertyIDs;

		NkCom::CComPtr<IOPCItemProperties> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetItemProperties(szItemID, PropertyIDs.size()
			, PropertyIDs, &vData, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			vData.size(PropertyIDs.size());
			Errors.size(PropertyIDs.size());
			stream << vData;
			stream << Errors;
		}
		stream.flush();
	}

	void COPCServerFar::LookupItemIDs(NkStream::CStream& stream)
	{
		NkCom::CComString szItemID;
		NkCom::CVector<DWORD, DWORD> PropertyIDs;
		NkCom::CStringVector<DWORD> szNewItemIDs;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> szItemID;
		stream >> PropertyIDs;

		NkCom::CComPtr<IOPCItemProperties> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->LookupItemIDs(szItemID, PropertyIDs.size()
			, PropertyIDs, &szNewItemIDs, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			szNewItemIDs.size(PropertyIDs.size());
			Errors.size(PropertyIDs.size());
			stream << szNewItemIDs;
			stream << Errors;
		}
		stream.flush();
	}

	void COPCServerFar::GetPublicGroupByName(NkStream::CStream& stream)
	{
		NkCom::CComString szName;
		IID iid;
		NkCom::CComPtrFar<IUnknown, COPCGroupFar> spUnk(m_pSrv);

		stream >> szName;
		stream >> iid;

		NkCom::CComPtr<IOPCServerPublicGroups> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetPublicGroupByName(szName, iid, &spUnk);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << spUnk;
		}
		stream.flush();
	}

	void COPCServerFar::RemovePublicGroup(NkStream::CStream& stream)
	{
		OPCHANDLE hServerGroup;
		BOOL bForce;

		stream >> hServerGroup;
		stream >> bForce;

		NkCom::CComPtr<IOPCServerPublicGroups> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->RemovePublicGroup(hServerGroup, bForce);

		stream << hr;
		stream.flush();
	}

	void COPCServerFar::QueryOrganization(NkStream::CStream& stream)
	{
		OPCNAMESPACETYPE NameSpaceType;

		NkCom::CComPtr<IOPCBrowseServerAddressSpace> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->QueryOrganization(&NameSpaceType);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << NameSpaceType;
		}
		stream.flush();
	}

	void COPCServerFar::ChangeBrowsePosition(NkStream::CStream& stream)
	{
		OPCBROWSEDIRECTION dwBrowseDirection;
		NkCom::CComString szString;

		stream >> dwBrowseDirection;
		stream >> szString;

		NkCom::CComPtr<IOPCBrowseServerAddressSpace> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->ChangeBrowsePosition(dwBrowseDirection, szString);

		stream << hr;
		stream.flush();
	}

	void COPCServerFar::BrowseOPCItemIDs(NkStream::CStream& stream)
	{
		OPCBROWSETYPE dwBrowseFilterType;
		NkCom::CComString szFilterCriteria;
		VARTYPE vtDataTypeFilter;
		DWORD dwAccessRightsFilter;
		NkCom::CComPtrFarEnumString	spEnum(m_pSrv);

		stream >> dwBrowseFilterType;
		stream >> szFilterCriteria;
		stream >> vtDataTypeFilter;
		stream >> dwAccessRightsFilter;

		NkCom::CComPtr<IOPCBrowseServerAddressSpace> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->BrowseOPCItemIDs(dwBrowseFilterType, szFilterCriteria
			, vtDataTypeFilter, dwAccessRightsFilter, &spEnum);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << spEnum;
		}
		stream.flush();
	}

	void COPCServerFar::GetItemID(NkStream::CStream& stream)
	{
		NkCom::CComString szItemDataID;
		NkCom::CComString szItemID;

		stream >> szItemDataID;

		NkCom::CComPtr<IOPCBrowseServerAddressSpace> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetItemID(szItemDataID, &szItemID);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << szItemID;
		}
		stream.flush();
	}

	void COPCServerFar::BrowseAccessPaths(NkStream::CStream& stream)
	{
		NkCom::CComString szItemID;
		NkCom::CComPtrFarEnumString	spEnum(m_pSrv);

		stream >> szItemID;

		NkCom::CComPtr<IOPCBrowseServerAddressSpace> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->BrowseAccessPaths(szItemID, &spEnum);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << spEnum;
		}
		stream.flush();
	}
}