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
