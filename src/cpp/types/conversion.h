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

//#ifdef min
//#undef min
//#endif

namespace NkType
{
	inline static BOOL to_BOOL(bool b)
	{
		return b == true ? TRUE : FALSE;
	}
	inline static bool to_bool(BOOL b)
	{
		return b == TRUE ? true : false;
	}

	inline static BOOL to_BOOL(DWORD dw)
	{
		return dw != 0 ? TRUE : FALSE;
	}
	inline static bool to_bool(DWORD dw)
	{
		return dw != 0 ? true : false;
	}

	template<typename PointerType>
	inline void zero(PointerType** pp)
	{
		if (pp) {
			*pp = 0;
		}
	}

	/*template <class T>
	const T& min(const T& a, const T& b)
	{
	return !(b<a)? a: b;
	}*/

	template<typename StringType = std::string>
	inline void to_Addr(unsigned long ip, unsigned long port, StringType& result)
	{
		std::string str;
		str = std::to_string((ip >> 24) & 0xFF);
		str += ".";
		str += std::to_string((ip >> 16) & 0xFF);
		str += ".";
		str += std::to_string((ip >> 8) & 0xFF);
		str += ".";
		str += std::to_string((ip >> 0) & 0xFF);
		str += "/";
		str += std::to_string(port);

		result = str.c_str();
	}
}