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
#include "opc/opcda.h"
#include "stream/stream.h"
#include "com/comstring.h"
#include "com/variant.h"
#include "com/enumfar.h"
#include "com/enumnear.h"
#include "opc/opccomn.h"

namespace NkOPC
{
	typedef NkCom::CComEnumFar<IOPCEnumGUID, GUID>  COPCEnumGUIDFar;

	inline NkStream::CStream& operator<<(NkStream::CStream& l, const LPWSTR& r) {
		NkCom::CComString::read(l, r);
		return l;
	}
	inline NkStream::CStream& operator<<(NkStream::CStream& l, LPCWSTR& r) {
		NkCom::CComString::read(l, r);
		return l;
	}
	inline NkStream::CStream& operator>>(NkStream::CStream& l, LPWSTR& r) {
		NkCom::CComString::write(l, &r);
		return l;
	}

	inline NkStream::CStream& operator<<(NkStream::CStream& l, const VARIANT& r) {
		NkCom::CVariant::read(l, r);
		return l;
	}
	inline NkStream::CStream& operator>>(NkStream::CStream& l, VARIANT& r) {
		NkCom::CVariant::write(l, r);
		return l;
	}

	template<class T1, class T2>
	class CStreamCoTaskMem
	{
	public:
		CStreamCoTaskMem() : _p(0), _auto_delete(true)
		{
		}
		~CStreamCoTaskMem()
		{
			__if_exists(T2::clear)
			{
				static_cast<T2*>(this)->clear();
			}
			__if_not_exists(T2::clear)
			{
				if (_p && _auto_delete)
				{
					CoTaskMemFree(_p);
				}
				_p = 0;
			}
		}
		T1** operator&()
		{
			_ASSERT(_p == 0);
			return &_p;
		}
		void Detach(T1** pp)
		{
			(*pp) = _p;
			_p = 0;
		}
		void read(NkStream::CStream& stream) const
		{
			__if_exists(T2::read)
			{
				static_cast<const T2*>(this)->read(stream);
			}
			__if_not_exists(T2::read)
			{
				stream.write_type(VT_USERDEFINED);
				stream.write(_p, sizeof(T1));
			}
		}
		void write(NkStream::CStream& stream)
		{
			__if_exists(T2::write)
			{
				static_cast<T2*>(this)->write(stream);
			}
			__if_not_exists(T2::write)
			{
				stream.check_type(VT_USERDEFINED);
				if (_p == 0)
				{
					_p = (T1*)CoTaskMemAlloc(sizeof(T1));
					NkError::CBaseException::check_alloc(_p, __FILE__, __LINE__);
				}
				stream.read(_p, sizeof(T1));
			}
		}

		T1* _p;
		bool _auto_delete;
	};

	class COPCServerStatus : public CStreamCoTaskMem < OPCSERVERSTATUS, COPCServerStatus >
	{
	public:

		void clear()
		{
			if (_p && _auto_delete) {
				CoTaskMemFree(_p->szVendorInfo);
				CoTaskMemFree(_p);
			}
			_p = 0;
		}
		void read(NkStream::CStream& stream) const
		{
			stream.write_type(VT_USERDEFINED);
			stream.write(_p, sizeof(OPCSERVERSTATUS));
			stream << NkCom::CComString(_p->szVendorInfo);
		}
		void write(NkStream::CStream& stream)
		{
			stream.check_type(VT_USERDEFINED);
			if (_p == 0) {
				_p = (OPCSERVERSTATUS*)CoTaskMemAlloc(sizeof(OPCSERVERSTATUS));
				NkError::CBaseException::check_alloc(_p, __FILE__, __LINE__);
			}
			stream.read(_p, sizeof(OPCSERVERSTATUS));
			stream >> _p->szVendorInfo;
		}
	};

	template<class T1, class T2>
	inline NkStream::CStream& operator<<(NkStream::CStream& l, const CStreamCoTaskMem<T1, T2>& r)
	{
		r.read(l);
		return l;
	}

	template<class T1, class T2>
	inline NkStream::CStream& operator>>(NkStream::CStream& l, CStreamCoTaskMem<T1, T2>& r)
	{
		r.write(l);
		return l;
	}

