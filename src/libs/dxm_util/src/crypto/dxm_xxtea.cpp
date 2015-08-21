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
#include "precompiled.h"
#include "dxm_xxtea.h"
#include <memory.h>
#include <stdlib.h>

static void dxm_uint32_encrypt(dxm_uint32 *v, dxm_uint32 len, dxm_uint32 *k)
{
    dxm_uint32 n = len - 1;
    dxm_uint32 z = v[n], y = v[0], p, q = 6 + 52 / (n + 1), sum = 0, e;
    if (n < 1) {
        return;
    }
    while (0 < q--) {
        sum += DXM_XXTEA_DELTA;
        e = sum >> 2 & 3;
        for (p = 0; p < n; p++) {
            y = v[p + 1];
            z = v[p] += DXM_XXTEA_MX;
        }
        y = v[0];
        z = v[n] += DXM_XXTEA_MX;
    }
}

static void dxm_uint32_decrypt(dxm_uint32 *v, dxm_uint32 len, dxm_uint32 *k)
{
    dxm_uint32 n = len - 1;
    dxm_uint32 z = v[n], y = v[0], p, q = 6 + 52 / (n + 1), sum = q * DXM_XXTEA_DELTA, e;
    if (n < 1) {
        return;
    }
    while (sum != 0) {
        e = sum >> 2 & 3;
        for (p = n; p > 0; p--) {
            z = v[p - 1];
            y = v[p] -= DXM_XXTEA_MX;
        }
        z = v[n];
        y = v[0] -= DXM_XXTEA_MX;
        sum -= DXM_XXTEA_DELTA;
    }
}

static unsigned char *fix_key_length(const unsigned char *key, dxm_uint32 key_len)
{
    unsigned char *tmp = (unsigned char *)malloc(16);
    memcpy(tmp, key, key_len);
    memset(tmp + key_len, '\0', 16 - key_len);
    return tmp;
}

static dxm_uint32 *xxtea_to_long_array(const unsigned char *data, dxm_uint32 len, int include_length, dxm_uint32 *ret_len) {
    dxm_uint32 i, n, *result;
    
    n = len >> 2;
    n = (((len & 3) == 0) ? n : n + 1);
    if (include_length) {
        result = (dxm_uint32 *)malloc((n + 1) << 2);
        result[n] = len;
        *ret_len = n + 1;
    } else {
        result = (dxm_uint32 *)malloc(n << 2);
        *ret_len = n;
    }
    memset(result, 0, n << 2);
    for (i = 0; i < len; i++) {
        result[i >> 2] |= (dxm_uint32)data[i] << ((i & 3) << 3);
    }
    
    return result;
}

static unsigned char *xxtea_to_byte_array(const dxm_uint32 *data, dxm_uint32 len, int include_length, dxm_uint32 *ret_len) {
    dxm_uint32 i, n, m;
    unsigned char *result;
    
    n = len << 2;
    if (include_length) {
        m = data[len - 1];
        if ((m < n - 7) || (m > n - 4)) return NULL;
        n = m;
    }
    result = (unsigned char *)malloc(n + 1);
    for (i = 0; i < n; i++) {
        result[i] = (unsigned char)((data[i >> 2] >> ((i & 3) << 3)) & 0xff);
    }
    result[n] = '\0';
    *ret_len = n;
    
    return result;
}

static unsigned char *do_xxtea_encrypt(const unsigned char *data, dxm_uint32 len, const unsigned char *key, dxm_uint32 *ret_len) {
    unsigned char *result;
    dxm_uint32 *v, *k, v_len, k_len;
    
    v = xxtea_to_long_array(data, len, 1, &v_len);
    k = xxtea_to_long_array(key, 16, 0, &k_len);
    dxm_uint32_encrypt(v, v_len, k);
    result = xxtea_to_byte_array(v, v_len, 0, ret_len);
    free(v);
    free(k);
    
    return result;
}

static unsigned char *do_xxtea_decrypt(const unsigned char *data, dxm_uint32 len, const unsigned char *key, dxm_uint32 *ret_len) {
    unsigned char *result;
    dxm_uint32 *v, *k, v_len, k_len;
    
    v = xxtea_to_long_array(data, len, 0, &v_len);
    k = xxtea_to_long_array(key, 16, 0, &k_len);
    dxm_uint32_decrypt(v, v_len, k);
    result = xxtea_to_byte_array(v, v_len, 1, ret_len);
    free(v);
    free(k);
    
    return result;
}

unsigned char *dxm_xxtea_encrypt(const unsigned char *data, dxm_uint32 data_len, const unsigned char *key, dxm_uint32 key_len, dxm_uint32 *ret_length)
{
    unsigned char *result;
    
    *ret_length = 0;
    
    if (key_len < 16) {
        unsigned char *key2 = fix_key_length(key, key_len);
        result = do_xxtea_encrypt(data, data_len, key2, ret_length);
        free(key2);
    }
    else
    {
        result = do_xxtea_encrypt(data, data_len, key, ret_length);
    }
    
    return result;
}

unsigned char *dxm_xxtea_decrypt(const unsigned char *data, dxm_uint32 data_len, const unsigned char *key, dxm_uint32 key_len, dxm_uint32 *ret_length)
{
    unsigned char *result;
    
    *ret_length = 0;
    
    if (key_len < 16) {
        unsigned char *key2 = fix_key_length(key, key_len);
        result = do_xxtea_decrypt(data, data_len, key2, ret_length);
        free(key2);
    }
    else
    {
        result = do_xxtea_decrypt(data, data_len, key, ret_length);
    }
    
    return result;
}

/* }}} */
