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
#include "opcgroupfar.h"

namespace NkOPC
{
	COPCGroupFar::COPCGroupFar(NkCom::CServer* pSrv, REFCLSID rclsid
		, DWORD dwClsContext, REFIID riid)
		: CComFarUnknown(pSrv, rclsid, dwClsContext, riid)
	{
		NkError::CBaseException::check_result(query_interface(riid), __FILE__, __LINE__);
	}

	COPCGroupFar::COPCGroupFar(NkCom::CServer* pSrv, IUnknown* p, ULONG32* pid /*= 0*/)
		: CComFarUnknown(pSrv, p, pid)
	{
	}


	COPCGroupFar::~COPCGroupFar()
	{
	}

	HRESULT COPCGroupFar::query_interface(REFIID iid)
	{
		if (iid == IID_IUnknown
			|| iid == IID_IOPCGroupStateMgt
			|| iid == IID_IOPCGroupStateMgt2
			|| iid == IID_IOPCItemMgt
			|| iid == IID_IConnectionPointContainer
			|| iid == IID_IOPCSyncIO
			|| iid == IID_IOPCSyncIO2
			|| iid == IID_IOPCAsyncIO
			|| iid == IID_IOPCAsyncIO2
			|| iid == IID_IOPCAsyncIO3
			|| iid == IID_IOPCItemDeadbandMgt
			|| iid == IID_IOPCItemSamplingMgt
			|| iid == IID_IDataObject
			|| iid == IID_IOPCPublicGroupStateMgt)
		{
			return internal_query_interface(iid);
		}

#ifdef NK_TRACE_QUERY_INTERFACE

		NkTrace::CTrace& Trace = NkTrace::CTrace::Instance();
		if (Trace.IsTraceLevel3()) {
			NkCom::CComString str;
			StringFromIID(iid, &str);
			Trace.Trace(NkTrace::CTrace::TraceDetailed
				, L"IOPCGroup::QueryInterface(%s) E_NOINTERFACE"
				, str.data());
		}

#endif

		return E_NOINTERFACE;
	}

