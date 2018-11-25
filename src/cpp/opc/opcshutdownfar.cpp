#include "stdafx.h"
#include "opcshutdownfar.h"
#include "com/comstring.h"

namespace NkOPC
{

	COPCShutdownFar::COPCShutdownFar(NkCom::CServer* pSrv, IUnknown* p, ULONG32* pid)
		: CComFarUnknown(pSrv, p, pid)
	{

	}

	HRESULT COPCShutdownFar::query_interface(REFIID iid)
	{
		if (iid == IID_IUnknown
			|| iid == IID_IOPCShutdown) {
			return internal_query_interface(iid);
		}

#ifdef NK_TRACE_QUERY_INTERFACE

		NkTrace::CTrace& Trace = NkTrace::CTrace::Instance();
		if (Trace.IsTraceLevel3()) {
			NkCom::CComString str;
			StringFromIID(iid, &str);
			Trace.Trace(NkTrace::CTrace::TraceDetailed
				, L"IOPCGroup::QueryInterface(%s) E_NOINTERFACE"
				, str.data());
		}

#endif
		return E_NOINTERFACE;
	}

	void COPCShutdownFar::call(ULONG32 index, NkStream::CStream& stream)
	{
		switch (index) {
		case 1: ShutdownRequest(stream); break;
		}

		if (NkTrace::CTrace::IsTraceLevel3()) {
			static const WCHAR* pszFctNames[] = {
				L"dummy",
				L"IOPCShutdown::ShutdownRequest()"
			};
			if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
				_ASSERT(0);
				return;
			}
			NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
		}
	}

	void COPCShutdownFar::ShutdownRequest(NkStream::CStream& stream)
	{
		NkCom::CComString Reason;
		stream >> Reason;

		NkCom::CComPtr<IOPCShutdown> sp;
		internal_get_interface(sp);

		stream << sp->ShutdownRequest(Reason);
		stream.flush();
	}


}


