#include "stdafx.h"
#include "../error/baseexception.h"
#include "../error/winapiexception.h"
#include "registry.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <strsafe.h>

namespace NkWin
{
	CRegistry::CRegistry() : m_hKey(0), m_pBuffer(0), m_cbBuffer(0), m_pChar(0), m_cbChar(0)
	{
	}

	CRegistry::CRegistry(LPCWSTR pszSubKey, HKEY hKey /*= HKEY_CLASSES_ROOT*/, REGSAM samDesired /*= KEY_READ */, DWORD ulOptions /*= 0*/) : m_hKey(0), m_pBuffer(0), m_cbBuffer(0), m_pChar(0), m_cbChar(0)
	{
		Open(pszSubKey, hKey, samDesired, ulOptions);
	}

	CRegistry::CRegistry(REFCLSID clsid, LPCWSTR pszSubKey /*= L""*/, REGSAM samDesired /*= KEY_READ*/) : m_pBuffer(0), m_cbBuffer(0), m_pChar(0), m_cbChar(0)
	{
		WCHAR* pszCLSID = 0;
		WCHAR szSubKey[MAX_PATH] = L"";
		NkError::CBaseException::check_result(StringFromCLSID(clsid, &pszCLSID), __FILE__, __LINE__);
		NkError::CBaseException::check_result(StringCchPrintf(szSubKey, MAX_PATH, L"clsid\\%s\\%s", pszCLSID, pszSubKey), __FILE__, __LINE__);
		LONG lRet = ::RegOpenKeyEx(HKEY_CLASSES_ROOT, szSubKey, 0, samDesired, &m_hKey);
		NkError::CWinApiException::check_error(lRet, "RegOpenKeyEx", __FILE__, __LINE__);
	}

	CRegistry::CRegistry(CRegistry& d)
	{
		*this = d;
	}

	CRegistry & CRegistry::operator=(const CRegistry& d)
	{
		Close();
		m_hKey = ::SHRegDuplicateHKey(d.m_hKey);
		if (d.m_cbBuffer) {
			m_pBuffer = new BYTE[d.m_cbBuffer];
			m_cbBuffer = d.m_cbBuffer;
			memcpy_s(m_pBuffer, d.m_cbBuffer, d.m_pBuffer, d.m_cbBuffer);
		}
		if (d.m_cbChar) {
			m_pChar = new char[d.m_cbChar];
			m_cbChar = d.m_cbChar;
			memcpy_s(m_pChar, d.m_cbChar, d.m_pChar, d.m_cbChar);
		}
		return *this;
	}

	CRegistry::~CRegistry()
	{
		delete[] m_pChar;
		delete[] m_pBuffer;
		Close();
	}

	bool CRegistry::IsOpen()
	{
		return m_hKey != 0;
	}

	CRegistry::operator HKEY()
	{
		return m_hKey;
	}

	HKEY* CRegistry::operator&()
	{
		return &m_hKey;
	}

	bool CRegistry::OpenIf(LPCWSTR pszSubKey, HKEY hKey /*= HKEY_CLASSES_ROOT*/, REGSAM samDesired /*= KEY_READ */, DWORD ulOptions /*= 0*/)
	{
		Close();
		LONG lRet = ::RegOpenKeyEx(hKey, pszSubKey, ulOptions, samDesired, &m_hKey);
		return lRet == ERROR_SUCCESS;
	}

	void CRegistry::Open(LPCWSTR pszSubKey, HKEY hKey /*= HKEY_CLASSES_ROOT*/, REGSAM samDesired /*= KEY_READ */, DWORD ulOptions /*= 0*/)
	{
		Close();
		LONG lRet = ::RegOpenKeyEx(hKey, pszSubKey, ulOptions, samDesired, &m_hKey);
		NkError::CWinApiException::check_error(lRet, "RegOpenKeyEx", __FILE__, __LINE__);
	}

	void CRegistry::Close()
	{
		if (m_hKey) {
			LONG lRet = ::RegCloseKey(m_hKey);
			m_hKey = 0;
			NkError::CWinApiException::check_error(lRet, "RegCloseKey", __FILE__, __LINE__);
		}
	}

	void CRegistry::QueryInfoKey(LPTSTR pchClass /*= 0 /*buffer for class name */, LPDWORD pcchClassName /*= 0 /*size of class string */, LPDWORD pcSubKeys /*= 0 /*number of subkeys */, LPDWORD pcbMaxSubKey /*= 0 /*longest subkey size */, LPDWORD pcchMaxClass /*= 0 /*longest class string */, LPDWORD pcValues /*= 0 /*number of values for key */, LPDWORD pcchMaxValue /*= 0 /*longest value name */, LPDWORD pcbMaxValueData /*= 0 /*longest value data */, LPDWORD pcbSecurityDescriptor /*= 0 /*size of security descriptor */, LPFILETIME pftLastWriteTime /*= 0 /*last write time */)
	{
		LONG lRet = ::RegQueryInfoKey(m_hKey, pchClass, pcchClassName, 0, pcSubKeys, pcbMaxSubKey
			, pcchMaxClass, pcValues, pcchMaxValue, pcbMaxValueData, pcbSecurityDescriptor
			, pftLastWriteTime);
		NkError::CWinApiException::check_error(lRet, "RegQueryInfoKey", __FILE__, __LINE__);
	}

