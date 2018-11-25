#include "StdAfx.h"
#include "trace.h"
#include "threading/lock.h"
#include <list>
#include <stringapiset.h>
#include <strsafe.h>

namespace NkTrace
{
	class CTrace::CImpl
	{
	public:
		CImpl() : m_TraceLevel(TraceLevel_1)
		{
		}

		TraceLevels m_TraceLevel;
		std::list<CTraceEvent*> m_TraceEventRcv;
		NkThreading::CCriticalSection m_Lock;

		void TraceOut(TraceClasses TraceClass, const wchar_t* pszText)
		{
			SYSTEMTIME st;
			::GetSystemTime(&st);

			wchar_t szBuffer[1024] = { 0 };
			HRESULT hr = StringCbPrintfW(szBuffer, sizeof(szBuffer)
				, L"%4d-%02d-%02d %02d:%02d:%02d.%03d [%d:%d] %s: %s"
				, st.wYear
				, st.wMonth
				, st.wDay
				, st.wHour
				, st.wMinute
				, st.wSecond
				, st.wMilliseconds
				, GetCurrentProcessId()
				, GetCurrentThreadId()
				, CTrace::ToString(TraceClass)
				, pszText);

			_ASSERT(SUCCEEDED(hr));

			std::list<CTraceEvent*>::iterator i;
			for (i = m_TraceEventRcv.begin(); i != m_TraceEventRcv.end(); ++i) {
				(*i)->TraceOut(TraceClass, szBuffer);
			}
		}

		void TraceOut(TraceClasses TraceClass, const char* pszText)
		{
			wchar_t szBuffer[1024] = { 0 };
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszText, -1, szBuffer, 1024);

