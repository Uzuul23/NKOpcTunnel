#pragma once
#include "types/string.h"
#include "threading/event.h"
#include <winsvc.h>

namespace NkService
{
	class CServiceBase
	{
	public:
		CServiceBase(LPCWSTR pszServiceName
			, bool bCanStop = true
			, bool bCanShutdown = false
			, bool bCanPauseContinue = false);

		virtual ~CServiceBase();

		// Register the executable for a service with the Service Control Manager 
		// (SCM). After you call Run(ServiceBase), the SCM issues a Start command, 
		// which results in a call to the OnStart method in the service. This 
		// method blocks until the service has stopped.
		static bool main(CServiceBase *pService, int argc, wchar_t *argv[]);

		// Stop the service.
		void Stop();

		//if the program is being run as a console application rather than as a service
		//this returns false
		bool StartedAsService();


	protected:
		// When implemented in a derived class, executes when a Start command is 
		// sent to the service by the SCM or when the operating system starts 
		// (for a service that starts automatically).
		virtual void OnStart(DWORD dwArgc, PWSTR *pszArgv);

		// When implemented in a derived class, executes when a Stop command is 
		// sent to the service by the SCM.
		virtual void OnStop();

		// When implemented in a derived class, executes when a Pause command is 
		// sent to the service by the SCM.
		virtual void OnPause();

		// When implemented in a derived class, OnContinue runs when a Continue 
		// command is sent to the service by the SCM.
		virtual void OnContinue();

		// When implemented in a derived class, executes when the system is 
		// shutting down.
		virtual void OnShutdown();

	private:
		static void WINAPI ServiceMain(DWORD dwNumServicesArgs, LPWSTR *lpServiceArgVectors);
		static void WINAPI ServiceCtrlHandler(DWORD dwCtrl);
		void SetServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode = NO_ERROR, DWORD dwWaitHint = 0);
		void Start(DWORD dwArgc, PWSTR *pszArgv);
		void Pause();
		void Continue();
		void Shutdown();

		static CServiceBase* m_pService;
		NkType::CString m_strServiceName;
		NkThreading::CEvent m_StopEvent;
		bool m_bStartedAsService;
		SERVICE_STATUS m_ServiceStatus;
		SERVICE_STATUS_HANDLE m_hStatus;
	};
}