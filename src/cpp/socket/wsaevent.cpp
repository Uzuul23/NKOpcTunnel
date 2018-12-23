/*	This file is part of NKOpcTunnel.
*
*	Copyright (c) Henryk Anschuetz 
*	Berlin, Germany
*
*	mailto:uzuul23@online.de
*
*	NKOpcTunnel is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NKOpcTunnel is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with NKOpcTunnel.  If not, see <http://www.gnu.org/licenses/>.
*
*/

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