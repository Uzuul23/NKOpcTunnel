#include "stdafx.h"
#include "error/winapiexception.h"
#include "ServiceManager.h"

namespace NkService
{
	CServiceManager::CServiceManager( const wchar_t* pszServiceName 
		, const wchar_t* pszMachineName /*= 0 */
		, const wchar_t* pszDatabaseName /*= 0 */
		, DWORD dwDesiredAccess /*= SC_MANAGER_CONNECT*/ )
		: m_strServiceName(pszServiceName)
	{
		m_hSCManager = ::OpenSCManager(pszMachineName, pszDatabaseName, dwDesiredAccess);
		NkError::CWinApiException::check(m_hSCManager, "OpenSCManager", __FILE__, __LINE__);
	}
	CServiceManager::~CServiceManager()
	{
		if (m_hSCManager)
		{
			CloseServiceHandle(m_hSCManager);
		}
	}
	void CServiceManager::create(const wchar_t* pszDisplayName
		, const wchar_t* pszDescription /*= 0*/
		, unsigned long dwStartType /*= SERVICE_AUTO_START*/, const wchar_t* pszDependencies /*= L""*/
		, const wchar_t* pszAccount /*= 0*/, const wchar_t* pszPassword /*= 0*/ )
	{
		SC_HANDLE hSCService = 0;

		try
		{
			NkType::CString strPath;

			//TODO:
			DWORD dwRet = ::GetModuleFileName(0, strPath.buffer(MAX_PATH+2), MAX_PATH);
			strPath.release_buffer();
			//NkError::CWinApiException::check_error(dwRet, "GetModuleFileName", __FILE__, __LINE__);
			
			strPath.release_buffer();
			strPath.quote();

			hSCService = ::CreateService(
				m_hSCManager,                   // SCManager database
				m_strServiceName,               // Name of service
				pszDisplayName,                 // Name to display
				SERVICE_CHANGE_CONFIG,          // Desired access
				SERVICE_WIN32_OWN_PROCESS,      // Service type
				dwStartType,                    // Service start type
				SERVICE_ERROR_NORMAL,           // Error control type
				strPath,				        // Service's binary
				NULL,                           // No load ordering group
				NULL,                           // No tag identifier
				pszDependencies,                // Dependencies
				pszAccount,                     // Service running account
				pszPassword                     // Password of the account
				);
			NkError::CWinApiException::check(hSCService, "CreateService", __FILE__, __LINE__);

			if (pszDescription)
			{
				SERVICE_DESCRIPTION descr;
				descr.lpDescription = const_cast<wchar_t*>(pszDescription);

				BOOL bRet = ::ChangeServiceConfig2(hSCService, SERVICE_CONFIG_DESCRIPTION, &descr);
				NkError::CWinApiException::check(bRet, "ChangeServiceConfig2", __FILE__, __LINE__);
			}

			CloseServiceHandle(hSCService);
		}
		catch (...)
		{
			CloseServiceHandle(hSCService);
			throw;
		}	
	}
	void CServiceManager::remove()
	{
		SC_HANDLE hSCService = 0;

		try
		{
			hSCService = ::OpenService(m_hSCManager, m_strServiceName, DELETE);
			NkError::CWinApiException::check(hSCService, "OpenService", __FILE__, __LINE__);

			BOOL bRet = ::DeleteService(hSCService);
			NkError::CWinApiException::check(bRet, "DeleteService", __FILE__, __LINE__);

			CloseServiceHandle(hSCService);
		}
		catch (...)
		{
			CloseServiceHandle(hSCService);
			throw;
		}
	}

