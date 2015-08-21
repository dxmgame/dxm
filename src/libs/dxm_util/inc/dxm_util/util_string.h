#ifndef __DXM_UTIL_STRING_H___
#define __DXM_UTIL_STRING_H___

#include "util_string_chs.h"
#include "util_string_utf8.h"

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

namespace string
{
	class DXM_UTIL_DECL common
	{
	public:
		static std::string s2utfs(const std::string& str);
		static std::string utfs2s(const std::string& str);
	};
}

NS_UTIL_END
NS_DXM_END

#endif