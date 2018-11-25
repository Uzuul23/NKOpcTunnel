#pragma once
#include "com/comfarunknown.h"
#include "stream/stream.h"
#include "opc/opccomn.h"

namespace NkOPC
{
	class COPCServerList2Far : public NkCom::CComFarUnknown
	{
	public:
		COPCServerList2Far(NkCom::CServer* pSrv, REFCLSID rclsid
			, DWORD dwClsContext, REFIID riid);

		NK_UNKNOWN_IMP;

		HRESULT query_interface(REFIID iid);
		void call(ULONG32 index, NkStream::CStream& stream);
		void EnumClassesOfCategories(NkStream::CStream& stream);
		void GetClassDetails(NkStream::CStream& stream);
		void CLSIDFromProgID(NkStream::CStream& stream);

	private:
		COPCServerList2Far();
		COPCServerList2Far &operator =(const COPCServerList2Far&);
		COPCServerList2Far(const COPCServerList2Far&);
		NkCom::CComPtr<IOPCServerList2> m_spOPCServerList2;
	};
}