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
#include "com/interfaces.h"
#include "com/unknownimpl.h"
#include "com/comptr.h"
#include "error/baseexception.h"
//TODO: 
//#include "opc/opccomn.h"

namespace NkCom
{
	class CComFarUnknown : public CUnknownImpl
	{
	public:
		CComFarUnknown(CServer* pSrv, ULONG32* pid = 0);
		CComFarUnknown(CServer* pSrv, IUnknown* p, ULONG32* pid = 0);
		CComFarUnknown(CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid, ULONG32* pid = 0);
		virtual ~CComFarUnknown();

		NK_UNKNOWN_IMP;

		ULONG32 id();

		virtual void call(ULONG32 index, NkStream::CStream& stream) = 0;
		virtual HRESULT query_interface(REFIID riid) = 0;

		template<class T>
		void internal_get_interface(CComPtr<T>& sp);
		HRESULT internal_query_interface(REFIID iid);

		void get_interface_from_global()
		{
			if (m_cookie_git > 0)
			{
				IGlobalInterfaceTable* pGIT = m_pSrv->get_git();
				NkError::CBaseException::check_pointer(pGIT, __FILE__, __LINE__);

				HRESULT hr = pGIT->GetInterfaceFromGlobal(m_cookie_git, IID_IUnknown
					, reinterpret_cast<void**>(&m_spUnknown));
				NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
			}
		}
		void release_interface_from_global()
		{
			if (m_cookie_git > 0)
			{
				m_spUnknown.Release();
			}
		}

	private:
		CComFarUnknown();
		CComFarUnknown(const CComFarUnknown&);
		CComFarUnknown& operator=(const CComFarUnknown&);

		ULONG32 m_id;
		DWORD m_cookie_git;

	protected:

		CComPtr<IUnknown> m_spUnknown;
		CServer* m_pSrv;
	};

	template<class T>
	void CComFarUnknown::internal_get_interface(CComPtr<T>& sp)
	{
		if (m_cookie_git > 0)
		{
			IGlobalInterfaceTable* pGIT = m_pSrv->get_git();
			NkError::CBaseException::check_pointer(pGIT, __FILE__, __LINE__);

			HRESULT hr = pGIT->GetInterfaceFromGlobal(m_cookie_git, __uuidof(T), reinterpret_cast<void**>(&sp));
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
		}
		else
		{
			//TODO: optimize
			HRESULT hr = m_spUnknown.QueryInterface(&sp);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
		}
	}
}