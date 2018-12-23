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

#pragma once
#include "error/baseexception.h"
#include "error/winapiexception.h"

#ifndef MYDLLEXPORT
#define MYDLLEXPORT
#endif

namespace NkThreading
{
	template <typename T1, typename T2 = void>
	class MYDLLEXPORT CWorkerThread
	{
	public:
		CWorkerThread();
		virtual ~CWorkerThread(void);

		void start_async(void (T1::*function)(T2*), T1 *object
			, T2* p_data = 0, LPSECURITY_ATTRIBUTES lpThreadAttributes = 0
			, SIZE_T dwStackSize = 0, DWORD dwCreationFlags = 0);

		void wait_for(DWORD dwTimeOut = INFINITE);
		bool wait_for_if(DWORD dwTimeOut = INFINITE);

		operator HANDLE() const;

		DWORD id() const;

	private:
		CWorkerThread &operator =(const CWorkerThread&);
		CWorkerThread(const CWorkerThread&);

		HANDLE m_thread_handle;
		DWORD m_thread_id;
		T1* m_object;
		void (T1::*m_function)(T2*);
		T2* m_p_data;
		static DWORD thread_start_routine(LPVOID lpThreadParameter);
	};

	template <typename T1, typename T2>
	NkThreading::CWorkerThread<T1, T2>::CWorkerThread()
		: m_thread_handle(INVALID_HANDLE_VALUE)
		, m_thread_id(0)
		, m_object(0)
		, m_function(0)
		, m_p_data(0)
	{
	}

	template <typename T1, typename T2>
	NkThreading::CWorkerThread<T1, T2>::~CWorkerThread(void)
	{
	}

	template <typename T1, typename T2>
	void NkThreading::CWorkerThread<T1, T2>::start_async(void (T1::*function)(T2*)
		, T1 *object, T2* p_data, LPSECURITY_ATTRIBUTES lpThreadAttributes
		, SIZE_T dwStackSize, DWORD dwCreationFlags)
	{
		NkError::CBaseException::check(m_thread_handle != INVALID_HANDLE_VALUE
			, NkError::CBaseException::invalid_argument, __FILE__, __LINE__);

		m_function = function;
		m_object = object;
		m_p_data = p_data;

		m_thread_handle = ::CreateThread(lpThreadAttributes, dwStackSize
			, reinterpret_cast<LPTHREAD_START_ROUTINE>(thread_start_routine)
			, this, dwCreationFlags, &m_thread_id);

		NkError::CWinApiException::check(m_thread_handle
			, "CreateThread", __FILE__, __LINE__);
	}

	template <typename T1, typename T2>
	void NkThreading::CWorkerThread<T1, T2>::wait_for(DWORD dwTimeOut /*= INFINITE*/)
	{
		NkError::CBaseException::check(m_thread_handle == INVALID_HANDLE_VALUE
			, NkError::CBaseException::invalid_argument, __FILE__, __LINE__);

		DWORD ret = WaitForSingleObject(m_thread_handle, dwTimeOut);
		if (ret == WAIT_FAILED) {
			NkError::CWinApiException::check("WaitForSingleObject", __FILE__, __LINE__);
		}
		m_thread_handle = INVALID_HANDLE_VALUE;
	}

	template <typename T1, typename T2>
	bool NkThreading::CWorkerThread<T1, T2>::wait_for_if(DWORD dwTimeOut /*= INFINITE*/)
	{
		if (m_thread_handle == INVALID_HANDLE_VALUE) {
			return false;
		}

		DWORD ret = WaitForSingleObject(m_thread_handle, dwTimeOut);
		if (ret == WAIT_FAILED) {
			NkError::CWinApiException::check("WaitForSingleObject", __FILE__, __LINE__);
		}
		m_thread_handle = INVALID_HANDLE_VALUE;
		return true;
	}

	template <typename T1, typename T2>
	NkThreading::CWorkerThread<T1, T2>::operator HANDLE() const
	{
		return m_thread_handle;
	}

	template <typename T1, typename T2>
	DWORD NkThreading::CWorkerThread<T1, T2>::id() const
	{
		return m_thread_id;
	}

	template <typename T1, typename T2>
	DWORD NkThreading::CWorkerThread<T1, T2>::thread_start_routine(LPVOID lpThreadParameter)
	{
		CWorkerThread<T1, T2>* p = static_cast<CWorkerThread<T1, T2>*>(lpThreadParameter);
		(p->m_object->*p->m_function)(p->m_p_data);
		return 0;
	}
}
