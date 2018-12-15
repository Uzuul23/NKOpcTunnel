#pragma once
#include "stream/stream.h"
#include "com/comstring.h"
#include "com/variant.h"

namespace NkCom
{
	//CVectorBase

	template<class T, typename VectorType, typename SizeType = DWORD>
	class CVectorBase
	{
	public:
		CVectorBase() : _p(0), _c(0), _const(false) {}
		CVectorBase(SizeType c) : _p(0), _c(c), _const(false)
		{
			_p = static_cast<VectorType*>(::CoTaskMemAlloc(_c * sizeof(VectorType)));
			NkError::CBaseException::check_alloc(_p, __FILE__, __LINE__);

			__if_exists(T::initialize_element) {
				for (size_t loop = 0; loop < _c; ++loop) {
					static_cast<T*>(this)->initialize_element(_p[loop]);
				}
			}
			__if_not_exists(T::initialize_element) {
				ZeroMemory(_p, _c * sizeof(VectorType));
			}
		}
		CVectorBase(const VectorType p[], SizeType c) : _c(c), _const(true)
		{
			_p = const_cast<VectorType*>(p);
		}
		~CVectorBase()
		{
			clear();
		}
		operator const VectorType*()
		{
			return const_cast<const VectorType*>(_p);
		}
		operator VectorType*()
		{
			return _p;
		}
		VectorType** operator&()
		{
			_ASSERT(_p == 0);
			return &_p;
		}
		VectorType& operator[](SizeType i)
		{
			NkError::CBaseException::check(i >= 0 && i < _c, E_INVALIDARG, __FILE__, __LINE__);
			return _p[i];
		}
		void detach_element(VectorType* p, SizeType i)
		{
			NkError::CBaseException::check(i >= 0 && i < _c, E_INVALIDARG, __FILE__, __LINE__);
			memcpy(p, &_p[i], sizeof(VectorType));
			_p[i] = 0;
		}
		VectorType* data()
		{
			return _p;
		}
		SizeType* size_ptr()
		{
			return &_c;
		}
		void size(size_t c)
		{
			_c = static_cast<SizeType>(c);
		}
		SizeType size()
		{
			return _c;
		}
		bool isconst()
		{
			return _const;
		}
		void changeable()
		{
			if (_const)
			{
				VectorType* p = static_cast<VectorType*>(::CoTaskMemAlloc(_c * sizeof(VectorType)));
				NkError::CBaseException::check_alloc(p, __FILE__, __LINE__);
				__if_exists(T::copy_element) {
					for (size_t loop = 0; loop < _c; ++loop) {
						static_cast<T*>(this)->copy_element(p[loop], _p[loop]);
					}
				}
				__if_not_exists(T::copy_element) {
					memcpy(p, _p, _c * sizeof(VectorType));
				}
				_p = p;
				_const = false;
			}
		}
		void copy_elem(VectorType* p, SizeType i)
		{
			NkError::CBaseException::check(p && i >= 0 && i < _c, E_INVALIDARG, __FILE__, __LINE__);

			__if_exists(T::copy_element) {
				static_cast<T*>(this)->copy_element(p, &_p[i]);
			}
			__if_not_exists(T::copy_element) {
				memcpy_s(p, sizeof(VectorType), &_p[i], sizeof(VectorType));
			}
		}
		void copy_to(CVectorBase<T, VectorType, SizeType>& vec)
		{
			vec.clear();
			VectorType* p = static_cast<VectorType*>(::CoTaskMemAlloc(_c * sizeof(VectorType)));
			NkError::CBaseException::check_alloc(vec._p, __FILE__, __LINE__);
			__if_exists(T::copy_element) {
				for (size_t loop = 0; loop < _c; ++loop) {
					static_cast<T*>(this)->copy_element(p[loop], _p[loop]);
				}
			}
			__if_not_exists(T::copy_element) {
				memcpy(p, _p, _c * sizeof(VectorType));
			}
			vec.attach(p, _c);
		}
		size_t detach(VectorType** pp)
		{
			if (pp == 0) {
				return 0;
			}
			changeable();
			(*pp) = _p;
			SizeType c = _c;
			_p = 0;
			_c = 0;
			return c;
		}
		void detach(VectorType** pp, SizeType* c)
		{
			if (pp == 0) {
				return;
			}
			changeable();
			(*pp) = _p;
			(*c) = _c;
			_p = 0;
			if (c) {
				(*c) = _c;
			}
		}
		void attach(CVectorBase<T, VectorType, SizeType>& vec)
		{
			clear();
			_c = vec.detach(&_p);
			_const = false;
		}
		void attach(VectorType* p, SizeType c)
		{
			clear();
			_p = p;
			_c = c;
			_const = false;
		}
		void clear_elem(VectorType* p)
		{
			if (p == 0) {
				return;
			}
			__if_exists(T::clear_element) {
				static_cast<T*>(this)->clear_element(p);
			}
		}
		void clear()
		{
			if (_p && !_const) {
				__if_exists(T::clear_element) {
					for (size_t loop = 0; loop < _c; ++loop) {
						static_cast<T*>(this)->clear_element(_p[loop]);
					}
				}
				::CoTaskMemFree(_p);
			}
			_p = 0;
			_c = 0;
		}
		void read(NkStream::CStream& stream) const
		{
			stream.write_type(VT_CARRAY);
			stream.write_count(_c);
			if (_c == 0) {
				return;
			}
			__if_exists(T::read_element) {
				for (size_t loop = 0; loop < _c; ++loop) {
					static_cast<const T*>(this)->read_element(_p[loop], stream);
				}
			}
			__if_not_exists(T::read_element) {
				stream.write(_p, _c * sizeof(VectorType));
			}
		}
		void write(NkStream::CStream& stream)
		{
			stream.check_type(VT_CARRAY);
			SizeType c = static_cast<SizeType>(stream.read_count());
			if (_p && _const) {
				if (c > _c) {
					NkError::CBaseException::check(E_INVALIDARG, __FILE__, __LINE__);
				}
			}
			else {
				clear();
				_p = static_cast<VectorType*>(::CoTaskMemAlloc(c * sizeof(VectorType)));
				NkError::CBaseException::check_alloc(_p, __FILE__, __LINE__);
			}
			_c = c;
			if (_c == 0) {
				return;
			}
			__if_exists(T::write_element) {
				for (size_t loop = 0; loop < _c; ++loop) {
					static_cast<T*>(this)->write_element(_p[loop], stream);
				}
			}
			__if_not_exists(T::write_element) {
				stream.read(_p, _c * sizeof(VectorType));
			}
		}
		VectorType* _p;
		SizeType _c;
		bool _const;
	};

