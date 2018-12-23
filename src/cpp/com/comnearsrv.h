/*	This file is part of NKOpcTunnel.
*
*	Copyright (c) Henryk Anschuetz 
*	Berlin, Germany
*
*	mailto:uzuul23@online.de
*
*	NKOpcTunnel is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NKOpcTunnel is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with NKOpcTunnel.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#pragma once
#include "stream/interfaces.h"
#include "com/interfaces.h"

namespace NkCom
{
	class CModule;
	class CComNearSrv : public CServer
	{
	public:
		CComNearSrv() = delete;
		CComNearSrv(NkStream::CStreamIODevice* pProvider
			, CModule *pModule
			, bool auto_delete_provider = true);
		CComNearSrv(const CComNearSrv&) = delete;
		CComNearSrv& operator=(const CComNearSrv&) = delete;

		virtual ~CComNearSrv();

		//IUnknown
		STDMETHOD_(ULONG, AddRef)();
		STDMETHOD_(ULONG, Release)();
		STDMETHOD(QueryInterface) (REFGUID iid, void **pp);

		//CServer
		virtual REFCLSID clsid();
		virtual void clsid(REFCLSID clsid);
		ULONG32 register_instance(CComFarUnknown* p);
		void unregister_instance(ULONG32 id);
		CComFarUnknown* get_instance(ULONG32 id);
		HRESULT remote_logon(const BYTE credentials[], size_t cb);
		HRESULT remote_create_instance(REFCLSID rclsid, DWORD dwClsContext
			, REFIID riid, ULONG32& id);
		HRESULT remote_create_class_factory(REFCLSID rclsid, DWORD dwClsContext
			, REFIID factory_iid, REFIID riid, LPVOID *ppv) throw();
		HRESULT remote_create_instance(REFCLSID rclsid, DWORD dwClsContext
			, REFIID proxy_iid, REFIID riid, LPVOID *ppv) throw();
		HRESULT remote_query_interface(REFIID riid, ULONG32 id);
		void remote_delete_instance(ULONG32 id);
		virtual void shutdown();

		virtual ULONG32 server_id();
		virtual void server_id(ULONG32 id);

		IGlobalInterfaceTable* get_git();
		NkStream::CStream& begin_fct_call(ULONG32 index, ULONG32 id);
		void end_fct_call();
		virtual HRESULT handle_error(NkError::CException& e);
		virtual void store_cp_advise(IConnectionPoint* p_cp, DWORD cookie) {};
		virtual void store_cp_unadvise(IConnectionPoint* p_cp, DWORD cookie) {};

	private:
		class CImpl;
		CImpl* _Impl;
	};
}