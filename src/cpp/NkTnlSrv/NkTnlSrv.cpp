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
#include "NkTnlSrvService.h"

CNkTnlSrvService g_Service(NKOPCTnl::ServiceName);
NkThreading::CEvent g_KillEvent;

BOOL ctrl_handler(DWORD fdwCtrlType)
{
	if (g_Service.StartedAsService()) {
		return FALSE;
	}
	switch (fdwCtrlType) {
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT :
		g_Service.Stop();
		return TRUE;
	default:
		return FALSE;
	}
}

void print_help()
{
	wprintf(L"Parameters:\n");
	wprintf(L" -register\t\"to add the application to the registry\"\n");
	wprintf(L" -unregister\t\"to remove the application the from registry\"\n");
	wprintf(L" -install\t\"to install the service\"\n");
	wprintf(L" -remove\t\"to remove the service\"\n");
	wprintf(L" -stop\t\t\"to stop the service\"\n");
	wprintf(L" -start\t\t\"to start the service\"\n");
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	int argc = 0;
	LPWSTR* argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

	NkService::CServiceBase::main(&g_Service, argc, argv);
	LocalFree(argv);
	return 0;
}

int wmain(int argc, wchar_t *argv[])
{
	try
	{
		if ((argc > 1) && ((*argv[1] == L'-' || (*argv[1] == L'/')))) {
			if (_wcsicmp(L"h", argv[1] + 1) == 0) {
				print_help();
			}
			else if (_wcsicmp(L"register", argv[1] + 1) == 0) {
				NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_CREATE_SUB_KEY);
				NkWin::CRegistry keySettings;

				key.CreateKey(NKOPCTnl::RegKeySettings, &keySettings, KEY_WRITE);
				keySettings.SetValue(argv[0], NKOPCTnl::RegValueServerInstallPath);

				key.CreateKey(NKOPCTnl::RegKeySettings, &keySettings, KEY_WRITE);
				keySettings.SetValue(NKOPCTnl::DefaultPort, NKOPCTnl::RegValueServerPort);

				key.CreateKey(NKOPCTnl::RegKeySettings, &keySettings, KEY_WRITE);
				keySettings.SetValue(NKOPCTnl::DefaultServerIP, NKOPCTnl::RegValueServerIPAddress);

				key.CreateKey(NKOPCTnl::RegKeySettings, &keySettings, KEY_WRITE);
				keySettings.SetValue(NKOPCTnl::DefaultServerUseSSL, NKOPCTnl::RegValueServerUseSSL);

				wprintf(L"%s successfully registered", NKOPCTnl::ServiceName);
			}
			else if (_wcsicmp(L"unregister", argv[1] + 1) == 0) {
				NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_WRITE);
				NkWin::CRegistry keySettings;
				keySettings.Open(NKOPCTnl::RegKeySettings, key);
				keySettings.DeleteKeyValue(NKOPCTnl::RegValueServerInstallPath);
				keySettings.DeleteKeyValue(NKOPCTnl::RegValueServerPort);
				keySettings.DeleteKeyValue(NKOPCTnl::RegValueServerIPAddress);
				keySettings.DeleteKeyValue(NKOPCTnl::RegValueServerUseSSL);

				wprintf(L"%s successfully registration removed", NKOPCTnl::ServiceName);
			}
			else if (_wcsicmp(L"install", argv[1] + 1) == 0) {
				//TODO:
				NkService::CServiceManager SCManager(NKOPCTnl::ServiceName, 0, 0
					, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
				SCManager.create(L"Nk OPC Tunnel Server"
					, L"Dieser Service stellt OPC Server auf entfernten Rechnern bereit");
				wprintf(L"Service %s successfully installed", NKOPCTnl::ServiceName);
			}
			else if (_wcsicmp(L"remove", argv[1] + 1) == 0) {
				NkService::CServiceManager SCManager(NKOPCTnl::ServiceName);
				SCManager.remove();
				wprintf(L"Service %s removed", NKOPCTnl::ServiceName);
			}
			else if (_wcsicmp(L"stop", argv[1] + 1) == 0) {
				NkService::CServiceManager SCManager(NKOPCTnl::ServiceName);
				SCManager.stop();
				wprintf(L"Service %s stopped", NKOPCTnl::ServiceName);
			}
			else if (_wcsicmp(L"start", argv[1] + 1) == 0) {
				NkService::CServiceManager SCManager(NKOPCTnl::ServiceName);
				SCManager.start();
				wprintf(L"Service %s started", NKOPCTnl::ServiceName);
			}
			else {
				print_help();
			}
		}
		else {
			SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrl_handler, TRUE);
			NkService::CServiceBase::main(&g_Service, argc, argv);
		}
	}
	catch (NkError::CException& e) {
		e.report();
		return e.error();
	}
	return 0;
}