	template<class T, typename VectorType, typename SizeType>
	inline NkStream::CStream& operator<<(NkStream::CStream& l, const CVectorBase<T, VectorType, SizeType>& r)
	{
		r.read(l);
		return l;
	}
	template<class T, typename VectorType, typename SizeType>
	inline NkStream::CStream& operator>>(NkStream::CStream& l, CVectorBase<T, VectorType, SizeType>& r)
	{
		r.write(l);
		return l;
	}

	//CVector
	template<typename VectorType, typename SizeType = DWORD>
	class CVector : public CVectorBase < CVector<SizeType>, VectorType, SizeType >
	{
	public:
		CVector() : CVectorBase() {}
		CVector(SizeType c) : CVectorBase(c) {}
		CVector(const VectorType p[], SizeType c) : CVectorBase(p, c) {}
	};

	//CStringVector
	template<typename SizeType = DWORD>
	class CStringVector : public CVectorBase < CStringVector<SizeType>, wchar_t*, SizeType >
	{
	public:
		CStringVector() : CVectorBase() {}
		CStringVector(SizeType c) : CVectorBase(c) {}
		CStringVector(LPCWSTR p[], SizeType c)
		{
			_p = const_cast<wchar_t**>(p);
			_c = c;
			_const = true;
		}
		CStringVector(LPWSTR p[], SizeType c)
		{
			_p = p;
			_c = c;
			_const = true;
		}
		operator LPCWSTR*()
		{
			return const_cast<LPCWSTR*>(_p);
		}
		void copy_element(wchar_t* dst, wchar_t*& src)
		{
			src = NkCom::CComString::copy(src);
		}
		void clear_element(wchar_t* elem)
		{
			::CoTaskMemFree(elem);
			elem = 0;
		}
		void read_element(wchar_t*& elem, NkStream::CStream& stream) const
		{
			NkCom::CComString::read(stream, elem);
		}
		void write_element(wchar_t*& elem, NkStream::CStream& stream)
		{
			NkCom::CComString::write(stream, &elem);
		}
	};

	//CVariantVector
	template<typename SizeType = size_t>
	class CVariantVector : public CVectorBase < CVariantVector<SizeType>, VARIANT, SizeType >
	{
	public:
		CVariantVector() : CVectorBase() {}
		CVariantVector(SizeType c) : CVectorBase(c) {}
		CVariantVector(const VARIANT a[], SizeType c) : CVectorBase(a, c) {}
		void initialize_element(VARIANT& elem)
		{
			::VariantInit(&elem);
		}
		void copy_element(VARIANT& dst, const VARIANT& src)
		{
			NkError::CBaseException::check_result(::VariantCopy(&dst, &src), __FILE__, __LINE__);
		}
		void clear_element(VARIANT& elem)
		{
			NkError::CBaseException::check_result(::VariantClear(&elem), __FILE__, __LINE__);
		}
		void read_element(const VARIANT& elem, NkStream::CStream& stream) const
		{
			stream << elem;
		}
		void write_element(VARIANT& elem, NkStream::CStream& stream)
		{
			stream >> elem;
		}
	};
}