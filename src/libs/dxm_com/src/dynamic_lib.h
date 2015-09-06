#ifndef __DYNAMIC_LIB_H__
#define __DYNAMIC_LIB_H__

#include "dxm/platform.h"
#include <memory>

#if (DXM_TARGET_PLATFORM == DXM_PLATFORM_WIN32)

#include <windows.h>
#    define DYNAMIC_LIB_HANDLE			hInstance
#    define DYNAMIC_LIB_LOAD( a )		LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNAMIC_LIB_GETSYM( a, b )	GetProcAddress( a, b )
#    define DYNAMIC_LIB_UNLOAD( a )		!FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#else
#ifdef __cplusplus
extern "C" {
#endif
	#include <dlfcn.h>
#ifdef __cplusplus
}
#endif 

#    define DYNAMIC_LIB_HANDLE			void*
#    define DYNAMIC_LIB_LOAD( a )		dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define DYNAMIC_LIB_GETSYM( a, b )	dlsym( a, b )
#    define DYNAMIC_LIB_UNLOAD( a )		dlclose( a )

#endif

#include <string>
using namespace std;

NS_DXM_BEGIN
class  CDynamicLib
{
	enum Status
	{
		ST_START = 0,
		ST_LOADED,
		ST_UNLOADED,
		ST_TOTAL
	};

public:
	typedef std::shared_ptr<CDynamicLib> Ptr;

	CDynamicLib(const string& name);
	CDynamicLib(const CDynamicLib& dl);
	CDynamicLib& operator=(const CDynamicLib& dl);
	~CDynamicLib();

public:
	bool Load();
	bool Unload();	 
	inline const std::string & name(void) const	{ return name_; }
	void* GetSymbol(const string& name) const throw();
	string GetLastError(void);

private:
	string					name_;
	Status					load_status_;
	DYNAMIC_LIB_HANDLE		dyn_lib_instance_; /// Handle to the loaded library.
};
NS_DXM_END
#endif // DYNAMIC_LIB_H
