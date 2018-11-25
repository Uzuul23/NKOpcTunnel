#pragma once
#include "types/uniquepointer.h"
#include "com/connectionpointfar.h"
#include "com/enumfar.h"
#include "opc/opcadvisesinknear.h"
#include "opc/opctypes.h"

namespace NkOPC
{
	class COPCGroupFar : public NkCom::CComFarUnknown
	{
	public:
		COPCGroupFar(NkCom::CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid);
		COPCGroupFar(NkCom::CServer* pSrv, IUnknown* p, ULONG32* pid = 0);
		virtual ~COPCGroupFar();

		HRESULT query_interface(REFIID iid);

		void call(ULONG32 index, NkStream::CStream& stream);

		//IOPCGroupStateMgt

		HRESULT GetState(NkStream::CStream& stream);
		HRESULT SetState(NkStream::CStream& stream);
		HRESULT SetName(NkStream::CStream& stream);
		HRESULT CloneGroup(NkStream::CStream& stream);
		HRESULT SetKeepAlive(NkStream::CStream& stream);
		HRESULT GetKeepAlive(NkStream::CStream& stream);

		//IOPCItemMgt

		HRESULT AddItems(NkStream::CStream& stream);
		HRESULT ValidateItems(NkStream::CStream& stream);
		HRESULT RemoveItems(NkStream::CStream& stream);
		HRESULT SetActiveState(NkStream::CStream& stream);
		HRESULT SetClientHandles(NkStream::CStream& stream);
		HRESULT SetDatatypes(NkStream::CStream& stream);
		HRESULT CreateEnumerator(NkStream::CStream& stream);

		//IConnectionPointContainer

		HRESULT EnumConnectionPoints(NkStream::CStream& stream);
		HRESULT FindConnectionPoint(NkStream::CStream& stream);

		//IOPCSyncIO

		HRESULT Read(NkStream::CStream& stream);
		HRESULT Write(NkStream::CStream& stream);

		//IOPCSyncIO2

		HRESULT ReadMaxAge(NkStream::CStream& stream);
		HRESULT WriteVQT(NkStream::CStream& stream);

		//IOPCAsyncIO

		HRESULT AsyncRead(NkStream::CStream& stream);
		HRESULT AsyncWrite(NkStream::CStream& stream);
		HRESULT Refresh(NkStream::CStream& stream);
		HRESULT Cancel(NkStream::CStream& stream);

		//IOPCAsyncIO2

		HRESULT AsyncRead2(NkStream::CStream& stream);
		HRESULT AsyncWrite2(NkStream::CStream& stream);
		HRESULT Refresh2(NkStream::CStream& stream);
		HRESULT Cancel2(NkStream::CStream& stream);
		HRESULT SetEnable(NkStream::CStream& stream);
		HRESULT GetEnable(NkStream::CStream& stream);

		//IOPCAsyncIO3

		HRESULT ReadMaxAge3(NkStream::CStream& stream);
		HRESULT WriteVQT3(NkStream::CStream& stream);
		HRESULT RefreshMaxAge(NkStream::CStream& stream);

		//IOPCItemDeadbandMgt

		HRESULT SetItemDeadband(NkStream::CStream& stream);
		HRESULT GetItemDeadband(NkStream::CStream& stream);
		HRESULT ClearItemDeadband(NkStream::CStream& stream);

		//IOPCItemSamplingMgt

		HRESULT SetItemSamplingRate(NkStream::CStream& stream);
		HRESULT GetItemSamplingRate(NkStream::CStream& stream);
		HRESULT ClearItemSamplingRate(NkStream::CStream& stream);
		HRESULT SetItemBufferEnable(NkStream::CStream& stream);
		HRESULT GetItemBufferEnable(NkStream::CStream& stream);

		//IDataObject

		HRESULT DAdvise(NkStream::CStream& stream);
		HRESULT DUnadvise(NkStream::CStream& stream);

		//IOPCPublicGroupStateMgt

		HRESULT GetState2(NkStream::CStream& stream);
		HRESULT MoveToPublic(NkStream::CStream& stream);

	private:
		COPCGroupFar();
		COPCGroupFar &operator =(const COPCGroupFar&);
		COPCGroupFar(const COPCGroupFar&);
	};

	typedef NkCom::CComEnumUnkFar<IEnumUnknown, IUnknown, COPCGroupFar> CEnumOPCGroupFar;
	typedef NkCom::CComPtrFar<IEnumUnknown, CEnumOPCGroupFar> CComPtrFarEnumOPCGroups;
}