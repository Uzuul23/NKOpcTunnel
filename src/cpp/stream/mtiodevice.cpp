#include "stdafx.h"
#include "mtiodevice.h"
#include "threading/lock.h"
#include "threading/workerthread.h"
#include "threading/event.h"
#include "error/baseexception.h"
#include "util/crc.h"
#include <vector>
#include <objbase.h>

//#define CMTIODeviceTrace

namespace NkStream
{
	class CMTIODevice::CImpl
	{
	public:

		size_t m_cb_buffer_size;
		CStreamIODevice* m_p_iodevice;
		bool m_use_sta;

		CImpl(CStreamIODevice* p, bool use_sta, size_t cb_buffer_size)
			: m_p_iodevice(p)
			, m_use_sta(use_sta)
			, m_can_async_read_event(0, TRUE, TRUE, 0)
			, m_shutdown_event(0, TRUE, FALSE, 0)
		{
			NkError::CBaseException::check_pointer(p, __FILE__, __LINE__);

			m_cb_buffer_size = max(cb_buffer_size, 512);
			m_cb_buffer_size += sizeof(CMsgHdr);
		}
		~CImpl()
		{
			shutdown();

			delete[] m_read_buffer_start_pos;
			delete[] m_write_buffer_start_pos;
		}

		bool m_shutdown_called = false;
		NkThreading::CEvent m_shutdown_event;
		void shutdown()
		{
			if (m_shutdown_called) {
				return;
			}
			m_shutdown_called = true;
			m_shutdown_event.set();
			m_Timer.wait_for_if();
			m_p_iodevice->close();
			m_Worker.wait_for_if();
		}

		class CWorkerThreadInfo
		{
		public:
			CWorkerThreadInfo() : m_p_event(0), m_stream_id(0) {}
			CWorkerThreadInfo(CStreamIODeviceEvent* p_event, ULONG32 stream_id)
				: m_p_event(p_event)
				, m_stream_id(stream_id) {}
			CStreamIODeviceEvent* m_p_event;
			ULONG32 m_stream_id;
		};

		NkThreading::CWorkerThread<CImpl, CWorkerThreadInfo> m_Worker;

		void start_callback_mta(CStreamIODeviceEvent* p_event, ULONG32 stream_id
			, bool server, UINT time_out)
		{
			NkError::CBaseException::check_pointer(p_event, __FILE__, __LINE__);

			//TODO:
			CWorkerThreadInfo *p_info = new CWorkerThreadInfo(p_event, stream_id);
			m_Worker.start_async(&CImpl::recv_worker_mta, this, p_info);

			m_time_out = time_out;
			if (server) {
			//	m_Timer.start_async(&CImpl::on_server_watchdog, this, p_info);
			}
			else {
			//	m_Timer.start_async(&CImpl::on_client_watchdog, this, p_info);
			}
		}

		CWorkerThreadInfo m_callback_info;

		/*
		void start_callback_sta(CStreamIODeviceEvent* p_event, ULONG32 stream_id)
		{
			//TODO: fix implementation
			//_ASSERT(m_use_sta == false);


			_ASSERT(m_shutdown_called == false);
			_ASSERT(m_callback_info.m_p_event == 0);

			if (m_callback_info.m_p_event) {
				return;
			}

			NkError::CBaseException::check_pointer(p_event, __FILE__, __LINE__);

			m_callback_info.m_p_event = p_event;
			m_callback_info.m_stream_id = stream_id;
			//TODO:
			//m_Worker.start_async(&CImpl::recv_worker_sta, this, 0);
		}

		bool m_b_pause_async_callback = false;
		*/

		/*
		void pause_async_callback()
		{
			_ASSERT(m_shutdown_called == false);
			_ASSERT(m_b_pause_async_callback == false);

			if (m_b_pause_async_callback) {
				return;
			}
			m_can_async_read_event.reset();
			m_p_iodevice->cancel_wait();
			m_b_pause_async_callback = true;
		}

		void resume_async_callback()
		{
			_ASSERT(m_shutdown_called == false);
			_ASSERT(m_b_pause_async_callback == true);

			if (!m_b_pause_async_callback) {
				return;
			}
			m_can_async_read_event.set();
			m_b_pause_async_callback = false;
		}
		*/

		NkThreading::CWorkerThread<CImpl, CWorkerThreadInfo> m_Timer;
		NkThreading::CCriticalSection m_watchdog_lock;
		UINT m_time_out = 0;
		bool m_ping_on_run = false;
		UINT m_watchdog = 1000;

