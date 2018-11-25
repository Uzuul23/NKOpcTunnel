#pragma once
#include "stream/interfaces.h"
#include "com/interfaces.h"
#include "com/comfarsrv.h"
#include "com/classfactoryfar.h"
#include "opc/opcadvisesinknear.h"
#include "opc/opcserverlist2far.h"
#include "opc/opcserverfar.h"
#include "opc/opcgroupfar.h"
#include "opc/opcshutdownnear.h"
#include "opc/opcdatacallbacknear.h"
#include "opc/opcenum.h"
#include <vector>

namespace NkOPC
{
	class COPCFarSrv : public NkCom::CComFarSrv
	{
	public:
		COPCFarSrv(NkStream::CStreamIODevice* provider
			, NkCom::CServerEvent* p_event = 0
			, bool auto_delete_provider = true);
		virtual ~COPCFarSrv();

		//IUnknown
		STDMETHOD_(ULONG, AddRef)();
		STDMETHOD_(ULONG, Release)();
		STDMETHOD(QueryInterface)(REFGUID iid, void **pp);

		virtual HRESULT logon(const BYTE* credentials, size_t cb);
		virtual HRESULT create(REFCLSID rclsid, DWORD dwClsContext, REFIID riid, ULONG32& id);
		virtual void create_near_proxy(REFIID proxy_iid, REFIID riid, void** ppv, ULONG32 id);
		virtual NkCom::CComFarUnknown* create_far_proxy(IUnknown* p, REFIID riid, ULONG32& id);

		enum COPCFarSrvAccess
		{
			BrowseOPCServer = 1
		};

		//access
		bool has_access(COPCFarSrvAccess level);
		bool filter_access(COPCFarSrvAccess level, const std::vector<GUID>& guids
			, std::vector<bool>& accesses, size_t* allowed_count = 0);

	private:
		COPCFarSrv();
		COPCFarSrv(const COPCFarSrv&);
		COPCFarSrv& operator=(const COPCFarSrv&);

		class CImpl;
		CImpl* _Impl;
	};
}