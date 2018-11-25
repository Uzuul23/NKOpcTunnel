#include "stdafx.h"
#include "safearray.h"
#include "stream/stream.h"
#include "com/variant.h"
#include "com/comstring.h"

namespace NkCom
{
	void CSafeArray::read(NkStream::CStream& stream)
	{
		read(stream, _psa);
	}

	void CSafeArray::write(NkStream::CStream& stream)
	{
		clear();
		write(stream, _psa);
	}

	void CSafeArray::read(NkStream::CStream& stream, SAFEARRAY*& psa)
	{
		VARTYPE vt = VT_EMPTY;
		NkError::CBaseException::check_result(::SafeArrayGetVartype(psa, &vt), __FILE__, __LINE__);
		stream << vt;
		stream << psa->cbElements;
		stream << psa->cDims;
		ULONG cElements = 0;
		for (USHORT loop = 0; loop < psa->cDims; loop++) {
			stream << psa->rgsabound[loop].lLbound;
			stream << psa->rgsabound[loop].cElements;
			cElements += psa->rgsabound[loop].cElements;
		}
		switch (vt)
		{
		case VT_I2: case VT_I4: case VT_R4: case VT_R8: case VT_CY: case VT_DATE: case VT_ERROR:
		case VT_BOOL:case VT_DECIMAL: case VT_I1: case VT_UI1: case VT_UI2: case VT_UI4:
		case VT_I8: case VT_UI8: case VT_INT:
		case VT_UINT: {
			const BYTE *p = 0;
			NkError::CBaseException::check_result(::SafeArrayAccessData(psa, (void HUGEP**)&p), __FILE__, __LINE__);
			size_t cb = cElements * psa->cbElements;
			stream.write(p, cb);
			NkError::CBaseException::check_result(::SafeArrayUnaccessData(psa), __FILE__, __LINE__);
			break;
		}
		case VT_BSTR: {
			const BSTR *p = 0;
			NkError::CBaseException::check_result(::SafeArrayAccessData(psa, (void HUGEP**)&p), __FILE__, __LINE__);
			for (ULONG loop = 0; loop < cElements; loop++) {
				CBSTR::read(stream, p[loop]);
			}
			NkError::CBaseException::check_result(::SafeArrayUnaccessData(psa), __FILE__, __LINE__);
			break;
		}
		case VT_VARIANT: {
			const VARIANT *p = 0;
			NkError::CBaseException::check_result(::SafeArrayAccessData(psa, (void HUGEP**)&p), __FILE__, __LINE__);
			for (ULONG loop = 0; loop < cElements; loop++) {
				CVariant::read(stream, p[loop]);
			}
			NkError::CBaseException::check_result(::SafeArrayUnaccessData(psa), __FILE__, __LINE__);
			break;
		}
		default: NkError::CBaseException::check(false, E_INVALIDARG, __FILE__, __LINE__);
		}
	}

	void CSafeArray::write(NkStream::CStream& stream, SAFEARRAY*& psa)
	{
		VARTYPE vt;
		ULONG cbElements;
		USHORT cDims;
		stream >> vt;
		stream >> cbElements;
		stream >> cDims;
		SAFEARRAYBOUND *prgsa = new SAFEARRAYBOUND[cDims];

		try {
			ULONG cElements = 0;
			for (USHORT loop = 0; loop < cDims; ++loop) {
				stream >> prgsa[loop].lLbound;
				stream >> prgsa[loop].cElements;
				cElements += prgsa[loop].cElements;
			}

			psa = ::SafeArrayCreate(vt, cDims, prgsa);
			NkError::CBaseException::check_alloc(psa, __FILE__, __LINE__);

			if (vt == VT_BSTR)
			{
				BSTR *p = 0;
				NkError::CBaseException::check_result(::SafeArrayAccessData(psa, (void HUGEP**)&p), __FILE__, __LINE__);
				for (ULONG loop = 0; loop < cElements; loop++) {
					CBSTR::write(stream, p[loop]);
				}
				NkError::CBaseException::check_result(::SafeArrayUnaccessData(psa), __FILE__, __LINE__);
			}
			else if (vt == VT_VARIANT)
			{
				VARIANT *p = 0;
				NkError::CBaseException::check_result(::SafeArrayAccessData(psa, (void HUGEP**)&p), __FILE__, __LINE__);
				for (ULONG loop = 0; loop < cElements; loop++) {
					CVariant::write(stream, p[loop]);
				}
				NkError::CBaseException::check_result(::SafeArrayUnaccessData(psa), __FILE__, __LINE__);
			}
			else
			{
				BYTE *p = 0;
				NkError::CBaseException::check_result(::SafeArrayAccessData(psa, (void HUGEP**)&p), __FILE__, __LINE__);
				size_t cb = cElements * psa->cbElements;
				stream.read(p, cb);
				NkError::CBaseException::check_result(::SafeArrayUnaccessData(psa), __FILE__, __LINE__);
			}

			delete[] prgsa;
		}
		catch (...) {
			delete[] prgsa;
			throw;
		}
	}

	CSafeArray::CSafeArray() : _psa(0)
	{
	}

	CSafeArray::CSafeArray(SAFEARRAY* psa) : _psa(copy(psa))
	{
	}

	CSafeArray::~CSafeArray()
	{
		clear();
	}

	CSafeArray::operator const SAFEARRAY*()
	{
		return _psa;
	}

	void CSafeArray::attach(SAFEARRAY* p)
	{
		clear();
		_psa = p;
	}

	void CSafeArray::detach(SAFEARRAY** pp)
	{
		(*pp) = _psa;
		_psa = 0;
	}

	void CSafeArray::clear()
	{
		if (_psa) {
			::SafeArrayDestroy(_psa);
			_psa = 0;
		}
	}

	SAFEARRAY* CSafeArray::copy(SAFEARRAY* psa)
	{
		SAFEARRAY* pNew = 0;
		NkError::CBaseException::check_result(::SafeArrayCopy(psa, &pNew), __FILE__, __LINE__);
		return pNew;
	}
}