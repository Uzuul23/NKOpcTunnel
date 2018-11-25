#pragma once
#include "com/comfarunknown.h"
#include "windows/globalmem.h"

namespace NkOPC
{
	class COPCAdviseSinkFar : public NkCom::CComFarUnknown
	{
	public:
		COPCAdviseSinkFar(NkCom::CServer* pSrv, IUnknown* p, ULONG32* pid);

		HRESULT query_interface(REFIID iid);

		void call(ULONG32 index, NkStream::CStream& stream);

		//IAdviseSink
		void OnDataChange(NkStream::CStream& stream);

		COPCAdviseSinkFar() = delete;
		COPCAdviseSinkFar &operator =(const COPCAdviseSinkFar&) = delete;
		COPCAdviseSinkFar(const COPCAdviseSinkFar&) = delete;
	
	private:
		static void RegisterClipboardFormats();
		static UINT m_cfOPCSTMFORMATDATA;
		static UINT m_cfOPCSTMFORMATDATATIME;
		static UINT m_cfOPCSTMFORMATWRITECOMPLETE;
	};
}