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

#include "stdafx.h"
#include "dllmodule.h"

CDllModule g_Module;

extern "C" 
{
	BOOL __stdcall DllMain(HANDLE hinstDLL, DWORD  ul_reason_for_call, LPVOID lpReserved)
	{
		return g_Module.DllMain(hinstDLL, ul_reason_for_call, lpReserved);
	}
	HRESULT __stdcall DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv)
	{
		return g_Module.DllGetClassObject(clsid, riid, ppv);
	}
	HRESULT __stdcall DllCanUnloadNow()
	{
		return g_Module.DllCanUnloadNow();
	}
	HRESULT __stdcall DllRegisterServer(void)
	{
		return g_Module.DllRegisterServer();
	}
	HRESULT __stdcall DllUnregisterServer(void)
	{
		return g_Module.DllUnregisterServer();
	}
};




