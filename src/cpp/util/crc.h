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

#pragma once;

namespace NkUtil
{
	static unsigned long update_crc(unsigned long crc, const unsigned char *buf, int len)
	{
		static bool crc_table_computed = false;
		static unsigned long crc_table[256] = {0};

		if (!crc_table_computed) {
			unsigned long c;
			int n, k;

			for (n = 0; n < 256; n++) {
				c = (unsigned long) n;
				for (k = 0; k < 8; k++) {
					if (c & 1)
						c = 0xedb88320L ^ (c >> 1);
					else
						c = c >> 1;
				}
				crc_table[n] = c;
			}
			crc_table_computed = true;
		}

		unsigned long c = crc;
		for (int n = 0; n < len; n++) {
			c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
		}
		return c;
	}
	static unsigned long crc(const unsigned char *buf, int len)
	{
		return NkUtil::update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
	}
}