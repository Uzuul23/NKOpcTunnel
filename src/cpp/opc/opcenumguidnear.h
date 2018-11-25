#pragma once
#include "com/nearunknown.h"
#include "opc/opcenum.h"
#include "com/comptr.h"

namespace NkOPC
{
	class COPCEnumGUIDNear : public NkCom::CNearUnknown<COPCEnumGUIDNear>, public IOPCEnumGUID
	{
	private:
		COPCEnumGUIDNear();
		COPCEnumGUIDNear &operator =(const COPCEnumGUIDNear&);
		COPCEnumGUIDNear(const COPCEnumGUIDNear&);

	public:
		COPCEnumGUIDNear(NkCom::CServer* p, ULONG32 id);;

		IMPL_INTERFACE1(IOPCEnumGUID);

		STDMETHOD(Next)(/* [in] */ ULONG celt,
			/* [length_is][size_is][out] */ GUID *rgelt,
			/* [out] */ ULONG *pceltFetched);

		STDMETHOD(Skip)(/* [in] */ ULONG celt);

		STDMETHOD(Reset)(void);

		STDMETHOD(Clone)(/* [out] */ IOPCEnumGUID **ppenum);
	};

	inline NkStream::CStream& operator<<(NkStream::CStream& l, const NkCom::CComPtr<IOPCEnumGUID>& r)
	{
		//r.read(l);
		return l;
	}

	inline NkStream::CStream& operator>>(NkStream::CStream& l, NkCom::CComPtr<IOPCEnumGUID>& r)
	{
		//r.write(l);
		return l;
	}
}
