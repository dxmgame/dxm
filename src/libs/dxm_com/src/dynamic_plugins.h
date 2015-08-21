#ifndef __DYNAMIC_PLUGINS_H___
#define __DYNAMIC_PLUGINS_H___


#include <vector>
#include <string>

#include "dynamic_lib.h"

class  CDynamicPlugins
{
public:
	CDynamicPlugins(void);
	~CDynamicPlugins(void);

	bool LoadPlugin(const std::string & plugin_name);
	bool LoadPlugin(const std::string & plugin_name, const std::string& plugin_path);

private:
	bool _LoadPlugin(const std::string& plugin_name, CDynamicLib::Ptr lib_ptr);
};

#endif // DYNAMIC_PLUGINS_H