	/*OPCITEMVQT
	typedef struct tagOPCITEMVQT
	{
	VARIANT vDataValue;
	BOOL bQualitySpecified;
	WORD wQuality;
	WORD wReserved;
	BOOL bTimeStampSpecified;
	DWORD dwReserved;
	FILETIME ftTimeStamp;
	} 	OPCITEMVQT;*/

	class CVectorOPCITEMVQT : public NkCom::CVectorBase < CVectorOPCITEMVQT, OPCITEMVQT, DWORD >
	{
	public:
		CVectorOPCITEMVQT() : CVectorBase() {}
		CVectorOPCITEMVQT(DWORD c) : CVectorBase(c) {}
		CVectorOPCITEMVQT(const OPCITEMVQT p[], DWORD c) : CVectorBase(p, c) {}
		void initialize_element(OPCITEMVQT& elem)
		{
			ZeroMemory(&elem, sizeof(OPCITEMVQT));
			::VariantInit(&elem.vDataValue);
		}
		void set_value(DWORD index, const VARIANT& var)
		{
			NkError::CBaseException::check(index >= 0 && index < _c, E_INVALIDARG, __FILE__, __LINE__);
			NkError::CBaseException::check_result(::VariantCopy(&_p[index].vDataValue, &var), __FILE__, __LINE__);
		}
		void copy_element(OPCITEMVQT& dst, const OPCITEMVQT& src)
		{
			memcpy(&dst, &src, sizeof(OPCITEMVQT));
			NkError::CBaseException::check_result(::VariantCopy(&dst.vDataValue, &src.vDataValue), __FILE__, __LINE__);
		}
		void clear_element(OPCITEMVQT& elem) {
			NkError::CBaseException::check_result(::VariantClear(&elem.vDataValue), __FILE__, __LINE__);
		}
		void read_element(const OPCITEMVQT& elem, NkStream::CStream& stream) const
		{
			stream << elem.vDataValue;
			stream << elem.bQualitySpecified;
			stream << elem.bTimeStampSpecified;
			stream << elem.dwReserved;
			stream << elem.ftTimeStamp;
			stream << elem.wQuality;
			stream << elem.wReserved;
		}
		void write_element(OPCITEMVQT& elem, NkStream::CStream& stream)
		{
			stream >> elem.vDataValue;
			stream >> elem.bQualitySpecified;
			stream >> elem.bTimeStampSpecified;
			stream >> elem.dwReserved;
			stream >> elem.ftTimeStamp;
			stream >> elem.wQuality;
			stream >> elem.wReserved;
		}
	};

	/*OPCBROWSEELEMENT

	typedef struct tagOPCITEMPROPERTY
	{
	VARTYPE vtDataType;
	WORD wReserved;
	DWORD dwPropertyID;
	LPWSTR szItemID;
	LPWSTR szDescription;
	VARIANT vValue;
	HRESULT hrErrorID;
	DWORD dwReserved;
	}
	OPCITEMPROPERTY;

	typedef struct tagOPCITEMPROPERTIES
	{
	HRESULT hrErrorID;
	DWORD dwNumProperties;
	OPCITEMPROPERTY *pItemProperties;
	DWORD dwReserved;
	} 	OPCITEMPROPERTIES;

	typedef struct tagOPCBROWSEELEMENT
	{
	LPWSTR szName;
	LPWSTR szItemID;
	DWORD dwFlagValue;
	DWORD dwReserved;
	OPCITEMPROPERTIES ItemProperties;
	} 	OPCBROWSEELEMENT;*/

	class CVectorOPCITEMPROPERTIES : public NkCom::CVectorBase < CVectorOPCITEMPROPERTIES, OPCITEMPROPERTIES, DWORD >
	{
	public:
		CVectorOPCITEMPROPERTIES() : CVectorBase() {}
		CVectorOPCITEMPROPERTIES(DWORD c) : CVectorBase(c) {}
		CVectorOPCITEMPROPERTIES(const OPCITEMPROPERTIES p[], DWORD c) : CVectorBase(p, c) {}

