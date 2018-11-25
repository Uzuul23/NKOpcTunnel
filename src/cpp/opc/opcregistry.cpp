#include "stdafx.h"
#include "opcregistry.h"
#include "NkOpcTunnel/defines.h"
#include "ssl/aes.h"
#include "windows/localmachine.h"

namespace NkOPC
{
	CTunnelRegEntry::CTunnelRegEntry(HKEY hBaseKey /*= HKEY_LOCAL_MACHINE*/)
		: m_hBaseKey(hBaseKey)
	{	
		m_KeyClasses.Open(L"SOFTWARE\\CLASSES", hBaseKey, KEY_READ);
	}

	CTunnelRegEntry::CTunnelRegEntry(LPCWSTR pszCLSID, HKEY hBaseKey /*= HKEY_LOCAL_MACHINE*/)
		: m_hBaseKey(hBaseKey)
		, m_strCLSID(pszCLSID)
	{
		m_KeyClasses.Open(L"SOFTWARE\\CLASSES", hBaseKey, KEY_READ);
		NkWin::CRegistry clsidkey(L"CLSID", m_KeyClasses);
		m_KeyClassesCLSIDClass.Open(m_strCLSID, clsidkey);
	}

	CTunnelRegEntry::CTunnelRegEntry(REFCLSID clsid, HKEY hBaseKey /*= HKEY_LOCAL_MACHINE*/)
		: m_hBaseKey(hBaseKey)
	{
		NkCom::CComString str;
		NkError::CBaseException::check_result(::StringFromCLSID(clsid, &str), __FILE__, __LINE__);
		m_strCLSID = str;

		m_KeyClasses.Open(L"SOFTWARE\\CLASSES", hBaseKey, KEY_READ);
		NkWin::CRegistry clsidkey(L"CLSID", m_KeyClasses);
		m_KeyClassesCLSIDClass.Open(m_strCLSID, clsidkey);
	}

	CTunnelRegEntry::CTunnelRegEntry(const CTunnelRegEntry& d)
	{
		*this = d;
	}

	CTunnelRegEntry & CTunnelRegEntry::operator=(const CTunnelRegEntry& d)
	{
		m_strCLSID = d.m_strCLSID;
		m_hBaseKey = d.m_hBaseKey;
		m_KeyClasses.Open(L"SOFTWARE\\CLASSES", m_hBaseKey, KEY_READ);
		NkWin::CRegistry clsidkey(L"CLSID", m_KeyClasses);
		m_KeyClassesCLSIDClass.Open(m_strCLSID, clsidkey);
		return *this;
	}

	void CTunnelRegEntry::Update()
	{
		InprocServer32(ClientInstallPath());
		ClsContext(CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER);
		ThreadingModel(NKOPCTnl::RegValueThreadingModel);
	}

	void CTunnelRegEntry::AddNew()
	{
		NkWin::CRegistry KeyClassesCLSID(L"CLSID", m_KeyClasses, KEY_CREATE_SUB_KEY);
		
		GUID guid = GUID_NULL;
		NkError::CBaseException::check_result(::CoCreateGuid(&guid), __FILE__, __LINE__);

		NkCom::CComString str;
		NkError::CBaseException::check_result(::StringFromCLSID(guid, &str), __FILE__, __LINE__);
		m_strCLSID = str;

		m_KeyClassesCLSIDClass.Close();
		KeyClassesCLSID.CreateKey(m_strCLSID, &m_KeyClassesCLSIDClass, KEY_ALL_ACCESS);
		m_KeyClassesCLSIDClass.CreateKey(TunnelRegEntryIdentString);

		Update();
	}

	void CTunnelRegEntry::Remove()
	{
		try {
			NkWin::CRegistry KeyStringProgID(L"ProgID", m_KeyClassesCLSIDClass);
			LPCWSTR ProgID = KeyStringProgID.QueryValueString();
			NkWin::CRegistry KeyClasses(L"SOFTWARE\\CLASSES", m_hBaseKey, KEY_ALL_ACCESS);
			KeyClasses.DeleteTree(ProgID);
		}
		catch (NkError::CException& e) {
			e.report();
		}

		NkWin::CRegistry KeyClassesCLSID(L"CLSID", m_KeyClasses, KEY_ALL_ACCESS);
		KeyClassesCLSID.DeleteTree(m_strCLSID);
	}

