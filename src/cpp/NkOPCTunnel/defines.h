#pragma once
#include <winsock2.h>

namespace NKOPCTnl
{
	const u_short DefaultPort = 1142;
	const DWORD DefaultServerIP = 0;

	static const wchar_t InprocServer32FileName[] = L"NKTnlClient.dll";
	static const wchar_t RegKeySettings[] = L"SOFTWARE\\WinPeSim\\NKOPCTunnel\\Settings\\";
	static const wchar_t RegValueClientInstallPath[] = L"ClientDllPath";
	static const wchar_t RegValueServerInstallPath[] = L"ServerPath";
	static const wchar_t RegValueThreadingModel[] = L"Free";
	static const wchar_t RegValueServerPort[] = L"ServerPort";
	static const wchar_t RegValueServerIPAddress[] = L"ServerIPAddress";
	static const wchar_t RegValueServerPass[] = L"ServerPass";
	static const wchar_t RegValueServerTraceLevel[] = L"ServerTraceLevel";
	static const wchar_t RegValueClientTraceLevel[] = L"ClientTraceLevel";
	static const wchar_t RegValueClientPass[] = L"Pass";
	static const wchar_t RegValueServerUseSSL[] = L"UseSSL";
	//static const wchar_t RegValueServerCertPath[] = L"ServerCertPath";


	//logs
	static const wchar_t LogPath[] = L"NkOPCTunnel"; //CSIDL_COMMON_APPDATA
	static const wchar_t ServerLogAppName[] = L"NkOPCTunnelSrv";
	static const wchar_t ClientLogAppName[] = L"NKOPCTunnelClient";
	
	//defaults
	static const int DefaultTraceLevel = 1;
	static const int DefaultServerUseSSL = 1;

	
	//service
	const wchar_t ServiceName[] = L"NkTnlSrv";

	//secret
	const wchar_t Salt[] = L"Vkf7fF8s9pTlXvjv4XW7fvEcrylv&peqboSAGs5CO7+wolJI10yGc*X;eVqkP";

	//names
	const wchar_t TunnelRealNameFormat[] = L"NKOPCTunnel::%s";
	const wchar_t TunnelRealNameFind[] = L"NKOPCTunnel::";
	const wchar_t TunnelProgIDFormat[] = L"NKOPCTunnel.%s";
}