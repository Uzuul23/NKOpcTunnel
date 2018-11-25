#pragma once

namespace NkStream
{
	class CStream;
}

namespace NkType
{
	template<typename Type>
	class CUniquePointer
	{
	public:
		CUniquePointer() : m_null(true)
		{
		}
		CUniquePointer(Type* p)
		{
			if (p) {
				m_t = *p;
				m_null = false;
			}
			else {
				m_null = true;
			}
		}
		operator Type*()
		{
			if (m_null) {
				return 0;
			}
			return &m_t;
		}

		static void read(NkStream::CStream& l, const CUniquePointer<Type>& r)
		{
			l.write_type(VT_PTR);
			l << r.m_null;
			if (!r.m_null) {
				l << r.m_t;
			}
		}
		static void write(NkStream::CStream& l, CUniquePointer<Type>& r)
		{
			l.check_type(VT_PTR);
			l >> r.m_null;
			if (!r.m_null) {
				l >> r.m_t;
			}
		}

		bool m_null;
		Type m_t;
	};

	template<typename Type>
	inline NkStream::CStream& operator<<(NkStream::CStream& l, const CUniquePointer<Type>& r)
	{
		CUniquePointer<Type>::read(l, r);
		return l;
	}
	template<typename Type>
	inline NkStream::CStream& operator>>(NkStream::CStream& l, CUniquePointer<Type>& r)
	{
		CUniquePointer<Type>::write(l, r);
		return l;
	}
}