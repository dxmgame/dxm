#ifndef UTIL_RANDOM_H
#define UTIL_RANDOM_H

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

/// \brief �����;
/// 
/// ��ϸ˵��;
class DXM_UTIL_DECL CRandom
{
public:
	CRandom(void);
	~CRandom(void);

	/// \brief ��ʼ��;
	void Init(dxm_uint32 seed);

	/// \brief �������;
	dxm_int32 RandomInt32(dxm_int32 min, dxm_int32 max);

	/// \brief ���������;
	dxm_float32 RandomFloat32(dxm_float32 min, dxm_float32 max);

private:
	dxm_uint32 seed_;
};

NS_UTIL_END
NS_DXM_END

#endif