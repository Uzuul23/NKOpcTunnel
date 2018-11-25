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