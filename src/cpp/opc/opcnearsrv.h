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
#include "com/comnearsrv.h"

namespace NkSSL
{
	class COpenSSLCtx;
}

namespace NkOPC
{
	class COPCNearSrv : public NkCom::CComNearSrv
	{
	public:
		COPCNearSrv(NkStream::CStreamIODevice* pNewProvider, NkCom::CModule *pModule
			, bool auto_delete_provider = true);

		static COPCNearSrv* create_new_server(const char* addr
			, NkCom::CModule *pModule = 0, NkCom::CLogonEvent *pLogon = 0);

		static COPCNearSrv* create_new_server_ssl(const char* addr
			, NkSSL::COpenSSLCtx& ctx, NkCom::CModule *pModule = 0
			, NkCom::CLogonEvent *pLogon = 0);

		virtual void create_near_proxy(REFIID proxy_iid, REFIID riid, void** ppv, ULONG32 id);
		virtual NkCom::CComFarUnknown* create_far_proxy(IUnknown* p, REFIID riid, ULONG32& id);
	};
}