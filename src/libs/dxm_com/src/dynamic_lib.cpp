#include "precompiled.h"
#include "dynamic_lib.h"
#include <stdio.h>
#include "dxm_util/util_log.h"

NS_DXM_BEGIN
//-----------------------------------------------------------------------
CDynamicLib::CDynamicLib(const string&  name) 
	:  dyn_lib_instance_(NULL), load_status_(CDynamicLib::ST_UNLOADED)
{
	name_ = name;
}

CDynamicLib::CDynamicLib(const CDynamicLib& _dl) 
    : dyn_lib_instance_(_dl.dyn_lib_instance_), load_status_(_dl.load_status_)
{
	name_ = _dl.name_;
}

CDynamicLib& CDynamicLib::operator=(const CDynamicLib& _dl)
{
	name_ = _dl.name_;
	dyn_lib_instance_ = _dl.dyn_lib_instance_;
	load_status_ = _dl.load_status_;
	return (*this);
}

CDynamicLib::~CDynamicLib()
{
	Unload();
}

bool CDynamicLib::Load()
{
	if (load_status_ == CDynamicLib::ST_UNLOADED)
	{
		sLogDebug("[component:info ] CDynamicLib::load dll(%s)", name_.c_str());

		std::string name = name_;
#if (DXM_TARGET_PLATFORM == DXM_PLATFORM_WIN32)
		if (name.substr(name.length() - 4, 4) != ".dll")
// #ifdef _DEBUG
// 			name += "d.dll";
// #else
			name += ".dll";
// #endif // _DEBUG

		dyn_lib_instance_ = (DYNAMIC_LIB_HANDLE)DYNAMIC_LIB_LOAD( name.c_str() );
#else
		if (name.substr(name.length() - 3, 3) != ".so")
			name += ".so";
		if (name.substr(0, 3) != "lib")
			name = std::string("lib") + name;

		dyn_lib_instance_ = (DYNAMIC_LIB_HANDLE)DYNAMIC_LIB_LOAD( name.c_str() );
#endif

		if( !dyn_lib_instance_ )
		{
			sLogDebug("[component:error] CDynamicLib::load dll(%s) failed [ none instance:error(%s) ]", name.c_str(), GetLastError().c_str());

			return false;
		}

		load_status_ = CDynamicLib::ST_LOADED;
		return true;
	}
	sLogDebug("[component:info ] CDynamicLib::load is loaded", name_.c_str());
	return true;
}

//-----------------------------------------------------------------------
bool CDynamicLib::Unload()
{
	if (load_status_ == CDynamicLib::ST_LOADED)
	{
		if ( DYNAMIC_LIB_UNLOAD( dyn_lib_instance_ ) )	
		{
			sLogDebug("[component:error] CDynamicLib::unload dll(%s) failed [ call dynamic_lib_UNLOAD failed ]", name_.c_str());
			return false;
		}

		sLogDebug("[component:info ] CDynamicLib::unload dll(%s) ok", name_.c_str());
		load_status_ = CDynamicLib::ST_UNLOADED;
		return true;
	}

	sLogDebug("[component:error] CDynamicLib::unload dll(%s) failed [ cant find the dll ]", name_.c_str());
	return false;
}

//-----------------------------------------------------------------------
void* CDynamicLib::GetSymbol(const string& name) const throw()
{
	return (void*)DYNAMIC_LIB_GETSYM( dyn_lib_instance_, name.c_str() );
}

//-----------------------------------------------------------------------
string CDynamicLib::GetLastError(void)
{
#if (DXM_TARGET_PLATFORM == DXM_PLATFORM_WIN32)
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		::GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
		);
	std::string ret = (char*)lpMsgBuf;
	// Free the buffer.
	LocalFree( lpMsgBuf );
	return ret;
#else
	return std::string(dlerror());
#endif
}
NS_DXM_END