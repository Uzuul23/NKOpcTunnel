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