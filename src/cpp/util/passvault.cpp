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
#include "passvault.h"
#include "types/conversion.h"
#include "types/String.h"
#include "error/winapiexception.h"
#include "error/baseexception.h"
#include <vector>
#include "Userenv.h"

namespace NkUtil
{
	class CPassVault::CImpl
	{
	public:
		~CImpl()
		{
			clear_blob();
		}

		void clear_blob()
		{
			if (m_cb_credential_blob)
			{
				SecureZeroMemory(m_p_credential_blop, m_cb_credential_blob);
				delete[] m_p_credential_blop;
				m_cb_credential_blob = 0;
				m_p_credential_blop = 0;
			}
		}

		DWORD m_dw_credentail_type = CRED_TYPE_GENERIC;
		BYTE* m_p_credential_blop = 0;
		size_t m_cb_credential_blob = 0;
		NkType::CString m_str_user_name;
		NkType::CString m_strTargetName;
	};

	CPassVault::CPassVault(LPCWSTR pszTargetName /*= 0*/
		, DWORD dwCredentailType/* = CRED_TYPE_GENERIC*/) : _Impl(new CImpl)
	{
		_Impl->m_dw_credentail_type = dwCredentailType;
		_Impl->m_strTargetName = pszTargetName;

		/*wchar_t sz_user_name[MAX_PATH];
		DWORD bufferLength = sizeof(sz_user_name) / sizeof(sz_user_name[0]);
		::GetUserName(sz_user_name, &bufferLength);

		HANDLE hToken;
		::OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);

		PROFILEINFO profile = {};
		profile.dwSize = sizeof(profile);
		profile.lpUserName = sz_user_name;
		::LoadUserProfile(hToken, &profile);*/
	}

	CPassVault::~CPassVault()
	{
		delete _Impl;
	}

	bool CPassVault::cred_dlg(LPCWSTR pszCaptionText
		, LPCWSTR pszMessageText
		, bool* pbsafe /*= 0*/
		, HWND hParentWnd /*= 0*/)
	{

		CREDUI_INFO credui = { 0 };
		credui.cbSize = sizeof(credui);
		credui.pszCaptionText = const_cast<LPWSTR>(pszCaptionText);
		credui.pszMessageText = const_cast<LPWSTR>(pszMessageText);
		credui.hwndParent = hParentWnd;

		DWORD dwAuthError = 0;
		ULONG ulauthPackage = 0;
		void* pvOutAuthBuffer = 0;
		ULONG ulOutAuthBufferSize = 0;

		DWORD cbPackedCredentials = 0;
		BOOL ret = ::CredPackAuthenticationBuffer(CRED_PACK_GENERIC_CREDENTIALS
			, _Impl->m_str_user_name, L"", 0, &cbPackedCredentials);

		std::vector<BYTE> vecPackedCredentials(cbPackedCredentials);

		ret = ::CredPackAuthenticationBuffer(CRED_PACK_GENERIC_CREDENTIALS
			, _Impl->m_str_user_name, L"", vecPackedCredentials.data(), &cbPackedCredentials);

		if (pbsafe) {
			BOOL bfSafe = FALSE;
			if (CredUIPromptForWindowsCredentials(&credui, dwAuthError, &ulauthPackage,
				vecPackedCredentials.data(), cbPackedCredentials, &pvOutAuthBuffer, &ulOutAuthBufferSize, &bfSafe
				, CREDUIWIN_GENERIC | CREDUIWIN_CHECKBOX) != ERROR_SUCCESS) {
				return false;
			}
			(*pbsafe) = NkType::to_bool(bfSafe);
		}
		else {
			if (CredUIPromptForWindowsCredentials(&credui, dwAuthError, &ulauthPackage,
				vecPackedCredentials.data(), cbPackedCredentials, &pvOutAuthBuffer, &ulOutAuthBufferSize, 0
				, CREDUIWIN_GENERIC) != ERROR_SUCCESS) {
				return false;
			}
		}

		wchar_t szUserName[CREDUI_MAX_USERNAME_LENGTH + 1] = L"";
		DWORD cchMaxUserName = CREDUI_MAX_USERNAME_LENGTH;
		wchar_t szDomainName[CREDUI_MAX_USERNAME_LENGTH + 1] = L"";
		DWORD  cchMaxDomainname = CREDUI_MAX_USERNAME_LENGTH;
		wchar_t szPassword[CREDUI_MAX_PASSWORD_LENGTH + 1] = L"";
		DWORD  cchMaxPassword = CREDUI_MAX_PASSWORD_LENGTH;

		ret = ::CredUnPackAuthenticationBuffer(0, pvOutAuthBuffer, ulOutAuthBufferSize
			, szUserName, &cchMaxUserName, szDomainName, &cchMaxDomainname
			, szPassword, &cchMaxPassword);

		SecureZeroMemory(pvOutAuthBuffer, ulOutAuthBufferSize);
		CoTaskMemFree(pvOutAuthBuffer);

		NkError::CWinApiException::check_bool(ret, "CredUnPackAuthenticationBuffer", __FILE__, __LINE__);

		_Impl->m_str_user_name = szUserName;
		_Impl->m_cb_credential_blob = cchMaxPassword * sizeof(wchar_t);
		_Impl->m_p_credential_blop = new BYTE[_Impl->m_cb_credential_blob];
		memcpy_s(_Impl->m_p_credential_blop, _Impl->m_cb_credential_blob, szPassword, cchMaxPassword * sizeof(wchar_t));

		return true;
	}

