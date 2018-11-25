#include "stdafx.h"
#include "opcenumguidnear.h"
#include "stream/stream.h"
#include "com/vector.h"

namespace NkOPC
{
	COPCEnumGUIDNear::COPCEnumGUIDNear(NkCom::CServer* p, ULONG32 id) : CNearUnknown(p, id)
	{
	}

	STDMETHODIMP COPCEnumGUIDNear::Next(/* [in] */ ULONG celt, /* [length_is][size_is][out] */ GUID *rgelt, /* [out] */ ULONG *pceltFetched)
	{
		try {
			NkError::CBaseException::check_arg(pceltFetched, __FILE__, __LINE__);
			CNearUnknown::CCallFctGuard srv(this, 1, L"IEnumGUID::Next()");

			srv.stream << celt;
			srv.stream.flush();
			srv.stream >> srv.result;

			if (SUCCEEDED(srv.result)) {
				NkCom::CVector<GUID> guids(rgelt, celt);
				srv.stream >> guids;
			}

			return srv.result;
		}
		catch (NkError::CException& e) {
			handle_error(e);
			return E_FAIL;
		}
	}

	STDMETHODIMP COPCEnumGUIDNear::Skip(/* [in] */ ULONG celt)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 2, L"IEnumGUID::Skip()");

			srv.stream << celt;
			srv.stream.flush();
			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e)
		{
			handle_error(e);
			return E_FAIL;
		}
	}

	STDMETHODIMP COPCEnumGUIDNear::Reset(void)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 3, L"IEnumGUID::Reset()");

			srv.stream.flush();
			srv.stream >> srv.result;
			return srv.result;
		}
		catch (NkError::CException& e) {
			handle_error(e);
			return E_FAIL;
		}
	}

	STDMETHODIMP COPCEnumGUIDNear::Clone(/* [out] */ IOPCEnumGUID **ppenum)
	{
		try {
			CNearUnknown::CCallFctGuard srv(this, 4, L"IEnumGUID::Clone()");

			srv.stream.flush();

			srv.stream >> srv.result;
			if (SUCCEEDED(srv.result)) {
				NkCom::CComPtrNear<IOPCEnumGUID, COPCEnumGUIDNear> spEnum(m_pSrv);
				srv.stream >> spEnum;
				(*ppenum) = spEnum.Detach();
			}
			return srv.result;
		}
		catch (NkError::CException& e) {
			handle_error(e);
			return E_FAIL;
		}
	}
}