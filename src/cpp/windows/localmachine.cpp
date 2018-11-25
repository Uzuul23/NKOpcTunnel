#include "stdafx.h"
#include "localmachine.h"
#include <winioctl.h>
#include <IPHlpApi.h>
#include "error/winapiexception.h"
#include "openssl/sha.h"
#include "error/sslexception.h"
#include "windows/registry.h"

namespace NkWin
{
	class CLocalMachine::CImpl
	{
	public:
		CImpl::CImpl()
		{

		}
		//NkType::CString m_strBuffer;
	};


	CLocalMachine::CLocalMachine() //: _Impl(new CImpl)
	{

	}

	CLocalMachine::~CLocalMachine()
	{
		//delete _Impl;
	}

	void CLocalMachine::get_physical_drive_serial_number(UINT drive_number
		, NkType::CString& serial
		, bool& removable_media)
	{
		HANDLE hDevice = 0;
		BYTE* pOutBuffer = 0;

		try {
			NkType::CString strDrivePath;
			strDrivePath.printf(L"\\\\.\\PhysicalDrive%u", drive_number);

			hDevice = ::CreateFile(strDrivePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL, OPEN_EXISTING, 0, NULL);
			NkError::CWinApiException::check(hDevice, "CreateFile", __FILE__, __LINE__);

			STORAGE_PROPERTY_QUERY storagePropertyQuery;
			ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
			storagePropertyQuery.PropertyId = StorageDeviceProperty;
			storagePropertyQuery.QueryType = PropertyStandardQuery;

			// Get the necessary output buffer size
			STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader = { 0 };
			DWORD dwBytesReturned = 0;

			BOOL ret = ::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
				&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
				&storageDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER),
				&dwBytesReturned, NULL);
			NkError::CWinApiException::check(ret, "DeviceIoControl", __FILE__, __LINE__);

			// Alloc the output buffer
			const DWORD dwOutBufferSize = storageDescriptorHeader.Size;
			pOutBuffer = new BYTE[dwOutBufferSize];
			ZeroMemory(pOutBuffer, dwOutBufferSize);

			// Get the storage device descriptor
			ret = ::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
				&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
				pOutBuffer, dwOutBufferSize, &dwBytesReturned, NULL);
			NkError::CWinApiException::check(ret, "DeviceIoControl", __FILE__, __LINE__);

			STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pOutBuffer;
			removable_media = pDeviceDescriptor->RemovableMedia ? true : false;

			const DWORD dwSerialNumberOffset = pDeviceDescriptor->SerialNumberOffset;
			if (dwSerialNumberOffset == 0) {
				serial = L"";
			}
			else {
				serial = reinterpret_cast<LPCSTR>(pOutBuffer + dwSerialNumberOffset);
			}
			
			::CloseHandle(hDevice);
			delete [] pOutBuffer;
		}
		catch (...) {
			::CloseHandle(hDevice);
			delete[] pOutBuffer;
			throw;
		}
	}
	void CLocalMachine::local_machine_dependent_key(BYTE key[32])
	{
		//TODO: is experimental
		SHA256_CTX sha256;
		int ret = SHA256_Init(&sha256);
		NkError::CSSLException::check_result(ret, "SHA256_Init", __FILE__, __LINE__);
		
		CLocalMachine machine;

		//get serial number for first not removable media
		for (UINT loop = 0; loop<10; ++loop) {
			NkType::CString serial;
			bool removable_media = true;
			machine.get_physical_drive_serial_number(loop, serial, removable_media);
			if (!removable_media) {
				ret = SHA256_Update(&sha256, serial.data(), serial.byte_lenght());
				NkError::CSSLException::check_result(ret, "SHA256_Update", __FILE__, __LINE__);
				break;
			}
		}

		//get mac address from first adapter
		IP_ADAPTER_INFO AdapterInfo;
		DWORD dwBufLen = sizeof(AdapterInfo);
		if (::GetAdaptersInfo(&AdapterInfo, &dwBufLen) == ERROR_SUCCESS) {
			ret = SHA256_Update(&sha256, AdapterInfo.Address, sizeof(AdapterInfo.Address));
			NkError::CSSLException::check_result(ret, "SHA256_Update", __FILE__, __LINE__);
		}

		//encrypted window product id
		NkWin::CRegistry reg_crypt(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"
			, HKEY_LOCAL_MACHINE, KEY_READ | KEY_WOW64_64KEY);
		size_t cb = 0;
		const BYTE *product_id = reg_crypt.QueryValueBin(cb, L"DigitalProductID");

		ret = SHA256_Update(&sha256, product_id, cb);
		NkError::CSSLException::check_result(ret, "SHA256_Update", __FILE__, __LINE__);

		ret = SHA256_Final(key, &sha256);
		NkError::CSSLException::check_result(ret, "SHA256_Final", __FILE__, __LINE__);
	}
}