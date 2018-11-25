#include "stdafx.h"
#include "opcshutdownnear.h"
#include "types/String.h"

namespace NkOPC
{
	COPCShutdownNear::COPCShutdownNear(NkCom::CServer* p, ULONG32 id) : CNearUnknown(p, id)
	{
	}

	NkOPC::COPCShutdownNear::~COPCShutdownNear()
	{
	}

	STDMETHODIMP COPCShutdownNear::ShutdownRequest(/* [string][in] */ LPCWSTR szReason)
	{
		try {
			NkType::CString str;
			if (NkTrace::CTrace::IsTraceLevel3()) {
				str.printf(L"IOPCShutdown::ShutdownRequest(%s)", szReason);
			}
			CNearUnknown::CCallFctGuard srv(this, 1, str);
			srv.stream << szReason;
			srv.stream.flush();

			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e) {
			handle_error(e);
			return S_OK;
		}
	}
}