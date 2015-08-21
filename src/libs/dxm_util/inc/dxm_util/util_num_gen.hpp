#ifndef _DXM_UTIL_NUM_GEN_H_
#define _DXM_UTIL_NUM_GEN_H_

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

/// \brief 数字生成器;
/// 
/// 详细说明：指定为各种位数，带不带符号的整型。闭区间[min,max];

template < typename IntType = dxm_int32 >
class CNumGen
{
public:
	CNumGen(IntType seed = 0): seed_(seed){}
	~CNumGen(){}

public:
	IntType operator()(IntType min, IntType max)
	{
		IntType tmp = seed_;
		++tmp;
		if (tmp > max || tmp < min)
		{
			tmp = min;
		}
		seed_ = tmp;
		return tmp;
	}

	DXM_PROPERTY(IntType, seed);
};

NS_UTIL_END
NS_DXM_END

#endif // _DXM_UTIL_NUM_GEN_H_
