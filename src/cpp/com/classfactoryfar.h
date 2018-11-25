#pragma once
#include "com/comptr.h"
#include "com/comfarunknown.h"
#include <OCIdl.h>

namespace NkCom
{
	class CClassFactoryFar : public NkCom::CComFarUnknown
	{
	public:

		CClassFactoryFar(CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid);

		HRESULT query_interface(REFIID iid);
		void call(ULONG32 index, NkStream::CStream& stream);
		void CreateInstance(NkStream::CStream& stream);
		void LockServer(NkStream::CStream& stream);
		void GetLicInfo(NkStream::CStream& stream);
		void RequestLicKey(NkStream::CStream& stream);
		void CreateInstanceLic(NkStream::CStream& stream);

	private:
		CClassFactoryFar();
		CClassFactoryFar &operator =(const CClassFactoryFar&);
		CClassFactoryFar(const CClassFactoryFar&);

		NkCom::CComPtr<IClassFactory> m_spClassFactory;
		NkCom::CComPtr<IClassFactory2> m_spClassFactory2;
	};
}