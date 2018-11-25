#pragma once

namespace NkStream
{
	class CStream;
}

namespace NkError
{
	class CBaseException;
}

namespace NkCom
{
	class CServer;

	template <class T>
	class CComPtr
	{
	public:
		CComPtr() : _p(0)
		{
		}
		CComPtr(T* p)
		{
			_p = p;
			if (_p){
				_p->AddRef();
			}
		}
		CComPtr(const CComPtr<T>& d)
		{
			_p = d._p;
			if (_p){
				_p->AddRef();
			}
		}
		~CComPtr()
		{
			if (_p){
				_p->Release();
			}
		}
		void Release()
		{
			if (_p) {
				_p->Release();
				_p = 0;
			}
		}
		operator T*() const
		{
			return _p;
		}
		T& operator*() const
		{
			return *_p;
		}
		T** operator&()
		{
			return &_p;
		}
		T* operator->() const
		{
			NkError::CBaseException::check_pointer(_p, __FILE__, __LINE__);
			return _p;
		}
		T* operator=(T* p)
		{
			if (p != 0){
				p->AddRef();
			}
			if (_p){
				_p->Release();
			}
			_p = p;
			return p;
		}
		CComPtr<T>& operator=(const CComPtr<T>& d)
		{
			if (_p){
				_p->Release();
			}
			_p = d._p;
			if (_p){
				_p->AddRef();
			}
			return *this;
		}
		bool operator!() const throw()
		{
			return (_p == 0);
		}
		template<class InterfaceType>
		HRESULT QueryInterface(InterfaceType** pp)
		{
			NkError::CBaseException::check_pointer(_p, __FILE__, __LINE__);
			return _p->QueryInterface(__uuidof(InterfaceType), reinterpret_cast<void**>(pp));
		}
		void Attach(T* p)
		{
			Release();
			_p = p;
		}
		T* Detach()
		{
			T* p = _p;
			_p = 0;
			return p;
		}
		HRESULT CoCreateInstance(CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext = CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER)
		{
			if (pSrv == 0){
				return E_POINTER;
			}
			return pSrv->remote_create_instance(rclsid, dwClsContext, __uuidof(T), __uuidof(T), (void**)&_p);
		}
		HRESULT CoCreateInstance(LPCWSTR lpszProgID, DWORD dwClsContext = CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, REFIID riid = __uuidof(T))
		{
			CLSID clsid = CLSID_NULL;
			HRESULT hr = CLSIDFromProgID(lpszProgID, &clsid);
			if (FAILED(hr)){
				return hr;
			}
			return ::CoCreateInstance(clsid, 0, dwClsContext, riid, reinterpret_cast<void**>(&_p));
		}
		HRESULT CoCreateInstance(REFCLSID rclsid, DWORD dwClsContext = CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, REFIID riid = __uuidof(T))
		{
			return ::CoCreateInstance(rclsid, 0, dwClsContext, riid, reinterpret_cast<void**>(&_p));
		}

	private:
		T* _p;
	};

	template <class T, class ProxyClass>
	class CComPtrFar
	{
	public:
		CComPtrFar(CServer* pSrv) : _pSrv(pSrv), _p(0)
		{
		}
		CComPtrFar(CServer* pSrv, T* p) : _pSrv(pSrv)
		{
			_p = p;
			if (_p) {
				_p->AddRef();
			}
		}
		CComPtrFar(T* p)
		{
			_p = p;
			if (_p) {
				_p->AddRef();
			}
		}
		CComPtrFar(const CComPtrFar<T, ProxyClass>& d)
		{
			_p = d._p;
			if (_p){
				_p->AddRef();
			}
		}
		~CComPtrFar()
		{
			if (_p) {
				_p->Release();
			}
		}
		void Release()
		{
			if (_p) {
				_p->Release();
				_p = 0;
			}
		}
		operator T*() const
		{
			return _p;
		}
		T& operator*() const
		{
			return *_p;
		}
		T** operator&()
		{
			return &_p;
		}
		T* operator->() const
		{
			return _p;
		}
		void Attach(T* p)
		{
			delete _p;
			_p = p;
		}
		T* Detach()
		{
			T* p = _p;
			_p = 0;
			return p;
		}
		void read(NkStream::CStream& stream)
		{
			NkError::CBaseException::check_pointer(_p, __FILE__, __LINE__);
			ULONG32 id;
			ProxyClass* pNew = new ProxyClass(_pSrv, _p, &id);
			stream.write_type(VT_UNKNOWN);
			stream << id;
		}
	private:
		T* _p;
		CServer* _pSrv;
		CComPtrFar();
	};

	template <class T, class ProxyClass>
	inline NkStream::CStream& operator<<(NkStream::CStream& l, CComPtrFar<T, ProxyClass>& r)
	{
		r.read(l);
		return l;
	}

	template <class T, class ProxyClass>
	class CComPtrNear
	{
	public:
		CComPtrNear(CServer* pSrv) : m_pSrv(pSrv), _p(0)
		{
		}
		CComPtrNear(T* p) : m_pSrv(0)
		{
			_p = p;
			if (_p){
				_p->AddRef();
			}
		}
		CComPtrNear(const CComPtrNear<T, ProxyClass>& d)
		{
			_p = d._p;
			if (_p){
				_p->AddRef();
			}
		}
		~CComPtrNear()
		{
			if (_p) {
				_p->Release();
			}
		}
		void Release()
		{
			if (_p) {
				_p->Release();
				_p = 0;
			}
		}
		operator T*() const
		{
			return _p;
		}
		T& operator*() const
		{
			return *_p;
		}
		T** operator&()
		{
			return &_p;
		}
		T* operator->() const
		{
			return _p;
		}
		T* operator=(T* p)
		{
			if (p != 0)
				p->AddRef();
			if (_p)
				_p->Release();
			_p = p;
			return p;
		}
		void Attach(T* p)
		{
			Release();
			_p = p;
		}
		void Detach(T** pp)
		{
			(*pp) = _p;
			_p = 0;
		}
		T* Detach()
		{
			T* p = _p;
			_p = 0;
			return p;
		}
		void create(REFCLSID rclsid, DWORD dwClsContext = CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER)
		{
			NkError::CBaseException::check_pointer(m_pSrv, __FILE__, __LINE__);
			Release();

			ULONG32 id = 0;
			HRESULT hr = m_pSrv->remote_create_instance(rclsid, dwClsContext, __uuidof(T), id);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			ProxyClass* pNew = new ProxyClass(m_pSrv, id);
			hr = pNew->QueryInterface(__uuidof(T), reinterpret_cast<void**>(&_p));
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
		}
		void write(NkStream::CStream& stream)
		{
			ULONG32 id = 0;
			stream.check_type(VT_UNKNOWN);
			stream >> id;
			ProxyClass* pNew = new ProxyClass(m_pSrv, id);
			HRESULT hr = pNew->QueryInterface(__uuidof(T), reinterpret_cast<void**>(&_p));
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
		}
	private:
		CComPtrNear();
		CServer* m_pSrv;
		T* _p;
	};

	template <class T, class ProxyClass>
	inline NkStream::CStream& operator>>(NkStream::CStream& l, CComPtrNear<T, ProxyClass>& r)
	{
		r.write(l);
		return l;
	}
}