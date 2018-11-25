#pragma once
#include "stream/stream.h"
#include "com/comfarsrv.h"
#include "com/enumfar.h"
#include <ocidl.h>

namespace NkCom
{
	class CConnectionPointFar : public CComFarUnknown
	{
	public:
		CConnectionPointFar(CServer* pSrv, IConnectionPoint* p, ULONG32* pid = 0);
		CConnectionPointFar(NkCom::CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid);

		HRESULT query_interface(REFIID iid);
		void call(ULONG32 index, NkStream::CStream& stream);
		void GetConnectionInterface(NkStream::CStream& stream);
		void GetConnectionPointContainer(NkStream::CStream& stream);
		void Advise(NkStream::CStream& stream);
		void Unadvise(NkStream::CStream& stream);
		void EnumConnections(NkStream::CStream& stream);

	private:
		CConnectionPointFar();
		CConnectionPointFar &operator =(const CConnectionPointFar&);
		CConnectionPointFar(const CConnectionPointFar&);

		NkCom::CComPtr<IConnectionPoint> m_spConnectionPoint;
	};

	typedef NkCom::CComPtrFar < IEnumConnectionPoints
		, CComEnumUnkFar < IEnumConnectionPoints, IConnectionPoint
		, CConnectionPointFar >> CComPtrFarEnumCP;
}