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