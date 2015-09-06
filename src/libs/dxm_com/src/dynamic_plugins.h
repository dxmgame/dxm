#ifndef __DYNAMIC_PLUGINS_H___
#define __DYNAMIC_PLUGINS_H___


#include <vector>
#include <string>

#include "dynamic_lib.h"

NS_DXM_BEGIN
class  CDynamicPlugins
{
public:
	CDynamicPlugins(void);
	~CDynamicPlugins(void);

	bool LoadPlugin(const std::string & plugin_name);

private:
	bool _LoadPlugin(const std::string& plugin_name, CDynamicLib::Ptr lib_ptr);
};
NS_DXM_END

#endif // DYNAMIC_PLUGINS_H
