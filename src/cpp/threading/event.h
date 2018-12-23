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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace NkThreading
{
	class CEvent
	{
	public:
		CEvent(bool bcreate = true);
		CEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset
			, BOOL bInitialState, LPCWSTR lpName);

		//open a existing event
		CEvent(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName);

		CEvent(const CEvent& d);
		CEvent & operator =(const CEvent& d);
		~CEvent(void);

		operator HANDLE();
		void create(LPSECURITY_ATTRIBUTES lpEventAttributes = 0
			, BOOL bManualReset = FALSE, BOOL bInitialState = FALSE
			, LPCWSTR lpName = 0);
		void open(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName);
		void close();
		void pulse();
		void reset();
		void set();
		//if the state of the specified object is signaled: returns WAIT_OBJECT_0
		//if the time-out interval elapsed: returns WAIT_TIMEOUT
		DWORD wait_for(DWORD dwMilliseconds = INFINITE);

	private:
		HANDLE m_hEvent;
	};
}