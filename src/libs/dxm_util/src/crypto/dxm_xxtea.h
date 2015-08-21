/***********************************************************************
 
 Copyright 2006-2009 Ma Bingyao
 Copyright 2013 Gao Chunhui, Liu Tao
 
 These sources is free software. Redistributions of source code must
 retain the above copyright notice. Redistributions in binary form
 must reproduce the above copyright notice. You can redistribute it
 freely. You can use it with any free or commercial software.
 
 These sources is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY. Without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 
 github: https://github.com/liut/pecl-xxtea
 
 *************************************************************************/

#ifndef __DXM_XXTEA_H__
#define __DXM_XXTEA_H__

#include "dxm/platform.h"

#define DXM_XXTEA_MX (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[p & 3 ^ e] ^ z)
#define DXM_XXTEA_DELTA 0x9e3779b9

unsigned char *dxm_xxtea_encrypt( const unsigned char *data, dxm_uint32 data_len, const unsigned char *key, dxm_uint32 key_len, dxm_uint32 *ret_length);
unsigned char *dxm_xxtea_decrypt( const unsigned char *data, dxm_uint32 data_len, const unsigned char *key, dxm_uint32 key_len, dxm_uint32 *ret_length);

#endif
