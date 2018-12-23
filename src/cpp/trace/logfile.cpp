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

#include "StdAfx.h"
#include "logfile.h"
#include <strsafe.h>
#include <winnt.h>
#include <ShlObj.h>
#include "threading/lock.h"
#include "error/baseexception.h"
#include "error/winapiexception.h"

namespace NkTrace
{
	class CLogFile::CImpl
	{
	public:
		CImpl(LPCWSTR pszApplicationName, LPCWSTR pszLogFileName, GUID guid /* = FOLDERID_ProgramData*/)
			: m_FolderGuid(guid)
			, m_hFile(INVALID_HANDLE_VALUE)
			, m_FileNumber(0)
			, m_CheckHint(0)
		{
			NkError::CBaseException::check_arg(pszApplicationName, __FILE__, __LINE__);
			NkError::CBaseException::check_arg(pszLogFileName, __FILE__, __LINE__);

			HRESULT hr = StringCbCopyW(m_szApplicationName, sizeof(m_szApplicationName), pszApplicationName);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			hr = StringCbCopyW(m_szLogFileName, sizeof(m_szLogFileName), pszLogFileName);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			WCHAR szBuffer[MAX_PATH] = { 0 };
			WCHAR* pszFolderPath = 0;
			hr = SHGetKnownFolderPath(guid, 0, 0, &pszFolderPath);
			if (SUCCEEDED(hr)) {
				hr = StringCbPrintf(m_szFilePath, sizeof(m_szFilePath), L"%s\\%s\\"
					, pszFolderPath, m_szApplicationName);
				CoTaskMemFree(pszFolderPath);
				NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
			}
			else {
				hr = StringCbPrintf(m_szFilePath, sizeof(m_szFilePath), L"c:\\%s\\"
					, m_szApplicationName);
				NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
			}

			BOOL bRet = ::CreateDirectory(m_szFilePath, 0);
			if (bRet == 0) {
				DWORD dwError = ::GetLastError();
				if (dwError != ERROR_ALREADY_EXISTS) {
					NkError::CWinApiException::check(bRet, "WriteFile", __FILE__, __LINE__);
				}
			}

			hr = StringCbPrintf(szBuffer, sizeof(szBuffer), L"%s%s_*.log"
				, m_szFilePath, m_szLogFileName);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			WIN32_FIND_DATA finddata = {0};
			HANDLE hSearch = FindFirstFile(szBuffer, &finddata);

			if (hSearch) {
				while (FindNextFile(hSearch, &finddata)) {
					m_FileNumber++;
				}
			}

			hr = StringCbPrintf(szBuffer, sizeof(szBuffer), L"%s%s_%03d.log", m_szFilePath
				, m_szLogFileName, m_FileNumber);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			m_hFile = CreateFile(szBuffer, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			if (m_hFile == INVALID_HANDLE_VALUE) {
				DWORD dwRet = GetLastError();

				if (dwRet == ERROR_ACCESS_DENIED) {
					m_FileNumber++;

					hr = StringCbPrintf(szBuffer, sizeof(szBuffer), L"%s%s_%03d.log"
						, m_szFilePath, m_szLogFileName, m_FileNumber);
					NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

					m_hFile = CreateFile(szBuffer, GENERIC_WRITE, FILE_SHARE_READ, 0
						, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

					if (m_hFile == INVALID_HANDLE_VALUE) {
						NkError::CWinApiException::check(m_hFile, "WriteFile", __FILE__, __LINE__);
					}
				}
				else {
					NkError::CWinApiException::check(0, "WriteFile", __FILE__, __LINE__);
				}
			}

			SetFilePointer(m_hFile, 0, 0, SEEK_END);
			CheckFileSize(true);
		}
		virtual ~CImpl()
		{
			if (m_hFile) {
				CloseHandle(m_hFile);
			}
		}

		void CheckFileSize(bool bNow = false)
		{
			if (bNow || m_CheckHint >= 1000) {
				m_CheckHint = 0;
			}
			else {
				m_CheckHint++;
				return;
			}

			DWORD dwFileSize = GetFileSize(m_hFile, 0);

			if (dwFileSize < 1048576 * 10) {
				return;
			}

			WCHAR szBuffer[MAX_PATH] = { 0 };

			m_FileNumber++;

			HRESULT hr = StringCbPrintf(szBuffer, sizeof(szBuffer)
				, L"==> %s_%03d.log", m_szLogFileName, m_FileNumber);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			size_t cb = 0;
			hr = StringCbLength(szBuffer, sizeof(szBuffer), &cb);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			DWORD cbw = 0;
			BOOL bRet = WriteFile(m_hFile, szBuffer, cb, &cbw, 0);
			NkError::CWinApiException::check(bRet, "WriteFile", __FILE__, __LINE__);

			CloseHandle(m_hFile);

			hr = StringCbPrintf(szBuffer, sizeof(szBuffer), L"%s%s_%03d.log"
				, m_szFilePath, m_szLogFileName, m_FileNumber);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			m_hFile = CreateFile(szBuffer, GENERIC_WRITE, FILE_SHARE_READ, 0
				, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			NkError::CWinApiException::check(m_hFile, "WriteFile", __FILE__, __LINE__);
		}

		void AddLog(LPCWSTR pszMessage)
		{
			size_t cb = 0;
			HRESULT hr = StringCbLength(pszMessage, 2048, &cb);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			DWORD cbw = 0;
			BOOL bRet = WriteFile(m_hFile, pszMessage, cb, &cbw, 0);
			NkError::CWinApiException::check(bRet, "WriteFile", __FILE__, __LINE__);

			CheckFileSize();
		}
		void AddLog(LPCSTR pszMessage)
		{
			WCHAR szBuffer[1024];

			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszMessage, -1
				, szBuffer, sizeof(szBuffer));

			AddLog(szBuffer);
		}

		NkThreading::CCriticalSection m_lock;
		HANDLE m_hFile;
		wchar_t m_szFilePath[MAX_PATH];
		wchar_t m_szApplicationName[MAX_PATH];
		wchar_t m_szLogFileName[MAX_PATH];
		GUID m_FolderGuid;
		int m_FileNumber;
		int m_CheckHint;
	};

	CLogFile::CLogFile(LPCWSTR pszApplicationName, LPCWSTR pszLogFileName/* = L"Log"*/
		, GUID guid /* = FOLDERID_ProgramData*/)
		: _Impl(*new CImpl(pszApplicationName, pszLogFileName, guid))
	{
	}

	CLogFile::~CLogFile(void)
	{
		delete &_Impl;
	}

	void CLogFile::AddLog(LPCWSTR pszMessage)
	{
		try {
			NkThreading::CLockGuard lock(_Impl.m_lock);
			_Impl.AddLog(pszMessage);
		}
		catch (...) {}
	}

	void CLogFile::AddLog(LPCSTR pszMessage)
	{
		try {
			NkThreading::CLockGuard lock(_Impl.m_lock);
			_Impl.AddLog(pszMessage);
		}
		catch (...) {}
	}

	void CLogFile::FormatLog(LPCSTR pszFormat, ...)
	{
		NkThreading::CLockGuard lock(_Impl.m_lock);

		char szBuffer[1024] = { 0 };

		va_list args;
		va_start(args, pszFormat);
		_vsnprintf_s(szBuffer, 1020, _TRUNCATE, pszFormat, args);

		try {
			_Impl.AddLog(szBuffer);
		}
		catch (...) {}
	}

	void CLogFile::FormatLog(LPCWSTR pszFormat, ...)
	{
		NkThreading::CLockGuard lock(_Impl.m_lock);

		WCHAR szBuffer[1024] = { 0 };

		va_list args;
		va_start(args, pszFormat);
		_vsnwprintf_s(szBuffer, 1020, _TRUNCATE, pszFormat, args);

		try {
			_Impl.AddLog(szBuffer);
		}
		catch (...) {}
	}

	LPCWSTR CLogFile::LogPath()
	{
		return _Impl.m_szFilePath;
	}
}