	void CPassVault::read()
	{
		_Impl->clear_blob();
		_Impl->m_str_user_name = L"";
		PCREDENTIAL rcred = 0;

		BOOL ret = ::CredRead(_Impl->m_strTargetName, _Impl->m_dw_credentail_type, 0, &rcred);
		NkError::CWinApiException::check_bool(ret, "CredRead", __FILE__, __LINE__);

		if (rcred->CredentialBlobSize) {
			_Impl->m_cb_credential_blob = rcred->CredentialBlobSize;
			_Impl->m_p_credential_blop = new BYTE[rcred->CredentialBlobSize];
			memcpy_s(_Impl->m_p_credential_blop, _Impl->m_cb_credential_blob
				, rcred->CredentialBlob, rcred->CredentialBlobSize);
			::SecureZeroMemory(rcred->CredentialBlob, rcred->CredentialBlobSize);
		}
		_Impl->m_str_user_name = rcred->UserName;

		::CredFree(rcred);
	}

	bool CPassVault::readIf()
	{
		_Impl->clear_blob();
		_Impl->m_str_user_name = L"";
		PCREDENTIAL rcred = 0;

		BOOL ret = ::CredRead(_Impl->m_strTargetName, _Impl->m_dw_credentail_type, 0, &rcred);
		if (ret == FALSE) {
			return false;
		}

		if (rcred->CredentialBlobSize) {
			_Impl->m_cb_credential_blob = rcred->CredentialBlobSize;
			_Impl->m_p_credential_blop = new BYTE[rcred->CredentialBlobSize];
			memcpy_s(_Impl->m_p_credential_blop, _Impl->m_cb_credential_blob
				, rcred->CredentialBlob, rcred->CredentialBlobSize);
			::SecureZeroMemory(rcred->CredentialBlob, rcred->CredentialBlobSize);
		}
		_Impl->m_str_user_name = rcred->UserName;

		::CredFree(rcred);
		return true;
	}

	void CPassVault::write()
	{
		CREDENTIAL cred = { 0 };
		cred.Type = _Impl->m_dw_credentail_type;
		cred.TargetName = const_cast<LPWSTR>(_Impl->m_strTargetName.data());
		cred.CredentialBlobSize = _Impl->m_cb_credential_blob;
		cred.CredentialBlob = _Impl->m_p_credential_blop;
		cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
		cred.UserName = const_cast<LPWSTR>(_Impl->m_str_user_name.data());
		NkError::CWinApiException::check_bool(::CredWrite(&cred, 0), "CredWrite", __FILE__, __LINE__);
	}