	void CServiceManager::start()
	{
		SC_HANDLE hSCService = 0;

		try
		{
			hSCService = ::OpenService(m_hSCManager, m_strServiceName, SERVICE_START|SERVICE_QUERY_STATUS);
			NkError::CWinApiException::check(hSCService, "OpenService", __FILE__, __LINE__);

			BOOL bRet = ::StartService(hSCService, 0, 0);
			NkError::CWinApiException::check(bRet, "StartService", __FILE__, __LINE__);

			SERVICE_STATUS_PROCESS ssStatus = {0};
			DWORD dwWaitTime = 0;
			DWORD dwBytesNeeded = 0;

			bRet = ::QueryServiceStatusEx(hSCService, SC_STATUS_PROCESS_INFO
				, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded);
			NkError::CWinApiException::check(bRet, "QueryServiceStatusEx", __FILE__, __LINE__);
			
			DWORD dwStartTickCount = GetTickCount();
			DWORD dwOldCheckPoint = ssStatus.dwCheckPoint;

			while (ssStatus.dwCurrentState == SERVICE_START_PENDING) 
			{ 
				// Do not wait longer than the wait hint. A good interval is 
				// one-tenth the wait hint, but no less than 1 second and no 
				// more than 10 seconds. 

				dwWaitTime = ssStatus.dwWaitHint / 10;

				if( dwWaitTime < 1000 )
				{
					dwWaitTime = 1000;
				}
				else if ( dwWaitTime > 10000 )
				{
					dwWaitTime = 10000;
				}

				Sleep(dwWaitTime);

				// Check the status again. 
				bRet = ::QueryServiceStatusEx(hSCService, SC_STATUS_PROCESS_INFO
					, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded);
				NkError::CWinApiException::check(bRet, "QueryServiceStatusEx", __FILE__, __LINE__);

				if (ssStatus.dwCheckPoint > dwOldCheckPoint)
				{
					// The service is making progress.
					dwStartTickCount = GetTickCount();
					dwOldCheckPoint = ssStatus.dwCheckPoint;
				}
				else
				{
					if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
					{
						// No progress made within the wait hint.
						break;
					}
				}
			}

			CloseServiceHandle(hSCService);
		}
		catch (...)
		{
			CloseServiceHandle(hSCService);
			throw;
		}
	}

	void CServiceManager::stop()
	{
		SC_HANDLE hSCService = 0;

		try
		{
			hSCService = ::OpenService(m_hSCManager, m_strServiceName, SERVICE_STOP|SERVICE_QUERY_STATUS);
			NkError::CWinApiException::check(hSCService, "OpenService", __FILE__, __LINE__);

			SERVICE_STATUS_PROCESS ssStatus; 
			DWORD dwBytesNeeded;
			DWORD dwStartTime = GetTickCount();
			DWORD dwTimeout = 30000; // 30-second time-out

			BOOL bRet = ::QueryServiceStatusEx(hSCService, SC_STATUS_PROCESS_INFO
				, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded);
			NkError::CWinApiException::check(bRet, "QueryServiceStatusEx", __FILE__, __LINE__);

			if(ssStatus.dwCurrentState == SERVICE_STOPPED )
				return;

			// If a stop is pending, wait for it.
			while(ssStatus.dwCurrentState == SERVICE_STOP_PENDING ) 
			{

				Sleep(ssStatus.dwWaitHint);

				bRet = ::QueryServiceStatusEx(hSCService, SC_STATUS_PROCESS_INFO
					, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded);
				NkError::CWinApiException::check(bRet, "QueryServiceStatusEx", __FILE__, __LINE__);

				if(ssStatus.dwCurrentState == SERVICE_STOPPED )
					return;

				if (GetTickCount() - dwStartTime > dwTimeout )
					return;
			}

			bRet = ::ControlService(hSCService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&ssStatus);
			NkError::CWinApiException::check(bRet, "ControlService", __FILE__, __LINE__);

			// Wait for the service to stop.

			while(ssStatus.dwCurrentState != SERVICE_STOPPED ) 
			{
				Sleep(ssStatus.dwWaitHint );

				bRet = ::QueryServiceStatusEx(hSCService, SC_STATUS_PROCESS_INFO
					, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded);
				NkError::CWinApiException::check(bRet, "QueryServiceStatusEx", __FILE__, __LINE__);

				if(ssStatus.dwCurrentState == SERVICE_STOPPED )
					return;

				if (GetTickCount() - dwStartTime > dwTimeout )
					return;
			}

			CloseServiceHandle(hSCService);
		}
		catch (...)
		{
			CloseServiceHandle(hSCService);
			throw;
		}
	}
}