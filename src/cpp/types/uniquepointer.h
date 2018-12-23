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