		void on_server_watchdog(CWorkerThreadInfo* p_event)
		{
			while (1) {
				DWORD ret = m_shutdown_event.wait_for(500);
				if (ret == WAIT_TIMEOUT && !m_shutdown_called) {
					NkThreading::CLockGuard lock(m_watchdog_lock);
					m_watchdog = m_watchdog > 0 ? m_watchdog - 500 : 0;
					if (m_watchdog == 0) {
						if (!p_event->m_p_event->on_time_out()) {
							break;
						}
					}
					else if (!m_ping_on_run && m_watchdog <= m_time_out / 2) {
#ifdef CMTIODeviceTrace
						NkTrace::CTrace::TraceDet(L"Send Ping");
#endif
						m_ping_on_run = true;
						write(0, 0, 0, CMsgHdr::MsgCmds::MsgCmdsPing);
					}
				}
				else {
					break;
				}
			}
		}

		void on_client_watchdog(CWorkerThreadInfo* p_event)
		{
			while (1) {
				DWORD ret = m_shutdown_event.wait_for(500);
				if (ret == WAIT_TIMEOUT && !m_shutdown_called) {
					NkThreading::CLockGuard lock(m_watchdog_lock);
					m_watchdog = m_watchdog > 0 ? m_watchdog - 500 : 0;
					if (m_watchdog == 0) {
						if (!p_event->m_p_event->on_time_out()) {
							break;
						}
					}
				}
				else {
					break;
				}
			}
		}

		void recv_worker_mta(CWorkerThreadInfo* p_info)
		{
			try {
				HRESULT hr = ::CoInitializeEx(0, COINIT_MULTITHREADED);
				NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

				while (1) {
					if (!p_info->m_p_event->on_receive(p_info->m_stream_id)) {
						break;
					}
					if (m_shutdown_called) {
						break;
					}
				}
			}
			catch (NkError::CException& e) {
				e.report();
			}
			delete p_info;
			::CoUninitialize();
		}

		class CMsgHdr
		{
		public:
			UINT8 m_cmd = 0;
			ULONG32 m_cb = 0;
			ULONG32 m_id = 0;

			enum MsgCmds
			{
				MsgCmdsUser = 0,
				MsgCmdsPing,
				MsgCmdsPingBack
			};
		};

		CMsgHdr m_msg;
		NkThreading::CCriticalSection m_read_lock;

		void buffer_read_msg()
		{
			if (m_msg.m_id == 0) {
				buffer_read(&m_msg, sizeof(m_msg));

				NkThreading::CLockGuard lock(m_watchdog_lock);
				m_watchdog = m_time_out;

				if (m_msg.m_cmd == CMsgHdr::MsgCmds::MsgCmdsPing) {
#ifdef CMTIODeviceTrace
					NkTrace::CTrace::TraceDet(L"Send Ping Back");
#endif
					write(0, 0, 0, CMsgHdr::MsgCmds::MsgCmdsPingBack);
					return;
				}
				if (m_msg.m_cmd == CMsgHdr::MsgCmds::MsgCmdsPingBack) {
					m_ping_on_run = false;
					return;
				}

				_ASSERT(m_msg.m_id > 0);
				_ASSERT(m_msg.m_cb > 0);

#ifdef CMTIODeviceTrace
				NkTrace::CTrace::trace_info(L"receive buffer size %d id: %d", m_msg.m_id, m_msg.m_cb);
#endif
			}
		}

		size_t read_mta(void* p, size_t cb, ULONG32 id)
		{
			NkThreading::CLockGuard lock(m_read_lock);

			_ASSERT(p != 0 && cb > 0 && id > 0);

			buffer_read_msg();

			if (m_msg.m_id == id) {
				size_t cb_read = min(m_msg.m_cb, cb);
				cb_read = buffer_read(p, cb_read);
				m_msg.m_cb -= cb_read;
				if (m_msg.m_cb == 0) {
					m_msg.m_id = 0;
				}
				return cb_read;
			}

			return 0;
		}

		BYTE* m_read_buffer_start_pos = 0;
		BYTE* m_read_buffer_cur_pos = 0;
		BYTE* m_read_buffer_end_pos = 0;

		size_t buffer_read(void* p, size_t cb)
		{
			_ASSERT(p != 0 && cb > 0);

			if (m_read_buffer_start_pos == 0) {
				m_read_buffer_start_pos = new byte[m_cb_buffer_size];
				m_read_buffer_cur_pos = m_read_buffer_start_pos;
				m_read_buffer_end_pos = m_read_buffer_start_pos;
			}

			size_t cb_read = 0;
			BYTE* read_pos = static_cast<BYTE*>(p);

			while (cb > 0) {
				size_t cb_buffer_contend = static_cast<size_t>(m_read_buffer_end_pos - m_read_buffer_cur_pos);
				if (cb_buffer_contend > 0) {
					size_t cb_buffer_read = min(cb, cb_buffer_contend);
					memcpy_s(read_pos, cb, m_read_buffer_cur_pos, cb_buffer_read);
					cb_read += cb_buffer_read;
					read_pos += cb_buffer_read;
					m_read_buffer_cur_pos += cb_buffer_read;
					cb -= cb_buffer_read;
					if (cb == 0) {
						break;
					}
				}
				size_t cb_device_read = m_p_iodevice->read(m_read_buffer_start_pos, m_cb_buffer_size, 0);
				m_read_buffer_end_pos = m_read_buffer_start_pos + cb_device_read;
				m_read_buffer_cur_pos = m_read_buffer_start_pos;
			}
			return cb_read;
		}