		void initialize_element(OPCITEMPROPERTIES& elem) {
			ZeroMemory(&elem, sizeof(OPCITEMPROPERTIES));
		}
		void copy_element(OPCITEMPROPERTIES& dst, const OPCITEMPROPERTIES& src) {
			memcpy(&dst, &src, sizeof(OPCITEMPROPERTIES));
			for (DWORD loop = 0; loop < src.dwNumProperties; ++loop) {
				dst.pItemProperties[loop].vtDataType = src.pItemProperties[loop].vtDataType;
				dst.pItemProperties[loop].wReserved = src.pItemProperties[loop].wReserved;
				dst.pItemProperties[loop].dwPropertyID = src.pItemProperties[loop].dwPropertyID;
				dst.pItemProperties[loop].szItemID = NkCom::CComString::copy(src.pItemProperties[loop].szItemID);
				dst.pItemProperties[loop].szDescription = NkCom::CComString::copy(src.pItemProperties[loop].szDescription);
				NkError::CBaseException::check_result(::VariantCopy(&dst.pItemProperties[loop].vValue, &src.pItemProperties[loop].vValue), __FILE__, __LINE__);
				dst.pItemProperties[loop].hrErrorID = src.pItemProperties[loop].hrErrorID;
				dst.pItemProperties[loop].dwReserved = src.pItemProperties[loop].dwReserved;
			}
		}
		void clear_element(OPCITEMPROPERTIES& elem) {
			for (DWORD loop = 0; loop < elem.dwNumProperties; ++loop) {
				::CoTaskMemFree(elem.pItemProperties[loop].szItemID);
				::CoTaskMemFree(elem.pItemProperties[loop].szDescription);
				NkError::CBaseException::check_result(::VariantClear(&elem.pItemProperties[loop].vValue), __FILE__, __LINE__);
			}
			::CoTaskMemFree(elem.pItemProperties);
		}
		void read_element(const OPCITEMPROPERTIES& elem, NkStream::CStream& stream) const {
			stream << elem.hrErrorID;
			stream << elem.dwNumProperties;
			for (DWORD loop = 0; loop < elem.dwNumProperties; ++loop) {
				const OPCITEMPROPERTY& prop = elem.pItemProperties[loop];
				stream << prop.vtDataType;
				stream << prop.wReserved;
				stream << prop.dwPropertyID;
				stream << prop.szItemID;
				stream << prop.szDescription;
				stream << prop.vValue;
				stream << prop.hrErrorID;
				stream << prop.dwReserved;
			}
			stream << elem.dwReserved;
		}
		void write_element(OPCITEMPROPERTIES& elem, NkStream::CStream& stream) {
			stream >> elem.hrErrorID;
			stream >> elem.dwNumProperties;
			if (elem.dwNumProperties > 0) {
				elem.pItemProperties = (OPCITEMPROPERTY*)CoTaskMemAlloc(sizeof(OPCITEMPROPERTY) * elem.dwNumProperties);
				NkError::CBaseException::check_alloc(elem.pItemProperties, __FILE__, __LINE__);
				for (DWORD loop = 0; loop < elem.dwNumProperties; ++loop) {
					OPCITEMPROPERTY& prop = elem.pItemProperties[loop];
					stream >> prop.vtDataType;
					stream >> prop.wReserved;
					stream >> prop.dwPropertyID;
					stream >> prop.szItemID;
					stream >> prop.szDescription;
					stream >> prop.vValue;
					stream >> prop.hrErrorID;
					stream >> prop.dwReserved;
				}
			}
			else {
				elem.pItemProperties = 0;
			}
			stream >> elem.dwReserved;
		}
	};

