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

namespace NkStream
{
	class CStreamIODevice
	{
	public:
		virtual ~CStreamIODevice() {};
		virtual size_t read(void* p, size_t cb, ULONG32 id = 0) = 0;
		virtual size_t write(const void* p, size_t cb, ULONG32 id = 0) = 0;
		virtual void close(ULONG32 id = 0) throw() = 0;
		virtual bool is_closed() const throw() = 0;
		//virtual bool can_read(ULONG32 id = 0) = 0;
		//virtual void cancel_wait(ULONG32 id = 0) = 0;
	};

	class CStreamIODeviceEvent
	{
	public:
		virtual bool on_receive(ULONG32 stream_id) = 0;
		virtual bool on_time_out() = 0;
	};
}