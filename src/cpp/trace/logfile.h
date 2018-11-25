#pragma once
#include <guiddef.h>
#include <KnownFolders.h>
#include "types/types.h"

namespace NkTrace
{
	class  CLogFile
	{
	public:
		CLogFile(LPCWSTR pszApplicationName, LPCWSTR pszLogFileName = L"Log"
			, GUID guid = FOLDERID_ProgramData);
		virtual ~CLogFile();

		void AddLog(LPCSTR pszMessage) throw();
		void AddLog(LPCWSTR pszMessage) throw();

		void FormatLog(LPCSTR pszFormat, ...) throw();
		void FormatLog(LPCWSTR pszFormat, ...) throw();

		LPCWSTR LogPath();

	private:
		CLogFile(const CLogFile&);
		CLogFile& operator =(const CLogFile&);

		class CImpl;
		CImpl& _Impl;
	};
}
