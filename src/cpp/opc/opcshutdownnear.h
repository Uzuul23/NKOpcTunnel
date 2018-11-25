#pragma once
#include "com/nearunknown.h"
#include "opc/opctypes.h"

namespace NkOPC
{
	class COPCShutdownNear : public NkCom::CNearUnknown<COPCShutdownNear>
		, public IOPCShutdown
	{
	public:
		COPCShutdownNear(NkCom::CServer* p, ULONG32 id);;
		virtual ~COPCShutdownNear();

		IMPL_INTERFACE1(IOPCShutdown);

		STDMETHOD(ShutdownRequest)(/* [string][in] */ LPCWSTR szReason);

	private:
		COPCShutdownNear();
		COPCShutdownNear &operator =(const COPCShutdownNear&);
		COPCShutdownNear(const COPCShutdownNear&);
	};
}