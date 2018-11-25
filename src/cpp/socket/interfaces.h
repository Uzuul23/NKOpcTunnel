#pragma once
#include "stream/interfaces.h"

namespace NkSocket
{
	/*class CService
	{
	public:
	CService(){};
	virtual ~CService(){};
	virtual const GUID& GetServiceId() = 0;
	virtual void OnOpen(NkStream::CStreamProvider*) = 0;
	};*/

	class CSocket;
	class CServiceServer
	{
	public:
		CServiceServer(){};
		virtual ~CServiceServer(){};
		virtual void NewProvider(const CSocket&, NkStream::CStreamIODevice**) = 0;
		virtual void DeleteProvider(NkStream::CStreamIODevice*) = 0;
	};
}