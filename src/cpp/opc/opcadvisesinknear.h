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

#include "com/nearunknown.h"

#include <ObjIdl.h>

namespace NkOPC
{
	class COPCAdviseSinkNear : public NkCom::CNearUnknown < COPCAdviseSinkNear >
		, public IAdviseSink
	{
	public:
		COPCAdviseSinkNear(NkCom::CServer* p, ULONG32 id);;

		IMPL_INTERFACE1(IAdviseSink);

		STDMETHOD_(void, OnDataChange)(FORMATETC *pFormatetc, STGMEDIUM *pStgmed);
		STDMETHOD_(void, OnViewChange)(DWORD dwAspect, LONG lindex);
		STDMETHOD_(void, OnRename)(IMoniker *pmk);
		STDMETHOD_(void, OnSave)(void);
		STDMETHOD_(void, OnClose)(void);

		enum CfFormats
		{
			OPCSTMFORMATDATA = 1,
			OPCSTMFORMATDATATIME,
			OPCSTMFORMATWRITECOMPLETE
		};

	private:
		COPCAdviseSinkNear();
		COPCAdviseSinkNear &operator =(const COPCAdviseSinkNear&);
		COPCAdviseSinkNear(const COPCAdviseSinkNear&);

		static void RegisterClipboardFormats();
		static UINT m_cfOPCSTMFORMATDATA;
		static UINT m_cfOPCSTMFORMATDATATIME;
		static UINT m_cfOPCSTMFORMATWRITECOMPLETE;
	};
}