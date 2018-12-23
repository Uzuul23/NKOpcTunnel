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
#include "stream/stream.h"
#include "com/comptr.h"
#include "com/comfarunknown.h"

namespace NkOPC
{
	class COPCServerFar : public NkCom::CComFarUnknown
	{
	public:
		COPCServerFar(NkCom::CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid);
		COPCServerFar(NkCom::CServer* pSrv, IUnknown* p);
		virtual ~COPCServerFar();

		HRESULT query_interface(REFIID iid);

		void call(ULONG32 index, NkStream::CStream& stream);

		// IOPCCommon

		void SetLocaleID(NkStream::CStream& stream);
		void GetLocaleID(NkStream::CStream& stream);
		void QueryAvailableLocaleIDs(NkStream::CStream& stream);
		void GetErrorString(NkStream::CStream& stream);
		void SetClientName(NkStream::CStream& stream);

		//IOPCServer

		void AddGroup(NkStream::CStream& stream);
		void GetErrorString2(NkStream::CStream& stream);
		void GetGroupByName(NkStream::CStream& stream);
		void GetStatus(NkStream::CStream& stream);
		void RemoveGroup(NkStream::CStream& stream);
		void CreateGroupEnumerator(NkStream::CStream& stream);

		//IConnectionPointContainer

		void EnumConnectionPoints(NkStream::CStream& stream);
		void FindConnectionPoint(NkStream::CStream& stream);

		//IOPCBrowse

		void GetProperties(NkStream::CStream& stream);
		void Browse(NkStream::CStream& stream);

		//IOPCItemIO

		void Read(NkStream::CStream& stream);
		void WriteVQT(NkStream::CStream& stream);

		//IOPCItemProperties

		void QueryAvailableProperties(NkStream::CStream& stream);
		void GetItemProperties(NkStream::CStream& stream);
		void LookupItemIDs(NkStream::CStream& stream);

		//IOPCServerPublicGroups

		void GetPublicGroupByName(NkStream::CStream& stream);
		void RemovePublicGroup(NkStream::CStream& stream);

		//IOPCBrowseServerAddressSpace

		void QueryOrganization(NkStream::CStream& stream);
		void ChangeBrowsePosition(NkStream::CStream& stream);
		void BrowseOPCItemIDs(NkStream::CStream& stream);
		void GetItemID(NkStream::CStream& stream);
		void BrowseAccessPaths(NkStream::CStream& stream);

	private:
		COPCServerFar();
		COPCServerFar &operator =(const COPCServerFar&);
		COPCServerFar(const COPCServerFar&);

		class CImpl;
		CImpl *_Impl;
	};
}