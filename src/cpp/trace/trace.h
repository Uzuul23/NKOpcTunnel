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

#ifndef MYDLLEXPORT
#define MYDLLEXPORT
#endif

namespace NkTrace
{
	class CTraceEvent;
	class MYDLLEXPORT CTrace
	{
	public:
		static CTrace& Instance() throw();

		enum TraceClasses
		{
			TraceDetailed,
			TraceInfo,
			TraceAlwaysInfo,
			TraceError
		};

		enum TraceLevels
		{
			TraceLevel_0, // 0 = no trace
			TraceLevel_1, // 1 = TraceError and TraceAlwaysInfo only
			TraceLevel_2, // 2 = TraceError and TraceInfo
			TraceLevel_3  // 3 = TraceError, TraceInfo and TraceDetailed
		};

		static LPCWSTR ToString(TraceClasses TraceClass);

		void Subscribe(CTraceEvent* pEvent) throw();
		void UnSubscribe(CTraceEvent* pEvent) throw();

		TraceLevels TraceLevel() const throw();
		void TraceLevel(TraceLevels level) throw();

		static inline bool IsTraceLevel1() throw();
		static inline bool IsTraceLevel2() throw();
		static inline bool IsTraceLevel3() throw();

		void Trace(TraceClasses TraceClass, const wchar_t* pszFormat, ...) throw();
		void Trace(TraceClasses TraceClass, const char* pszFormat, ...) throw();

		static void TraceErr(const wchar_t* pszFormat, ...) throw();
		static void TraceDet(const wchar_t* pszFormat, ...) throw();

		static void trace_info(const wchar_t* pszFormat, ...) throw();
		static void trace_info(const char* pszFormat, ...) throw();

		/*void NKSetPrefix(LPCWSTR lpszPrefix);

		void NKTraceSubscribe(CTraceEvent* p, TraceLevel level);
		void NKTraceUnsubscribe(CTraceEvent* p);

		void NKTrace(TraceClass traceclass, LPCWSTR pszText);
		void NKTrace(TraceClass traceclass, LPCSTR pszText);

		void NKFormatTrace(TraceClass traceclass, LPCSTR pszFormat, ...);
		void NKFormatTrace(TraceClass traceclass, LPCWSTR pszFormat, ...);

		void NKLogError(LPSTR lpszfunction, LPSTR lpszfile, int line, int cause, DWORD code);
		void NKLogOpenSSLError(unsigned long Err, LPSTR lpszfile, int line);
		LPCSTR NKGetErrorText(int cause);

		void NKFormatSystemMessage(HRESULT hr, LPWSTR pszMessage, size_t cbMessage);
		void NKFormatSystemMessage(HRESULT hr, LPSTR pszMessage, size_t cbMessage);*/

	private:
		CTrace(void);
		~CTrace(void);
		CTrace & operator =(const CTrace&);
		CTrace(const CTrace&);
		class CImpl;
		CImpl* _Impl;
	};

	bool CTrace::IsTraceLevel1() throw()
	{
		return Instance().TraceLevel() >= TraceLevel_1;
	}

	bool CTrace::IsTraceLevel2() throw()
	{
		return Instance().TraceLevel() >= TraceLevel_2;
	}

	bool CTrace::IsTraceLevel3() throw()
	{
		return Instance().TraceLevel() >= TraceLevel_3;
	}

	class CTraceEvent
	{
	public:
		virtual void TraceOut(CTrace::TraceClasses traceclass, const wchar_t* pszText) = 0;
	};
}