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




