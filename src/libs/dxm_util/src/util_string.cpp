#include "precompiled.h"
#include "util_string.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

std::string string::common::s2utfs( const std::string& str )
{
	return uft8::ws2s(chs::s2ws(str));
}

std::string string::common::utfs2s( const std::string& str )
{
	return chs::ws2s(uft8::s2ws(str));
}

NS_UTIL_END
NS_DXM_END