	void CRegistry::CreateKey(LPCTSTR lpSubKey /*name of a sub key that this function opens or creates */
		, PHKEY phkResult /*= 0 /*pointer to a variable that receives a handle to the opened or created key */
		, REGSAM samDesired /*= KEY_READ /*mask that specifies the access rights for the key to be created */
		, LPTSTR lpClass /*= 0 /*user-defined class type of this key */
		, DWORD dwOptions /*= REG_OPTION_NON_VOLATILE /*options */
		, LPSECURITY_ATTRIBUTES lpSecurityAttributes /*= 0 /*pointer to a SECURITY_ATTRIBUTES structure */
		, LPDWORD lpdwDisposition /*= 0 /*pointer to a variable that receives the disposition values */)
	{
		if (phkResult == 0) {
			CRegistry Key;
			LONG lRet = ::RegCreateKeyEx(m_hKey, lpSubKey, 0, lpClass, dwOptions, samDesired
				, lpSecurityAttributes, &Key, lpdwDisposition);
			NkError::CWinApiException::check_error(lRet, "RegCreateKeyEx", __FILE__, __LINE__);
			return;
		}

		LONG lRet = ::RegCreateKeyEx(m_hKey, lpSubKey, 0, lpClass, dwOptions, samDesired
			, lpSecurityAttributes, phkResult, lpdwDisposition);
		NkError::CWinApiException::check_error(lRet, "RegCreateKeyEx", __FILE__, __LINE__);
	}

	void CRegistry::DeleteKey(LPCTSTR lpSubKey /*name of the key to be deleted */
		, REGSAM samDesired /*= KEY_WOW64_32KEY /*access mask the specifies the platform-specific view of the registry */)
	{
		LONG lRet = ::RegDeleteKeyEx(m_hKey, lpSubKey, samDesired, 0);
		NkError::CWinApiException::check_error(lRet, "RegDeleteKeyEx", __FILE__, __LINE__);
	}

	void CRegistry::DeleteTree(LPCTSTR lpSubKey /*name of the key to be deleted */)
	{
		LONG lRet = ::RegDeleteTree(m_hKey, lpSubKey);
		NkError::CWinApiException::check_error(lRet, "RegDeleteTree", __FILE__, __LINE__);
	}

	bool CRegistry::RegEnumKey(DWORD dwIndex /*index of the sub key to retrieve */
		, LPTSTR lpName /*pointer to a buffer that receives the name of the sub key */
		, LPDWORD lpcName /*size of the buffer specified by the lpName */
		, LPTSTR lpClass /*= 0 /*pointer to a buffer that receives the user-defined class */
		, LPDWORD lpcClass /*= 0 /* size of the buffer specified by the lpClass */
		, PFILETIME lpftLastWriteTime /*= 0 /*the time at which the sub key was last written */)
	{
		LONG lRet = ::RegEnumKeyEx(m_hKey, dwIndex, lpName, lpcName, 0
			, lpClass, lpcClass, lpftLastWriteTime);

		if (lRet == ERROR_NO_MORE_ITEMS) {
			return false;
		}
		NkError::CWinApiException::check_error(lRet, "RegEnumKeyEx", __FILE__, __LINE__);
		return true;
	}

	void CRegistry::DeleteKeyValue(LPCWSTR pszValueName /*the name of the value to delete */)
	{
		LONG lRet = ::RegDeleteKeyValueW(m_hKey, 0, pszValueName);
		NkError::CWinApiException::check_error(lRet, "RegDeleteKeyValueW", __FILE__, __LINE__);
	}

	void CRegistry::QueryValue(NkType::CString& strValue, LPCWSTR pszValueName /*= 0*/)
	{
		size_t cb = 0;
		DWORD Type;
		const BYTE* pData = QueryValueBin(cb, pszValueName, &Type);
		NkError::CBaseException::check(Type == REG_SZ, E_INVALIDARG, __FILE__, __LINE__);
		strValue = reinterpret_cast<const WCHAR*>(pData);
	}

	LPCSTR CRegistry::QueryValueAnsiString(LPCWSTR pszValueName /*= 0*/)
	{
		size_t cb = 0;
		DWORD Type;
		const BYTE* pData = QueryValueBin(cb, pszValueName, &Type);
		NkError::CBaseException::check(Type == REG_SZ, E_INVALIDARG, __FILE__, __LINE__);

		if (cb == 0)
		{
			return 0;
		}

		if (cb > m_cbChar)
		{
			delete[] m_pChar;
			m_pChar = new char[cb];
			m_cbChar = cb;
			m_pChar[0] = 0;
		}

		int iRet = ::WideCharToMultiByte(CP_ACP, 0, reinterpret_cast<const WCHAR*>(pData), -1, m_pChar, cb, 0, 0);
		NkError::CWinApiException::check(iRet, "WideCharToMultiBYTE", __FILE__, __LINE__);

		return m_pChar;
	}

