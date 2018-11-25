#pragma once
#include "types/types.h"
#include "types/String.h"

namespace NkWin
{
	class CLocalMachine
	{
	public:
		CLocalMachine();
		virtual ~CLocalMachine();

		void get_physical_drive_serial_number(UINT drive_number, NkType::CString& serial
			, bool& removable_media);

		static void local_machine_dependent_key(BYTE key[32]);

	private:
		CLocalMachine(const CLocalMachine&);
		CLocalMachine& operator =(const CLocalMachine&);
		class CImpl;
		CImpl* _Impl;
	};
}
