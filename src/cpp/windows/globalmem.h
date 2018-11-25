#pragma once
#include "error/winapiexception.h"

namespace NkWin
{
	//TODO: optimize!  reuse memory
	class CGlobalMem
	{
	public:
		CGlobalMem() : _hGlobal(0)
			, _const(false)
			, _locks(0)
			, _size(0)
			, _flags(0)
		{
		}
		CGlobalMem(HGLOBAL h, bool is_const = true) : _hGlobal(h)
			, _const(is_const)
			, _locks(0)
			, _size(0)
			, _flags(0)
		{
		}
		~CGlobalMem()
		{
			clear();
		}
		template<typename _lock_type>
		void lock(_lock_type** pp)
		{
			_lock_type* p = static_cast<_lock_type*>(::GlobalLock(_hGlobal));
			NkError::CWinApiException::check_ptr(p, "GlobalLock", __FILE__, __LINE__);
			_locks++;
			(*pp) = p;
		}
		void unlock()
		{
			::GlobalUnlock(_hGlobal);
			_locks--;
		}
		operator HGLOBAL()
		{
			return _hGlobal;
		}
		void clear()
		{
			while (_hGlobal && _locks > 0){
				::GlobalUnlock(_hGlobal);
				_locks--;
			}

			if (_hGlobal && !_const){
				::GlobalFree(_hGlobal);
			}

			_hGlobal = 0;
			_const = false;
			_size = 0;
		}
		void alloc(size_t cb, UINT uFlags = GMEM_MOVEABLE)
		{
			if (!_const && (_hGlobal != 0) && (_flags == uFlags) && (cb <= _size)){
				return;
			}

			clear();
			_hGlobal = ::GlobalAlloc(uFlags, cb);
			NkError::CWinApiException::check_ptr(_hGlobal, "GlobalLock", __FILE__, __LINE__);

			_size = cb;
			_flags = uFlags;
			_const = false;
		}
		void attach(HGLOBAL h)
		{
			clear();
			_hGlobal = h;
		}

	private:
		CGlobalMem &operator =(const CGlobalMem&);
		CGlobalMem(const CGlobalMem&);

		HGLOBAL _hGlobal;
		bool _const;
		size_t _locks;
		size_t _size;
		UINT _flags;
	};
}