	LPCWSTR CRegistry::QueryValueString(LPCWSTR pszValueName /*= 0*/)
	{
		size_t cb = 0;
		DWORD Type;
		const BYTE* pData = QueryValueBin(cb, pszValueName, &Type);
		NkError::CBaseException::check(Type == REG_SZ, E_INVALIDARG, __FILE__, __LINE__);
		return reinterpret_cast<const WCHAR*>(pData);
	}

	DWORD CRegistry::QueryValueDWORD(LPCWSTR pszValueName /*= 0*/)
	{
		DWORD dwType = 0;
		DWORD dwValue = 0;
		DWORD cbValueData = sizeof(DWORD);
		LONG lRet = ::RegQueryValueEx(m_hKey, pszValueName, 0, &dwType
			, reinterpret_cast<LPBYTE>(&dwValue), &cbValueData);
		NkError::CWinApiException::check_error(lRet, "RegQueryValueEx", __FILE__, __LINE__);
		NkError::CBaseException::check(dwType == REG_DWORD, E_INVALIDARG, __FILE__, __LINE__);
		return dwValue;
	}

	DWORD CRegistry::QueryValueDWORD(DWORD dwDefault, LPCWSTR pszValueName /*= 0*/)
	{
		DWORD dwType = 0;
		DWORD dwValue = 0;
		DWORD cbValueData = sizeof(DWORD);
		LONG lRet = ::RegQueryValueEx(m_hKey, pszValueName, 0, &dwType
			, reinterpret_cast<LPBYTE>(&dwValue), &cbValueData);
		if (lRet == ERROR_SUCCESS && dwType == REG_DWORD) {
			return dwValue;
		}
		return dwDefault;
	}

	CLSID CRegistry::QueryValueCLSID(LPCWSTR pszValueName /*= 0*/)
	{
		size_t cb = 0;
		DWORD Type;
		BYTE* pData = const_cast<BYTE*>(QueryValueBin(cb, pszValueName, &Type));
		NkError::CBaseException::check(Type == REG_SZ, E_INVALIDARG, __FILE__, __LINE__);
		CLSID clsid = CLSID_NULL;
		NkError::CBaseException::check_result(CLSIDFromString(reinterpret_cast<LPOLESTR>(pData), &clsid), __FILE__, __LINE__);
		return clsid;
	}

	const BYTE* CRegistry::QueryValueBin(size_t& cb, LPCWSTR pszValueName /*= 0*/, DWORD* lpType /*= 0*/)
	{
		DWORD cbMaxValueData = 0;
		LONG lRet = ::RegQueryInfoKey(m_hKey, 0, 0, 0, 0, 0, 0, 0, 0, 0, &cbMaxValueData, 0);
		NkError::CWinApiException::check_error(lRet, "RegQueryInfoKey", __FILE__, __LINE__);

		if (cbMaxValueData > m_cbBuffer)
		{
			delete[] m_pBuffer;
			m_pBuffer = new BYTE[cbMaxValueData];
			m_cbBuffer = cbMaxValueData;
		}

		DWORD cbValueData = cbMaxValueData;
		ZeroMemory(m_pBuffer, cbMaxValueData);
		lRet = ::RegQueryValueEx(m_hKey, pszValueName, 0, lpType, m_pBuffer, &cbValueData);
		NkError::CWinApiException::check_error(lRet, "RegQueryValueEx", __FILE__, __LINE__);
		cb = static_cast<size_t>(cbValueData);
		return m_pBuffer;
	}

	void CRegistry::SetValue(DWORD NewValue, LPCWSTR pszValueName /*= 0*/)
	{
		SetValueBin(reinterpret_cast<const BYTE*>(&NewValue), sizeof(NewValue)
			, pszValueName, REG_DWORD);
	}

	void CRegistry::SetValue(CLSID NewValue, LPCWSTR pszValueName /*= 0*/)
	{
		LPWSTR pszCLSID = 0;
		NkError::CBaseException::check_result(StringFromCLSID(NewValue, &pszCLSID), __FILE__, __LINE__);
		try {
			SetValue(pszCLSID, pszValueName);
		}
		catch (...)
		{
			CoTaskMemFree(pszCLSID);
			throw;
		}
		CoTaskMemFree(pszCLSID);
	}

	void CRegistry::SetValue(LPCWSTR pszNewValue, LPCWSTR pszValueName /*= 0*/)
	{
		size_t cb = 0;
		HRESULT hr = StringCbLengthW(pszNewValue, STRSAFE_MAX_CCH, &cb);
		NkError::CWinApiException::check(hr, "StringCchLengthW", __FILE__, __LINE__);
		SetValueBin(reinterpret_cast<const BYTE*>(pszNewValue), cb, pszValueName, REG_SZ);
	}

	void CRegistry::SetValueBin(const BYTE* pData, size_t cbData, LPCWSTR pszValueName /*= 0 */, DWORD dwType /*= REG_BINARY*/)
	{
		LONG lRet = ::RegSetValueEx(m_hKey, pszValueName, 0, dwType, pData, cbData);
		NkError::CWinApiException::check_error(lRet, "RegSetValueEx", __FILE__, __LINE__);
	}
}