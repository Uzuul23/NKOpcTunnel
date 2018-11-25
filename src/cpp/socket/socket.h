#pragma once
#include <winsock2.h>

#pragma warning(disable : 4995)
#include <WS2TCPIP.H>
#pragma warning(default : 4995)

#include "stream/interfaces.h"
#include <string>

namespace NkSocket
{
	class CSocket : public NkStream::CStreamIODevice
	{
	public:
		CSocket() noexcept;
		CSocket(SOCKET so) noexcept;
		CSocket(const CSocket&) = delete;
		CSocket & operator =(const CSocket&) = delete;
		CSocket & operator =(const SOCKET &d);
		virtual ~CSocket(void);

		//NkStream::CStreamIODevice
		virtual size_t read(void* p, size_t cb, ULONG32 id = 0);
		virtual size_t write(const void* p, size_t cb, ULONG32 id = 0);
		virtual void close(ULONG32 id = 0) noexcept;
		virtual bool is_closed() const noexcept;

		void socket();
		void shutdown(int how = SD_SEND);

		bool non_blocking_mode();
		void non_blocking_mode(bool set);

		void reuse_addr();
		void exclusive_addr_use();

		void bind(const sockaddr_in* psa);
		void bind(const char* addr, u_short port);
		void bind(const sockaddr* paddr, u_short port);

		//"IP/PORT" e.g. "0.0.0.0/1001"
		void bind(LPCSTR addr);

		int listen(int backlog = SOMAXCONN);
		SOCKET accept()	const;

		void connect(const sockaddr_in* name, int namelen);
		//"IP/PORT" e.g. "127.0.0.1/1001"
		void connect(const char* addr);

		int recv(char* buf, int len, int flag = 0);
		int send(const char* buf, int len, int flag = 0);

		void get_peer_addr(sockaddr_in& addr);
		void get_peer_addr(std::string& addr);

		bool issocket() const noexcept;
		operator SOCKET() noexcept;

		static void initialize(WORD wsa_version = MAKEWORD(2, 2));
		static void cleanup();

	private:
		class CImpl;
		CImpl* _Impl;
	};
}
