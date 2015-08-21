#ifndef __DXM_UTIL_STRING_CHS_H___
#define __DXM_UTIL_STRING_CHS_H___

#include <string>
#include <list>

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

namespace string 
{
	class DXM_UTIL_DECL chs 
	{
	public:
		static std::string ws2s(const std::wstring& ws);
		static std::wstring s2ws(const std::string& s);

		static dxm_boolean find(const std::string& str, const std::list<std::string>& word_list);
		static std::string replace(const std::string& str, const std::list<std::string>& word_list, const std::string& replace_word);
	};
}

NS_UTIL_END
NS_DXM_END

#endif