		BYTE* m_write_buffer_start_pos = 0;
		size_t m_cb_write_buffer_size = 0;
		NkThreading::CCriticalSection m_write_lock;

		size_t write(const void* p, size_t cb, ULONG32 id)
		{
			_ASSERT(m_shutdown_called == false);
			return write(p, cb, id, CMsgHdr::MsgCmds::MsgCmdsUser);
		}

		size_t write(const void* p, size_t cb, ULONG32 id, CMsgHdr::MsgCmds cmd)
		{
			NkThreading::CLockGuard lock(m_write_lock);

			//adjust write buffer
			size_t cb_buffer_necessarily = cb + sizeof(CMsgHdr);
			cb_buffer_necessarily = max(cb_buffer_necessarily, m_cb_buffer_size);

			if (m_write_buffer_start_pos == 0) {
				m_write_buffer_start_pos = new BYTE[cb_buffer_necessarily];
				m_cb_write_buffer_size = cb_buffer_necessarily;
			}
			else if (cb_buffer_necessarily > m_cb_write_buffer_size) {
				delete m_write_buffer_start_pos;
				m_write_buffer_start_pos = 0;
				m_write_buffer_start_pos = new BYTE[cb_buffer_necessarily];
				m_cb_write_buffer_size = cb_buffer_necessarily;
			}

			CMsgHdr msg;
			msg.m_cmd = cmd;
			msg.m_id = id;
			msg.m_cb = static_cast<ULONG32>(cb);

			size_t cb_buffer_left = m_cb_write_buffer_size;
			size_t cb_buffer_contend = 0;
			BYTE* write_pos = m_write_buffer_start_pos;

			memcpy_s(write_pos, cb_buffer_left, &msg, sizeof(msg));
			write_pos += sizeof(msg);
			cb_buffer_contend += sizeof(msg);
			cb_buffer_left -= sizeof(msg);

			if (p != 0 && cb > 0) {
				memcpy_s(write_pos, cb_buffer_left, p, cb);
				cb_buffer_contend += cb;
			}

			size_t cb_written = 0;
			write_pos = m_write_buffer_start_pos;

			while (cb_buffer_contend > 0) {
				size_t cb_device_written = m_p_iodevice->write(write_pos, cb_buffer_contend);
				write_pos += cb_device_written;
				cb_written += cb_device_written;
				cb_buffer_contend -= cb_device_written;
			}

#ifdef CMTIODeviceTrace
			NkTrace::CTrace::trace_info(L"send buffer size %d id: %d", cb, id);
#endif

			return cb;
		}

		NkThreading::CEvent m_can_async_read_event;

		
		//bool m_b_callback_in_progress = false;
		
	};

	CMTIODevice::CMTIODevice(CStreamIODevice* p, bool use_sta /*= false*/, size_t cb_buffer_size /*= 8192*/)
		: _Impl(new CImpl(p, use_sta, cb_buffer_size))
	{
	}

	CMTIODevice::~CMTIODevice()
	{
		delete _Impl;
	}

	void CMTIODevice::start_async_callback(CStreamIODeviceEvent* p_event, ULONG32 stream_id
		, bool server /*= false*/, UINT time_out /*= 5000*/)
	{
		_Impl->start_callback_mta(p_event, stream_id, server, time_out);
	}

	/*
	void CMTIODevice::pause_async_callback()
	{
		_Impl->pause_async_callback();
	}

	void CMTIODevice::resume_async_callback()
	{
		_Impl->resume_async_callback();
	}
	*/

	size_t CMTIODevice::read(void* p, size_t cb, ULONG32 id /*= 0*/)
	{
		return _Impl->read_mta(p, cb, id);
	}

	size_t CMTIODevice::write(const void* p, size_t cb, ULONG32 id /*= 0*/)
	{
		return _Impl->write(p, cb, id);
	}

	bool CMTIODevice::can_read(ULONG32 id /*= 0*/)
	{
		//not supported
		_ASSERT(0);
		return false;
	}

	void CMTIODevice::cancel_wait(ULONG32 id /*= 0*/)
	{
		//not supported
		_ASSERT(0);
	}

	void CMTIODevice::close(ULONG32 id /*= 0*/) throw()
	{
		_Impl->shutdown();
	}

	bool CMTIODevice::is_closed() const throw()
	{
		return _Impl->m_p_iodevice->is_closed();
	}
}