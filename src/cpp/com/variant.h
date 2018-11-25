#pragma once
#include "stream/stream.h"
#include "com/bstr.h"
#include "com/safearray.h"
#include <OaIdl.h>

namespace NkCom
{
	class CVariant
	{
	public:
		CVariant() : _const(false) {
			::VariantInit(&_var);
		}
		CVariant(const VARIANT* p) : _const(true) {
			memcpy(&_var, p, sizeof(VARIANT));
		}
		~CVariant() {
			clear();
		}
		operator VARIANT() {
			return _var;
		}
		void attach(VARIANT* p) {
			clear();
			memcpy(&_var, p, sizeof(VARIANT));
		}
		void detach(VARIANT* p) {
			changeable();
			memcpy(p, &_var, sizeof(VARIANT));
			::VariantInit(&_var);
			_const = false;
		}
		bool isconst() {
			return _const;
		}
		void changeable()
		{
			if (_const) {
				VARIANT var;
				::VariantInit(&var);
				::VariantInit(&_var);
				NkError::CBaseException::check_result(::VariantCopy(&var, &_var), __FILE__, __LINE__);
				memcpy(&_var, &var, sizeof(VARIANT));
				_const = false;
			}
		}
		void clear()
		{
			if (!_const) {
				NkError::CBaseException::check_result(::VariantClear(&_var), __FILE__, __LINE__);
			}
			else {
				::VariantInit(&_var);
			}
			_const = false;
		}
		static void read(NkStream::CStream& stream, const VARIANT& var)
		{
			stream.write_type(VT_VARIANT);
			stream << var.vt;
			stream << var.wReserved1;
			stream << var.wReserved2;
			stream << var.wReserved3;

			if (var.vt & VT_ARRAY) {
				SAFEARRAY* psa = var.parray;
				stream << psa;
				return;
			}

			switch (var.vt)
			{
			case VT_UI1:
			case VT_I1:
			case VT_I2:
			case VT_UI2:
			case VT_BOOL:
			case VT_I4:
			case VT_UI4:
			case VT_R4:
			case VT_INT:
			case VT_UINT:
			case VT_ERROR: stream << var.lVal; break;
			case VT_I8:
			case VT_UI8: stream << var.llVal; break;
			case VT_R8:
			case VT_CY:
			case VT_DATE: stream << var.dblVal; break;
			case VT_BSTR: NkCom::CBSTR::read(stream, var.bstrVal); break;
			case VT_EMPTY: break;
			default: NkError::CBaseException::check(false, E_INVALIDARG, __FILE__, __LINE__);
			}
		}
		static void write(NkStream::CStream& stream, VARIANT& var)
		{
			stream.check_type(VT_VARIANT);
			stream >> var.vt;
			stream >> var.wReserved1;
			stream >> var.wReserved2;
			stream >> var.wReserved3;

			if (var.vt & VT_ARRAY) {
				stream >> var.parray;
				return;
			}

			switch (var.vt)
			{
			case VT_UI1:
			case VT_I1:
			case VT_I2:
			case VT_UI2:
			case VT_BOOL:
			case VT_I4:
			case VT_UI4:
			case VT_R4:
			case VT_INT:
			case VT_UINT:
			case VT_ERROR: stream >> var.lVal; break;
			case VT_I8:
			case VT_UI8: stream >> var.llVal; break;
			case VT_R8:
			case VT_CY:
			case VT_DATE: stream >> var.dblVal; break;
			case VT_BSTR: NkCom::CBSTR::write(stream, var.bstrVal); break;
			case VT_EMPTY: break;
			default: NkError::CBaseException::check(false, E_INVALIDARG, __FILE__, __LINE__);
			}
		}
		void read(NkStream::CStream& stream) const {
			read(stream, _var);
		}
		void write(NkStream::CStream& stream) {
			clear();
			write(stream, _var);
		}
	private:
		VARIANT _var;
		bool _const;
	};

	inline NkStream::CStream& operator<<(NkStream::CStream& l, const CVariant& r) {
		r.read(l);
		return l;
	}
	inline NkStream::CStream& operator>>(NkStream::CStream& l, CVariant& r) {
		r.write(l);
		return l;
	}
	inline NkStream::CStream& operator<<(NkStream::CStream& l, const VARIANT& r) {
		NkCom::CVariant::read(l, r);
		return l;
	}
	inline NkStream::CStream& operator>>(NkStream::CStream& l, VARIANT& r) {
		NkCom::CVariant::write(l, r);
		return l;
	}
}
