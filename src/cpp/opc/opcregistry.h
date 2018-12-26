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
#include "windows/registry.h"
#include "types/String.h"
#include "com/comstring.h"
#include "error/baseexception.h"
#include <vector>
#include <guiddef.h>

namespace NkSSL {
	class COpenSSLCtx;
}

namespace NkOPC
{
	// {C9C2E5C2-B518-4F9E-A766-C30D01D4D46D}
	static const wchar_t TunnelRegEntryIdentString[] =
		L"{C9C2E5C2-B518-4F9E-A766-C30D01D4D46D}";
	static const GUID TunnelRegEntryIdent =
	{ 0xc9c2e5c2, 0xb518, 0x4f9e, { 0xa7, 0x66, 0xc3, 0xd, 0x1, 0xd4, 0xd4, 0x6d } };

	class CTunnelRegEntry
	{
	public:
		CTunnelRegEntry(HKEY hBaseKey = HKEY_LOCAL_MACHINE);
		CTunnelRegEntry(LPCWSTR pszCLSID, HKEY hBaseKey = HKEY_LOCAL_MACHINE);
		CTunnelRegEntry(REFCLSID clsid, HKEY hBaseKey = HKEY_LOCAL_MACHINE);
		CTunnelRegEntry(const CTunnelRegEntry& d);
		CTunnelRegEntry &operator =(const CTunnelRegEntry& d);
		void Update();
		void AddNew();
		void Remove();
		LPCWSTR ClientInstallPath();
		// check client installation
		bool ClientInstalled();
		LPCWSTR ProgID();
		void ProgID(LPCWSTR pszProgID);
		LPCWSTR Name();
		LPCWSTR FullName();
		void Name(LPCWSTR psz);
		LPCWSTR ThreadingModel();
		void ThreadingModel(LPCWSTR psz);
		LPCWSTR InprocServer32();
		void InprocServer32(LPCWSTR psz);
		DWORD RemoteServerIPAddress();
		void RemoteServerIPAddress(DWORD psz);
		DWORD RemoteServerPort();
		void RemoteServerPort(DWORD Value);
		BOOL RemoteServerUseSSL();
		void RemoteServerUseSSL(BOOL val);
		DWORD ClsContext();
		void ClsContext(DWORD Value);
		CLSID RemoteServerCLSID();
		void RemoteServerCLSID(CLSID Value);
		LPCWSTR RemoteServerProgID();
		void RemoteServerProgID(LPCWSTR psz);
		LPCWSTR RemoteServerUserType();
		void RemoteServerUserType(LPCWSTR psz);
		BOOL RemoteServerUseSSl();
		void RemoteServerUseSSl(BOOL val);
		BOOL RemoteServerDA1();
		void RemoteServerDA1(BOOL Implemented);
		BOOL RemoteServerDA2();
		void RemoteServerDA2(BOOL Implemented);
		BOOL RemoteServerDA3();
		void RemoteServerDA3(BOOL Implemented);
		void EncryptRemoteServerPass(const BYTE plain[], size_t cb_plain);
		void DecryptRemoteServerPass(BYTE plain[], size_t cb_plain, size_t& cb_plain_used);
		size_t RemoteServerPassPlainSize();
		void RemoteClientVerifyServer(BOOL value);
		BOOL RemoteClientVerifyServer();
		void RemoteClientUseCertificate(BOOL value);
		BOOL RemoteClientUseCertificate();
		void Setup(NkSSL::COpenSSLCtx& ctx);

		static void EnumEntries(std::vector<CTunnelRegEntry>& entries, HKEY hBaseKey = HKEY_LOCAL_MACHINE);
		static void EnumEntries(std::vector<GUID>& entries, HKEY hBaseKey = HKEY_LOCAL_MACHINE);

		//server
		static DWORD ServerPort();
		static void ServerPort(DWORD value);
		static DWORD ServerIP();
		static void ServerIP(DWORD value);
		static void GetPassHash(BYTE digest[], size_t cb);
		static void SetPassHash(const BYTE digest[], size_t cb);
		static int ServerTraceLevel();
		static void ServerTraceLevel(int val);
		static void ServerUseSSL(BOOL value);
		static BOOL ServerUseSSL();
		static void ServerVerifyClient(BOOL value);
		static BOOL ServerVerifyClient();

		static void SetServerCertPath(const char* value);
		static void GetServerCertPath(std::string& value);


		//client
		static int ClientTraceLevel();
		static void ClientTraceLevel(int val);



	private:
		NkType::CString m_strCLSID;
		NkType::CString m_strBuffer;
		NkWin::CRegistry m_TmpKey;
		NkWin::CRegistry m_KeyClasses;
		NkWin::CRegistry m_KeyClassesCLSIDClass;
		HKEY m_hBaseKey;
	};
}
