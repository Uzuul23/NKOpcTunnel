#pragma once
#include "com/nearunknown.h"
#include "com/comnearsrv.h"
#include "com/enumnear.h"
#include "com/comptr.h"
#include <OCIdl.h>

namespace NkCom
{
	class CConnectionPointNear : public CNearUnknown < CConnectionPointNear >
		, public IConnectionPoint
	{
	public:
		CConnectionPointNear(CServer* p, ULONG32 id) : CNearUnknown(p, id){};

		IMPL_INTERFACE1(IConnectionPoint);

		STDMETHOD(GetConnectionInterface)(IID *pIID);
		STDMETHOD(GetConnectionPointContainer)(IConnectionPointContainer **ppCPC);
		STDMETHOD(Advise)(IUnknown *pUnkSink, DWORD *pdwCookie);
		STDMETHOD(Unadvise)(DWORD dwCookie);
		STDMETHOD(EnumConnections)(IEnumConnections **ppEnum);

	private:
		CConnectionPointNear();
		CConnectionPointNear &operator =(const CConnectionPointNear&);
		CConnectionPointNear(const CConnectionPointNear&);
	};

	typedef CComPtrNear < IEnumConnectionPoints, CComEnumUnkNear < IEnumConnectionPoints
		, IConnectionPoint, CConnectionPointNear >> CComPtrNearEnumCP;
}