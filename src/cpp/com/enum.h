#pragma once
#include "com/nearunknown.h"
#include "com/comptr.h"
#include "com/vector.h"
#include "com/unknownimpl.h"

namespace NkCom
{
	// CComEnum

	template<class EnumType, class ElementType, typename SizeType = DWORD
		, class VectoreType = NkCom::CVector < ElementType, SizeType >>
	class CComEnum : public EnumType
	{
	private:
		ULONG m_cRef = 0;
		VectoreType m_vec;
		SizeType m_iter = 0;
		CComEnum()
		{
		}
	public:

		static void create(CComEnum<EnumType, ElementType>** pp)
		{
			(*pp) = new CComEnum<EnumType, ElementType>;
		}

		virtual ~CComEnum()
		{
		}

		void take_ownership(NkCom::CVector<ElementType, SizeType>& vec)
		{
			m_vec.attach(vec);
		}

		STDMETHOD_(ULONG, AddRef)()
		{
			return ++m_cRef;
		}
		STDMETHOD_(ULONG, Release)()
		{
			if (--m_cRef != 0)  \
				return m_cRef;
			delete this;
			return 0;
		}
		STDMETHOD(QueryInterface)(REFGUID iid, void **pp)
		{
			if (pp == 0) {
				return E_INVALIDARG;
			}
			if (iid == IID_IUnknown) {
				(*pp) = static_cast<EnumType*>(this);
				AddRef();
				return S_OK;
			}
			if (iid == __uuidof(EnumType)) {
				(*pp) = static_cast<EnumType*>(this);
				AddRef();
				return S_OK;
			}
			return E_NOINTERFACE;
		}

		STDMETHOD(Next)(/* [in] */ ULONG celt,
			/* [length_is][size_is][out] */ ElementType *p,
			/* [out] */ ULONG *pcFetched)
		{
			if (p == 0 || (celt > 1 && pcFetched == 0)) {
				return E_POINTER;
			}
			if (pcFetched) {
				*pcFetched = 0;
			}
			if (celt == 0) {
				return E_INVALIDARG;
			}
			if (p == 0 || (celt != 1 && pcFetched == 0)) {
				return E_POINTER;
			}
			if (m_vec.size() == 0 || m_iter == m_vec.size()) {
				return E_FAIL;
			}
			HRESULT hr = S_OK;
			SizeType left = m_vec.size() - m_iter;
			if (left < (static_cast<SizeType>(celt))) {
				hr = S_FALSE;
			}
			SizeType count = (static_cast<SizeType>(celt) > left) ? left : celt;
			try {
				for (SizeType loop = 0; loop < count; ++loop) {
					m_vec.copy_elem(&p[loop], m_iter + loop);
				}		
			}
			catch (NkError::CException& e) {
				e.report();
				return E_FAIL;
			}
			m_iter += count;
			if (pcFetched) {
				*pcFetched = count;
			}
			return hr;
		}

		STDMETHOD(Skip)(/* [in] */ ULONG celt)
		{
			if (celt == 0) {
				return E_INVALIDARG;
			}
			SizeType left = m_vec.size() - m_iter;
			SizeType skip = (static_cast<SizeType>(celt) > left) ? left : celt;
			m_iter += skip;
			return (static_cast<SizeType>(celt) == skip) ? S_OK : S_FALSE;
		}

		STDMETHOD(Reset)(void)
		{
			m_iter = 0;
			return S_OK;
		}

		STDMETHOD(Clone)( /* [out] */EnumType **ppEnum)
		{
			if (ppEnum == 0) {
				return E_POINTER;
			}

			CComEnum<EnumType, ElementType, SizeType, VectoreType>* p = 0;
			try {
				p = new CComEnum<EnumType, ElementType, SizeType, VectoreType>;
				m_vec.copy_to(p->m_vec);
				(*ppEnum) = p;
			}
			catch (NkError::CException& e) {
				delete p;
				e.report();
				return E_FAIL;
			}
			return S_OK;
		}
	};
}