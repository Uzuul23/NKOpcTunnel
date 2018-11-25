#pragma once

namespace NkStream
{
	class CStreamIODevice
	{
	public:
		virtual ~CStreamIODevice() {};
		virtual size_t read(void* p, size_t cb, ULONG32 id = 0) = 0;
		virtual size_t write(const void* p, size_t cb, ULONG32 id = 0) = 0;
		virtual void close(ULONG32 id = 0) throw() = 0;
		virtual bool is_closed() const throw() = 0;
		//virtual bool can_read(ULONG32 id = 0) = 0;
		//virtual void cancel_wait(ULONG32 id = 0) = 0;
	};

	class CStreamIODeviceEvent
	{
	public:
		virtual bool on_receive(ULONG32 stream_id) = 0;
		virtual bool on_time_out() = 0;
	};
}