			TraceOut(TraceClass, szBuffer);
		}
	};

	CTrace::CTrace() : _Impl(new CImpl)
	{
	}

	CTrace::~CTrace(void)
	{
		delete _Impl;
	}

	CTrace& CTrace::Instance() throw()
	{
		static CTrace _Instance;
		return _Instance;
	}

	CTrace::TraceLevels CTrace::TraceLevel() const throw()
	{
		return _Impl->m_TraceLevel;
	}

	void CTrace::TraceLevel(TraceLevels level) throw()
	{
		_Impl->m_TraceLevel = level;
	}

	void CTrace::Trace(TraceClasses TraceClass, const wchar_t* pszFormat, ...) throw()
	{
		NkThreading::CLockGuard lock(_Impl->m_Lock);

		if (_Impl->m_TraceLevel >= TraceClass)
		{
			wchar_t szBuffer[1024] = { 0 };

			va_list args;
			va_start(args, pszFormat);
			_vsnwprintf_s(szBuffer, 1024, _TRUNCATE, pszFormat, args);

			_Impl->TraceOut(TraceClass, szBuffer);
		}
	}

	void CTrace::Trace(TraceClasses TraceClass, const char* pszFormat, ...) throw()
	{
		NkThreading::CLockGuard lock(_Impl->m_Lock);

		if (_Impl->m_TraceLevel >= TraceClass)
		{
			char szBuffer[1024] = { 0 };

			va_list args;
			va_start(args, pszFormat);
			_vsnprintf_s(szBuffer, 1024, _TRUNCATE, pszFormat, args);

			_Impl->TraceOut(TraceClass, szBuffer);
		}
	}

	void CTrace::Subscribe(CTraceEvent* pEvent) throw()
	{
		NkThreading::CLockGuard lock(_Impl->m_Lock);
		_Impl->m_TraceEventRcv.push_back(pEvent);
	}

	void CTrace::UnSubscribe(CTraceEvent* pEvent) throw()
	{
		NkThreading::CLockGuard lock(_Impl->m_Lock);
		_Impl->m_TraceEventRcv.remove(pEvent);
	}

	void CTrace::TraceErr(const wchar_t* pszFormat, ...) throw()
	{
		CTrace& trace = Instance();

		if (trace._Impl->m_TraceLevel >= TraceLevel_1)
		{
			wchar_t szBuffer[1024] = { 0 };

			va_list args;
			va_start(args, pszFormat);
			_vsnwprintf_s(szBuffer, 1024, _TRUNCATE, pszFormat, args);

			trace._Impl->TraceOut(TraceError, szBuffer);
		}
	}

	void CTrace::TraceDet(const wchar_t* pszFormat, ...) throw()
	{
		CTrace& trace = Instance();

		if (trace._Impl->m_TraceLevel >= TraceLevel_3) {
			wchar_t szBuffer[1024] = { 0 };

			va_list args;
			va_start(args, pszFormat);
			_vsnwprintf_s(szBuffer, 1024, _TRUNCATE, pszFormat, args);

			trace._Impl->TraceOut(TraceDetailed, szBuffer);
		}
	}

	void CTrace::trace_info(const wchar_t* pszFormat, ...) throw()
	{
		CTrace& trace = Instance();

		if (trace._Impl->m_TraceLevel >= TraceLevel_1)
		{
			wchar_t szBuffer[1024] = { 0 };

			va_list args;
			va_start(args, pszFormat);
			_vsnwprintf_s(szBuffer, 1024, _TRUNCATE, pszFormat, args);

			trace._Impl->TraceOut(TraceInfo, szBuffer);
		}
	}

	void CTrace::trace_info(const char* pszFormat, ...) throw()
	{
		CTrace& trace = Instance();

		if (trace._Impl->m_TraceLevel >= TraceLevel_1)
		{
			char szBuffer[1024] = { 0 };

			va_list args;
			va_start(args, pszFormat);
			_vsnprintf_s(szBuffer, 1024, _TRUNCATE, pszFormat, args);

			trace._Impl->TraceOut(TraceInfo, szBuffer);
		}
	}

	LPCWSTR CTrace::ToString(TraceClasses TraceClass)
	{
		switch (TraceClass)
		{
		case NkTrace::CTrace::TraceDetailed:
		case NkTrace::CTrace::TraceInfo:
		case NkTrace::CTrace::TraceAlwaysInfo:
			return L"Message";
		case NkTrace::CTrace::TraceError:
			return L"Error";
		default: return L"";
		}
	}
	

	//void CTrace::NKLogError(LPSTR lpszfunction, LPSTR lpszfile, int line, int cause, DWORD code)
	//{
	//	/*CNKLockGuard lock(_Impl.m_Lock);

	//	CHAR buffer[1024] = {0};

	//	if (code == 0)
	//	{
	//		StringCbPrintfA(buffer, sizeof(buffer)
	//			, "%s: %s file: %s line: %d"
	//			, lpszfunction, CNKExcpt::ErrorText(cause)
	//			, _Impl.TrimFilePath(lpszfile), line);
	//	}
	//	else
	//	{
	//		CHAR szError[1024] = {0};
	//		NKFormatSystemMessage(code, szError, sizeof(szError));

	//		StringCbPrintfA(buffer, sizeof(buffer)
	//			, "%s: %s file: %s line: %d"
	//			, lpszfunction, szError
	//			, _Impl.TrimFilePath(lpszfile), line);
	//	}

	//	_Impl.TraceOut(TraceError, buffer);*/
	//}

	//void CNKTrace::NKLogOpenSSLError(unsigned long Err, LPSTR lpszfile, int line)
	//{
	//	//CNKLockGuard lock(_Impl.m_Lock);

	//	//CHAR buffer[1024] = {0};
	//	//CHAR szFile[MAX_PATH] = {0};
	//	//char errbuffer[1024];

	//	//ERR_error_string_n(Err, errbuffer, sizeof(errbuffer));

	//	//// trim file path
	//	//size_t cbLenght = 0;
	//	//StringCbLengthA(lpszfile, sizeof(szFile), &cbLenght);
	//	//StringCbCopyA(szFile, sizeof(szFile), lpszfile);

	//	//size_t loop = cbLenght;
	//	//while (--loop)
	//	//{
	//	//	if(lpszfile[loop] == '\\')
	//	//	{
	//	//		loop++;
	//	//		StringCbCopyA(szFile, sizeof(szFile), &lpszfile[loop]);
	//	//		break;
	//	//	}
	//	//}

	//	//StringCbPrintfA(buffer, sizeof(buffer), "OpenSSL %s -> NK call in file: %s line: %d", errbuffer, szFile, line);

	//	//_Impl.TraceOut(TraceError, buffer);
	//}

	//CNKTrace::TraceLevel CNKTrace::NKTraceLevel() const
	//{
	//	return _Impl.m_tracelevel;
	//}

	//void CNKTrace::NKTraceLevel(TraceLevel level)
	//{
	//	_Impl.m_tracelevel = level;
	//}

	//void CNKTrace::NKTraceSubscribe( CNKTraceEvent* p, TraceLevel level)
	//{
	//	CNKLockGuard lock(_Impl.m_Lock);

	//	_Impl.TraceSubscribe(p, level);
	//}

	//void CNKTrace::NKTraceUnsubscribe( CNKTraceEvent* p )
	//{
	//	CNKLockGuard lock(_Impl.m_Lock);

	//	_Impl.TraceUnsubscribe(p);
	//}

	//void CNKTrace::NKFormatSystemMessage( HRESULT hr, LPWSTR pszMessage, size_t cbMessage )
	//{
	//	CNKLockGuard lock(_Impl.m_Lock);

	//	::FormatMessageW( FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS,
	//		NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
	//		, pszMessage, cbMessage/sizeof(wchar_t), NULL );

	//	// trim message
	//	size_t cbLenght = 0;
	//	StringCbLength(pszMessage, cbMessage, &cbLenght);

	//	size_t loop = cbLenght;
	//	while (loop > 0 && --loop > 0)
	//	{
	//		if(isspace(pszMessage[loop]))
	//		{
	//			pszMessage[loop] = 0;
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}
	//}

	//void CNKTrace::NKFormatSystemMessage( HRESULT hr, LPSTR pszMessage, size_t cbMessage )
	//{
	//	CNKLockGuard lock(_Impl.m_Lock);

	//	::FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS,
	//		NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
	//		, pszMessage, cbMessage, NULL );

	//	// trim message
	//	size_t cbLenght = 0;
	//	StringCbLengthA(pszMessage, cbMessage, &cbLenght);

	//	size_t loop = cbLenght;
	//	while (loop > 0 && --loop > 0)
	//	{
	//		if(isspace(pszMessage[loop]))
	//		{
	//			pszMessage[loop] = 0;
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}
	//}
}