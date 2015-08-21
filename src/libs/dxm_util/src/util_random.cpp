#include "precompiled.h"
#include "util_random.h"

#include "util_timer.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

CRandom::CRandom( void )
{
	seed_ = dxm::util::CTimer::GetMilliSecond();
}

CRandom::~CRandom( void )
{
	
}

void CRandom::Init( dxm_uint32 seed )
{
	if(!seed) {
		seed_ = dxm::util::CTimer::GetMilliSecond();
	}
	else {
		seed_ = seed;
	}
}

dxm_int32 CRandom::RandomInt32( dxm_int32 min, dxm_int32 max )
{
	if(min > max) {
		dxm_int32 nTemp = max;
		min = nTemp;
		max = min;
	}
	seed_ = 214013*seed_ + 2531011;

	dxm_int32 result = min + (seed_ ^ seed_>>15)%(max-min+1);
	return result;
}

dxm_float32 CRandom::RandomFloat32( dxm_float32 min, dxm_float32 max )
{
	if(min > max) {
		dxm_float32 fTemp = max;
		min = fTemp;
		max = min;
	}
	seed_ = 214013*seed_ + 2531011;

	dxm_float32 result = min + ((seed_ ^ seed_>>15) & 0x0000FFFF)*(1.0f/65535.0f)*(max-min);
	return result;
}

NS_UTIL_END
NS_DXM_END