	class CVectorOPCBROWSEELEMENT : public NkCom::CVectorBase < CVectorOPCBROWSEELEMENT, OPCBROWSEELEMENT, DWORD >
	{
	private:
		LPCWSTR* _item_ids;
	public:
		CVectorOPCBROWSEELEMENT() : CVectorBase(), _item_ids(0) {}
		CVectorOPCBROWSEELEMENT(DWORD c) : CVectorBase(c), _item_ids(0) {}
		CVectorOPCBROWSEELEMENT(const OPCBROWSEELEMENT p[], DWORD c) : CVectorBase(p, c), _item_ids(0) {}
		~CVectorOPCBROWSEELEMENT() {
			delete[] _item_ids;
		}
		void initialize_element(OPCBROWSEELEMENT& elem) {
			ZeroMemory(&elem, sizeof(OPCBROWSEELEMENT));
		}
		void copy_element(OPCBROWSEELEMENT& dst, const OPCBROWSEELEMENT& src) {
			memcpy(&dst, &src, sizeof(OPCBROWSEELEMENT));
			dst.szName = NkCom::CComString::copy(src.szName);
			dst.szItemID = NkCom::CComString::copy(src.szItemID);
		}
		void clear_element(OPCBROWSEELEMENT& elem) {
			::CoTaskMemFree(elem.szName);
			::CoTaskMemFree(elem.szItemID);
			for (DWORD loop = 0; loop < elem.ItemProperties.dwNumProperties; ++loop) {
				OPCITEMPROPERTY& prop = elem.ItemProperties.pItemProperties[loop];
				::CoTaskMemFree(prop.szItemID);
				::CoTaskMemFree(prop.szDescription);
				NkError::CBaseException::check_result(::VariantClear(&prop.vValue), __FILE__, __LINE__);
			}
			::CoTaskMemFree(elem.ItemProperties.pItemProperties);
		}
		void read_element(const OPCBROWSEELEMENT& elem, NkStream::CStream& stream) const {
			stream << elem.szName;
			stream << elem.szItemID;
			stream << elem.dwFlagValue;
			stream << elem.dwReserved;
			stream << elem.ItemProperties.hrErrorID;
			stream << elem.ItemProperties.dwReserved;
			stream << elem.ItemProperties.dwNumProperties;
			for (DWORD loop = 0; loop < elem.ItemProperties.dwNumProperties; ++loop) {
				const OPCITEMPROPERTY& prop = elem.ItemProperties.pItemProperties[loop];
				stream << prop.vtDataType;
				stream << prop.wReserved;
				stream << prop.dwPropertyID;
				stream << prop.szItemID;
				stream << prop.szDescription;
				stream << prop.vValue;
				stream << prop.hrErrorID;
				stream << prop.dwReserved;
			}
		}
		void write_element(OPCBROWSEELEMENT& elem, NkStream::CStream& stream) {
			stream >> elem.szName;
			stream >> elem.szItemID;
			stream >> elem.dwFlagValue;
			stream >> elem.dwReserved;
			stream >> elem.ItemProperties.hrErrorID;
			stream >> elem.ItemProperties.dwReserved;
			stream >> elem.ItemProperties.dwNumProperties;
			if (elem.ItemProperties.dwNumProperties > 0) {
				elem.ItemProperties.pItemProperties = (OPCITEMPROPERTY*)CoTaskMemAlloc(sizeof(OPCITEMPROPERTY) * elem.ItemProperties.dwNumProperties);
				NkError::CBaseException::check_alloc(elem.ItemProperties.pItemProperties, __FILE__, __LINE__);
				for (DWORD loop = 0; loop < elem.ItemProperties.dwNumProperties; ++loop) {
					OPCITEMPROPERTY& prop = elem.ItemProperties.pItemProperties[loop];
					stream >> prop.vtDataType;
					stream >> prop.wReserved;
					stream >> prop.dwPropertyID;
					stream >> prop.szItemID;
					stream >> prop.szDescription;
					stream >> prop.vValue;
					stream >> prop.hrErrorID;
					stream >> prop.dwReserved;
				}
			}
			else {
				elem.ItemProperties.pItemProperties = 0;
			}
		}
		LPCWSTR* item_ids()
		{
			delete[] _item_ids;
			_item_ids = new LPCWSTR[_c];
			for (DWORD loop = 0; loop < _c; ++loop) {
				_item_ids[loop] = _p[loop].szItemID;
			}
			return _item_ids;
		}
	};

	/*typedef struct tagOPCITEMDEF
	{
	LPWSTR szAccessPath;
	LPWSTR szItemID;
	BOOL bActive;
	OPCHANDLE hClient;
	DWORD dwBlobSize;
	BYTE *pBlob;
	VARTYPE vtRequestedDataType;
	WORD wReserved;
	} 	OPCITEMDEF;*/

	class CVectorOPCITEMDEF : public NkCom::CVectorBase < CVectorOPCITEMDEF, OPCITEMDEF, DWORD >
	{
	public:
		CVectorOPCITEMDEF() : CVectorBase() {}
		CVectorOPCITEMDEF(DWORD c) : CVectorBase(c) {}
		CVectorOPCITEMDEF(const OPCITEMDEF p[], DWORD c) : CVectorBase(p, c) {}

