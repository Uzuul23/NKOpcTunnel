#pragma once
#include "com/nearunknown.h"
#include "OCIdl.h"

namespace NkCom
{
	class CClassFactoryNear : public CNearUnknown < CClassFactoryNear >
		, public IClassFactory2
	{
	public:
		CClassFactoryNear(CServer* p, ULONG32 id);;

		IMPL_INTERFACE1(IClassFactory);

		STDMETHOD(CreateInstance)(IUnknown *pUnkOuter, REFIID riid, void **ppvObject);
		STDMETHOD(LockServer)(BOOL fLock);
		STDMETHOD(GetLicInfo)(LICINFO *pLicInfo);
		STDMETHOD(RequestLicKey)(DWORD dwReserved, BSTR *pBstrKey);
		STDMETHOD(CreateInstanceLic)(IUnknown *pUnkOuter, IUnknown *pUnkReserved
			, REFIID riid, BSTR bstrKey, PVOID *ppvObj);

		CLSID creator_iid() const;
		void creator_iid(CLSID val);

	private:
		CClassFactoryNear();
		CClassFactoryNear &operator =(const CClassFactoryNear&);
		CClassFactoryNear(const CClassFactoryNear&);

		CLSID m_iid;
	};
}