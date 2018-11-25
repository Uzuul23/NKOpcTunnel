#include "stdafx.h"
#include "opcgroupnear.h"
#include "com/comptr.h"

namespace NkOPC
{
	COPCGroupNear::COPCGroupNear(NkCom::CServer* p, ULONG32 id) : CNearUnknown(p, id)
	{
	}

	STDMETHODIMP COPCGroupNear::QueryInterface(REFIID riid, void **ppvObject)
	{
		if (ppvObject == 0) {
			return E_POINTER;
		}

		if (FAILED(remote_query_interface(riid))) {
			return E_NOINTERFACE;
		}

		if (riid == IID_IUnknown) {
			(*ppvObject) = static_cast<IOPCGroupStateMgt*>(this);
		}
		else if (riid == IID_IOPCGroupStateMgt) {
			(*ppvObject) = static_cast<IOPCGroupStateMgt*>(this);
		}
		else if (riid == IID_IOPCGroupStateMgt2) {
			(*ppvObject) = static_cast<IOPCGroupStateMgt2*>(this);
		}
		else if (riid == IID_IConnectionPointContainer) {
			(*ppvObject) = static_cast<IConnectionPointContainer*>(this);
		}
		else if (riid == IID_IOPCItemMgt) {
			(*ppvObject) = static_cast<IOPCItemMgt*>(this);
		}
		else if (riid == IID_IOPCSyncIO) {
			(*ppvObject) = static_cast<IOPCSyncIO*>(this);
		}
		else if (riid == IID_IOPCSyncIO2) {
			(*ppvObject) = static_cast<IOPCSyncIO2*>(this);
		}
		else if (riid == IID_IOPCAsyncIO) {
			(*ppvObject) = static_cast<IOPCAsyncIO*>(this);
		}
		else if (riid == IID_IOPCAsyncIO2) {
			(*ppvObject) = static_cast<IOPCAsyncIO2*>(this);
		}
		else if (riid == IID_IOPCAsyncIO3) {
			(*ppvObject) = static_cast<IOPCAsyncIO3*>(this);
		}
		else if (riid == IID_IOPCItemDeadbandMgt) {
			(*ppvObject) = static_cast<IOPCItemDeadbandMgt*>(this);
		}
		else if (riid == IID_IOPCItemSamplingMgt) {
			(*ppvObject) = static_cast<IOPCItemSamplingMgt*>(this);
		}
		else if (riid == IID_IDataObject) {
			(*ppvObject) = static_cast<IDataObject*>(this);
		}
		else if (riid == IID_IOPCPublicGroupStateMgt) {
			(*ppvObject) = static_cast<IOPCPublicGroupStateMgt*>(this);
		}
		else {
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	//IOPCGroupStateMgt

	STDMETHODIMP COPCGroupNear::GetState( /* [out] */ DWORD *pUpdateRate
		, /* [out] */ BOOL *pActive
		, /* [string][out] */ LPWSTR *ppName
		, /* [out] */ LONG *pTimeBias
		, /* [out] */ FLOAT *pPercentDeadband
		, /* [out] */ DWORD *pLCID
		, /* [out] */ OPCHANDLE *phClientGroup
		, /* [out] */ OPCHANDLE *phServerGroup)
	{
		try{
			NkError::CBaseException::check_arg(pUpdateRate, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pActive, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppName, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pTimeBias, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pPercentDeadband, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pLCID, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(phClientGroup, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(phServerGroup, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 1, L"IOPCGroupStateMgt::GetState()");
			srv.stream.flush();
			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CComString strName;
				srv.stream >> *pUpdateRate;
				srv.stream >> *pActive;
				srv.stream >> strName;
				srv.stream >> *pTimeBias;
				srv.stream >> *pPercentDeadband;
				srv.stream >> *pLCID;
				srv.stream >> *phClientGroup;
				srv.stream >> *phServerGroup;
				strName.detach(ppName);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::SetState( /* [in][unique] */ DWORD *pRequestedUpdateRate
		, /* [out] */ DWORD *pRevisedUpdateRate
		, /* [in][unique] */ BOOL *pActive
		, /* [in][unique] */ LONG *pTimeBias
		, /* [in][unique] */ FLOAT *pPercentDeadband
		, /* [in][unique] */ DWORD *pLCID
		, /* [in][unique] */ OPCHANDLE *phClientGroup)
	{
		try{
			NkError::CBaseException::check_arg(pRevisedUpdateRate, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 2, L"IOPCGroupStateMgt::SetState()");
			srv.stream << NkType::CUniquePointer<DWORD>(pRequestedUpdateRate);
			srv.stream << NkType::CUniquePointer<BOOL>(pActive);
			srv.stream << NkType::CUniquePointer<LONG>(pTimeBias);
			srv.stream << NkType::CUniquePointer<FLOAT>(pPercentDeadband);
			srv.stream << NkType::CUniquePointer<DWORD>(pLCID);
			srv.stream << NkType::CUniquePointer<OPCHANDLE>(phClientGroup);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				srv.stream >> *pRevisedUpdateRate;
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::SetName( /* [string][in] */ LPCWSTR szName)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 3, L"IOPCGroupStateMgt::SetName()");
			srv.stream << szName;
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::CloneGroup( /* [string][in] */ LPCWSTR szName
		, /* [in] */ REFIID riid
		, /* [iid_is][out] */ LPUNKNOWN *ppUnk)
	{
		try{
			NkError::CBaseException::check_arg(ppUnk, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 4, L"IOPCGroupStateMgt::CloneGroup()");
			srv.stream << szName;
			srv.stream << riid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CComPtrNear<IUnknown, COPCGroupNear> spGroup(m_pSrv);
				srv.stream >> spGroup;
				spGroup.Detach(ppUnk);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	//IOPCGroupStateMgt2

	STDMETHODIMP COPCGroupNear::SetKeepAlive( /* [in] */ DWORD dwKeepAliveTime
		, /* [out] */ DWORD *pdwRevisedKeepAliveTime)
	{
		try{
			NkError::CBaseException::check_arg(pdwRevisedKeepAliveTime, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 5, L"IOPCGroupStateMgt2::SetKeepAlive()");
			srv.stream << dwKeepAliveTime;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				srv.stream >> *pdwRevisedKeepAliveTime;
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::GetKeepAlive( /* [out] */ DWORD *pdwKeepAliveTime)
	{
		try{
			NkError::CBaseException::check_arg(pdwKeepAliveTime, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 6, L"IOPCGroupStateMgt2::GetKeepAlive()");
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				srv.stream >> *pdwKeepAliveTime;
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	//IOPCItemMgt

	STDMETHODIMP COPCGroupNear::AddItems( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCITEMDEF *pItemArray
		, /* [size_is][size_is][out] */ OPCITEMRESULT **ppAddResults
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(pItemArray, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppAddResults, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 7, L"IOPCItemMgt::AddItems()");
			srv.stream << CVectorOPCITEMDEF(pItemArray, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				CVectorOPCITEMRESULT AddResult;
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> AddResult;
				srv.stream >> Errors;
				AddResult.detach(ppAddResults);
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::ValidateItems( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCITEMDEF *pItemArray
		, /* [in] */ BOOL bBlobUpdate
		, /* [size_is][size_is][out] */ OPCITEMRESULT **ppValidationResults
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(pItemArray, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppValidationResults, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 8, L"IOPCItemMgt::ValidateItems()");
			srv.stream << CVectorOPCITEMDEF(pItemArray, dwCount);
			srv.stream << bBlobUpdate;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				CVectorOPCITEMRESULT ValidationResults;
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> ValidationResults;
				srv.stream >> Errors;
				ValidationResults.detach(ppValidationResults);
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::RemoveItems( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 9, L"IOPCItemMgt::RemoveItems()");
			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::SetActiveState( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [in] */ BOOL bActive
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 10, L"IOPCItemMgt::SetActiveState()");
			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << bActive;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::SetClientHandles( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ OPCHANDLE *phClient
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(phClient, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 11, L"IOPCItemMgt::SetClientHandles()");
			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phClient, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::SetDatatypes( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ VARTYPE *pRequestedDatatypes
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pRequestedDatatypes, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 12, L"IOPCItemMgt::SetDatatypes()");
			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkCom::CVector<VARTYPE, DWORD>(pRequestedDatatypes, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::CreateEnumerator( /* [in] */ REFIID riid
		, /* [iid_is][out] */ LPUNKNOWN *ppUnk)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 13, L"IOPCItemMgt::CreateEnumerator()");

			NkError::CBaseException::check_arg(ppUnk, __FILE__, __LINE__);

			srv.stream << riid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				if (riid == IID_IEnumOPCItemAttributes) {
					CComPtrNearEnumOPCItemAttributes spEnum(m_pSrv);
					srv.stream >> spEnum;
					(*ppUnk) = spEnum.Detach();
				}
				else{
					return E_NOINTERFACE;
				}
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	//IConnectionPointContainer

	STDMETHODIMP COPCGroupNear::EnumConnectionPoints( /* [out] */ IEnumConnectionPoints **ppEnum)
	{
		try{
			NkError::CBaseException::check_arg(ppEnum, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 14, L"IConnectionPointContainer::EnumConnectionPoints()");

			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CComPtrNearEnumCP spEnum(m_pSrv);
				srv.stream >> spEnum;
				spEnum.Detach(ppEnum);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::FindConnectionPoint( /* [in] */ REFIID riid
		, /* [out] */ IConnectionPoint **ppCP)
	{
		try{
			NkError::CBaseException::check_arg(ppCP, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 15, L"IConnectionPointContainer::FindConnectionPoint()");

			srv.stream << riid;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CComPtrNear<IConnectionPoint, NkCom::CConnectionPointNear> spCP(m_pSrv);
				srv.stream >> spCP;
				spCP.Detach(ppCP);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	//IOPCSyncIO

	STDMETHODIMP COPCGroupNear::Read( /* [in] */ OPCDATASOURCE dwSource
		, /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][size_is][out] */ OPCITEMSTATE **ppItemValues
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppItemValues, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 16, L"IOPCSyncIO::Read()");

			srv.stream << dwSource;
			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				CVectorOPCITEMSTATE ItemValues;
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> ItemValues;
				srv.stream >> Errors;
				ItemValues.detach(ppItemValues);
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::Write( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ VARIANT *pItemValues
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pItemValues, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 17, L"IOPCSyncIO::Write()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkCom::CVariantVector<DWORD>(pItemValues, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	//IOPCSyncIO2

	STDMETHODIMP COPCGroupNear::ReadMaxAge( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ DWORD *pdwMaxAge
		, /* [size_is][size_is][out] */ VARIANT **ppvValues
		, /* [size_is][size_is][out] */ WORD **ppwQualities
		, /* [size_is][size_is][out] */ FILETIME **ppftTimeStamps
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwMaxAge, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppvValues, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppwQualities, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppftTimeStamps, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 18, L"IOPCSyncIO2::ReadMaxAge()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkCom::CVector<DWORD, DWORD>(pdwMaxAge, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVariantVector<DWORD> vValues;
				NkCom::CVector<WORD, WORD> wQualities;
				NkCom::CVector<FILETIME, DWORD> ftTimeStamps;
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> vValues;
				srv.stream >> wQualities;
				srv.stream >> ftTimeStamps;
				srv.stream >> Errors;
				vValues.detach(ppvValues);
				wQualities.detach(ppwQualities);
				ftTimeStamps.detach(ppftTimeStamps);
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::WriteVQT( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ OPCITEMVQT *pItemVQT
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pItemVQT, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 19, L"IOPCSyncIO2::WriteWriteVQT()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkOPC::CVectorOPCITEMVQT(pItemVQT, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	//IOPCAsyncIO

	STDMETHODIMP COPCGroupNear::Read(
		/* [in] */ DWORD dwConnection
		, /* [in] */ OPCDATASOURCE dwSource
		, /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [out] */ DWORD *pTransactionID
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pTransactionID, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 20, L"IOPCAsyncIO::Read()");

			srv.stream << dwConnection;
			srv.stream << dwSource;
			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> *pTransactionID;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::Write(
		/* [in] */ DWORD dwConnection
		, /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ VARIANT *pItemValues
		, /* [out] */ DWORD *pTransactionID
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pItemValues, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 21, L"IOPCAsyncIO::Write()");

			srv.stream << dwConnection;
			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkCom::CVariantVector<DWORD>(pItemValues, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> *pTransactionID;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::Refresh( /* [in] */ DWORD dwConnection
		, /* [in] */ OPCDATASOURCE dwSource
		, /* [out] */ DWORD *pTransactionID)
	{
		try{
			NkError::CBaseException::check_arg(pTransactionID, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 22, L"IOPCAsyncIO::Refresh()");

			srv.stream << dwConnection;
			srv.stream << dwSource;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> *pTransactionID;
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::Cancel( /* [in] */ DWORD dwTransactionID)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 23, L"IOPCAsyncIO::Cancel()");

			srv.stream << dwTransactionID;
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	//IOPCAsyncIO2

	STDMETHODIMP COPCGroupNear::Read( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [in] */ DWORD dwTransactionID
		, /* [out] */ DWORD *pdwCancelID
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwCancelID, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 24, L"IOPCAsyncIO2::Read()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << dwTransactionID;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> *pdwCancelID;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::Write(/* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ VARIANT *pItemValues
		, /* [in] */ DWORD dwTransactionID
		, /* [out] */ DWORD *pdwCancelID
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pItemValues, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwCancelID, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 25, L"IOPCAsyncIO2::Write()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkCom::CVariantVector<DWORD>(pItemValues, dwCount);
			srv.stream << dwTransactionID;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> *pdwCancelID;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::Refresh2(/* [in] */ OPCDATASOURCE dwSource
		, /* [in] */ DWORD dwTransactionID
		, /* [out] */ DWORD *pdwCancelID)
	{
		try{
			NkError::CBaseException::check_arg(pdwCancelID, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 26, L"IOPCAsyncIO2::Refresh2()");

			srv.stream << dwSource;
			srv.stream << dwTransactionID;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> *pdwCancelID;
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::Cancel2( /* [in] */ DWORD dwCancelID)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 27, L"IOPCAsyncIO2::Cancel2()");

			srv.stream << dwCancelID;
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::SetEnable( /* [in] */ BOOL bEnable)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 28, L"IOPCAsyncIO2::SetEnable()");

			srv.stream << bEnable;
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::GetEnable( /* [out] */ BOOL *pbEnable)
	{
		try{
			NkError::CBaseException::check_arg(pbEnable, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 29, L"IOPCAsyncIO2::GetEnable()");

			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				srv.stream >> *pbEnable;
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	//IOPCAsyncIO3

	STDMETHODIMP COPCGroupNear::ReadMaxAge( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ DWORD *pdwMaxAge
		, /* [in] */ DWORD dwTransactionID
		, /* [out] */ DWORD *pdwCancelID
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwMaxAge, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwCancelID, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 30, L"IOPCAsyncIO3::ReadMaxAge()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkCom::CVector<DWORD, DWORD>(pdwMaxAge, dwCount);
			srv.stream << dwTransactionID;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> *pdwCancelID;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::WriteVQT(/* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ OPCITEMVQT *pItemVQT
		, /* [in] */ DWORD dwTransactionID
		, /* [out] */ DWORD *pdwCancelID
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pItemVQT, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwCancelID, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 31, L"IOPCAsyncIO3::WriteVQT()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkOPC::CVectorOPCITEMVQT(pItemVQT, dwCount);
			srv.stream << dwTransactionID;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> *pdwCancelID;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::RefreshMaxAge(/* [in] */ DWORD dwMaxAge
		, /* [in] */ DWORD dwTransactionID
		, /* [out] */ DWORD *pdwCancelID)
	{
		try{
			NkError::CBaseException::check_arg(pdwCancelID, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 32, L"IOPCAsyncIO3::RefreshMaxAge()");

			srv.stream << dwMaxAge;
			srv.stream << dwTransactionID;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> *pdwCancelID;
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	//IOPCItemDeadbandMgt

	STDMETHODIMP COPCGroupNear::SetItemDeadband( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ FLOAT *pPercentDeadband
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pPercentDeadband, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 33, L"IOPCItemDeadbandMgt::SetItemDeadband()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkCom::CVector<FLOAT, DWORD>(pPercentDeadband, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::GetItemDeadband( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][size_is][out] */ FLOAT **ppPercentDeadband
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppPercentDeadband, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 34, L"IOPCItemDeadbandMgt::GetItemDeadband()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<FLOAT, DWORD> PercentDeadband;
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> PercentDeadband;
				srv.stream >> Errors;
				PercentDeadband.detach(ppPercentDeadband);
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::ClearItemDeadband( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 35, L"IOPCItemDeadbandMgt::ClearItemDeadband()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	//IOPCItemSamplingMgt

	STDMETHODIMP COPCGroupNear::SetItemSamplingRate(/* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ DWORD *pdwRequestedSamplingRate
		, /* [size_is][size_is][out] */ DWORD **ppdwRevisedSamplingRate
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwRequestedSamplingRate, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppdwRevisedSamplingRate, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 36, L"IOPCItemSamplingMgt::SetItemSamplingRate()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkCom::CVector<DWORD, DWORD>(pdwRequestedSamplingRate, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<DWORD, DWORD> dwRevisedSamplingRate;
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> dwRevisedSamplingRate;
				srv.stream >> Errors;
				dwRevisedSamplingRate.detach(ppdwRevisedSamplingRate);
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::GetItemSamplingRate(/* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][size_is][out] */ DWORD **ppdwSamplingRate
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppdwSamplingRate, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 37, L"IOPCItemSamplingMgt::GetItemSamplingRate()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<DWORD, DWORD> dwSamplingRate;
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> dwSamplingRate;
				srv.stream >> Errors;
				dwSamplingRate.detach(ppdwSamplingRate);
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::ClearItemSamplingRate(/* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 38, L"IOPCItemSamplingMgt::ClearItemSamplingRate()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::SetItemBufferEnable( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][in] */ BOOL *pbEnable
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pbEnable, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 39, L"IOPCItemSamplingMgt::SetItemBufferEnable()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream << NkCom::CVector<BOOL, DWORD>(pbEnable, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				srv.stream >> Errors;
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::GetItemBufferEnable( /* [in] */ DWORD dwCount
		, /* [size_is][in] */ OPCHANDLE *phServer
		, /* [size_is][size_is][out] */ BOOL **ppbEnable
		, /* [size_is][size_is][out] */ HRESULT **ppErrors)
	{
		try{
			NkError::CBaseException::check_arg(phServer, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppbEnable, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(ppErrors, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 40, L"IOPCItemSamplingMgt::GetItemBufferEnable()");

			srv.stream << NkCom::CVector<OPCHANDLE, DWORD>(phServer, dwCount);
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				NkCom::CVector<HRESULT, DWORD> Errors;
				NkCom::CVector<BOOL, DWORD> bEnable;
				srv.stream >> bEnable;
				srv.stream >> Errors;
				bEnable.detach(ppbEnable);
				Errors.detach(ppErrors);
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	//IDataObject

	STDMETHODIMP COPCGroupNear::GetData( /* [unique][in] */ FORMATETC *pformatetcIn
		, /* [out] */ STGMEDIUM *pmedium)
	{
		NkTrace::CTrace::trace_info("client call: COPCGroupNear::GetData() but not implemented");
		return E_NOTIMPL;
	}

	STDMETHODIMP COPCGroupNear::GetDataHere( /* [unique][in] */ FORMATETC *pformatetc
		, /* [out][in] */ STGMEDIUM *pmedium)
	{
		NkTrace::CTrace::trace_info("client call: COPCGroupNear::GetDataHere() but not implemented");
		return E_NOTIMPL;
	}

	STDMETHODIMP COPCGroupNear::QueryGetData( /* [unique][in] */ FORMATETC *pformatetc)
	{
		NkTrace::CTrace::trace_info("client call: COPCGroupNear::QueryGetData() but not implemented");
		return E_NOTIMPL;
	}

	STDMETHODIMP COPCGroupNear::GetCanonicalFormatEtc( /* [unique][in] */ FORMATETC *pformatectIn
		, /* [out] */ FORMATETC *pformatetcOut)
	{
		NkTrace::CTrace::trace_info("client call: COPCGroupNear::GetCanonicalFormatEtc() but not implemented");
		return E_NOTIMPL;
	}

	STDMETHODIMP COPCGroupNear::SetData( /* [unique][in] */ FORMATETC *pformatetc
		, /* [unique][in] */ STGMEDIUM *pmedium
		, /* [in] */ BOOL fRelease)
	{
		NkTrace::CTrace::trace_info("client call: COPCGroupNear::SetData() but not implemented");
		return E_NOTIMPL;
	}

	STDMETHODIMP COPCGroupNear::EnumFormatEtc( /* [in] */ DWORD dwDirection
		, /* [out] */ IEnumFORMATETC **ppenumFormatEtc)
	{
		NkTrace::CTrace::trace_info("client call: COPCGroupNear::EnumFormatEtc() but not implemented");
		return E_NOTIMPL;
	}

	STDMETHODIMP COPCGroupNear::DAdvise(
		/* [in] */ FORMATETC *pformatetc
		, /* [in] */ DWORD advf
		, /* [unique][in] */ IAdviseSink *pAdvSink
		, /* [out] */ DWORD *pdwConnection)
	{
		try{
			NkError::CBaseException::check_arg(pformatetc, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pAdvSink, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pdwConnection, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 41, L"IDataObject::DAdvise()");

			ULONG32 id;
			NkCom::CComPtr<IUnknown> spSink(m_pSrv->create_far_proxy(pAdvSink, IID_IAdviseSink, id));

			srv.stream << id;
			srv.stream.write(pformatetc, sizeof(FORMATETC));
			srv.stream << advf;
			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				srv.stream >> *pdwConnection;
				spSink.Detach();
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::DUnadvise( /* [in] */ DWORD dwConnection)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 42, L"IDataObject::DUnadvise()");

			srv.stream << dwConnection;
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::EnumDAdvise( /* [out] */ IEnumSTATDATA **ppenumAdvise)
	{
		NkTrace::CTrace::trace_info("client call: COPCGroupNear::EnumDAdvise() but not implemented");
		return E_NOTIMPL;
	}

	//IOPCPublicGroupStateMgt

	STDMETHODIMP COPCGroupNear::GetState( /* [out] */ BOOL *pPublic)
	{
		try{
			NkError::CBaseException::check_arg(pPublic, __FILE__, __LINE__);

			CNearUnknown::CCallFctGuard srv(this, 43, L"IOPCPublicGroupStateMgt::GetState()");

			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)){
				srv.stream >> *pPublic;
			}
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}

	STDMETHODIMP COPCGroupNear::MoveToPublic(void)
	{
		try{
			CNearUnknown::CCallFctGuard srv(this, 44, L"IOPCPublicGroupStateMgt::MoveToPublic()");

			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e){
			return handle_error(e);
		}
	}
}