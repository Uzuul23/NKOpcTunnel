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
