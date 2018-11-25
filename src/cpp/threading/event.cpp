#include "stdafx.h"
#include "Event.h"
#include "../error/winapiexception.h"

namespace NkThreading
{
	CEvent::CEvent(bool bcreate /*= true*/) : m_hEvent(INVALID_HANDLE_VALUE)
	{
		if (bcreate) {
			create(0, FALSE, FALSE, 0);
		}
	}
	CEvent::CEvent(LPSECURITY_ATTRIBUTES lpEventAttributes
		, BOOL bManualReset, BOOL bInitialState
		, LPCWSTR lpName) : m_hEvent(INVALID_HANDLE_VALUE)
	{
		create(lpEventAttributes, bManualReset, bInitialState, lpName);
	}
	CEvent::CEvent(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName)
	{
		open(dwDesiredAccess, bInheritHandle, lpName);
	}
	CEvent::CEvent(const CEvent& d)
	{
		*this = d;
	}
	CEvent & CEvent::operator=(const CEvent& d)
	{
		close();

		BOOL ret = ::DuplicateHandle(GetCurrentProcess(), d.m_hEvent
			, GetCurrentProcess(), &m_hEvent, 0, FALSE, DUPLICATE_SAME_ACCESS);
		NkError::CWinApiException::check(ret, "DuplicateHandle", __FILE__, __LINE__);

		return *this;
	}
	CEvent::~CEvent(void)
	{
		close();
	}
	void CEvent::close()
	{
		if (m_hEvent != INVALID_HANDLE_VALUE)
		{
			BOOL ret = ::CloseHandle(m_hEvent);
			m_hEvent = INVALID_HANDLE_VALUE;
			NkError::CWinApiException::check(ret, "CloseHandle", __FILE__, __LINE__);
		}
	}
	CEvent::operator HANDLE()
	{
		return	m_hEvent;
	}
	void CEvent::pulse()
	{
		BOOL ret = ::PulseEvent(m_hEvent);
		NkError::CWinApiException::check(ret, "PulseEvent", __FILE__, __LINE__);
	}
	void CEvent::reset()
	{
		BOOL ret = ::ResetEvent(m_hEvent);
		NkError::CWinApiException::check(ret, "ResetEvent", __FILE__, __LINE__);
	}
	void CEvent::set()
	{
		BOOL ret = ::SetEvent(m_hEvent);
		NkError::CWinApiException::check(ret, "SetEvent", __FILE__, __LINE__);
	}
	DWORD CEvent::wait_for(DWORD dwMilliseconds /*= INFINITE*/)
	{
		DWORD ret = ::WaitForSingleObject(m_hEvent, dwMilliseconds);
		if (ret == WAIT_FAILED)
		{
			NkError::CWinApiException::check("WaitForSingleObject", __FILE__, __LINE__);
		}
		return ret;
	}
	void CEvent::create(LPSECURITY_ATTRIBUTES lpEventAttributes /*= 0 */
		, BOOL bManualReset /*= FALSE*/
		, BOOL bInitialState /*= FALSE */, LPCWSTR lpName /*= 0*/)
	{
		close();
		m_hEvent = ::CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);
		NkError::CWinApiException::check(m_hEvent, "CreateEvent", __FILE__, __LINE__);
	}
	void CEvent::open(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName)
	{
		close();
		m_hEvent = ::OpenEvent(dwDesiredAccess, bInheritHandle, lpName);
		NkError::CWinApiException::check(m_hEvent, "OpenEvent", __FILE__, __LINE__);
	}
}