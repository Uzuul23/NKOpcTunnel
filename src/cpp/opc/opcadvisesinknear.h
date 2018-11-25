#pragma once
#include "com/nearunknown.h"
#include <objidl.h>

namespace NkOPC
{
	class COPCAdviseSinkNear : public NkCom::CNearUnknown < COPCAdviseSinkNear >
		, public IAdviseSink
	{
	public:
		COPCAdviseSinkNear(NkCom::CServer* p, ULONG32 id);;

		IMPL_INTERFACE1(IAdviseSink);

		STDMETHOD_(void, OnDataChange)(FORMATETC *pFormatetc, STGMEDIUM *pStgmed);
		STDMETHOD_(void, OnViewChange)(DWORD dwAspect, LONG lindex);
		STDMETHOD_(void, OnRename)(IMoniker *pmk);
		STDMETHOD_(void, OnSave)(void);
		STDMETHOD_(void, OnClose)(void);

		enum CfFormats
		{
			OPCSTMFORMATDATA = 1,
			OPCSTMFORMATDATATIME,
			OPCSTMFORMATWRITECOMPLETE
		};

	private:
		COPCAdviseSinkNear();
		COPCAdviseSinkNear &operator =(const COPCAdviseSinkNear&);
		COPCAdviseSinkNear(const COPCAdviseSinkNear&);

		static void RegisterClipboardFormats();
		static UINT m_cfOPCSTMFORMATDATA;
		static UINT m_cfOPCSTMFORMATDATATIME;
		static UINT m_cfOPCSTMFORMATWRITECOMPLETE;
	};
}