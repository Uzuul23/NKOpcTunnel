#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace NkThreading
{
	class CEvent
	{
	public:
		CEvent(bool bcreate = true);
		CEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset
			, BOOL bInitialState, LPCWSTR lpName);

		//open a existing event
		CEvent(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName);

		CEvent(const CEvent& d);
		CEvent & operator =(const CEvent& d);
		~CEvent(void);

		operator HANDLE();
		void create(LPSECURITY_ATTRIBUTES lpEventAttributes = 0
			, BOOL bManualReset = FALSE, BOOL bInitialState = FALSE
			, LPCWSTR lpName = 0);
		void open(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName);
		void close();
		void pulse();
		void reset();
		void set();
		//if the state of the specified object is signaled: returns WAIT_OBJECT_0
		//if the time-out interval elapsed: returns WAIT_TIMEOUT
		DWORD wait_for(DWORD dwMilliseconds = INFINITE);

	private:
		HANDLE m_hEvent;
	};
}