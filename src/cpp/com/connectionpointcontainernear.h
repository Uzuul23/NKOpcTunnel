#pragma once
#include "com/comptr.h"
#include "com/nearunknown.h"
#include "com/comnearsrv.h"

namespace NkCom
{
	class CConnectionPointContainerNear : public NkCom::CNearUnknown < CConnectionPointContainerNear >
		, public IConnectionPointContainer
	{
	public:
		CConnectionPointContainerNear(CServer* p, ULONG32 id) : CNearUnknown(p, id){};

		IMPL_INTERFACE1(IConnectionPointContainer);

		STDMETHOD(EnumConnectionPoints)(IEnumConnectionPoints **ppEnum);
		STDMETHOD(FindConnectionPoint)(REFIID riid, IConnectionPoint **ppCP);

	private:
		CConnectionPointContainerNear();
		CConnectionPointContainerNear &operator =(const CConnectionPointContainerNear&);
		CConnectionPointContainerNear(const CConnectionPointContainerNear&);
	};

	typedef CComPtrNear<IConnectionPointContainer, CConnectionPointContainerNear> CComPtrNearCPC;
}