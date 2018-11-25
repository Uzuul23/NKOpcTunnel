#include "stdafx.h"
#include "opcserverlist2far.h"
#include "com/comptr.h"
#include "com/comstring.h"
#include "com/comfarsrv.h"
#include "stream/stream.h"
#include "com/vector.h"
#include "opc/opctypes.h"
#include "com/interfaces.h"
#include "opc/opcfarsrv.h"


namespace NkOPC
{
	COPCServerList2Far::COPCServerList2Far(NkCom::CServer* pSrv, REFCLSID rclsid
		, DWORD dwClsContext, REFIID riid)
		: CComFarUnknown(pSrv, rclsid, dwClsContext, riid)
	{
		NkError::CBaseException::check_result(query_interface(riid), __FILE__, __LINE__);
	}

	HRESULT COPCServerList2Far::query_interface(REFIID iid)
	{
		if (iid == IID_IUnknown) {
			return S_OK;
		}
		if (iid == IID_IOPCServerList2) {
			if (!m_spOPCServerList2) {
				return m_spUnknown.QueryInterface(&m_spOPCServerList2);
			}
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	void COPCServerList2Far::call(ULONG32 index, NkStream::CStream& stream)
	{
		switch (index) {
		case 1: EnumClassesOfCategories(stream); break;
		case 2: GetClassDetails(stream); break;
		case 3: CLSIDFromProgID(stream); break;
		}

		if (NkTrace::CTrace::IsTraceLevel3()) {
			static const WCHAR* pszFctNames[] = {
				L"dummy",
				L"IOPCServerList2::EnumClassesOfCategories()",
				L"IOPCServerList2::GetClassDetails()",
				L"IOPCServerList2::CLSIDFromProgID()",
			};
			if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
				_ASSERT(0);
				return;
			}
			NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
		}
	}

	void COPCServerList2Far::EnumClassesOfCategories(NkStream::CStream& stream)
	{
		NkCom::CVector<CATID> rgcatidImpl;
		NkCom::CVector<CATID> rgcatidReq;

		stream >> rgcatidImpl;
		stream >> rgcatidReq;

		COPCFarSrv *pFarSrvOPC = static_cast<COPCFarSrv*>(m_pSrv);
		_ASSERT(dynamic_cast<COPCFarSrv*>(m_pSrv));

		if (!pFarSrvOPC->has_access(COPCFarSrv::COPCFarSrvAccess::BrowseOPCServer)) {
			stream << static_cast<HRESULT>(E_ACCESSDENIED);
			stream.flush();
			return;
		}

		NkCom::CComPtr<IOPCEnumGUID> sp_enum_guid;
		HRESULT hr = m_spOPCServerList2->EnumClassesOfCategories(rgcatidImpl.size(), rgcatidImpl
			, rgcatidReq.size(), rgcatidReq, &sp_enum_guid);

		if (FAILED(hr)) {
			stream << hr;
			stream.flush();
			return;
		}

		GUID guid = GUID_NULL;
		ULONG celtFetched = 0;
		std::vector<GUID> all_guids;
		std::vector<bool> accesses;
		while (sp_enum_guid->Next(1, &guid, &celtFetched) == S_OK) {
			all_guids.push_back(guid);
		}

		size_t allowed_count = 0;
		pFarSrvOPC->filter_access(COPCFarSrv::COPCFarSrvAccess::BrowseOPCServer
			, all_guids, accesses, &allowed_count);

		if (allowed_count == 0) {
			stream << static_cast<HRESULT>(E_ACCESSDENIED);
			stream.flush();
			return;
		}

		NkCom::CVector<GUID> guids(allowed_count);
		DWORD index = 0;
		for (size_t loop = 0; loop < accesses.size(); ++loop) {
			if (accesses[loop]) {
				guids[index] = all_guids[loop];
				++index;
			}
		}

		stream << hr;
		stream << guids;
		stream.flush();
	}

	void COPCServerList2Far::GetClassDetails(NkStream::CStream& stream)
	{
		CLSID clsid;
		NkCom::CComString strProgID;
		NkCom::CComString strUserType;
		NkCom::CComString strVerIndProgID;

		stream >> clsid;

		HRESULT hr = m_spOPCServerList2->GetClassDetails(clsid
			, &strProgID, &strUserType, &strVerIndProgID);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << strProgID;
			stream << strUserType;
			stream << strVerIndProgID;
		}

		stream.flush();
	}

	void COPCServerList2Far::CLSIDFromProgID(NkStream::CStream& stream)
	{
		NkCom::CComString strProgID;
		CLSID clsid;

		stream >> strProgID;

		HRESULT hr = m_spOPCServerList2->CLSIDFromProgID(strProgID.data(), &clsid);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << clsid;
		}
		stream.flush();
	}
}