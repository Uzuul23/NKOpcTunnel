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