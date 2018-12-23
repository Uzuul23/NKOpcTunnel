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
#include "types/string.h"


namespace NkService
{
	class CServiceManager
	{
	public:
		
		CServiceManager(const wchar_t* pszServiceName
			, const wchar_t* pszMachineName = 0
			, const wchar_t* pszDatabaseName = 0
			, DWORD dwDesiredAccess = SC_MANAGER_CONNECT);

		virtual ~CServiceManager(); 

		void create(const wchar_t* pszDisplayName,
			const wchar_t* pszDescription = 0,
			unsigned long dwStartType = SERVICE_AUTO_START,
			const wchar_t* pszDependencies = L"", 
			const wchar_t* pszAccount = 0, 
			const wchar_t* pszPassword = 0);

		void remove();
		void start();
		void stop();

	private:
		NkType::CString m_strServiceName;
		SC_HANDLE m_hSCManager;
	};
}