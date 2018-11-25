#pragma once

#include "../types/string.h"
#include <winreg.h>

namespace NkWin
{
	class CRegistry
	{
	public:
		CRegistry();
		CRegistry(LPCWSTR pszSubKey, HKEY hKey = HKEY_CLASSES_ROOT, REGSAM samDesired = KEY_READ
			, DWORD ulOptions = 0);
		CRegistry(REFCLSID clsid, LPCWSTR pszSubKey = L"", REGSAM samDesired = KEY_READ);
		CRegistry(CRegistry& d);
		CRegistry &operator =(const CRegistry& d);
		~CRegistry();
		bool IsOpen();

		operator HKEY();
		HKEY* operator&();

		bool OpenIf(LPCWSTR pszSubKey, HKEY hKey = HKEY_CLASSES_ROOT, REGSAM samDesired = KEY_READ
			, DWORD ulOptions = 0);

		void Open(LPCWSTR pszSubKey, HKEY hKey = HKEY_CLASSES_ROOT, REGSAM samDesired = KEY_READ
			, DWORD ulOptions = 0);

		void Close();

		void QueryInfoKey(LPTSTR pchClass = 0	//buffer for class name
			, LPDWORD pcchClassName = 0			//size of class string
			, LPDWORD pcSubKeys = 0				//number of subkeys
			, LPDWORD pcbMaxSubKey = 0			//longest subkey size
			, LPDWORD pcchMaxClass = 0			//longest class string
			, LPDWORD pcValues = 0				//number of values for key
			, LPDWORD pcchMaxValue = 0			//longest value name
			, LPDWORD pcbMaxValueData = 0		//longest value data
			, LPDWORD pcbSecurityDescriptor = 0	//size of security descriptor
			, LPFILETIME pftLastWriteTime = 0	//last write time
			);

		void CreateKey(LPCTSTR lpSubKey							//name of a sub key that this function opens or creates
			, PHKEY phkResult = 0								//pointer to a variable that receives a handle to the opened or created key
			, REGSAM samDesired = KEY_READ						//mask that specifies the access rights for the key to be created
			, LPTSTR lpClass = 0								//user-defined class type of this key
			, DWORD dwOptions = REG_OPTION_NON_VOLATILE			//options
			, LPSECURITY_ATTRIBUTES lpSecurityAttributes = 0	//pointer to a SECURITY_ATTRIBUTES structure
			, LPDWORD lpdwDisposition = 0						//pointer to a variable that receives the disposition values
			);

		void DeleteKey(LPCTSTR lpSubKey				//name of the key to be deleted
			, REGSAM  samDesired = KEY_WOW64_32KEY	//access mask the specifies the platform-specific view of the registry
			);

		void DeleteTree(LPCTSTR lpSubKey //name of the key to be deleted
			);

		bool RegEnumKey(DWORD dwIndex			//index of the subkey to retrieve
			, LPTSTR lpName						//pointer to a buffer that receives the name of the subkey
			, LPDWORD lpcName					//size of the buffer specified by the lpName
			, LPTSTR lpClass = 0				//pointer to a buffer that receives the user-defined class
			, LPDWORD lpcClass = 0				// size of the buffer specified by the lpClass
			, PFILETIME lpftLastWriteTime = 0	//the time at which the subkey was last written
			);

		void DeleteKeyValue(LPCWSTR pszValueName //the name of the value to delete
			);

		void QueryValue(NkType::CString& strValue, LPCWSTR pszValueName = 0);
		LPCSTR QueryValueAnsiString(LPCWSTR pszValueName = 0);
		LPCWSTR QueryValueString(LPCWSTR pszValueName = 0);
		DWORD QueryValueDWORD(LPCWSTR pszValueName = 0);
		DWORD QueryValueDWORD(DWORD dwDefault, LPCWSTR pszValueName = 0);
		CLSID QueryValueCLSID(LPCWSTR pszValueName = 0);
		const BYTE* QueryValueBin(size_t& cb, LPCWSTR pszValueName = 0, DWORD* lpType = 0);

		void SetValue(DWORD NewValue, LPCWSTR pszValueName = 0);
		void SetValue(LPCWSTR pszNewValue, LPCWSTR pszValueName = 0);
		void SetValue(CLSID NewValue, LPCWSTR pszValueName = 0);
		void SetValueBin(const BYTE* pData, size_t cbData, LPCWSTR pszValueName = 0
			, DWORD dwType = REG_BINARY);

	private:
		char* m_pChar;
		size_t m_cbChar;
		BYTE* m_pBuffer;
		size_t m_cbBuffer;
		HKEY m_hKey;
	};
}