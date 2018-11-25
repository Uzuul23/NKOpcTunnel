#pragma once
#include "com/nearunknown.h"
#include "opc/opcda.h"

namespace NkOPC
{
	class COPCDataCallbackNear : public NkCom::CNearUnknown < COPCDataCallbackNear >
		, public IOPCDataCallback
	{
	public:
		COPCDataCallbackNear(NkCom::CServer* p, ULONG32 id);
		virtual ~COPCDataCallbackNear();

		IMPL_ADDREF_RELEASE;
		STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject);

		STDMETHOD(OnDataChange)(DWORD dwTransid, OPCHANDLE hGroup
			, HRESULT hrMasterquality, HRESULT hrMastererror
			, DWORD dwCount, OPCHANDLE *phClientItems, VARIANT *pvValues
			, WORD *pwQualities, FILETIME *pftTimeStamps, HRESULT *pErrors);
		STDMETHOD(OnReadComplete)(DWORD dwTransid, OPCHANDLE hGroup
			, HRESULT hrMasterquality, HRESULT hrMastererror
			, DWORD dwCount, OPCHANDLE *phClientItems
			, VARIANT *pvValues, WORD *pwQualities
			, FILETIME *pftTimeStamps, HRESULT *pErrors);
		STDMETHOD(OnWriteComplete)(DWORD dwTransid, OPCHANDLE hGroup
			, HRESULT hrMastererr, DWORD dwCount
			, OPCHANDLE *pClienthandles, HRESULT *pErrors);
		STDMETHOD(OnCancelComplete)(DWORD dwTransid, OPCHANDLE hGroup);

	private:
		COPCDataCallbackNear();
		COPCDataCallbackNear &operator =(const COPCDataCallbackNear&);
		COPCDataCallbackNear(const COPCDataCallbackNear&);
	};
}