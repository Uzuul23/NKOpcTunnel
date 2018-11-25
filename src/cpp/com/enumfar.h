#pragma once
#include "stream/stream.h"
#include "com/comfarunknown.h"
#include "com/comptr.h"
#include "com/vector.h"
#include <vector>

namespace NkCom
{
	// CComEnumFar

	template <class EnumType, class ElementType, class ElementVector = CVector<ElementType>>
	class CComEnumFar : public CComFarUnknown
	{
	public:
		CComEnumFar(CServer* pSrv, EnumType* p, ULONG32* pid = 0)
			: CComFarUnknown(pSrv, p, pid)
			, m_spEnum(p)
		{
		}

		void call(ULONG32 index, NkStream::CStream& stream)
		{
			switch (index) {
			case 1: Next(stream); break;
			case 2: Skip(stream); break;
			case 3: Reset(stream); break;
			case 4: Clone(stream); break;
			}

			if (NkTrace::CTrace::IsTraceLevel3()) {
				static const WCHAR* pszFctNames[] = {
					L"dummy",
					L"CComEnum::Next()",
					L"CComEnum::Skip()",
					L"CComEnum::Reset()",
					L"CComEnum::Clone()"
				};
				if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
					_ASSERT(0);
					return;
				}
				NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
			}
		}

		HRESULT query_interface(REFIID riid)
		{
			if (riid == IID_IUnknown || riid == __uuidof(EnumType))
				return S_OK;
			return E_NOINTERFACE;
		}

		void Next(NkStream::CStream& stream)
		{
			ULONG celt;
			stream >> celt;
			ElementVector gelt(celt);
			ULONG celtFetched = 0;
			HRESULT hr = m_spEnum->Next(celt, gelt.data(), &celtFetched);
			stream << hr;
			if (SUCCEEDED(hr)) {
				gelt.size(celtFetched);
				stream << gelt;
			}
			stream.flush();
		}

		void Skip(NkStream::CStream& stream)
		{
			ULONG celt;
			stream >> celt;
			HRESULT hr = m_spEnum->Skip(celt);
			stream << hr;
			stream.flush();
		}

		void Reset(NkStream::CStream& stream)
		{
			HRESULT hr = m_spEnum->Reset();
			stream << hr;
			stream.flush();
		}

		void Clone(NkStream::CStream& stream)
		{
			NkCom::CComPtrFar<EnumType, CComEnumFar<EnumType, ElementType>> spEnum(m_pSrv);
			HRESULT hr = m_spEnum->Clone(&spEnum);
			stream << hr;
			if (SUCCEEDED(hr)) {
				stream << spEnum;
			}
			stream.flush();
		}

