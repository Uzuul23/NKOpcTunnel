#include "StdAfx.h"
#include "stream.h"
#include "interfaces.h"
#include "error/baseexception.h"
#include "error/winapiexception.h"
#include "threading/event.h"
#include "threading/lock.h"
#include <winsock2.h>
#include <crtdbg.h>
#include <strsafe.h>

namespace NkStream
{
	class CStream::CImpl
	{
	public:
		CImpl(CStreamIODevice* p, size_t cb_buffer_size, ULONG32 id)
			: m_pIODevice(p)
			, m_read_buffer_start_pos(0)
			, m_read_buffer_cur_pos(0)
			, m_read_buffer_end_pos(0)
			, m_write_buffer_start_pos(0)
			, m_write_buffer_cur_pos(0)
			, m_write_buffer_end_pos(0)
			, m_id(id)
		{
			m_cb_buffer_size = max(cb_buffer_size, 512);
		}
		~CImpl()
		{
			delete[] m_read_buffer_start_pos;
			delete[] m_write_buffer_start_pos;
		}

		CStreamIODevice* m_pIODevice;
		size_t m_cb_buffer_size;
		byte* m_read_buffer_start_pos;
		byte* m_read_buffer_cur_pos;
		byte* m_read_buffer_end_pos;
		byte* m_write_buffer_start_pos;
		byte* m_write_buffer_cur_pos;
		byte* m_write_buffer_end_pos;
		ULONG32 m_id;

#ifdef _DEBUG
		DWORD m_dw_thread_read = 0;
		NkThreading::CCriticalSection m_lock_debug;
#endif // DEBUG

		void read(void* p, size_t cb)
		{
#ifdef _DEBUG
			m_lock_debug.lock();
			if (m_read_buffer_end_pos == m_read_buffer_cur_pos) {
				m_dw_thread_read = 0;
			}
			if (m_dw_thread_read != 0) {
				_ASSERT(m_dw_thread_read == GetCurrentThreadId());
			}
			m_lock_debug.unlock();
#endif // DEBUG

			_ASSERT(p != 0 && cb > 0);

			if (m_read_buffer_start_pos == 0) {
				m_read_buffer_start_pos = new byte[m_cb_buffer_size];
				m_read_buffer_cur_pos = m_read_buffer_start_pos;
				m_read_buffer_end_pos = m_read_buffer_start_pos;
			}

			byte* read_pos = static_cast<byte*>(p);

			while (cb > 0) {
				size_t cb_buffer_contend = static_cast<size_t>(m_read_buffer_end_pos - m_read_buffer_cur_pos);
				if (cb_buffer_contend > 0) {
					size_t cb_buffer_read = cb_buffer_contend > cb ? cb : cb_buffer_contend;
					memcpy_s(read_pos, cb, m_read_buffer_cur_pos, cb_buffer_read);
					read_pos += cb_buffer_read;
					m_read_buffer_cur_pos += cb_buffer_read;
					cb -= cb_buffer_read;
					if (cb == 0) {
						break;
					}
				}
				NkError::CBaseException::check_pointer(m_pIODevice, __FILE__, __LINE__);
				size_t cb_read = m_pIODevice->read(m_read_buffer_start_pos, m_cb_buffer_size, m_id);
				m_read_buffer_end_pos = m_read_buffer_start_pos + cb_read;
				m_read_buffer_cur_pos = m_read_buffer_start_pos;

#ifdef _DEBUG
				m_lock_debug.lock();
				m_dw_thread_read = GetCurrentThreadId();
				m_lock_debug.unlock();
#endif // DEBUG
			}
		}

