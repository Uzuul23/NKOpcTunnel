#pragma once
#include "stream/stream.h"
#include "com/comptr.h"
#include "com/comfarunknown.h"

namespace NkOPC
{
	class COPCShutdownFar : public NkCom::CComFarUnknown
	{
	public:
		COPCShutdownFar(NkCom::CServer* pSrv, IUnknown* p, ULONG32* pid);

		HRESULT query_interface(REFIID iid);
		void call(ULONG32 index, NkStream::CStream& stream);
		void ShutdownRequest(NkStream::CStream& stream);

	private:
		COPCShutdownFar();
		COPCShutdownFar &operator =(const COPCShutdownFar&);
		COPCShutdownFar(const COPCShutdownFar&);
	};
}