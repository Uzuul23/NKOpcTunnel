#pragma once
#include "interfaces.h"

#ifndef MYDLLEXPORT
#define MYDLLEXPORT
#endif

namespace NkStream
{
	class CStreamIODevice;
	class MYDLLEXPORT CMTIODevice : public CStreamIODevice
	{
	public:
		CMTIODevice(CStreamIODevice* p, bool use_sta = false, size_t cb_buffer_size = 8192);
		virtual ~CMTIODevice();

		//CStreamIODevice
		virtual size_t read(void* p, size_t cb, ULONG32 id);
		virtual size_t write(const void* p, size_t cb, ULONG32 id);
		virtual bool can_read(ULONG32 id = 0);
		virtual void cancel_wait(ULONG32 id = 0);
		virtual void close(ULONG32 id = 0) throw();
		virtual bool is_closed() const throw();

		void start_async_callback(CStreamIODeviceEvent* p_event, ULONG32 stream_id
			, bool server = false, UINT time_out = 5000);
		//void pause_async_callback();
		//void resume_async_callback();

	private:
		CMTIODevice(const CMTIODevice&);
		CMTIODevice& operator=(const CMTIODevice&);
		class CImpl;
		CImpl* _Impl;
	};
}