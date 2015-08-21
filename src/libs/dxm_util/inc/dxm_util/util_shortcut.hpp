#ifndef UTIL_SHORTCUT_H
#define UTIL_SHORTCUT_H

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

/// \brief ��ݲ���;
/// 
class DXM_UTIL_DECL CShortCut
{
public:

	// ����ֵ��������Χ��;
	template<class T>
	static T FixInRange(T& v, T min, T max) {

		if(v < min) {
			v = min;
		}
		else if(v > max) {
			v = max;
		}
		return v;
	}

	// �ж���ֵ�Ƿ��ڷ�Χ��;
	template<class T>
	static dxm_boolean CheckInRange(T v, T min, T max) {

		if(v < min) {
			return false;
		}
		else if(v > max) {
			return false;
		}
		return true;
	}
};

NS_UTIL_END
NS_DXM_END

#define DXM_SHORTCUT_FIX_IN_RANGE(v, min, max) dxm::util::CShortCut::FixInRange(v, min, max)
#define DXM_SHORTCUT_CHECK_IN_RANGE(v, min, max) dxm::util::CShortCut::CheckInRange(v, min, max)

#endif