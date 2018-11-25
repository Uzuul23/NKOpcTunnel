#pragma once
#include "com/comfarunknown.h"
#include "opc/opcda.h"

namespace NkOPC
{
	class COPCDataCallbackFar : public NkCom::CComFarUnknown
	{
	public:
		COPCDataCallbackFar(NkCom::CServer* pSrv, IUnknown* p, ULONG32* pid)
			: CComFarUnknown(pSrv, p, pid) {}

		HRESULT query_interface(REFIID iid);
		void call(ULONG32 index, NkStream::CStream& stream);

		//IOPCDataCallback

		void OnDataChange(NkStream::CStream& stream);
		void OnReadComplete(NkStream::CStream& stream);
		void OnWriteComplete(NkStream::CStream& stream);
		void OnCancelComplete(NkStream::CStream& stream);

	private:
		COPCDataCallbackFar();
		COPCDataCallbackFar &operator =(const COPCDataCallbackFar&);
		COPCDataCallbackFar(const COPCDataCallbackFar&);
	};
}