	void COPCGroupFar::call(ULONG32 index, NkStream::CStream& stream)
	{
		HRESULT hr = S_OK;

		switch (index) {
		case 1: hr = GetState(stream); break;
		case 2: hr = SetState(stream); break;
		case 3: hr = SetName(stream); break;
		case 4: hr = CloneGroup(stream); break;
		case 5: hr = SetKeepAlive(stream); break;
		case 6: hr = GetKeepAlive(stream); break;
		case 7: hr = AddItems(stream); break;
		case 8: hr = ValidateItems(stream); break;
		case 9: hr = RemoveItems(stream); break;
		case 10: hr = SetActiveState(stream); break;
		case 11: hr = SetClientHandles(stream); break;
		case 12: hr = SetDatatypes(stream); break;
		case 13: hr = CreateEnumerator(stream); break;
		case 14: hr = EnumConnectionPoints(stream); break;
		case 15: hr = FindConnectionPoint(stream); break;
		case 16: hr = Read(stream); break;
		case 17: hr = Write(stream); break;
		case 18: hr = ReadMaxAge(stream); break;
		case 19: hr = WriteVQT(stream); break;
		case 20: hr = AsyncRead(stream); break;
		case 21: hr = AsyncWrite(stream); break;
		case 22: hr = Refresh(stream); break;
		case 23: hr = Cancel(stream); break;
		case 24: hr = AsyncRead2(stream); break;
		case 25: hr = AsyncWrite2(stream); break;
		case 26: hr = Refresh2(stream); break;
		case 27: hr = Cancel2(stream); break;
		case 28: hr = SetEnable(stream); break;
		case 29: hr = GetEnable(stream); break;
		case 30: hr = ReadMaxAge3(stream); break;
		case 31: hr = WriteVQT3(stream); break;
		case 32: hr = RefreshMaxAge(stream); break;
		case 33: hr = SetItemDeadband(stream); break;
		case 34: hr = GetItemDeadband(stream); break;
		case 35: hr = ClearItemDeadband(stream); break;
		case 36: hr = SetItemSamplingRate(stream); break;
		case 37: hr = GetItemSamplingRate(stream); break;
		case 38: hr = ClearItemSamplingRate(stream); break;
		case 39: hr = SetItemBufferEnable(stream); break;
		case 40: hr = GetItemBufferEnable(stream); break;
		case 41: hr = DAdvise(stream); break;
		case 42: hr = DUnadvise(stream); break;
		case 43: hr = GetState2(stream); break;
		case 44: hr = MoveToPublic(stream); break;
		}

		if (NkTrace::CTrace::IsTraceLevel3()) {
			static const WCHAR* pszFctNames[] = {
				L"dummy",
				L"IOPCGroupStateMgt::GetState()",
				L"IOPCGroupStateMgt::SetState()",
				L"IOPCGroupStateMgt::SetName()",
				L"IOPCGroupStateMgt::CloneGroup()",
				L"IOPCGroupStateMgt::SetKeepAlive()",
				L"IOPCGroupStateMgt::GetKeepAlive()",
				L"IOPCItemMgt::AddItems()",
				L"IOPCItemMgt::ValidateItems()",
				L"IOPCItemMgt::RemoveItems()",
				L"IOPCItemMgt::SetActiveState()",
				L"IOPCItemMgt::SetClientHandles()",
				L"IOPCItemMgt::SetDatatypes()",
				L"IOPCItemMgt::CreateEnumerator()",
				L"IConnectionPointContainer::EnumConnectionPoints()",
				L"IConnectionPointContainer::FindConnectionPoint()",
				L"IOPCSyncIO::Read()",
				L"IOPCSyncIO::Write()",
				L"IOPCSyncIO2::ReadMaxAge()",
				L"IOPCSyncIO2::WriteVQT()",
				L"IOPCAsyncIO::AsyncRead()",
				L"IOPCAsyncIO::AsyncWrite()",
				L"IOPCAsyncIO::Refresh()",
				L"IOPCAsyncIO::Cancel()",
				L"IOPCAsyncIO2::AsyncRead()",
				L"IOPCAsyncIO2::AsyncWrite()",
				L"IOPCAsyncIO2::Refresh()",
				L"IOPCAsyncIO2::Cancel2()",
				L"IOPCAsyncIO2::SetEnable()",
				L"IOPCAsyncIO2::GetEnable()",
				L"IOPCAsyncIO3::ReadMaxAge()",
				L"IOPCAsyncIO3::WriteVQT()",
				L"IOPCAsyncIO3::RefreshMaxAge()",
				L"IOPCItemDeadbandMgt::SetItemDeadband()",
				L"IOPCItemDeadbandMgt::GetItemDeadband()",
				L"IOPCItemDeadbandMgt::ClearItemDeadband()",
				L"IOPCItemSamplingMgt::SetItemSamplingRate()",
				L"IOPCItemSamplingMgt::GetItemSamplingRate()",
				L"IOPCItemSamplingMgt::ClearItemSamplingRate()",
				L"IOPCItemSamplingMgt::SetItemBufferEnable()",
				L"IOPCItemSamplingMgt::GetItemBufferEnable()",
				L"IDataObject::DAdvise()",
				L"IDataObject::DUnadvise()",
				L"IOPCPublicGroupStateMgt::GetState()",
				L"IOPCPublicGroupStateMgt::MoveToPublic()"
			};
			if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
				_ASSERT(0);
				return;
			}
			NkTrace::CTrace::TraceDet(L"remote call: %s result(0x%x)"
				, pszFctNames[index], hr);
		}
	}

	//IOPCGroupStateMgt

	HRESULT COPCGroupFar::GetState(NkStream::CStream& stream)
	{
		DWORD UpdateRate;
		BOOL Active;
		NkCom::CComString strName;
		LONG TimeBias;
		FLOAT PercentDeadband;
		DWORD lcid;
		OPCHANDLE hClientGroup;
		OPCHANDLE hServerGroup;

		NkCom::CComPtr<IOPCGroupStateMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetState(&UpdateRate, &Active, &strName
			, &TimeBias, &PercentDeadband, &lcid
			, &hClientGroup, &hServerGroup);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << UpdateRate;
			stream << Active;
			stream << strName;
			stream << TimeBias;
			stream << PercentDeadband;
			stream << lcid;
			stream << hClientGroup;
			stream << hServerGroup;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::SetState(NkStream::CStream& stream)
	{
		NkType::CUniquePointer<DWORD> pRequestedUpdateRate;
		NkType::CUniquePointer<BOOL> pActive;
		NkType::CUniquePointer<LONG> pTimeBias;
		NkType::CUniquePointer<FLOAT> pPercentDeadband;
		NkType::CUniquePointer<DWORD> pLCID;
		NkType::CUniquePointer<OPCHANDLE> phClientGroup;
		DWORD dwRevisedUpdateRate;

		stream >> pRequestedUpdateRate;
		stream >> pActive;
		stream >> pTimeBias;
		stream >> pPercentDeadband;
		stream >> pLCID;
		stream >> phClientGroup;

		NkCom::CComPtr<IOPCGroupStateMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->SetState(pRequestedUpdateRate
			, &dwRevisedUpdateRate
			, pActive, pTimeBias
			, pPercentDeadband
			, pLCID, phClientGroup);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << dwRevisedUpdateRate;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::SetName(NkStream::CStream& stream)
	{
		NkCom::CComString szName;
		stream >> szName;

		NkCom::CComPtr<IOPCGroupStateMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->SetName(szName);

		stream << hr;
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::CloneGroup(NkStream::CStream& stream)
	{
		NkCom::CComString szName;
		IID iid;
		NkCom::CComPtrFar<IUnknown, COPCGroupFar> spGroup(m_pSrv);

		stream >> szName;
		stream >> iid;

		NkCom::CComPtr<IOPCGroupStateMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->CloneGroup(szName, iid, &spGroup);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << spGroup;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::SetKeepAlive(NkStream::CStream& stream)
	{
		DWORD dwKeepAliveTime;
		DWORD dwRevisedKeepAliveTime;

		stream >> dwKeepAliveTime;

		NkCom::CComPtr<IOPCGroupStateMgt2> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->SetKeepAlive(dwKeepAliveTime
			, &dwRevisedKeepAliveTime);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << dwRevisedKeepAliveTime;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::GetKeepAlive(NkStream::CStream& stream)
	{
		DWORD dwKeepAliveTime;

		NkCom::CComPtr<IOPCGroupStateMgt2> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetKeepAlive(&dwKeepAliveTime);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << dwKeepAliveTime;
		}
		stream.flush();
		return hr;
	}

	//IOPCItemMgt

	HRESULT COPCGroupFar::AddItems(NkStream::CStream& stream)
	{
		CVectorOPCITEMDEF ItemArray;
		CVectorOPCITEMRESULT AddResult;
		NkCom::CVector<HRESULT> Errors;

		stream >> ItemArray;

		NkCom::CComPtr<IOPCItemMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->AddItems(ItemArray.size(), ItemArray
			, &AddResult, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			AddResult.size(ItemArray.size());
			Errors.size(ItemArray.size());
			stream << AddResult;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::ValidateItems(NkStream::CStream& stream)
	{
		CVectorOPCITEMDEF ItemArray;
		BOOL bBlobUpdate;
		CVectorOPCITEMRESULT ValidationResults;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> ItemArray;
		stream >> bBlobUpdate;

		NkCom::CComPtr<IOPCItemMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->ValidateItems(ItemArray.size(), ItemArray
			, bBlobUpdate, &ValidationResults, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			ValidationResults.size(ItemArray.size());
			Errors.size(ItemArray.size());
			stream << ValidationResults;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::RemoveItems(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;

		NkCom::CComPtr<IOPCItemMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->RemoveItems(hServer.size(), hServer, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::SetActiveState(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		BOOL bActive;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> bActive;

		NkCom::CComPtr<IOPCItemMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->SetActiveState(hServer.size(), hServer, bActive, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::SetClientHandles(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<OPCHANDLE, DWORD> hClient;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> hClient;

		NkCom::CComPtr<IOPCItemMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->SetClientHandles(hServer.size(), hServer, hClient, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::SetDatatypes(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<VARTYPE, DWORD> RequestedDatatypes;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> RequestedDatatypes;

		NkCom::CComPtr<IOPCItemMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->SetDatatypes(hServer.size()
			, hServer, RequestedDatatypes, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::CreateEnumerator(NkStream::CStream& stream)
	{
		IID iid;
		CComPtrFarEnumOPCItemAttributes spEnum(m_pSrv);

		stream >> iid;

		NkCom::CComPtr<IOPCItemMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->CreateEnumerator(iid, reinterpret_cast<IUnknown**>(&spEnum));

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << spEnum;
		}
		stream.flush();
		return hr;
	}

	//IConnectionPointContainer

	HRESULT COPCGroupFar::EnumConnectionPoints(NkStream::CStream& stream)
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
		return hr;
	}

	HRESULT COPCGroupFar::FindConnectionPoint(NkStream::CStream& stream)
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
		return hr;
	}

	//IOPCSyncIO

	HRESULT COPCGroupFar::Read(NkStream::CStream& stream)
	{
		OPCDATASOURCE dwSource;
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		CVectorOPCITEMSTATE ItemValues;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> dwSource;
		stream >> hServer;

		NkCom::CComPtr<IOPCSyncIO> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Read(dwSource, hServer.size(), hServer
			, &ItemValues, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			ItemValues.size(hServer.size());
			Errors.size(hServer.size());
			stream << ItemValues;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::Write(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVariantVector<DWORD> ItemValues;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> ItemValues;

		NkCom::CComPtr<IOPCSyncIO> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Write(hServer.size(), hServer
			, ItemValues, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	//IOPCSyncIO2

	HRESULT COPCGroupFar::ReadMaxAge(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<DWORD, DWORD> dwMaxAge;
		NkCom::CVariantVector<DWORD> vValues;
		NkCom::CVector<WORD, WORD> wQualities;
		NkCom::CVector<FILETIME, DWORD> ftTimeStamps;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> dwMaxAge;

		NkCom::CComPtr<IOPCSyncIO2> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->ReadMaxAge(hServer.size(), hServer
			, dwMaxAge, &vValues, &wQualities, &ftTimeStamps, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			vValues.size(hServer.size());
			wQualities.size(hServer.size());
			ftTimeStamps.size(hServer.size());
			Errors.size(hServer.size());
			stream << vValues;
			stream << wQualities;
			stream << ftTimeStamps;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::WriteVQT(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkOPC::CVectorOPCITEMVQT ItemVQT;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> ItemVQT;

		NkCom::CComPtr<IOPCSyncIO2> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->WriteVQT(hServer.size(), hServer, ItemVQT, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	//IOPCAsyncIO

	HRESULT COPCGroupFar::AsyncRead(NkStream::CStream& stream)
	{
		DWORD dwConnection;
		OPCDATASOURCE dwSource;
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		DWORD TransactionID;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> dwConnection;
		stream >> dwSource;
		stream >> hServer;

		NkCom::CComPtr<IOPCAsyncIO> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Read(dwConnection, dwSource, hServer.size()
			, hServer, &TransactionID, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << TransactionID;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::AsyncWrite(NkStream::CStream& stream)
	{
		DWORD dwConnection;
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVariantVector<DWORD> ItemValues;
		DWORD TransactionID;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> dwConnection;
		stream >> hServer;
		stream >> ItemValues;

		NkCom::CComPtr<IOPCAsyncIO> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Write(dwConnection, hServer.size()
			, hServer, ItemValues, &TransactionID, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << TransactionID;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::Refresh(NkStream::CStream& stream)
	{
		DWORD dwConnection;
		OPCDATASOURCE dwSource;
		DWORD TransactionID;

		stream >> dwConnection;
		stream >> dwSource;

		NkCom::CComPtr<IOPCAsyncIO> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Refresh(dwConnection, dwSource, &TransactionID);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << TransactionID;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::Cancel(NkStream::CStream& stream)
	{
		DWORD TransactionID;

		stream >> TransactionID;

		NkCom::CComPtr<IOPCAsyncIO> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Cancel(TransactionID);

		stream << hr;
		stream.flush();
		return hr;
	}

	//IOPCAsyncIO2

	HRESULT COPCGroupFar::AsyncRead2(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		DWORD dwTransactionID;
		DWORD dwCancelID;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> dwTransactionID;

		NkCom::CComPtr<IOPCAsyncIO2> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Read(hServer.size(), hServer, dwTransactionID
			, &dwCancelID, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << dwCancelID;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::AsyncWrite2(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVariantVector<DWORD> ItemValues;
		DWORD dwTransactionID;
		DWORD dwCancelID;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> ItemValues;
		stream >> dwTransactionID;

		NkCom::CComPtr<IOPCAsyncIO2> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Write(hServer.size(), hServer, ItemValues
			, dwTransactionID, &dwCancelID, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << dwCancelID;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::Refresh2(NkStream::CStream& stream)
	{
		OPCDATASOURCE dwSource;
		DWORD dwTransactionID;
		DWORD dwCancelID;

		stream >> dwSource;
		stream >> dwTransactionID;

		NkCom::CComPtr<IOPCAsyncIO2> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Refresh2(dwSource, dwTransactionID, &dwCancelID);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << dwCancelID;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::Cancel2(NkStream::CStream& stream)
	{
		DWORD dwCancelID;

		stream >> dwCancelID;

		NkCom::CComPtr<IOPCAsyncIO2> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->Cancel2(dwCancelID);

		stream << hr;
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::SetEnable(NkStream::CStream& stream)
	{
		BOOL bEnable;

		stream >> bEnable;

		NkCom::CComPtr<IOPCAsyncIO2> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->SetEnable(bEnable);

		stream << hr;
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::GetEnable(NkStream::CStream& stream)
	{
		BOOL bEnable;

		NkCom::CComPtr<IOPCAsyncIO2> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetEnable(&bEnable);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << bEnable;
		}
		stream.flush();
		return hr;
	}

	//IOPCAsyncIO3

	HRESULT COPCGroupFar::ReadMaxAge3(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<DWORD, DWORD> dwMaxAge;
		DWORD dwTransactionID;
		DWORD dwCancelID;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> dwMaxAge;
		stream >> dwTransactionID;

		NkCom::CComPtr<IOPCAsyncIO3> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->ReadMaxAge(hServer.size(), hServer, dwMaxAge
			, dwTransactionID, &dwCancelID, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << dwCancelID;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::WriteVQT3(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkOPC::CVectorOPCITEMVQT ItemVQT;
		DWORD dwTransactionID;
		DWORD dwCancelID;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> ItemVQT;
		stream >> dwTransactionID;

		NkCom::CComPtr<IOPCAsyncIO3> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->WriteVQT(hServer.size(), hServer, ItemVQT
			, dwTransactionID, &dwCancelID, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << dwCancelID;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::RefreshMaxAge(NkStream::CStream& stream)
	{
		DWORD dwMaxAge;
		DWORD dwTransactionID;
		DWORD dwCancelID;

		stream >> dwMaxAge;
		stream >> dwTransactionID;

		NkCom::CComPtr<IOPCAsyncIO3> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->RefreshMaxAge(dwMaxAge, dwTransactionID, &dwCancelID);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << dwCancelID;
		}
		stream.flush();
		return hr;
	}

	//IOPCItemDeadbandMgt

	HRESULT COPCGroupFar::SetItemDeadband(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<FLOAT, DWORD> PercentDeadband;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> PercentDeadband;

		NkCom::CComPtr<IOPCItemDeadbandMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->SetItemDeadband(hServer.size(), hServer
			, PercentDeadband, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::GetItemDeadband(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<FLOAT, DWORD> PercentDeadband;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;

		NkCom::CComPtr<IOPCItemDeadbandMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetItemDeadband(hServer.size(), hServer
			, &PercentDeadband, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			PercentDeadband.size(hServer.size());
			Errors.size(hServer.size());
			stream << PercentDeadband;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::ClearItemDeadband(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;

		NkCom::CComPtr<IOPCItemDeadbandMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->ClearItemDeadband(hServer.size(), hServer, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	//IOPCItemSamplingMgt

	HRESULT COPCGroupFar::SetItemSamplingRate(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<DWORD, DWORD> wRequestedSamplingRate;
		NkCom::CVector<DWORD, DWORD> dwRevisedSamplingRate;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> wRequestedSamplingRate;

		NkCom::CComPtr<IOPCItemSamplingMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->SetItemSamplingRate(hServer.size()
			, hServer, wRequestedSamplingRate
			, &dwRevisedSamplingRate, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			dwRevisedSamplingRate.size(hServer.size());
			Errors.size(hServer.size());
			stream << dwRevisedSamplingRate;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::GetItemSamplingRate(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<DWORD, DWORD> dwSamplingRate;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;

		NkCom::CComPtr<IOPCItemSamplingMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetItemSamplingRate(hServer.size()
			, hServer, &dwSamplingRate, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			dwSamplingRate.size(hServer.size());
			Errors.size(hServer.size());
			stream << dwSamplingRate;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::ClearItemSamplingRate(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;

		NkCom::CComPtr<IOPCItemSamplingMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->ClearItemSamplingRate(hServer.size()
			, hServer, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::SetItemBufferEnable(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<BOOL, DWORD> bEnable;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;
		stream >> bEnable;

		NkCom::CComPtr<IOPCItemSamplingMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->SetItemBufferEnable(hServer.size()
			, hServer, bEnable, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			Errors.size(hServer.size());
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::GetItemBufferEnable(NkStream::CStream& stream)
	{
		NkCom::CVector<OPCHANDLE, DWORD> hServer;
		NkCom::CVector<BOOL, DWORD> bEnable;
		NkCom::CVector<HRESULT, DWORD> Errors;

		stream >> hServer;

		NkCom::CComPtr<IOPCItemSamplingMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetItemBufferEnable(hServer.size()
			, hServer, &bEnable, &Errors);

		stream << hr;
		if (SUCCEEDED(hr)) {
			bEnable.size(hServer.size());
			Errors.size(hServer.size());
			stream << bEnable;
			stream << Errors;
		}
		stream.flush();
		return hr;
	}

	//IDataObject

	HRESULT COPCGroupFar::DAdvise(NkStream::CStream& stream)
	{
		ULONG32 id;
		FORMATETC formatetc;
		DWORD advf;
		DWORD dwConnection;

		stream >> id;
		stream.read(&formatetc, sizeof(FORMATETC));
		stream >> advf;

		NkCom::CComPtr<IDataObject> sp;
		internal_get_interface(sp);

		NkCom::CComPtr<IAdviseSink> spSink;
		m_pSrv->create_near_proxy(IID_IAdviseSink, IID_IAdviseSink
			, reinterpret_cast<void**>(&spSink), id);

		HRESULT hr = sp->DAdvise(&formatetc, advf, spSink, &dwConnection);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << dwConnection;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::DUnadvise(NkStream::CStream& stream)
	{
		DWORD dwConnection;

		stream >> dwConnection;

		NkCom::CComPtr<IDataObject> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->DUnadvise(dwConnection);

		stream << hr;
		stream.flush();
		return hr;
	}

	//IOPCPublicGroupStateMgt

	HRESULT COPCGroupFar::GetState2(NkStream::CStream& stream)
	{
		BOOL Public;

		NkCom::CComPtr<IOPCPublicGroupStateMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->GetState(&Public);

		stream << hr;
		if (SUCCEEDED(hr)) {
			stream << Public;
		}
		stream.flush();
		return hr;
	}

	HRESULT COPCGroupFar::MoveToPublic(NkStream::CStream& stream)
	{
		NkCom::CComPtr<IOPCPublicGroupStateMgt> sp;
		internal_get_interface(sp);

		HRESULT hr = sp->MoveToPublic();

		stream << hr;
		stream.flush();
		return hr;
	}

}