		void set_element(DWORD i
			, LPCWSTR szItemID
			, OPCHANDLE hClient
			, LPWSTR szAccessPath = 0
			, BOOL bActive = TRUE
			, VARTYPE vtRequestedDataType = VT_EMPTY)
		{
			NkError::CBaseException::check(i >= 0 && i < _c, E_INVALIDARG, __FILE__, __LINE__);
			_p[i].szAccessPath = NkCom::CComString::copy(szAccessPath);
			_p[i].szItemID = NkCom::CComString::copy(szItemID);
			_p[i].bActive = bActive;
			_p[i].hClient = hClient;
			_p[i].vtRequestedDataType = vtRequestedDataType;
		}

		//TODO:
		//void initialize_element(OPCITEMDEF& elem) {
		//	::ZeroMemory(&elem, sizeof(OPCITEMDEF));
		//}

		void copy_element(OPCITEMDEF& dst, const OPCITEMDEF& src)
		{
			dst.szAccessPath = NkCom::CComString::copy(src.szAccessPath);
			dst.szItemID = NkCom::CComString::copy(src.szItemID);
			dst.bActive = src.bActive;
			dst.hClient = src.hClient;
			dst.dwBlobSize = src.dwBlobSize;
			if (src.dwBlobSize > 0) {
				dst.pBlob = (BYTE*)::CoTaskMemAlloc(sizeof(BYTE)*src.dwBlobSize);
				NkError::CBaseException::check_alloc(dst.pBlob, __FILE__, __LINE__);
				memcpy(dst.pBlob, src.pBlob, src.dwBlobSize);
			}
			else {
				dst.pBlob = 0;
			}
			dst.vtRequestedDataType = src.vtRequestedDataType;
			dst.wReserved = src.wReserved;
		}
		void clear_element(OPCITEMDEF& elem) {
			::CoTaskMemFree(elem.szAccessPath);
			::CoTaskMemFree(elem.szItemID);
			::CoTaskMemFree(elem.pBlob);
		}
		void read_element(const OPCITEMDEF& elem, NkStream::CStream& stream) const {
			stream << elem.szAccessPath;
			stream << elem.szItemID;
			stream << elem.bActive;
			stream << elem.hClient;
			stream << elem.dwBlobSize;
			if (elem.dwBlobSize > 0) {
				stream.write(elem.pBlob, elem.dwBlobSize);
			}
			stream << elem.vtRequestedDataType;
			stream << elem.wReserved;
		}
		void write_element(OPCITEMDEF& elem, NkStream::CStream& stream) {
			stream >> elem.szAccessPath;
			stream >> elem.szItemID;
			stream >> elem.bActive;
			stream >> elem.hClient;
			stream >> elem.dwBlobSize;
			if (elem.dwBlobSize > 0) {
				elem.pBlob = (BYTE*)::CoTaskMemAlloc(sizeof(BYTE)*elem.dwBlobSize);
				NkError::CBaseException::check_alloc(elem.pBlob, __FILE__, __LINE__);
				stream.read(elem.pBlob, elem.dwBlobSize);
			}
			else {
				elem.pBlob = 0;
			}
			stream >> elem.vtRequestedDataType;
			stream >> elem.wReserved;
		}
	};

	/*typedef struct tagOPCITEMRESULT
	{
	OPCHANDLE hServer;
	VARTYPE vtCanonicalDataType;
	WORD wReserved;
	DWORD dwAccessRights;
	DWORD dwBlobSize;
	BYTE *pBlob;
	} 	OPCITEMRESULT;*/

	class CVectorOPCITEMRESULT : public NkCom::CVectorBase < CVectorOPCITEMRESULT, OPCITEMRESULT, DWORD >
	{
	public:
		CVectorOPCITEMRESULT() : CVectorBase() {}
		CVectorOPCITEMRESULT(DWORD c) : CVectorBase(c) {}
		CVectorOPCITEMRESULT(const OPCITEMRESULT p[], DWORD c) : CVectorBase(p, c) {}

