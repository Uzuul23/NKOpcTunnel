#include "stdafx.h"
#include "ServiceBase.h"
#include "types/conversion.h"
#include "error/winapiexception.h"
#include "error/baseexception.h"
#include "trace/trace.h"

namespace NkService
{
	CServiceBase* CServiceBase::m_pService = 0;

	CServiceBase::CServiceBase( LPCWSTR pszServiceName 
		, bool bCanStop /*= true */
		, bool bCanShutdown /*= true */
		, bool bCanPauseContinue /*= true*/ )
		: m_strServiceName(pszServiceName)
		, m_bStartedAsService(true)
		, m_hStatus(0)
		, m_StopEvent(false)
	{

		// The service runs in its own process.
		m_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

		// The service is starting.
		m_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;

		// The accepted commands of the service.
		DWORD dwControlsAccepted = 0;

		if (bCanStop) 
			dwControlsAccepted |= SERVICE_ACCEPT_STOP;

		if (bCanShutdown) 
			dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;

		if (bCanPauseContinue) 
			dwControlsAccepted |= SERVICE_ACCEPT_PAUSE_CONTINUE;

		m_ServiceStatus.dwControlsAccepted = dwControlsAccepted;

		m_ServiceStatus.dwWin32ExitCode = NO_ERROR;
		m_ServiceStatus.dwServiceSpecificExitCode = 0;
		m_ServiceStatus.dwCheckPoint = 0;
		m_ServiceStatus.dwWaitHint = 0;
	}
	CServiceBase::~CServiceBase()
	{
	}
	bool CServiceBase::StartedAsService()
	{
		return m_bStartedAsService;
	}
	bool CServiceBase::main( CServiceBase *pService, int argc, wchar_t *argv[] )
	{
		NkError::CBaseException::check_arg(pService, __FILE__, __LINE__);
		m_pService = pService;

		SERVICE_TABLE_ENTRYW serviceTable[] = 
		{
			{ m_pService->m_strServiceName, ServiceMain },
			{ NULL, NULL }
		};

		BOOL bRet = ::StartServiceCtrlDispatcher(serviceTable);
		if (bRet == FALSE)
		{
			if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
			{
				//The program is being run as a console application rather than as a service
				m_pService->m_bStartedAsService = false;
				m_pService->m_StopEvent.create();
				ServiceMain(argc, argv);
			}
			else
			{
				NkError::CWinApiException::check(bRet, "StartServiceCtrlDispatcher", __FILE__, __LINE__);
			}
		}

		return NkType::to_bool(bRet);
	}

	void WINAPI CServiceBase::ServiceMain(DWORD dwNumServicesArgs, LPWSTR *lpServiceArgVectors)
	{
		if (m_pService->m_bStartedAsService)
		{
			SERVICE_STATUS_HANDLE hStatus = RegisterServiceCtrlHandler(m_pService->m_strServiceName, ServiceCtrlHandler);
			NkError::CWinApiException::check(hStatus, "RegisterServiceCtrlHandler", __FILE__, __LINE__);

			m_pService->m_hStatus = hStatus;
			m_pService->Start(dwNumServicesArgs, lpServiceArgVectors);
		}
		else
		{
			m_pService->Start(dwNumServicesArgs, lpServiceArgVectors);
			m_pService->m_StopEvent.wait_for();
		}
	}

	void WINAPI CServiceBase::ServiceCtrlHandler(DWORD dwCtrl)
	{
		switch (dwCtrl)
		{
		case SERVICE_CONTROL_STOP: m_pService->Stop(); break;
		case SERVICE_CONTROL_PAUSE: m_pService->Pause(); break;
		case SERVICE_CONTROL_CONTINUE: m_pService->Continue(); break;
		case SERVICE_CONTROL_SHUTDOWN: m_pService->Shutdown(); break; 
		case SERVICE_CONTROL_INTERROGATE: break;
		default: break;
		}
	}
	void CServiceBase::SetServiceStatus(DWORD dwCurrentState
		, DWORD dwWin32ExitCode, DWORD dwWaitHint)
	{
		static DWORD dwCheckPoint = 1;

		m_ServiceStatus.dwCurrentState = dwCurrentState;
		m_ServiceStatus.dwWin32ExitCode = dwWin32ExitCode;
		m_ServiceStatus.dwWaitHint = dwWaitHint;
		m_ServiceStatus.dwCheckPoint = ((dwCurrentState == SERVICE_RUNNING) ||
			(dwCurrentState == SERVICE_STOPPED)) ? 0 : dwCheckPoint++;

		BOOL bRet = ::SetServiceStatus(m_hStatus, &m_ServiceStatus);
		NkError::CWinApiException::check(bRet, "SetServiceStatus", __FILE__, __LINE__);
	}
	void CServiceBase::Start(DWORD dwArgc, PWSTR *pszArgv)
	{
		try
		{
			if (m_bStartedAsService)
			{
				SetServiceStatus(SERVICE_START_PENDING);
				OnStart(dwArgc, pszArgv);
				NkTrace::CTrace::TraceDet(L"service %s started", m_strServiceName.data());
				SetServiceStatus(SERVICE_RUNNING);	
			}
			else
			{
				OnStart(dwArgc, pszArgv);
			}
		}
		catch (...)
		{
			if (m_bStartedAsService)
			{
				SetServiceStatus(SERVICE_STOPPED);
			}
		}
	}

	void CServiceBase::Stop()
	{
		DWORD dwOriginalState = m_ServiceStatus.dwCurrentState;

		try
		{
			if (m_bStartedAsService)
			{
				SetServiceStatus(SERVICE_STOP_PENDING);
				NkTrace::CTrace::TraceDet(L"service %s stopped", m_strServiceName.data());
				OnStop();
				SetServiceStatus(SERVICE_STOPPED);
			}
			else
			{
				OnStop();
				m_StopEvent.set();
			}
		}
		catch (...)
		{
			NkTrace::CTrace::TraceErr(L"Service failed to stop.");

			if (m_bStartedAsService)
			{
				SetServiceStatus(dwOriginalState);
			}
		}
	}
	void CServiceBase::Pause()
	{
		try
		{
			SetServiceStatus(SERVICE_PAUSE_PENDING);
			OnPause();
			NkTrace::CTrace::TraceDet(L"service %s paused", m_strServiceName.data());
			SetServiceStatus(SERVICE_PAUSED);
		}
		catch (...)
		{
			SetServiceStatus(SERVICE_RUNNING);
			NkTrace::CTrace::TraceErr(L"Service failed to pause.");
		}
	}

	void CServiceBase::Continue()
	{
		try
		{
			SetServiceStatus(SERVICE_CONTINUE_PENDING);
			OnContinue();
			NkTrace::CTrace::TraceDet(L"service %s continued", m_strServiceName.data());
			SetServiceStatus(SERVICE_RUNNING);
		}
		catch (...)
		{
			SetServiceStatus(SERVICE_PAUSED);
			NkTrace::CTrace::TraceErr(L"Service failed to continue.");
		}
	}

	void CServiceBase::Shutdown()
	{
		try
		{
			OnShutdown();
			SetServiceStatus(SERVICE_STOPPED);
		}
		catch (...)
		{
		}
	}
	void CServiceBase::OnStart( DWORD dwArgc, PWSTR *pszArgv )
	{
		//default do nothing
	}
	void CServiceBase::OnStop()
	{
		//default do nothing
	}
	void CServiceBase::OnPause()
	{
		//default do nothing
	}
	void CServiceBase::OnContinue()
	{
		//default do nothing
	}
	void CServiceBase::OnShutdown()
	{
		//default do nothing
	}
}