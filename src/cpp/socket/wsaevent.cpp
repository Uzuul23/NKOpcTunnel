#include "stdafx.h"
#include "wsaevent.h"
#include "error/socketexception.h"

namespace NkSocket
{
	CWsaEvent::CWsaEvent()
	{
		m_hEvent = WSACreateEvent();
		NkError::CSocketException::check(m_hEvent, "WSACreateEvent", __FILE__, __LINE__);
	}

	CWsaEvent::~CWsaEvent(void)
	{
		BOOL ret = WSACloseEvent(m_hEvent);
		NkError::CSocketException::check_bool(ret, "WSACloseEvent", __FILE__, __LINE__);
	}

	void CWsaEvent::reset()
	{
		BOOL ret = WSAResetEvent(m_hEvent);
		NkError::CSocketException::check_bool(ret, "WSAResetEvent", __FILE__, __LINE__);
	}

	void CWsaEvent::set()
	{
		BOOL ret = WSASetEvent(m_hEvent);
		NkError::CSocketException::check_bool(ret, "WSASetEvent", __FILE__, __LINE__);
	}

	void CWsaEvent::event_select(SOCKET sd, long lNetworkEvent)
	{
		int ret = WSAEventSelect(sd, m_hEvent, lNetworkEvent);
		NkError::CSocketException::check(ret, "WSAEventSelect", __FILE__, __LINE__);
	}

	// returns WSA_WAIT_TIMEOUT if timeout
	// returns WSA_WAIT_EVENT_0 for event
	DWORD CWsaEvent::wait(DWORD dwTimeOut /*= INFINITE*/)
	{
		DWORD dwRet = WSAWaitForMultipleEvents(1, &m_hEvent, TRUE, dwTimeOut, FALSE);
		if (dwRet == WSA_WAIT_FAILED)
		{
			NkError::CSocketException::check("WSAWaitForMultipleEvents", __FILE__, __LINE__);
		}

		return dwRet;
	}

	void CWsaEvent::enum_events(SOCKET sd, WSANETWORKEVENTS& events)
	{
		ZeroMemory(&events, sizeof(events));

		int ret = WSAEnumNetworkEvents(sd, m_hEvent, &events);
		NkError::CSocketException::check(ret, "WSAEnumNetworkEvents", __FILE__, __LINE__);
	}

	CWsaEvent::operator WSAEVENT() const
	{
		return m_hEvent;
	}
}