		void initialize_element(OPCITEMRESULT& elem)
		{
			ZeroMemory(&elem, sizeof(OPCITEMRESULT));
		}
		template<typename SizeType>
		void get_server_handles(NkCom::CVector<OPCHANDLE, SizeType>& ServerHandles)
		{
			OPCHANDLE* pHandle = (OPCHANDLE*)::CoTaskMemAlloc(sizeof(OPCHANDLE)*_c);
			NkError::CBaseException::check_alloc(pHandle, __FILE__, __LINE__);
			for (DWORD loop = 0; loop < _c; ++loop) {
				pHandle[loop] = _p[loop].hServer;
			}
			ServerHandles.attach(pHandle, _c);
		}
		void copy_element(OPCITEMRESULT& dst, const OPCITEMRESULT& src)
		{
			dst.hServer = src.hServer;
			dst.vtCanonicalDataType = src.vtCanonicalDataType;
			dst.wReserved = src.wReserved;
			dst.dwAccessRights = src.dwAccessRights;
			dst.dwBlobSize = src.dwBlobSize;
			if (src.dwBlobSize > 0) {
				dst.pBlob = (BYTE*)::CoTaskMemAlloc(sizeof(BYTE)*src.dwBlobSize);
				NkError::CBaseException::check_alloc(dst.pBlob, __FILE__, __LINE__);
				memcpy(dst.pBlob, src.pBlob, src.dwBlobSize);
			}
			else {
				dst.pBlob = 0;
			}
		}
		void clear_element(OPCITEMRESULT& elem)
		{
			::CoTaskMemFree(elem.pBlob);
		}
		void read_element(const OPCITEMRESULT& elem, NkStream::CStream& stream) const
		{
			stream << elem.hServer;
			stream << elem.vtCanonicalDataType;
			stream << elem.wReserved;
			stream << elem.dwAccessRights;
			stream << elem.dwBlobSize;
			if (elem.dwBlobSize > 0) {
				stream.write(elem.pBlob, elem.dwBlobSize);
			}
		}
		void write_element(OPCITEMRESULT& elem, NkStream::CStream& stream)
		{
			stream >> elem.hServer;
			stream >> elem.vtCanonicalDataType;
			stream >> elem.wReserved;
			stream >> elem.dwAccessRights;
			stream >> elem.dwBlobSize;
			if (elem.dwBlobSize > 0) {
				elem.pBlob = (BYTE*)::CoTaskMemAlloc(sizeof(BYTE)*elem.dwBlobSize);
				NkError::CBaseException::check_alloc(elem.pBlob, __FILE__, __LINE__);
				stream.read(elem.pBlob, elem.dwBlobSize);
			}
			else {
				elem.pBlob = 0;
			}
		}
	};

	/*typedef struct tagOPCITEMATTRIBUTES
	{
	LPWSTR szAccessPath;
	LPWSTR szItemID;
	BOOL bActive;
	OPCHANDLE hClient;
	OPCHANDLE hServer;
	DWORD dwAccessRights;
	DWORD dwBlobSize;
	BYTE *pBlob;
	VARTYPE vtRequestedDataType;
	VARTYPE vtCanonicalDataType;
	OPCEUTYPE dwEUType;
	VARIANT vEUInfo;
	} 	OPCITEMATTRIBUTES;*/

	inline NkStream::CStream& operator>>(NkStream::CStream& l, OPCEUTYPE& r) {
		l.read(&r, sizeof(OPCEUTYPE));
		return l;
	}
	inline NkStream::CStream& operator<<(NkStream::CStream& l, const OPCEUTYPE& r) {
		l.write(&r, sizeof(OPCEUTYPE));
		return l;
	}

	class CVectorOPCITEMATTRIBUTES : public NkCom::CVectorBase < CVectorOPCITEMATTRIBUTES, OPCITEMATTRIBUTES, DWORD >
	{
	public:
		CVectorOPCITEMATTRIBUTES() : CVectorBase() {}
		CVectorOPCITEMATTRIBUTES(DWORD c) : CVectorBase(c) {}
		CVectorOPCITEMATTRIBUTES(const OPCITEMATTRIBUTES p[], DWORD c) : CVectorBase(p, c) {}