	void CPassVault::rename(LPCWSTR pszNewTargetName)
	{
		if (wcscmp(_Impl->m_strTargetName.data(), pszNewTargetName) == 0) {
			return;
		}

		PCREDENTIAL rcred = 0;
		BOOL ret = ::CredRead(_Impl->m_strTargetName.data(), _Impl->m_dw_credentail_type, 0, &rcred);
		NkError::CWinApiException::check_bool(ret, "CredRead", __FILE__, __LINE__);

		CREDENTIAL cred = { 0 };
		cred.Type = _Impl->m_dw_credentail_type;
		cred.TargetName = const_cast<LPWSTR>(pszNewTargetName);
		cred.CredentialBlobSize = rcred->CredentialBlobSize;
		cred.CredentialBlob = rcred->CredentialBlob;
		cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
		cred.UserName = rcred->UserName;
		ret = ::CredWrite(&cred, 0);
		::SecureZeroMemory(rcred->CredentialBlob, rcred->CredentialBlobSize);
		::CredFree(rcred);
		NkError::CWinApiException::check_bool(ret, "CredWrite", __FILE__, __LINE__);

		ret = ::CredDelete(_Impl->m_strTargetName.data(), _Impl->m_dw_credentail_type, 0);
		NkError::CWinApiException::check_bool(ret, "CredDelete", __FILE__, __LINE__);
	}

	LPCWSTR CPassVault::userName()
	{
		return _Impl->m_str_user_name;
	}

	void CPassVault::userName(LPCWSTR psz)
	{
		_Impl->m_str_user_name = psz;
	}

	LPCBYTE CPassVault::credentailBlob(size_t& cb)
	{
		cb = _Impl->m_cb_credential_blob;
		return _Impl->m_p_credential_blop;
	}

	void CPassVault::credentailBlob(LPCBYTE p, size_t cb)
	{
		_Impl->clear_blob();
		if (cb) {
			_Impl->m_cb_credential_blob = cb;
			_Impl->m_p_credential_blop = new BYTE[cb];
			memcpy_s(_Impl->m_p_credential_blop, _Impl->m_cb_credential_blob, p, cb);
		}
	}

	void CPassVault::remove()
	{
		BOOL ret = ::CredDelete(_Impl->m_strTargetName, _Impl->m_dw_credentail_type, 0);
		NkError::CWinApiException::check_bool(ret, "CredDelete", __FILE__, __LINE__);
	}

	const wchar_t szIdent[] = L"NkAuthBlob_1";

	// Identification String UserName String Password
	// L"NkAuthBlob_1"L"User""secret"

	void CPassVault::createAuthenticationBlob(LPBYTE* pp, size_t& cb)
	{
		size_t cb2 = sizeof(szIdent) + _Impl->m_str_user_name.byte_lenght()
			+ _Impl->m_cb_credential_blob;

		cb = cb2;

		LPBYTE p = new BYTE[cb];
		(*pp) = p;

		memcpy_s(p, cb2, szIdent, sizeof(szIdent));
		p += sizeof(szIdent);
		cb2 -= sizeof(szIdent);

		memcpy_s(p, cb2, _Impl->m_str_user_name.data(), _Impl->m_str_user_name.byte_lenght());
		p += _Impl->m_str_user_name.byte_lenght();
		cb2 -= _Impl->m_str_user_name.byte_lenght();

		memcpy_s(p, cb2, _Impl->m_p_credential_blop, _Impl->m_cb_credential_blob);
	}

	void CPassVault::deleteAuthenticationBlob(LPBYTE p, size_t cb)
	{
		SecureZeroMemory(p, cb);
		delete[] p;
	}

	void CPassVault::setAuthenticationBlob(LPCBYTE p, size_t cb)
	{
		_Impl->clear_blob();
		_Impl->m_str_user_name = L"";

		if (memcmp(p, szIdent, min(sizeof(szIdent), cb)) != 0) {
			throw NkError::CBaseException(NkError::CBaseException::invalid_argument, __FILE__, __LINE__);
		}
		p += sizeof(szIdent);
		cb -= sizeof(szIdent);

		_Impl->m_str_user_name = reinterpret_cast<const wchar_t*>(p);
		p += _Impl->m_str_user_name.byte_lenght();
		cb -= _Impl->m_str_user_name.byte_lenght();

		if (cb) {
			_Impl->m_cb_credential_blob = cb;
			_Impl->m_p_credential_blop = new BYTE[cb];
			memcpy_s(_Impl->m_p_credential_blop, _Impl->m_cb_credential_blob, p, cb);
		}
	}
}