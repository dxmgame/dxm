

#ifndef __DXM_PLATFORM_TYPE_H__
#define __DXM_PLATFORM_TYPE_H__

#include <stdint.h>

typedef bool				dxm_boolean;

typedef signed char			dxm_int8;
typedef unsigned char		dxm_uint8;
typedef short				dxm_int16;
typedef unsigned short		dxm_uint16;
typedef int					dxm_int32;
typedef unsigned int		dxm_uint32;
typedef long long			dxm_int64;
typedef unsigned long long	dxm_uint64;

typedef char				dxm_char;
typedef wchar_t				dxm_wchar;
typedef float				dxm_float32;
typedef double				dxm_float64;
typedef dxm_uint8			dxm_byte;


typedef unsigned char		dxm_uchar;
typedef unsigned short		dxm_ushort;
typedef unsigned int		dxm_uint;
typedef unsigned long		dxm_ulong;

typedef intptr_t			dxm_intptr;

struct dxm_point{
	int x;
	int y;
};


#endif  // __DXM_PLATFORM_TYPE_H__