		void initialize_element(OPCITEMATTRIBUTES& elem) {
			ZeroMemory(&elem, sizeof(OPCITEMATTRIBUTES));
		}
		void copy_element(OPCITEMATTRIBUTES& dst, const OPCITEMATTRIBUTES& src) {
			dst.szAccessPath = NkCom::CComString::copy(src.szAccessPath);
			dst.szItemID = NkCom::CComString::copy(src.szItemID);
			dst.bActive = src.bActive;
			dst.hClient = src.hClient;
			dst.hServer = src.hServer;
			dst.dwAccessRights = src.dwAccessRights;
			dst.dwBlobSize = src.dwBlobSize;
			if (src.dwBlobSize > 0) {
				dst.pBlob = (BYTE*)::CoTaskMemAlloc(sizeof(BYTE)*src.dwBlobSize);
				NkError::CBaseException::check_alloc(dst.pBlob, __FILE__, __LINE__);
				memcpy(dst.pBlob, src.pBlob, src.dwBlobSize);
			}
			else {
				dst.pBlob = 0;
			}
			dst.vtRequestedDataType = src.vtRequestedDataType;
			dst.vtCanonicalDataType = src.vtCanonicalDataType;
			dst.dwEUType = src.dwEUType;
			NkError::CBaseException::check_result(::VariantCopy(&dst.vEUInfo, &src.vEUInfo), __FILE__, __LINE__);
		}
		void clear_element(OPCITEMATTRIBUTES& elem) {
			::CoTaskMemFree(elem.szAccessPath);
			::CoTaskMemFree(elem.szItemID);
			::CoTaskMemFree(elem.pBlob);
			NkError::CBaseException::check_result(::VariantClear(&elem.vEUInfo), __FILE__, __LINE__);
		}
		void read_element(const OPCITEMATTRIBUTES& elem, NkStream::CStream& stream) const {
			stream << elem.szAccessPath;
			stream << elem.szItemID;
			stream << elem.bActive;
			stream << elem.hClient;
			stream << elem.hServer;
			stream << elem.dwAccessRights;
			stream << elem.dwBlobSize;
			if (elem.dwBlobSize > 0) {
				stream.write(elem.pBlob, elem.dwBlobSize);
			}
			stream << elem.vtRequestedDataType;
			stream << elem.vtCanonicalDataType;
			stream << elem.dwEUType;
			stream << elem.vEUInfo;
		}
		void write_element(OPCITEMATTRIBUTES& elem, NkStream::CStream& stream) {
			stream >> elem.szAccessPath;
			stream >> elem.szItemID;
			stream >> elem.bActive;
			stream >> elem.hClient;
			stream >> elem.hServer;
			stream >> elem.dwAccessRights;
			stream >> elem.dwBlobSize;
			if (elem.dwBlobSize > 0) {
				elem.pBlob = (BYTE*)::CoTaskMemAlloc(sizeof(BYTE)*elem.dwBlobSize);
				NkError::CBaseException::check_alloc(elem.pBlob, __FILE__, __LINE__);
				stream.read(elem.pBlob, elem.dwBlobSize);
			}
			else {
				elem.pBlob = 0;
			}
			stream >> elem.vtRequestedDataType;
			stream >> elem.vtCanonicalDataType;
			stream >> elem.dwEUType;
			stream >> elem.vEUInfo;
		}
	};

	//IEnumOPCItemAttributes

	typedef NkCom::CComEnumNear<IEnumOPCItemAttributes, OPCITEMATTRIBUTES, CVectorOPCITEMATTRIBUTES> CEnumOPCItemAttributesNear;
	typedef NkCom::CComPtrNear<IEnumOPCItemAttributes, CEnumOPCItemAttributesNear> CComPtrNearEnumOPCItemAttributes;

	typedef NkCom::CComEnumFar2<IEnumOPCItemAttributes, OPCITEMATTRIBUTES, CVectorOPCITEMATTRIBUTES> CEnumOPCItemAttributesFar;
	typedef NkCom::CComPtrFar<IEnumOPCItemAttributes, CEnumOPCItemAttributesFar> CComPtrFarEnumOPCItemAttributes;

	/*typedef struct tagOPCITEMSTATE
	{
	OPCHANDLE hClient;
	FILETIME ftTimeStamp;
	WORD wQuality;
	WORD wReserved;
	VARIANT vDataValue;
	} 	OPCITEMSTATE;*/

	class CVectorOPCITEMSTATE : public NkCom::CVectorBase < CVectorOPCITEMSTATE, OPCITEMSTATE, DWORD >
	{
	public:
		CVectorOPCITEMSTATE() : CVectorBase() {}
		CVectorOPCITEMSTATE(DWORD c) : CVectorBase(c) {}
		CVectorOPCITEMSTATE(const OPCITEMSTATE p[], DWORD c) : CVectorBase(p, c) {}

