#pragma once

namespace NkType
{
	class CConnectionAddressInfo
	{
	public:
		virtual ~CConnectionAddressInfo(){}
		virtual bool compare_address(const CConnectionAddressInfo& d) const throw() = 0;
	};
}