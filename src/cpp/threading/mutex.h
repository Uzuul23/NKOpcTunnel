#pragma once
#include "error/winapiexception.h"
#include "error/baseexception.h"
#include "lock.h"

namespace NkThreading
{
	class CMutex
	{
	public:
		CMutex(bool bcreate = true) : m_hMutex(INVALID_HANDLE_VALUE)
		{
			if (bcreate)
			{
				create(0, FALSE, 0);
			}
		}

		CMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName)
		{
			create(lpMutexAttributes, bInitialOwner, lpName);
		}

		~CMutex()
		{
			close();
		}

		void create(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName)
		{
			close();

			m_hMutex = ::CreateMutex(lpMutexAttributes, bInitialOwner, lpName);
			NkError::CWinApiException::check(m_hMutex, "CreateMutex", __FILE__, __LINE__);
		}

		void close()
		{
			if (m_hMutex != INVALID_HANDLE_VALUE)
			{
				BOOL ret = ::CloseHandle(m_hMutex);
				NkError::CWinApiException::check(ret, "CloseHandle", __FILE__, __LINE__);
			}
		}

		//if the thread got ownership of the mutex: returns WAIT_OBJECT_0
		//if the time-out interval elapsed: returns WAIT_TIMEOUT
		DWORD lock(DWORD dwMilliseconds = INFINITE)
		{
			DWORD ret = ::WaitForSingleObject(m_hMutex, dwMilliseconds);
			if (ret == WAIT_ABANDONED)
			{
				NkError::CBaseException::check(
					NkError::CBaseException::mutex_abandoned_state
					, __FILE__, __LINE__);
			}
			return ret;
		}

		void unlock()
		{
			BOOL ret = ::ReleaseMutex(m_hMutex);
			NkError::CWinApiException::check(ret, "ReleaseMutex", __FILE__, __LINE__);
		}

	private:
		HANDLE m_hMutex;
	};

	typedef CGuard<CMutex> CMutexGuard;
}