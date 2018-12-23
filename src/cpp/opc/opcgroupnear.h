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

#pragma once
#include "types/uniquepointer.h"
#include "com/unknownimpl.h"
#include "com/comstring.h"
#include "opc/opctypes.h"
#include "com/enumnear.h"
#include "com/connectionpointnear.h"
#include "opc/opccomn.h"
#include "opc/opcenum.h"

namespace NkOPC
{
	class COPCGroupNear
		: public NkCom::CNearUnknown < COPCGroupNear >
		, public IOPCGroupStateMgt2
		, public IOPCItemMgt
		, public IConnectionPointContainer
		, public IOPCSyncIO2
		, public IOPCAsyncIO
		, public IOPCAsyncIO3
		, public IOPCItemDeadbandMgt
		, public IOPCItemSamplingMgt
		, public IDataObject
		, public IOPCPublicGroupStateMgt
	{
	public:
		COPCGroupNear(NkCom::CServer* p, ULONG32 id);

		IMPL_ADDREF_RELEASE;
		STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject);

		//IOPCGroupStateMgt

		STDMETHOD(GetState)(DWORD *pUpdateRate, BOOL *pActive, LPWSTR *ppName
			, LONG *pTimeBias, FLOAT *pPercentDeadband, DWORD *pLCID
			, OPCHANDLE *phClientGroup, OPCHANDLE *phServerGroup);
		STDMETHOD(SetState)(DWORD *pRequestedUpdateRate, DWORD *pRevisedUpdateRate
			, BOOL *pActive, LONG *pTimeBias, FLOAT *pPercentDeadband
			, DWORD *pLCID, OPCHANDLE *phClientGroup);
		STDMETHOD(SetName)(LPCWSTR szName);
		STDMETHOD(CloneGroup)(LPCWSTR szName, REFIID riid, LPUNKNOWN *ppUnk);

		//IOPCGroupStateMgt2

		STDMETHOD(SetKeepAlive)(DWORD dwKeepAliveTime, DWORD *pdwRevisedKeepAliveTime);
		STDMETHOD(GetKeepAlive)(DWORD *pdwKeepAliveTime);

		//IOPCItemMgt

		STDMETHOD(AddItems)(DWORD dwCount, OPCITEMDEF *pItemArray
			, OPCITEMRESULT **ppAddResults, HRESULT **ppErrors);
		STDMETHOD(ValidateItems)(DWORD dwCount, OPCITEMDEF *pItemArray
			, BOOL bBlobUpdate, OPCITEMRESULT **ppValidationResults
			, HRESULT **ppErrors);
		STDMETHOD(RemoveItems)(DWORD dwCount, OPCHANDLE *phServer
			, HRESULT **ppErrors);
		STDMETHOD(SetActiveState)(DWORD dwCount, OPCHANDLE *phServer
			, BOOL bActive, HRESULT **ppErrors);
		STDMETHOD(SetClientHandles)(DWORD dwCount, OPCHANDLE *phServer
			, OPCHANDLE *phClient, HRESULT **ppErrors);
		STDMETHOD(SetDatatypes)(DWORD dwCount, OPCHANDLE *phServer
			, VARTYPE *pRequestedDatatypes, HRESULT **ppErrors);
		STDMETHOD(CreateEnumerator)(REFIID riid, LPUNKNOWN *ppUnk);

		//IConnectionPointContainer

		STDMETHOD(EnumConnectionPoints)(IEnumConnectionPoints **ppEnum);
		STDMETHOD(FindConnectionPoint)(REFIID riid, IConnectionPoint **ppCP);

		//IOPCSyncIO

		STDMETHOD(Read)(OPCDATASOURCE dwSource, DWORD dwCount, OPCHANDLE *phServer
			, OPCITEMSTATE **ppItemValues, HRESULT **ppErrors);
		STDMETHOD(Write)(DWORD dwCount, OPCHANDLE *phServer, VARIANT *pItemValues
			, HRESULT **ppErrors);

		//IOPCSyncIO2

		STDMETHOD(ReadMaxAge)(DWORD dwCount, OPCHANDLE *phServer
			, DWORD *pdwMaxAge, VARIANT **ppvValues, WORD **ppwQualities
			, FILETIME **ppftTimeStamps, HRESULT **ppErrors);
		STDMETHOD(WriteVQT)(DWORD dwCount, OPCHANDLE *phServer
			, OPCITEMVQT *pItemVQT, HRESULT **ppErrors);

		//IOPCAsyncIO

