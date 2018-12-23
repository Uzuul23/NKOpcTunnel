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
#include "threading/lock.h"

namespace NkCom
{
	class CModule
	{
	public:
		CModule() : m_hModule(0), m_cServerLocks(0)
		{
		}
		BOOL DllMain(HANDLE hinstDLL, DWORD  ul_reason_for_call, LPVOID lpReserved)
		{
			switch (ul_reason_for_call)
			{
			case DLL_PROCESS_ATTACH:
			{
				m_hModule = static_cast<HMODULE>(hinstDLL);
			}
			/*case DLL_THREAD_ATTACH:
			case DLL_THREAD_DETACH:
			case DLL_PROCESS_DETACH:*/
			break;
			}

			return TRUE;
		}
		HRESULT DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv)
		{
			return CLASS_E_CLASSNOTAVAILABLE;
		}
		HRESULT DllCanUnloadNow()
		{
			NkThreading::CLockGuard lock_guard(m_Lock);

			if (m_cServerLocks == 0) {
				return S_OK;
			}
			return S_FALSE;
		}
		HRESULT DllRegisterServer()
		{
			return S_OK;
		}
		HRESULT DllUnregisterServer()
		{
			return S_OK;
		}
		void lock_server(bool lock)
		{
			NkThreading::CLockGuard lock_guard(m_Lock);

			if (lock) {
				++m_cServerLocks;
			}
			else if (m_cServerLocks > 0) {
				--m_cServerLocks;
			}
		}

		NkThreading::CCriticalSection& get_global_lock()
		{
			return m_Lock;
		}

		HMODULE get_module()
		{
			return m_hModule;
		}

	private:
		NkThreading::CCriticalSection m_Lock;
		HMODULE m_hModule;
		LONG m_cServerLocks;
	};
}