		void initialize_element(OPCITEMSTATE& elem)
		{
			ZeroMemory(&elem, sizeof(OPCITEMATTRIBUTES));
		}
		void copy_element(OPCITEMSTATE& dst, const OPCITEMSTATE& src)
		{
			dst.hClient = src.hClient;
			dst.ftTimeStamp.dwHighDateTime = src.ftTimeStamp.dwHighDateTime;
			dst.ftTimeStamp.dwLowDateTime = src.ftTimeStamp.dwLowDateTime;
			dst.wQuality = src.wQuality;
			dst.wReserved = src.wReserved;
			NkError::CBaseException::check_result(::VariantCopy(&dst.vDataValue, &src.vDataValue), __FILE__, __LINE__);
		}
		void clear_element(OPCITEMSTATE& elem)
		{
			NkError::CBaseException::check_result(::VariantClear(&elem.vDataValue), __FILE__, __LINE__);
		}
		void read_element(const OPCITEMSTATE& elem, NkStream::CStream& stream) const
		{
			stream << elem.hClient;
			stream << elem.ftTimeStamp;
			stream << elem.wQuality;
			stream << elem.wReserved;
			stream << elem.vDataValue;
		}
		void write_element(OPCITEMSTATE& elem, NkStream::CStream& stream)
		{
			stream >> elem.hClient;
			stream >> elem.ftTimeStamp;
			stream >> elem.wQuality;
			stream >> elem.wReserved;
			stream >> elem.vDataValue;
		}
	};

	//OPCBROWSEFILTER

	inline NkStream::CStream& operator>>(NkStream::CStream& l, OPCBROWSEFILTER& r) {
		l.read(&r, sizeof(OPCBROWSEFILTER));
		return l;
	}
	inline NkStream::CStream& operator<<(NkStream::CStream& l, const OPCBROWSEFILTER& r) {
		l.write(&r, sizeof(OPCBROWSEFILTER));
		return l;
	}

	//OPCENUMSCOPE

	inline NkStream::CStream& operator>>(NkStream::CStream& l, OPCENUMSCOPE& r) {
		l.read(&r, sizeof(OPCENUMSCOPE));
		return l;
	}
	inline NkStream::CStream& operator<<(NkStream::CStream& l, const OPCENUMSCOPE& r) {
		l.write(&r, sizeof(OPCENUMSCOPE));
		return l;
	}

	//OPCDATASOURCE

	inline NkStream::CStream& operator>>(NkStream::CStream& l, OPCDATASOURCE& r) {
		l.read(&r, sizeof(OPCDATASOURCE));
		return l;
	}
	inline NkStream::CStream& operator<<(NkStream::CStream& l, const OPCDATASOURCE& r) {
		l.write(&r, sizeof(OPCDATASOURCE));
		return l;
	}

	//OPCNAMESPACETYPE

	inline NkStream::CStream& operator>>(NkStream::CStream& l, OPCNAMESPACETYPE& r) {
		l.read(&r, sizeof(OPCNAMESPACETYPE));
		return l;
	}
	inline NkStream::CStream& operator<<(NkStream::CStream& l, const OPCNAMESPACETYPE& r) {
		l.write(&r, sizeof(OPCNAMESPACETYPE));
		return l;
	}

	//OPCBROWSEDIRECTION

	inline NkStream::CStream& operator>>(NkStream::CStream& l, OPCBROWSEDIRECTION& r) {
		l.read(&r, sizeof(OPCBROWSEDIRECTION));
		return l;
	}
	inline NkStream::CStream& operator<<(NkStream::CStream& l, const OPCBROWSEDIRECTION& r) {
		l.write(&r, sizeof(OPCBROWSEDIRECTION));
		return l;
	}

	//OPCBROWSETYPE

	inline NkStream::CStream& operator>>(NkStream::CStream& l, OPCBROWSETYPE& r) {
		l.read(&r, sizeof(OPCBROWSETYPE));
		return l;
	}
	inline NkStream::CStream& operator<<(NkStream::CStream& l, const OPCBROWSETYPE& r) {
		l.write(&r, sizeof(OPCBROWSETYPE));
		return l;
	}
}