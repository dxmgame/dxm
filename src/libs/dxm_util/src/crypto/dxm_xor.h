#ifndef __DXM_XOR_H__
#define __DXM_XOR_H__
#include "dxm/platform.h"

unsigned char *dxm_xor_encrypt( const unsigned char *data, dxm_uint32 data_len, const unsigned char *key, dxm_uint32 key_len, dxm_uint32 *ret_length);
unsigned char *dxm_xor_decrypt( const unsigned char *data, dxm_uint32 data_len, const unsigned char *key, dxm_uint32 key_len, dxm_uint32 *ret_length);

void dxm_xor_encrypt_self( unsigned char *data, dxm_uint32 data_len, const unsigned char *key, dxm_uint32 key_len);
void dxm_xor_decrypt_self( unsigned char *data, dxm_uint32 data_len, const unsigned char *key, dxm_uint32 key_len);

#endif /* __DXM_XOR_H__ */
