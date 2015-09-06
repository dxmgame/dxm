#include "precompiled.h"
#include "dynamic_plugins.h"
#include <fstream>
#include "dynamic_lib.h"
#include "dxm_util/util_log.h"
		
NS_DXM_BEGIN
typedef void (*DLL_START_PLUGIN)(CDynamicLib::Ptr&);

CDynamicPlugins::CDynamicPlugins( void )
{

}

CDynamicPlugins::~CDynamicPlugins( void )
{

}

bool CDynamicPlugins::LoadPlugin( const std::string& plugin_name )
{
#ifdef DXM_COM_AS_DLL
	CDynamicLib::Ptr lib_ptr(new CDynamicLib(plugin_name.c_str()));
	return _LoadPlugin(plugin_name, lib_ptr);
#endif // DXM_COM_AS_DLL

}

bool CDynamicPlugins::_LoadPlugin( const std::string& plugin_name, CDynamicLib::Ptr lib_ptr )
{
	sLogWarn("[component:warn] CComponentPlugins::loadPlugin begin");

	if( !lib_ptr || !lib_ptr->Load() ) {
		return false;
	}

	// 同时要有以上三种接口的dll才是标准插件，否则不承认;
	DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)lib_ptr->GetSymbol("DllPlugin");
	if ( !pFunc ) {
		sLogWarn("[component:warn] CComponentPlugins::loadPlugin cant find entry(DllPlugin)");
		return false;
	}

	sLogInfo("[component:info ] CComponentPlugins::loadPlugin load dll(%s)'s plugins", plugin_name.c_str());
	(*pFunc)(lib_ptr);

	sLogInfo("[component:info ] CComponentPlugins::loadPlugin load dll(%s)'s plugins finished", plugin_name.c_str());
	return true;
}
NS_DXM_END