#pragma once
#include <winsock2.h>

namespace NkSocket
{
	class CWsaEvent
	{
	public:
		CWsaEvent();
		virtual ~CWsaEvent(void);

		void reset();
		void set();
		void event_select(SOCKET sd, long lNetworkEvent);
		DWORD wait(DWORD dwTimeOut = INFINITE);
		void enum_events(SOCKET sd, WSANETWORKEVENTS& events);
		operator WSAEVENT() const;

	private:
		CWsaEvent &operator =(const CWsaEvent&);
		CWsaEvent(const CWsaEvent&);

		WSAEVENT m_hEvent;
	};
}