		/*void read_raw(void* p, size_t cb)
		{
			NkError::CBaseException::check_pointer(m_pIODevice, __FILE__, __LINE__);

			byte* read_pos = static_cast<byte*>(p);

			size_t cb_buffer_contend = static_cast<size_t>(m_read_buffer_end_pos - m_read_buffer_cur_pos);

			if (cb_buffer_contend > 0)
			{
				size_t cb_buffer_read = cb_buffer_contend > cb ? cb : cb_buffer_contend;
				memcpy_s(read_pos, cb, m_read_buffer_cur_pos, cb_buffer_read);
				read_pos += cb_buffer_read;
				m_read_buffer_cur_pos += cb_buffer_read;
				cb -= cb_buffer_read;

				if (cb == 0)
				{
					return;
				}
			}

			while (cb > 0)
			{
				size_t cb_read = m_pIODevice->read(read_pos, cb, m_id);
				read_pos += cb_read;
				cb -= cb_read;
			}
		}

		void write_raw(const void* p, size_t cb)
		{
			NkError::CBaseException::check_pointer(m_pIODevice, __FILE__, __LINE__);

			flush();

			const byte* write_pos = static_cast<const byte*>(p);
			while (cb > 0)
			{
				size_t cb_written = m_pIODevice->write(write_pos, cb, m_id);
				write_pos += cb_written;
				cb -= cb_written;
			}
		}*/

#ifdef _DEBUG
		DWORD m_dw_thread_write = 0;
#endif // DEBUG

		void write(const void* p, size_t cb)
		{

#ifdef _DEBUG
			m_lock_debug.lock();
			if (m_dw_thread_write != 0) {
				_ASSERT(m_dw_thread_write == GetCurrentThreadId());
			}
			else {
				m_dw_thread_write = GetCurrentThreadId();
			}
			m_lock_debug.unlock();
#endif // DEBUG

			if (m_write_buffer_start_pos == 0)
			{
				m_write_buffer_start_pos = new byte[m_cb_buffer_size];
				m_write_buffer_cur_pos = m_write_buffer_start_pos;
				m_write_buffer_end_pos = m_write_buffer_start_pos + m_cb_buffer_size;
			}

			const BYTE* write_pos = static_cast<const BYTE*>(p);

			while (cb > 0) {
				if (m_write_buffer_cur_pos == m_write_buffer_end_pos) {
					flush();
				}
				size_t cb_buffer_left = static_cast<size_t>(m_write_buffer_end_pos - m_write_buffer_cur_pos);
				size_t cb_write = min(cb_buffer_left, cb);
				memcpy_s(m_write_buffer_cur_pos, cb_buffer_left, write_pos, cb_write);
				m_write_buffer_cur_pos += cb_write;
				cb -= cb_write;
				write_pos += cb_write;
			}
		}

		void flush()
		{
			size_t cb = m_write_buffer_cur_pos - m_write_buffer_start_pos;

			if (cb > 0) {
				NkError::CBaseException::check_pointer(m_pIODevice, __FILE__, __LINE__);

				byte* write_pos = m_write_buffer_start_pos;

				while (cb > 0) {
					size_t cb_device_written = m_pIODevice->write(write_pos, cb, m_id);
					write_pos += cb_device_written;
					cb -= cb_device_written;
				}
				m_write_buffer_cur_pos = m_write_buffer_start_pos;
			}

#ifdef _DEBUG
			m_lock_debug.lock();
			m_dw_thread_write = 0;
			m_lock_debug.unlock();
#endif // DEBUG

		}

		void close()
		{
			if (m_pIODevice) {
				m_pIODevice->close(m_id);
			}
		}

		void write_type(VARTYPE vt)
		{
			write(&vt, sizeof(VARTYPE));
		}

		void check_type(VARTYPE vt)
		{
			VARTYPE vt2 = VT_EMPTY;
			read(&vt2, sizeof(VARTYPE));
			NkError::CBaseException::check_type(vt == vt2, __FILE__, __LINE__);
		}

		void write_count(size_t count)
		{
			write_type(VT_UI4);
			u_long ulCount = count;
			write(&ulCount, sizeof(u_long));
		}

		size_t read_count()
		{
			u_long ulCount = 0;
			check_type(VT_UI4);
			read(&ulCount, sizeof(u_long));
			return ulCount;
		}
	};

	CStream::CStream(CStreamIODevice* p/* = 0*/, size_t cb_buffer_size /*= 4096*/
		, ULONG32 id /*= 0*/)
		: _Impl(new CImpl(p, cb_buffer_size, id))
	{
	}

	CStream::~CStream(void)
	{
		delete _Impl;
	}

	void CStream::flush()
	{
		_Impl->flush();
	}

	void CStream::close() throw()
	{
		_Impl->close();
	}

	void CStream::stream_device(CStreamIODevice* p)
	{
		_Impl->m_pIODevice = p;
	}

	CStreamIODevice* CStream::stream_device()
	{
		return _Impl->m_pIODevice;
	}

	void CStream::write_count(size_t count)
	{
		_Impl->write_count(count);
	}

