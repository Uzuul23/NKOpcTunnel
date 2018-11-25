#pragma once
#include "opc/opccomn.h"
#include "com/nearunknown.h"
#include "opc/opcenum.h"

namespace NkOPC
{
	class COPCServerList2Near : public NkCom::CNearUnknown<COPCServerList2Near>
		, public IOPCServerList2
	{
	public:
		COPCServerList2Near(NkCom::CServer* p, ULONG32 id);

		IMPL_INTERFACE1(IOPCServerList2);

		STDMETHOD(EnumClassesOfCategories)(
			/* [in] */ ULONG cImplemented,
			/* [size_is][in] */ CATID rgcatidImpl[],
			/* [in] */ ULONG cRequired,
			/* [size_is][in] */ CATID rgcatidReq[],
			/* [out] */ IOPCEnumGUID **ppenumClsid);
		STDMETHOD(GetClassDetails)(
			/* [in] */ REFCLSID clsid,
			/* [out] */ LPOLESTR *ppszProgID,
			/* [out] */ LPOLESTR *ppszUserType,
			/* [out] */ LPOLESTR *ppszVerIndProgID);
		STDMETHOD(CLSIDFromProgID)(
			/* [in] */ LPCOLESTR szProgId,
			/* [out] */ LPCLSID clsid);

	private:
		COPCServerList2Near();
		COPCServerList2Near &operator =(const COPCServerList2Near&);
		COPCServerList2Near(const COPCServerList2Near&);
	};
}