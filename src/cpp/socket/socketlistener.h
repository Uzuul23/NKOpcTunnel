#pragma once
#include "socket/socket.h"
#include "error/socketexception.h"
#include "threading/workerthread.h"

namespace NkSocket
{
	template <typename T>
	class CSocketListener
	{
	public:
		CSocketListener();
		~CSocketListener();

		typedef void (T::*accept_function)(SOCKET so);

		void shutdown();
		void listen(accept_function fct, T *object, const char* local_address);

	private:
		T* m_object;
		accept_function m_function;
		void listen_worker(CSocketListener<T>*);
		CSocket m_socket;
		NkThreading::CWorkerThread<CSocketListener<T>, CSocketListener<T>> m_worker;
	};

	template <typename T>
	CSocketListener<T>::CSocketListener()
	{
	}

	template <typename T>
	CSocketListener<T>::~CSocketListener()
	{
		shutdown();
	}

	template <typename T>
	void CSocketListener<T>::shutdown()
	{	
		m_socket.close();
		m_worker.wait_for_if();
	}

	template <typename T>
	void CSocketListener<T>::listen(accept_function fct, T *object, const char* local_address)
	{
		shutdown();

		m_object = object;
		m_function = fct;

		m_socket.socket();
		m_socket.bind(local_address);
		m_socket.listen();

		NkTrace::CTrace::Instance().Trace(NkTrace::CTrace::TraceAlwaysInfo
			, "CSocketListener -- start listening on : %s"
			, local_address);

		m_worker.start_async(&CSocketListener::listen_worker, this, this);
	}

	template <typename T>
	void CSocketListener<T>::listen_worker(CSocketListener<T>* p)
	{		
		try
		{
			while (true)
			{
				SOCKET so = ::accept(m_socket, 0, 0);	
				if (!m_socket.issocket())
				{
					NkTrace::CTrace::Instance().Trace(NkTrace::CTrace::TraceAlwaysInfo
						, "CSocketListener -- stop listening");
					return;
				}
				NkError::CSocketException::check(so, "accept", __FILE__, __LINE__);
				
				(m_object->*m_function)(so);
			}
		}
		catch (NkError::CException& e)
		{
			e.report();
			return;
		}
	}
}