		STDMETHOD(Read)(DWORD dwConnection, OPCDATASOURCE dwSource
			, DWORD dwCount, OPCHANDLE *phServer, DWORD *pTransactionID
			, HRESULT **ppErrors);
		STDMETHOD(Write)(DWORD dwConnection, DWORD dwCount, OPCHANDLE *phServer
			, VARIANT *pItemValues, DWORD *pTransactionID, HRESULT **ppErrors);
		STDMETHOD(Refresh)(DWORD dwConnection, OPCDATASOURCE dwSource
			, DWORD *pTransactionID);
		STDMETHOD(Cancel)(DWORD dwTransactionID);

		//IOPCAsyncIO2

		STDMETHOD(Read)(DWORD dwCount, OPCHANDLE *phServer, DWORD dwTransactionID
			, DWORD *pdwCancelID, HRESULT **ppErrors);
		STDMETHOD(Write)(DWORD dwCount, OPCHANDLE *phServer, VARIANT *pItemValues
			, DWORD dwTransactionID, DWORD *pdwCancelID, HRESULT **ppErrors);
		STDMETHOD(Refresh2)(OPCDATASOURCE dwSource, DWORD dwTransactionID
			, DWORD *pdwCancelID);
		STDMETHOD(Cancel2)(DWORD dwCancelID);
		STDMETHOD(SetEnable)(BOOL bEnable);
		STDMETHOD(GetEnable)(BOOL *pbEnable);

		//IOPCAsyncIO3

		STDMETHOD(ReadMaxAge)(DWORD dwCount, OPCHANDLE *phServer, DWORD *pdwMaxAge
			, DWORD dwTransactionID, DWORD *pdwCancelID, HRESULT **ppErrors);
		STDMETHOD(WriteVQT)(DWORD dwCount, OPCHANDLE *phServer
			, OPCITEMVQT *pItemVQT, DWORD dwTransactionID, DWORD *pdwCancelID
			, HRESULT **ppErrors);
		STDMETHOD(RefreshMaxAge)(DWORD dwMaxAge, DWORD dwTransactionID
			, DWORD *pdwCancelID);

		//IOPCItemDeadbandMgt

		STDMETHOD(SetItemDeadband)(DWORD dwCount, OPCHANDLE *phServer
			, FLOAT *pPercentDeadband, HRESULT **ppErrors);
		STDMETHOD(GetItemDeadband)(DWORD dwCount, OPCHANDLE *phServer
			, FLOAT **ppPercentDeadband, HRESULT **ppErrors);
		STDMETHOD(ClearItemDeadband)(DWORD dwCount, OPCHANDLE *phServer
			, HRESULT **ppErrors);

		//IOPCItemSamplingMgt

		STDMETHOD(SetItemSamplingRate)(DWORD dwCount, OPCHANDLE *phServer
			, DWORD *pdwRequestedSamplingRate, DWORD **ppdwRevisedSamplingRate
			, HRESULT **ppErrors);
		STDMETHOD(GetItemSamplingRate)(DWORD dwCount, OPCHANDLE *phServer
			, DWORD **ppdwSamplingRate, HRESULT **ppErrors);
		STDMETHOD(ClearItemSamplingRate)(DWORD dwCount, OPCHANDLE *phServer
			, HRESULT **ppErrors);
		STDMETHOD(SetItemBufferEnable)(DWORD dwCount, OPCHANDLE *phServer
			, BOOL *pbEnable, HRESULT **ppErrors);
		STDMETHOD(GetItemBufferEnable)(DWORD dwCount, OPCHANDLE *phServer
			, BOOL **ppbEnable, HRESULT **ppErrors);

		//IDataObject

		STDMETHOD(GetData)(FORMATETC *pformatetcIn, STGMEDIUM *pmedium);
		STDMETHOD(GetDataHere)(FORMATETC *pformatetc
			, STGMEDIUM *pmedium);
		STDMETHOD(QueryGetData)(FORMATETC *pformatetc);
		STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *pformatectIn
			, FORMATETC *pformatetcOut);
		STDMETHOD(SetData)(FORMATETC *pformatetc, STGMEDIUM *pmedium
			, BOOL fRelease);
		STDMETHOD(EnumFormatEtc)(DWORD dwDirection
			, IEnumFORMATETC **ppenumFormatEtc);
		STDMETHOD(DAdvise)(FORMATETC *pformatetc, DWORD advf
			, IAdviseSink *pAdvSink, DWORD *pdwConnection);
		STDMETHOD(DUnadvise)(DWORD dwConnection);
		STDMETHOD(EnumDAdvise)(IEnumSTATDATA **ppenumAdvise);

		//IOPCPublicGroupStateMgt

		STDMETHOD(GetState)(BOOL *pPublic);
		STDMETHOD(MoveToPublic)(void);
	};

	typedef NkCom::CComEnumUnkNear<IEnumUnknown, IUnknown, COPCGroupNear> CEnumOPCGroupNear;
	typedef NkCom::CComPtrNear<IEnumUnknown, CEnumOPCGroupNear> CComPtrNearEnumOPCGroups;
}