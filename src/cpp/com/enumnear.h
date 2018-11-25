#pragma once
#include "com/nearunknown.h"
#include "com/comptr.h"
#include "com/vector.h"

namespace NkCom
{
	// CComEnumNear

	template<class EnumType, class ElementType, class ElementVector = NkCom::CVector<ElementType>>
	class CComEnumNear : public CNearUnknown < CComEnumNear<EnumType, ElementType> >
		, public EnumType
	{
	public:
		IMPL_INTERFACE1(EnumType);

		CComEnumNear(CServer* pSrv, ULONG32 id) : CNearUnknown(pSrv, id)
		{
		}

		STDMETHOD(Next)(/* [in] */ ULONG celt,
			/* [length_is][size_is][out] */ ElementType **pp,
			/* [out] */ ULONG *pcFetched)
		{
			if (pp == 0 || (celt > 1 && pcFetched == 0)){
				return E_POINTER;
			}

			try{
				CComEnumNear::CCallFctGuard srv(this, 1, L"CComEnumNear::Next()");
				srv.stream << celt;
				srv.stream.flush();

				srv.stream >> srv.result;

				if (SUCCEEDED(srv.result)){
					ElementVector fetch;
					srv.stream >> fetch;
					fetch.detach(pp, pcFetched);
				}

				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}

		STDMETHOD(Skip)(/* [in] */ ULONG celt)
		{
			try{
				CComEnumNear::CCallFctGuard srv(this, 2, L"CComEnumNear::Skip()");
				srv.stream << celt;
				srv.stream.flush();

				srv.stream >> srv.result;
				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}

		STDMETHOD(Reset)(void)
		{
			try{
				CComEnumNear::CCallFctGuard srv(this, 3, L"CComEnumNear::Reset()");
				srv.stream.flush();

				srv.stream >> srv.result;
				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}

		STDMETHOD(Clone)( /* [out] */EnumType **ppEnum)
		{
			if (ppEnum == 0){
				return E_POINTER;
			}

			try{  //TODO:
				CComEnumNear::CCallFctGuard srv(this, 4, L"CComEnumNear::Clone()");
				srv.stream.flush();

				srv.stream >> srv.result;
				if (SUCCEEDED(srv.result)){
					ULONG32 id;
					srv.stream >> id;
					(*ppEnum) = create_instance(m_pSrv, id);
				}
				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}
	};

	// CComEnumUnkNear

	template<class EnumType, class ElementType, class ElementProxy>
	class CComEnumUnkNear : public CNearUnknown<CComEnumUnkNear<EnumType, ElementType, ElementProxy>>, public EnumType
	{
	public:
		IMPL_INTERFACE1(EnumType);

		CComEnumUnkNear(CServer* pSrv, ULONG32 id) : CNearUnknown(pSrv, id)
		{
		}

		STDMETHOD(Next)(/* [in] */ ULONG celt,
			/* [length_is][size_is][out] */ ElementType **pp,
			/* [out] */ ULONG *pcFetched)
		{
			try{
				CComEnumUnkNear::CCallFctGuard srv(this, 1, L"CComEnumUnkNear::Next()");
				srv.stream << celt;
				srv.stream.flush();

				srv.stream >> srv.result;
				if (SUCCEEDED(srv.result)){
					ULONG c;
					srv.stream >> c;
					for (ULONG loop = 0; loop < c; ++loop){
						CComPtrNear<ElementType, ElementProxy> spElem(m_pSrv);
						srv.stream >> spElem;
						spElem.Detach(&pp[loop]);
					}
					*pcFetched = c;
				}
				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}

		STDMETHOD(Skip)(/* [in] */ ULONG celt)
		{
			try{
				CComEnumUnkNear::CCallFctGuard srv(this, 2, L"CComEnumUnkNear::Skip()");
				srv.stream << celt;
				srv.stream.flush();

				srv.stream >> srv.result;
				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}

		STDMETHOD(Reset)(void)
		{
			try{
				CComEnumUnkNear::CCallFctGuard srv(this, 3, L"CComEnumUnkNear::Reset()");
				srv.stream.flush();

				srv.stream >> srv.result;
				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}

		STDMETHOD(Clone)( /* [out] */EnumType **ppEnum)
		{
			try{
				CComEnumUnkNear::CCallFctGuard srv(this, 4, L"CComEnumUnkNear::Clone()");
				srv.stream.flush();

				srv.stream >> srv.result;
				if (SUCCEEDED(srv.result)){
					CComPtrNear<EnumType, CComEnumUnkNear<EnumType, ElementType, ElementProxy>> spEnum(m_pSrv);
					srv.stream >> spEnum;
					spEnum.Detach(ppEnum);
				}
				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}
	};

	// CComEnumStringNear
	class CComEnumStringNear;
	typedef CComPtrNear<IEnumString, CComEnumStringNear> CComPtrNearEnumString;

	class CComEnumStringNear : public CNearUnknown < CComEnumStringNear >
		, public IEnumString
	{
	private:
		CComEnumStringNear();
		CComEnumStringNear &operator =(const CComEnumStringNear&);
		CComEnumStringNear(const CComEnumStringNear&);

	public:
		IMPL_INTERFACE1(IEnumString);

		CComEnumStringNear(CServer* pSrv, ULONG32 id) : CNearUnknown(pSrv, id)
		{
		}

		STDMETHOD(Next)(/* [in] */ ULONG celt,
			/* [length_is][size_is][out] */ LPOLESTR *rgelt,
			/* [out] */ ULONG *pceltFetched)
		{
			try{
				CComEnumStringNear::CCallFctGuard srv(this, 1, L"CComEnumStringNear::Next()");
				srv.stream << celt;
				srv.stream.flush();

				srv.stream >> srv.result;
				if (SUCCEEDED(srv.result)){
					CStringVector<ULONG> gelt;
					srv.stream >> gelt;
					for (ULONG loop = 0; loop < gelt.size(); ++loop){
						gelt.detach_element(&rgelt[loop], loop);
					}
					(*pceltFetched) = gelt.size();
				}
				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}

		STDMETHOD(Skip)(/* [in] */ ULONG celt)
		{
			try{
				CComEnumStringNear::CCallFctGuard srv(this, 2, L"CComEnumStringNear::Skip()");
				srv.stream << celt;
				srv.stream.flush();

				srv.stream >> srv.result;
				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}

		STDMETHOD(Reset)(void)
		{
			try{
				CComEnumStringNear::CCallFctGuard srv(this, 3, L"CComEnumStringNear::Reset()");
				srv.stream.flush();

				srv.stream >> srv.result;
				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}

		STDMETHOD(Clone)( /* [out] */IEnumString **ppEnum)
		{
			try{
				CComEnumStringNear::CCallFctGuard srv(this, 4, L"CComEnumStringNear::Clone()");
				srv.stream.flush();

				srv.stream >> srv.result;
				if (SUCCEEDED(srv.result)){
					CComPtrNearEnumString spEnum(m_pSrv);
					srv.stream >> spEnum;
					spEnum.Detach(ppEnum);
				}
				return srv.result;
			}
			catch (NkError::CException& e) {
				return handle_error(e);
			}
		}
	};
}