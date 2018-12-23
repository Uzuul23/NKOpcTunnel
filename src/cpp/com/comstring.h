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
#include <wtypes.h>

namespace NkStream
{
	class CStream;
}

namespace NkCom
{
	//memory allocation with CoTaskMemAlloc; freeing with CoTaskMemFree

	class CComString
	{
	public:
		CComString();
		CComString(const wchar_t* p);
		~CComString();

		void attach(LPOLESTR p);
		void detach(LPOLESTR* pp);

		void clear();

		LPOLESTR data();
		LPOLESTR* data_ptr();
		operator LPCOLESTR() const;
		operator LPOLESTR() const;
		LPOLESTR* operator&();

		static LPOLESTR copy(LPCOLESTR src);

		void read(NkStream::CStream& stream) const;
		void write(NkStream::CStream& stream);

		static void read(NkStream::CStream& stream, const wchar_t* p);
		static void write(NkStream::CStream& stream, wchar_t** pp);

		LPOLESTR str;
	};

	inline LPOLESTR CComString::data()
	{
		return str;
	}

	inline LPOLESTR* CComString::data_ptr()
	{
		return &str;
	}

	inline CComString::operator LPCOLESTR() const
	{
		return str;
	}

	inline CComString::operator LPOLESTR() const
	{
		return str;
	}

	inline NkStream::CStream& operator<<(NkStream::CStream& l, const CComString& r){
		r.read(l);
		return l;
	}

	inline NkStream::CStream& operator>>(NkStream::CStream& l, CComString& r){
		r.write(l);
		return l;
	}

}