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
#include <WinCred.h>
#include "../types/types.h"

namespace NkUtil
{
	class CPassVault
	{
	public:
		CPassVault(LPCWSTR pszTargetName = 0, DWORD dwCredentailType = CRED_TYPE_GENERIC);
		virtual ~CPassVault();

		bool cred_dlg(LPCWSTR pszCaptionText, LPCWSTR pszMessageText, bool* pbsafe = 0
			, HWND hParentWnd = 0);

		void read();
		bool readIf();
		void write();
		void remove();
		void rename(LPCWSTR pszNewTargetName);
		LPCWSTR userName();
		void userName(LPCWSTR psz);
		LPCBYTE credentailBlob(size_t& cb);
		void credentailBlob(LPCBYTE p, size_t cb);
		void setAuthenticationBlob(LPCBYTE p, size_t cb);
		void createAuthenticationBlob(LPBYTE* pp, size_t& cb);
		void deleteAuthenticationBlob(LPBYTE p, size_t cb);

	private:
		CPassVault &operator =(const CPassVault&);
		CPassVault(const CPassVault&);
		class CImpl;
		CImpl* _Impl;
	};
}
