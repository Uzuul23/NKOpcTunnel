#pragma once
#include <OaIdl.h>

namespace NkStream
{
	class CStream;
}

namespace NkCom
{
	class CSafeArray
	{
	public:
		CSafeArray();
		CSafeArray(SAFEARRAY* psa);
		~CSafeArray();

		operator const SAFEARRAY*();

		void attach(SAFEARRAY* p);
		void detach(SAFEARRAY** pp);

		void clear();

		static SAFEARRAY* copy(SAFEARRAY* psa);
		static void read(NkStream::CStream& stream, SAFEARRAY*& psa);
		static void write(NkStream::CStream& stream, SAFEARRAY*& psa);
		void read(NkStream::CStream& stream);
		void write(NkStream::CStream& stream);
	private:
		SAFEARRAY* _psa;
	};

	inline NkStream::CStream& operator<<(NkStream::CStream& l, CSafeArray& r) {
		r.read(l);
		return l;
	}
	inline NkStream::CStream& operator>>(NkStream::CStream& l, CSafeArray& r) {
		r.write(l);
		return l;
	}
	inline NkStream::CStream& operator<<(NkStream::CStream& l, SAFEARRAY*& r) {
		NkCom::CSafeArray::read(l, r);
		return l;
	}
	inline NkStream::CStream& operator>>(NkStream::CStream& l, SAFEARRAY*& r) {
		NkCom::CSafeArray::write(l, r);
		return l;
	}
}