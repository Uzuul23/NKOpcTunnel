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

#include "error/winapiexception.h"
#include "types/conversion.h"
#include <synchapi.h>

namespace NkThreading
{
	class CCriticalSection
	{
	public:
		CCriticalSection(DWORD dwSpinCount = 1000) : m_initialized(false)
		{
			BOOL ret = ::InitializeCriticalSectionAndSpinCount(&m_critical_section, dwSpinCount);
			NkError::CWinApiException::check(ret, "InitializeCriticalSectionAndSpinCount", __FILE__, __LINE__);
			m_initialized = true;
		}
		~CCriticalSection(void)
		{
			if (m_initialized) {
				::DeleteCriticalSection(&m_critical_section);
			}
		}
		operator CRITICAL_SECTION*()
		{
			return	&m_critical_section;
		}
		void lock()
		{
			::EnterCriticalSection(&m_critical_section);
		}
		void unlock()
		{
			::LeaveCriticalSection(&m_critical_section);
		}
		bool try_enter()
		{
			return NkType::to_bool(::TryEnterCriticalSection(&m_critical_section));
		}
		bool locked()
		{
			return m_critical_section.RecursionCount > 0;
		}

	private:
		CCriticalSection & operator =(const CCriticalSection&);
		CCriticalSection(const CCriticalSection&);
		CRITICAL_SECTION m_critical_section;
		bool m_initialized;
	};

	template <class T1>
	class CGuard
	{
	public:
		CGuard(T1& lock) : m_lock(lock)
		{
			m_lock.lock();
		}
		~CGuard()
		{
			m_lock.unlock();
		}
	private:
		CGuard & operator =(const CGuard&);
		CGuard(const CGuard&);
		T1& m_lock;
	};

	typedef CGuard<CCriticalSection> CLockGuard;
}
