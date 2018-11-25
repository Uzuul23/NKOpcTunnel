#pragma once
#include "com/comfarunknown.h"
#include "opc/opcfarsrv.h"
#include <crtdbg.h>

namespace NkOPC
{
	class COpcFarUnknown : public NkCom::CComFarUnknown
	{
	public:
		COPCFarSrv* get_server_opc()
		{
			_ASSERT(dynamic_cast<COPCFarSrv*>(m_pSrv));
			return static_cast<COPCFarSrv*>(m_pSrv);
		}
	};
}