	LPCWSTR CTunnelRegEntry::ClientInstallPath()
	{
		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_READ);
		m_TmpKey.Open(NKOPCTnl::RegKeySettings, key, KEY_READ);
		return m_TmpKey.QueryValueString(NKOPCTnl::RegValueClientInstallPath);
	}

	bool CTunnelRegEntry::ClientInstalled()
	{
		try {
			ClientInstallPath();
		}
		catch (...) {
			return false;
		}
		return true;
	}

	void CTunnelRegEntry::ProgID(LPCWSTR pszProgID)
	{
		m_TmpKey.Close();
		m_KeyClassesCLSIDClass.CreateKey(L"ProgID", &m_TmpKey, KEY_WRITE);
		m_TmpKey.SetValue(pszProgID);
	}

	LPCWSTR CTunnelRegEntry::ProgID()
	{
		m_TmpKey.Open(L"ProgID", m_KeyClassesCLSIDClass, KEY_READ);
		return m_TmpKey.QueryValueString();
	}

	void CTunnelRegEntry::Name(LPCWSTR psz)
	{
		NkWin::CRegistry KeyClasses(L"SOFTWARE\\CLASSES", m_hBaseKey, KEY_ALL_ACCESS);

		NkWin::CRegistry clsidkey(L"CLSID", m_KeyClasses);
		m_TmpKey.Open(m_strCLSID, clsidkey, KEY_WRITE);

		m_strBuffer.printf(NKOPCTnl::TunnelRealNameFormat, psz);
		m_TmpKey.SetValue(m_strBuffer);

		NkWin::CRegistry KeyStringProgID;
		if (KeyStringProgID.OpenIf(L"ProgID", m_KeyClassesCLSIDClass)) {
			LPCWSTR ProgID = KeyStringProgID.QueryValueString();
			NkWin::CRegistry KeyClass;
			if (KeyClass.OpenIf(ProgID, m_hBaseKey, KEY_READ)) {
				KeyClass.Close();
				KeyClasses.DeleteTree(ProgID);
			}
		}

		NkType::CString strProgId;
		strProgId.printf(NKOPCTnl::TunnelProgIDFormat, psz);
		ProgID(strProgId);

		NkWin::CRegistry KeyClass;
		KeyClasses.CreateKey(strProgId, &KeyClass, KEY_WRITE);
		NkWin::CRegistry KeyEntry;
		KeyClass.CreateKey(L"CLSID", &KeyEntry, KEY_WRITE);
		KeyEntry.SetValue(m_strCLSID);
	}

	LPCWSTR CTunnelRegEntry::Name()
	{
		NkType::CString strValue;
		m_KeyClassesCLSIDClass.QueryValue(strValue);
		long pos = strValue.find(NKOPCTnl::TunnelRealNameFind);
		if (pos == 0) {
			m_strBuffer = strValue.right(sizeof(NKOPCTnl::TunnelRealNameFind) 
				/ sizeof(NKOPCTnl::TunnelRealNameFind[0]) - 1);
			return m_strBuffer;
		}
		return L"--error--";
	}

	LPCWSTR CTunnelRegEntry::FullName()
	{
		m_KeyClassesCLSIDClass.QueryValue(m_strBuffer);
		return m_strBuffer;
	}

	void CTunnelRegEntry::ThreadingModel(LPCWSTR psz)
	{
		m_TmpKey.Close();
		m_KeyClassesCLSIDClass.CreateKey(L"InprocServer32", &m_TmpKey, KEY_WRITE);
		m_TmpKey.SetValue(psz, L"ThreadingModel");
	}

	LPCWSTR CTunnelRegEntry::ThreadingModel()
	{
		m_TmpKey.Open(L"InprocServer32", m_KeyClassesCLSIDClass, KEY_READ);
		return m_TmpKey.QueryValueString(L"ThreadingModel");
	}

	void CTunnelRegEntry::InprocServer32(LPCWSTR psz)
	{
		m_TmpKey.Close();
		m_KeyClassesCLSIDClass.CreateKey(L"InprocServer32", &m_TmpKey, KEY_WRITE);
		m_TmpKey.SetValue(psz);
	}

	LPCWSTR CTunnelRegEntry::InprocServer32()
	{
		m_TmpKey.Open(L"InprocServer32", m_KeyClassesCLSIDClass);
		return m_TmpKey.QueryValueString();
	}

	void CTunnelRegEntry::RemoteServerIPAddress(DWORD psz)
	{
		m_TmpKey.Close();
		m_KeyClassesCLSIDClass.CreateKey(L"RemoteServer", &m_TmpKey, KEY_WRITE);
		m_TmpKey.SetValue(psz, L"Address");
	}

	DWORD CTunnelRegEntry::RemoteServerIPAddress()
	{
		m_TmpKey.Open(L"RemoteServer", m_KeyClassesCLSIDClass, KEY_READ);
		return m_TmpKey.QueryValueDWORD(L"Address");
	}

	void CTunnelRegEntry::RemoteServerPort(DWORD Value)
	{
		m_TmpKey.Close();
		m_KeyClassesCLSIDClass.CreateKey(L"RemoteServer", &m_TmpKey, KEY_WRITE);
		m_TmpKey.SetValue(Value, L"Port");
	}

	DWORD CTunnelRegEntry::RemoteServerPort()
	{
		m_TmpKey.Open(L"RemoteServer", m_KeyClassesCLSIDClass, KEY_READ);
		return m_TmpKey.QueryValueDWORD(L"Port");
	}

	void CTunnelRegEntry::ClsContext(DWORD Value)
	{
		m_TmpKey.Close();
		m_KeyClassesCLSIDClass.CreateKey(L"RemoteServer", &m_TmpKey, KEY_WRITE);
		m_TmpKey.SetValue(Value, L"ClsContext");
	}

	DWORD CTunnelRegEntry::ClsContext()
	{
		m_TmpKey.Open(L"RemoteServer", m_KeyClassesCLSIDClass, KEY_READ);
		return m_TmpKey.QueryValueDWORD(L"ClsContext");
	}

	void CTunnelRegEntry::RemoteServerCLSID(CLSID Value)
	{
		m_TmpKey.Close();
		m_KeyClassesCLSIDClass.CreateKey(L"RemoteServer", &m_TmpKey, KEY_WRITE);
		m_TmpKey.SetValue(Value, L"CLSID");
	}

	CLSID CTunnelRegEntry::RemoteServerCLSID()
	{
		m_TmpKey.Open(L"RemoteServer", m_KeyClassesCLSIDClass, KEY_READ);
		return m_TmpKey.QueryValueCLSID(L"CLSID");
	}

	void CTunnelRegEntry::RemoteServerProgID(LPCWSTR psz)
	{
		m_TmpKey.Close();
		m_KeyClassesCLSIDClass.CreateKey(L"RemoteServer", &m_TmpKey, KEY_WRITE);
		m_TmpKey.SetValue(psz, L"ProgID");
	}

	LPCWSTR CTunnelRegEntry::RemoteServerProgID()
	{
		m_TmpKey.Open(L"RemoteServer", m_KeyClassesCLSIDClass, KEY_READ);
		return m_TmpKey.QueryValueString(L"ProgID");
	}

	void CTunnelRegEntry::RemoteServerUserType(LPCWSTR psz)
	{
		m_TmpKey.Close();
		m_KeyClassesCLSIDClass.CreateKey(L"RemoteServer", &m_TmpKey, KEY_WRITE);
		m_TmpKey.SetValue(psz, L"UserType");
	}

	LPCWSTR CTunnelRegEntry::RemoteServerUserType()
	{
		m_TmpKey.Open(L"RemoteServer", m_KeyClassesCLSIDClass, KEY_READ);
		return m_TmpKey.QueryValueString(L"UserType");
	}

	void CTunnelRegEntry::RemoteServerUseSSl(BOOL val)
	{
		m_TmpKey.Close();
		m_KeyClassesCLSIDClass.CreateKey(L"RemoteServer", &m_TmpKey, KEY_WRITE);
		m_TmpKey.SetValue(val, L"UseSSL");
	}

	BOOL CTunnelRegEntry::RemoteServerUseSSl()
	{
		m_TmpKey.Open(L"RemoteServer", m_KeyClassesCLSIDClass, KEY_READ);
		return m_TmpKey.QueryValueDWORD(L"UseSSL");
	}

	void CTunnelRegEntry::RemoteServerDA1(BOOL Implemented)
	{
		NkCom::CComString strIID;
		NkError::CBaseException::check_result(StringFromIID(CATID_OPCDAServer10, &strIID), __FILE__, __LINE__);

		if (Implemented) {
			NkWin::CRegistry KeyImplCat;
			m_KeyClassesCLSIDClass.CreateKey(L"Implemented Categories", &KeyImplCat, KEY_WRITE);
			KeyImplCat.CreateKey(strIID);
		}
		else {
			NkWin::CRegistry KeyCat;
			NkWin::CRegistry KeyImplCat;
			if (KeyCat.OpenIf(L"Implemented Categories", m_KeyClassesCLSIDClass, KEY_WRITE)
				&& KeyImplCat.OpenIf(strIID, KeyCat, KEY_WRITE)) {
				KeyImplCat.Close();
				KeyCat.DeleteKey(strIID);
			}
		}
	}

	BOOL CTunnelRegEntry::RemoteServerDA1()
	{
		NkCom::CComString strIID;
		NkError::CBaseException::check_result(StringFromIID(CATID_OPCDAServer10, &strIID), __FILE__, __LINE__);

		NkWin::CRegistry KeyCat;
		NkWin::CRegistry KeyImplCat;
		if (KeyCat.OpenIf(L"Implemented Categories", m_KeyClassesCLSIDClass, KEY_READ)
			&& KeyImplCat.OpenIf(strIID, KeyCat, KEY_READ)) {
			return TRUE;
		}
		return FALSE;
	}

	void CTunnelRegEntry::RemoteServerDA2(BOOL Implemented)
	{
		NkCom::CComString strIID;
		NkError::CBaseException::check_result(StringFromIID(CATID_OPCDAServer20, &strIID), __FILE__, __LINE__);

		if (Implemented) {
			NkWin::CRegistry KeyImplCat;
			m_KeyClassesCLSIDClass.CreateKey(L"Implemented Categories", &KeyImplCat, KEY_WRITE);
			KeyImplCat.CreateKey(strIID);
		}
		else {
			NkWin::CRegistry KeyCat;
			NkWin::CRegistry KeyImplCat;
			if (KeyCat.OpenIf(L"Implemented Categories", m_KeyClassesCLSIDClass, KEY_WRITE)
				&& KeyImplCat.OpenIf(strIID, KeyCat, KEY_WRITE)) {
				KeyImplCat.Close();
				KeyCat.DeleteKey(strIID);
			}
		}
	}

	BOOL CTunnelRegEntry::RemoteServerDA2()
	{
		NkCom::CComString strIID;
		NkError::CBaseException::check_result(StringFromIID(CATID_OPCDAServer20, &strIID), __FILE__, __LINE__);

		NkWin::CRegistry KeyCat;
		NkWin::CRegistry KeyImplCat;
		if (KeyCat.OpenIf(L"Implemented Categories", m_KeyClassesCLSIDClass, KEY_READ)
			&& KeyImplCat.OpenIf(strIID, KeyCat, KEY_READ)) {
			return TRUE;
		}
		return FALSE;
	}

	void CTunnelRegEntry::RemoteServerDA3(BOOL Implemented)
	{
		NkCom::CComString strIID;
		NkError::CBaseException::check_result(StringFromIID(CATID_OPCDAServer30, &strIID), __FILE__, __LINE__);

		if (Implemented) {
			NkWin::CRegistry KeyImplCat;
			m_KeyClassesCLSIDClass.CreateKey(L"Implemented Categories", &KeyImplCat, KEY_WRITE);
			KeyImplCat.CreateKey(strIID);
		}
		else {
			NkWin::CRegistry KeyCat;
			NkWin::CRegistry KeyImplCat;
			if (KeyCat.OpenIf(L"Implemented Categories", m_KeyClassesCLSIDClass, KEY_WRITE)
				&& KeyImplCat.OpenIf(strIID, KeyCat, KEY_WRITE)) {
				KeyImplCat.Close();
				KeyCat.DeleteKey(strIID);
			}
		}
	}

	BOOL CTunnelRegEntry::RemoteServerDA3()
	{
		NkCom::CComString strIID;
		NkError::CBaseException::check_result(StringFromIID(CATID_OPCDAServer30, &strIID), __FILE__, __LINE__);

		NkWin::CRegistry KeyCat;
		NkWin::CRegistry KeyImplCat;
		if (KeyCat.OpenIf(L"Implemented Categories", m_KeyClassesCLSIDClass, KEY_READ)
			&& KeyImplCat.OpenIf(strIID, KeyCat, KEY_READ)) {
			return TRUE;
		}
		return FALSE;
	}

	void CTunnelRegEntry::EncryptRemoteServerPass(const BYTE plain[], size_t cb_plain)
	{
		BYTE key[32] = { 0 };
		NkWin::CLocalMachine::local_machine_dependent_key(key);

		size_t cb_cipher = cb_plain + AES_BLOCK_SIZE;
		size_t cb_cipher_used = 0;
		BYTE *pcipher = new BYTE[cb_cipher];

		NkSSL::CAES256::encrypt(plain, cb_plain, key, sizeof(key), pcipher, cb_cipher
			, cb_cipher_used);

		m_TmpKey.Close();
		m_KeyClassesCLSIDClass.CreateKey(L"RemoteServer", &m_TmpKey, KEY_WRITE);
		m_TmpKey.SetValueBin(pcipher, cb_cipher_used, NKOPCTnl::RegValueClientPass);
	}

	void CTunnelRegEntry::DecryptRemoteServerPass(BYTE plain[], size_t cb_plain
		, size_t& cb_plain_used)
	{
		m_TmpKey.Open(L"RemoteServer", m_KeyClassesCLSIDClass, KEY_READ);

		size_t cb_cipher = 0;
		const BYTE *p = m_TmpKey.QueryValueBin(cb_cipher, NKOPCTnl::RegValueClientPass);

		BYTE key[32] = { 0 };
		NkWin::CLocalMachine::local_machine_dependent_key(key);

		NkSSL::CAES256::decrypt(p, cb_cipher, key, sizeof(key), plain, cb_plain, cb_plain_used);
	}

	size_t CTunnelRegEntry::RemoteServerPassPlainSize()
	{
		try {
			m_TmpKey.Open(L"RemoteServer", m_KeyClassesCLSIDClass, KEY_READ);

			size_t cb_cipher = 0;
			m_TmpKey.QueryValueBin(cb_cipher, NKOPCTnl::RegValueClientPass);
			return cb_cipher + AES_BLOCK_SIZE;
		}
		catch (...) {
			return 0;
		}
	}

	void CTunnelRegEntry::EnumEntries(std::vector<CTunnelRegEntry>& entries
		, HKEY hBaseKey /*= HKEY_LOCAL_MACHINE*/)
	{
		entries.clear();

		NkWin::CRegistry clsidskey(L"SOFTWARE\\CLASSES\\CLSID", hBaseKey, KEY_READ);

		DWORD cSubKeys = 0;
		clsidskey.QueryInfoKey(0, 0, &cSubKeys);

		TCHAR chName[MAX_PATH] = L"";
		DWORD cchName = MAX_PATH;

		NkWin::CRegistry subkey;
		NkWin::CRegistry identkey;

		for (DWORD loop = 0; loop < cSubKeys; ++loop) {
			clsidskey.RegEnumKey(loop, chName, &cchName);
			subkey.Open(chName, clsidskey);

			if (identkey.OpenIf(TunnelRegEntryIdentString, subkey)) {
				entries.push_back(CTunnelRegEntry(chName, hBaseKey));
			}
			cchName = MAX_PATH;
		}
	}

	void CTunnelRegEntry::EnumEntries(std::vector<GUID>& entries
		, HKEY /*hBaseKey = HKEY_LOCAL_MACHINE*/)
	{
		entries.clear();

		NkWin::CRegistry clsidkey;
		clsidkey.Open(L"CLSID", HKEY_CLASSES_ROOT, KEY_READ);

		DWORD cSubKeys = 0;
		clsidkey.QueryInfoKey(0, 0, &cSubKeys);

		TCHAR chName[MAX_PATH] = L"";
		DWORD cchName = MAX_PATH;

		NkWin::CRegistry subkey;
		NkWin::CRegistry identkey;

		for (DWORD loop = 0; loop < cSubKeys; ++loop) {
			clsidkey.RegEnumKey(loop, chName, &cchName);
			subkey.Open(chName, clsidkey);

			if (identkey.OpenIf(TunnelRegEntryIdentString, subkey)) {
				GUID guid = GUID_NULL;
				NkError::CBaseException::check_result(::IIDFromString(chName, &guid), __FILE__, __LINE__);
				entries.push_back(guid);
			}
			cchName = MAX_PATH;
		}
	}

	DWORD CTunnelRegEntry::ServerPort()
	{
		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_READ);
		NkWin::CRegistry keySettings;
		keySettings.Open(NKOPCTnl::RegKeySettings, key);
		return keySettings.QueryValueDWORD(NKOPCTnl::RegValueServerPort);
	}

	void CTunnelRegEntry::ServerPort(DWORD value)
	{
		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_READ);
		NkWin::CRegistry keySettings;
		keySettings.Open(NKOPCTnl::RegKeySettings, key, KEY_WRITE);
		keySettings.SetValue(value, NKOPCTnl::RegValueServerPort);
	}

	DWORD CTunnelRegEntry::ServerIP()
	{
		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_READ);
		NkWin::CRegistry keySettings;
		keySettings.Open(NKOPCTnl::RegKeySettings, key);
		return keySettings.QueryValueDWORD(NKOPCTnl::RegValueServerIPAddress);
	}

	void CTunnelRegEntry::ServerIP(DWORD value)
	{
		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_READ);
		NkWin::CRegistry keySettings;
		keySettings.Open(NKOPCTnl::RegKeySettings, key, KEY_WRITE);
		keySettings.SetValue(value, NKOPCTnl::RegValueServerIPAddress);
	}

	void CTunnelRegEntry::GetPassHash(BYTE digest[], size_t cb)
	{
		NkWin::CRegistry key(NKOPCTnl::RegKeySettings, HKEY_LOCAL_MACHINE, KEY_READ);

		size_t cb_read = 0;
		const BYTE *p = key.QueryValueBin(cb_read, NKOPCTnl::RegValueServerPass);

		if (cb != cb_read) {
			throw NkError::CBaseException(NkError::CBaseException::invalid_argument
				, __FILE__, __LINE__);
		}
		memcpy_s(digest, cb, p, cb_read);
	}

	void CTunnelRegEntry::SetPassHash(const BYTE digest[], size_t cb)
	{
		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_READ);
		NkWin::CRegistry keySettings(NKOPCTnl::RegKeySettings, key, KEY_WRITE);
		keySettings.SetValueBin(&digest[0], cb, NKOPCTnl::RegValueServerPass);
	}

	int CTunnelRegEntry::ServerTraceLevel()
	{
		NkWin::CRegistry key(NKOPCTnl::RegKeySettings, HKEY_LOCAL_MACHINE, KEY_READ);
		return key.QueryValueDWORD(NKOPCTnl::DefaultTraceLevel, NKOPCTnl::RegValueServerTraceLevel);
	}

	void CTunnelRegEntry::ServerTraceLevel(int val)
	{
		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_READ);
		NkWin::CRegistry keySettings;
		keySettings.Open(NKOPCTnl::RegKeySettings, key, KEY_WRITE);
		keySettings.SetValue(static_cast<DWORD>(val), NKOPCTnl::RegValueServerTraceLevel);
	}

	int CTunnelRegEntry::ClientTraceLevel()
	{
		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_READ);
		NkWin::CRegistry keySettings;
		keySettings.Open(NKOPCTnl::RegKeySettings, key);
		try {
			return keySettings.QueryValueDWORD(NKOPCTnl::RegValueClientTraceLevel);
		}
		catch (...) {
			return NKOPCTnl::DefaultTraceLevel;
		}
	}

	void CTunnelRegEntry::ClientTraceLevel(int val)
	{
		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_READ);
		NkWin::CRegistry keySettings;
		keySettings.Open(NKOPCTnl::RegKeySettings, key, KEY_WRITE);
		keySettings.SetValue(static_cast<DWORD>(val), NKOPCTnl::RegValueClientTraceLevel);
	}
}