	size_t CStream::read_count()
	{
		return _Impl->read_count();
	}

	void CStream::write(const wchar_t* psz, size_t cch /*= -1*/)
	{
		if (psz == 0)
		{
			// write null string
			cch = 0;
		}
		else if (cch == -1)
		{
			HRESULT hr = StringCchLengthW(psz, STRSAFE_MAX_CCH, &cch);
			NkError::CWinApiException::check(hr, "StringCchLengthW", __FILE__, __LINE__);
			cch += 1; //terminating null character
		}
		else
		{
			cch += 1; //terminating null character
		}

		_Impl->write_type(VT_LPWSTR);
		_Impl->write_count(cch);

		if (cch > 0)
		{
			_Impl->write(psz, cch*sizeof(wchar_t));
		}
	}

	size_t CStream::read(wchar_t* psz, size_t cch)
	{
		NkError::CBaseException::check_arg(psz, __FILE__, __LINE__);

		_Impl->check_type(VT_LPWSTR);
		size_t cch_read = _Impl->read_count();

		if (cch_read == 0)
		{
			//null string
			psz = 0;
			return 0;
		}

		NkError::CBaseException::check((cch_read - 1) <= cch, NkError::CBaseException::buffer_to_small, __FILE__, __LINE__);

		_Impl->read(psz, cch_read*sizeof(wchar_t));

		return cch_read - 1;
	}

	CStream& CStream::operator>>(bool& l)
	{
		_Impl->check_type(VT_I1);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const bool& l)
	{
		_Impl->write_type(VT_I1);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(char& l)
	{
		_Impl->check_type(VT_I1);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const char& l)
	{
		_Impl->write_type(VT_I1);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(unsigned char& l)
	{
		_Impl->check_type(VT_UI1);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const unsigned char& l)
	{
		_Impl->write_type(VT_UI1);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(short& l)
	{
		_Impl->check_type(VT_I2);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const short& l)
	{
		_Impl->write_type(VT_I2);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(unsigned short& l)
	{
		_Impl->check_type(VT_UI2);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const unsigned short& l)
	{
		_Impl->write_type(VT_UI2);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(int& l)
	{
		_Impl->check_type(VT_I4);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const int& l)
	{
		_Impl->write_type(VT_I4);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(unsigned int& l)
	{
		_Impl->check_type(VT_UI4);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const unsigned int& l)
	{
		_Impl->write_type(VT_UI4);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(long& l)
	{
		_Impl->check_type(VT_I4);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const long& l)
	{
		_Impl->write_type(VT_I4);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(unsigned long& l)
	{
		_Impl->check_type(VT_UI4);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const unsigned long& l)
	{
		_Impl->write_type(VT_UI4);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(float& l)
	{
		_Impl->check_type(VT_R4);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const float& l)
	{
		_Impl->write_type(VT_R4);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(double& l)
	{
		_Impl->check_type(VT_R8);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const double& l)
	{
		_Impl->write_type(VT_R8);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(CLSID& l)
	{
		_Impl->check_type(VT_CLSID);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const CLSID& l)
	{
		_Impl->write_type(VT_CLSID);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(FILETIME& l)
	{
		_Impl->check_type(VT_FILETIME);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const FILETIME& l)
	{
		_Impl->write_type(VT_FILETIME);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(LONGLONG& l)
	{
		_Impl->check_type(VT_I8);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const LONGLONG& l)
	{
		_Impl->write_type(VT_I8);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator>>(ULONGLONG& l)
	{
		_Impl->check_type(VT_UI8);
		_Impl->read(&l, sizeof(l));
		return *this;
	}

	CStream& CStream::operator<<(const ULONGLONG& l)
	{
		_Impl->write_type(VT_UI8);
		_Impl->write(&l, sizeof(l));
		return *this;
	}

	void CStream::write_type(VARTYPE vt)
	{
		_Impl->write_type(vt);
	}

	void CStream::check_type(VARTYPE vt) const
	{
		_Impl->check_type(vt);
	}

	void CStream::write(const void* p, size_t cb)
	{
		//TODO
		//_Impl->write_type(VT_PTR);
		_Impl->write(p, cb);
	}

	void CStream::read(void* p, size_t cb)
	{
		//_Impl->check_type(VT_PTR);
		_Impl->read(p, cb);
	}
}