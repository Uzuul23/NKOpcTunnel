/*	This file is part of NKOpcTunnel.
*
*	Copyright (c) Henryk Anschuetz 
*	Berlin, Germany
*
*	mailto:uzuul23@online.de
*
*	NKOpcTunnel is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NKOpcTunnel is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with NKOpcTunnel.  If not, see <http://www.gnu.org/licenses/>.
*
*/

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