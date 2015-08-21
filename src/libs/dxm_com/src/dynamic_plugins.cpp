#include "precompiled.h"
#include <fstream>
#include "dynamic_lib.h"

typedef void (*DLL_START_PLUGIN)(CDynamicLib::Ptr&);

CDynamicPlugins::CDynamicPlugins( void )
{

}

CDynamicPlugins::~CDynamicPlugins( void )
{

}

bool CDynamicPlugins::LoadPlugin( const std::string& plugin_name )
{
#ifdef COMPONENT_SERVICE_AS_DLL
	CDynamicLib::Ptr lib_ptr(new CDynamicLib(plugin_name.c_str()));
	return _LoadPlugin(plugin_name, lib_ptr);
#endif // COMPONENT_SERVICE_AS_DLL

}

bool CDynamicPlugins::LoadPlugin( const std::string& plugin_name, const std::string& plugin_path )
{
#ifdef COMPONENT_SERVICE_AS_DLL
	CDynamicLib::Ptr lib_ptr(new CDynamicLib(plugin_name.c_str(), plugin_path.c_str()));
	return _LoadPlugin(plugin_name, lib_ptr);
#endif // COMPONENT_SERVICE_AS_DLL

}

bool CDynamicPlugins::_LoadPlugin( const std::string& plugin_name, CDynamicLib::Ptr lib_ptr )
{
	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_WARN, "[component:warn] CComponentPlugins::loadPlugin begin");

	if( !lib_ptr || !lib_ptr->Load() ) {
		return false;
	}

	// 同时要有以上三种接口的dll才是标准插件，否则不承认;
	DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)lib_ptr->GetSymbol("DllPlugin");
	if ( !pFunc ) {
		XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_WARN, "[component:warn] CComponentPlugins::loadPlugin cant find entry(DllPlugin)");
		return false;
	}

	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentPlugins::loadPlugin load dll(%s)'s plugins", plugin_name.c_str());
	(*pFunc)(lib_ptr);

	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentPlugins::loadPlugin load dll(%s)'s plugins finished", plugin_name.c_str());
	return true;
}

// 
// BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
// {
// 	printf("hModule.%p lpReserved.%p ", hModule, lpReserved);
// 
// 	switch (ul_reason_for_call)
// 	{
// 	case DLL_PROCESS_ATTACH:
// 		printf("Process attach. \n");
// 		break;
// 
// 	case DLL_PROCESS_DETACH:
// 		printf("Process detach. \n");
// 		break;
// 
// 	case DLL_THREAD_ATTACH:
// 		printf("Thread attach. \n");
// 		break;
// 
// 	case DLL_THREAD_DETACH:
// 		printf("Thread detach. \n");
// 		break;
// 	}
// 
// 	return (TRUE);
// }