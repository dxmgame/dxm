#include "precompiled.h"
#include "util_endian.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

void CEndianHelper::FlipEndian( void* data, int size )
{
	char swapByte;
	for(int byteIndex=0; byteIndex<size/2; byteIndex++)
	{
		swapByte = *(char*)((dxm_intptr)data + byteIndex);
		*(char*)((dxm_intptr)data + byteIndex) = *(char*)((dxm_intptr)data + size - byteIndex - 1);
		*(char*)((dxm_intptr)data + size - byteIndex - 1) = swapByte;
	}
}

float CEndianHelper::FlipFloat( float p )
{
	union { float asfloat; unsigned char asbytes[4]; } u1, u2;
	u1.asfloat = p;
	/* flip! */
	u2.asbytes[0] = u1.asbytes[3];
	u2.asbytes[1] = u1.asbytes[2];
	u2.asbytes[2] = u1.asbytes[1];
	u2.asbytes[3] = u1.asbytes[0];

	return u2.asfloat;
}

void CEndianHelper::FlipFloat( float* p )
{
	union { float asfloat; unsigned char asbytes[4]; } u1, u2;
	u1.asfloat = *p;
	/* flip! */
	u2.asbytes[0] = u1.asbytes[3];
	u2.asbytes[1] = u1.asbytes[2];
	u2.asbytes[2] = u1.asbytes[1];
	u2.asbytes[3] = u1.asbytes[0];
	*p = u2.asfloat;
}

double CEndianHelper::FlipDouble( double p )
{
	union { double asfloat; unsigned char asbytes[8]; } u1, u2;
	u1.asfloat = p;
	/* flip! */
	u2.asbytes[0] = u1.asbytes[7];
	u2.asbytes[1] = u1.asbytes[6];
	u2.asbytes[2] = u1.asbytes[5];
	u2.asbytes[3] = u1.asbytes[4];
	u2.asbytes[4] = u1.asbytes[3];
	u2.asbytes[5] = u1.asbytes[2];
	u2.asbytes[6] = u1.asbytes[1];
	u2.asbytes[7] = u1.asbytes[0];

	return u2.asfloat;
}

void CEndianHelper::FlipDouble( double* p )
{
	union { double asfloat; unsigned char asbytes[8]; } u1, u2;
	u1.asfloat = *p;
	/* flip! */
	u2.asbytes[0] = u1.asbytes[7];
	u2.asbytes[1] = u1.asbytes[6];
	u2.asbytes[2] = u1.asbytes[5];
	u2.asbytes[3] = u1.asbytes[4];
	u2.asbytes[4] = u1.asbytes[3];
	u2.asbytes[5] = u1.asbytes[2];
	u2.asbytes[6] = u1.asbytes[1];
	u2.asbytes[7] = u1.asbytes[0];
	*p = u2.asfloat;
}

NS_UTIL_END
NS_DXM_END