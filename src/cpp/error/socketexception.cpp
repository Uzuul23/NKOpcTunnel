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
#include "socketexception.h"
#include "trace/trace.h"
#include <crtdbg.h>

namespace NkError
{
	CSocketException::CSocketException() : m_Error(ERROR_SUCCESS), m_nSourceLine(-1)
	{
	}

	CSocketException::CSocketException(const char* pszFunctionName, int error, const char* lpszfile, int line) : m_strFunctionName(pszFunctionName)
		, m_Error(error)
		, m_nSourceLine(line)
	{
		m_strSourceFile.file_name(lpszfile);
	}

	CSocketException::CSocketException(const CSocketException& d)
	{
		*this = d;
	}

	CSocketException & CSocketException::operator=(const CSocketException& d)
	{
		m_strFunctionName = d.m_strFunctionName;
		m_strSourceFile = d.m_strSourceFile;
		m_strErrorString = d.m_strErrorString;
		m_Error = d.m_Error;
		m_nSourceLine = d.m_nSourceLine;
		return *this;
	}

	HRESULT CSocketException::error() const
	{
		return HRESULT_FROM_WIN32(m_Error);
	}

	const wchar_t* CSocketException::error_text(DWORD dwLanguageId /*= MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
	{
		m_strErrorString.format_system_message(m_Error, dwLanguageId);
		return m_strErrorString;
	}

	void CSocketException::report(DWORD dwLanguageId /*= MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
	{
		NkTrace::CTrace::Instance().Trace(NkTrace::CTrace::TraceError
			, L"%s(): (%d) %s file: %s line: %d", m_strFunctionName.data()
			, m_Error, error_text(dwLanguageId), m_strSourceFile.data(), m_nSourceLine);
	}

	void CSocketException::check(const WSANETWORKEVENTS& wsaevents, long NetworkEvent, const char* lpszfile, int line)
	{
		if (NetworkEvent & FD_READ)
		{
			if (wsaevents.iErrorCode[FD_READ_BIT])
			{
				throw CSocketException("read", wsaevents.iErrorCode[FD_READ_BIT], lpszfile, line);
			}
		}
		if (NetworkEvent & FD_WRITE)
		{
			if (wsaevents.iErrorCode[FD_WRITE_BIT])
			{
				throw CSocketException("write", wsaevents.iErrorCode[FD_WRITE_BIT], lpszfile, line);
			}
		}
		if (NetworkEvent & FD_OOB)
		{
			if (wsaevents.iErrorCode[FD_OOB_BIT])
			{
				throw CSocketException("OOB", wsaevents.iErrorCode[FD_OOB_BIT], lpszfile, line);
			}
		}
		if (NetworkEvent & FD_ACCEPT)
		{
			if (wsaevents.iErrorCode[FD_ACCEPT_BIT])
			{
				throw CSocketException("accept", wsaevents.iErrorCode[FD_ACCEPT_BIT], lpszfile, line);
			}
		}
		if (NetworkEvent & FD_CONNECT)
		{
			if (wsaevents.iErrorCode[FD_CONNECT_BIT])
			{
				throw CSocketException("connect", wsaevents.iErrorCode[FD_CONNECT_BIT], lpszfile, line);
			}
		}
		if (NetworkEvent & FD_CLOSE)
		{
			if (wsaevents.iErrorCode[FD_CLOSE_BIT])
			{
				throw CSocketException("close", wsaevents.iErrorCode[FD_CLOSE_BIT], lpszfile, line);
			}
		}
		if (NetworkEvent & FD_QOS)
		{
			if (wsaevents.iErrorCode[FD_QOS_BIT])
			{
				throw CSocketException("QOS", wsaevents.iErrorCode[FD_QOS_BIT], lpszfile, line);
			}
		}
		if (NetworkEvent & FD_GROUP_QOS)
		{
			if (wsaevents.iErrorCode[FD_GROUP_QOS_BIT])
			{
				throw CSocketException("Group QOS", wsaevents.iErrorCode[FD_GROUP_QOS_BIT], lpszfile, line);
			}
		}
		if (NetworkEvent & FD_ROUTING_INTERFACE_CHANGE)
		{
			if (wsaevents.iErrorCode[FD_ROUTING_INTERFACE_CHANGE_BIT])
			{
				throw CSocketException("interface change", wsaevents.iErrorCode[FD_ROUTING_INTERFACE_CHANGE_BIT], lpszfile, line);
			}
		}
		if (NetworkEvent & FD_ADDRESS_LIST_CHANGE)
		{
			if (wsaevents.iErrorCode[FD_ADDRESS_LIST_CHANGE_BIT])
			{
				throw CSocketException("address list change", wsaevents.iErrorCode[FD_ADDRESS_LIST_CHANGE_BIT], lpszfile, line);
			}
		}
	}

	int CSocketException::base_error() const
	{
		return 0;
	}
}