	protected:
		CComPtr<EnumType> m_spEnum;
	};

	// CComEnumFar2

	template <class EnumType, class ElementType, class ElementVector = CVector<ElementType>>
	class CComEnumFar2 : public CComFarUnknown
	{
	public:
		CComEnumFar2(CServer* pSrv, EnumType* p, ULONG32* pid = 0)
			: CComFarUnknown(pSrv, p, pid)
			, m_spEnum(p)
		{
		}

		void call(ULONG32 index, NkStream::CStream& stream)
		{
			switch (index) {
			case 1: Next(stream); break;
			case 2: Skip(stream); break;
			case 3: Reset(stream); break;
			case 4: Clone(stream); break;
			}

			if (NkTrace::CTrace::IsTraceLevel3()) {
				static const WCHAR* pszFctNames[] = {
					L"dummy",
					L"CComEnum::Next()",
					L"CComEnum::Skip()",
					L"CComEnum::Reset()",
					L"CComEnum::Clone()"
				};
				if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
					_ASSERT(0);
					return;
				}
				NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
			}
		}

		HRESULT query_interface(REFIID riid)
		{
			if (riid == IID_IUnknown || riid == __uuidof(EnumType))
				return S_OK;
			return E_NOINTERFACE;
		}

		void Next(NkStream::CStream& stream)
		{
			ULONG celt;
			stream >> celt;
			ElementVector gelt;
			ULONG celtFetched = 0;
			HRESULT hr = m_spEnum->Next(celt, &gelt, &celtFetched);
			stream << hr;
			if (SUCCEEDED(hr)) {
				gelt.size(celtFetched);
				stream << gelt;
			}
			stream.flush();
		}

		void Skip(NkStream::CStream& stream)
		{
			ULONG celt;
			stream >> celt;
			HRESULT hr = m_spEnum->Skip(celt);
			stream << hr;
			stream.flush();
		}

		void Reset(NkStream::CStream& stream)
		{
			HRESULT hr = m_spEnum->Reset();
			stream << hr;
			stream.flush();
		}

		void Clone(NkStream::CStream& stream)
		{
			NkCom::CComPtrFar<EnumType, CComEnumFar2<EnumType, ElementType>> spEnum(m_pSrv);
			HRESULT hr = m_spEnum->Clone(&spEnum);
			stream << hr;
			if (SUCCEEDED(hr)) {
				stream << spEnum;
			}
			stream.flush();
		}

	protected:
		CComPtr<EnumType> m_spEnum;
	};

	// CComEnumUnkFar

	template <class EnumType, class ElementType, class ElementProxy>
	class CComEnumUnkFar : public CComFarUnknown
	{
	public:
		CComEnumUnkFar(CServer* pSrv, EnumType* p, ULONG32* pid = 0)
			: CComFarUnknown(pSrv, p, pid)
			, m_spEnum(p)
		{
		}

		void call(ULONG32 index, NkStream::CStream& stream)
		{
			switch (index) {
			case 1: Next(stream); break;
			case 2: Skip(stream); break;
			case 3: Reset(stream); break;
			case 4: Clone(stream); break;
			}

			if (NkTrace::CTrace::IsTraceLevel3()) {
				static const WCHAR* pszFctNames[] = {
					L"dummy",
					L"CComEnumUnk::Next()",
					L"CComEnumUnk::Skip()",
					L"CComEnumUnk::Reset()",
					L"CComEnumUnk::Clone()"
				};
				if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
					_ASSERT(0);
					return;
				}
				NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
			}
		}

		HRESULT query_interface(REFIID riid)
		{
			if (riid == IID_IUnknown || riid == __uuidof(EnumType))
				return S_OK;
			return E_NOINTERFACE;
		}

		void Next(NkStream::CStream& stream)
		{
			ULONG celt;
			stream >> celt;
			CVector<ElementType*, ULONG> gelt(celt);
			ULONG celtFetched = 0;
			HRESULT hr = m_spEnum->Next(celt, gelt, &celtFetched);
			stream << hr;
			if (SUCCEEDED(hr)) {
				gelt.size(celtFetched);
				stream << celtFetched;
				for (ULONG loop = 0; loop < gelt.size(); ++loop) {
					CComPtrFar<ElementType, ElementProxy> spElem(m_pSrv, gelt[loop]);
					stream << spElem;
				}
			}
			stream.flush();
		}

		void Skip(NkStream::CStream& stream)
		{
			ULONG celt;
			stream >> celt;
			HRESULT hr = m_spEnum->Skip(celt);
			stream << hr;
			stream.flush();
		}

		void Reset(NkStream::CStream& stream)
		{
			HRESULT hr = m_spEnum->Reset();
			stream << hr;
			stream.flush();
		}

		void Clone(NkStream::CStream& stream)
		{
			NkCom::CComPtrFar<EnumType, CComEnumUnkFar<EnumType, ElementType, ElementProxy>> spEnum(m_pSrv);
			HRESULT hr = m_spEnum->Clone(&spEnum);
			stream << hr;
			if (SUCCEEDED(hr)) {
				stream << spEnum;
			}
			stream.flush();
		}

		CComPtr<EnumType> m_spEnum;
	};

	//CComEnumStringFar

	class CComEnumStringFar;
	typedef NkCom::CComPtrFar<IEnumString, CComEnumStringFar> CComPtrFarEnumString;

	class CComEnumStringFar : public CComFarUnknown
	{
	public:
		CComEnumStringFar(CServer* pSrv, IEnumString* p, ULONG32* pid = 0)
			: CComFarUnknown(pSrv, p, pid)
			, m_spEnum(p)
		{
		}

		void call(ULONG32 index, NkStream::CStream& stream)
		{
			switch (index) {
			case 1: Next(stream); break;
			case 2: Skip(stream); break;
			case 3: Reset(stream); break;
			case 4: Clone(stream); break;
			}

			if (NkTrace::CTrace::IsTraceLevel3()) {
				static const WCHAR* pszFctNames[] = {
					L"dummy",
					L"CComEnumString::Next()",
					L"CComEnumString::Skip()",
					L"CComEnumString::Reset()",
					L"CComEnumString::Clone()"
				};
				if (index > (sizeof(pszFctNames) / sizeof(WCHAR*))) {
					_ASSERT(0);
					return;
				}
				NkTrace::CTrace::TraceDet(L"remote call: %s", pszFctNames[index]);
			}
		}

		HRESULT query_interface(REFIID riid)
		{
			if (riid == IID_IUnknown || riid == IID_IEnumString) {
				return S_OK;
			}
			return E_NOINTERFACE;
		}

		void Next(NkStream::CStream& stream)
		{
			ULONG celt;
			stream >> celt;
			CStringVector<ULONG> gelt(celt);
			ULONG celtFetched = 0;
			HRESULT hr = m_spEnum->Next(celt, gelt, &celtFetched);
			stream << hr;
			if (SUCCEEDED(hr)) {
				gelt.size(celtFetched);
				stream << gelt;
			}
			stream.flush();
		}

		void Skip(NkStream::CStream& stream)
		{
			ULONG celt;
			stream >> celt;
			HRESULT hr = m_spEnum->Skip(celt);
			stream << hr;
			stream.flush();
		}

		void Reset(NkStream::CStream& stream)
		{
			HRESULT hr = m_spEnum->Reset();
			stream << hr;
			stream.flush();
		}

		void Clone(NkStream::CStream& stream)
		{
			CComPtrFarEnumString spEnum(m_pSrv);
			HRESULT hr = m_spEnum->Clone(&spEnum);
			stream << hr;
			if (SUCCEEDED(hr)) {
				stream << spEnum;
			}
			stream.flush();
		}

		CComPtr<IEnumString> m_spEnum;
	};
}