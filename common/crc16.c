//-----------------------------------------------------------------------------
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// CRC16
//-----------------------------------------------------------------------------

#include "crc16.h"


unsigned short update_crc16( unsigned short crc, unsigned char c )
{
	unsigned short i, v, tcrc = 0;

	v = (crc ^ c) & 0xff;
	for (i = 0; i < 8; i++) {
		tcrc = ( (tcrc ^ v) & 1 ) ? ( tcrc >> 1 ) ^ 0x8408 : tcrc >> 1;
		v >>= 1;
	}

	return ((crc >> 8) ^ tcrc)&0xffff;
}

uint16_t crc16(uint8_t const *message, int length, uint16_t remainder, uint16_t polynomial) {

	if (length == 0) return (~remainder);

	for (int byte = 0; byte < length; ++byte) {
		remainder ^= (message[byte] << 8);
		for (uint8_t bit = 8; bit > 0; --bit) {
			if (remainder & 0x8000) {
				remainder = (remainder << 1) ^ polynomial;
			} else {
				remainder = (remainder << 1);
			}
		}
	}
	return remainder;
}

uint16_t crc16_ccitt(uint8_t const *message, int length) {
	return crc16(message, length, 0xffff, 0x1021);
}

uint16_t crc16_ccitt_kermit(uint8_t const *message, int length) {
	if (length == 0) return 0;
	uint32_t crc = 0, q = 0;
	uint8_t c;

	for (int i = 0; i < length; i++){
		c = message[i];
		q = (crc ^ c ) & 0xF;
		crc = (crc >> 4) ^ (q * 0x1081);
		q = (crc ^ (c >> 4)) & 0xF;
		crc = (crc >> 4) ^ (q * 0x1081);
	}
	return crc;
}
