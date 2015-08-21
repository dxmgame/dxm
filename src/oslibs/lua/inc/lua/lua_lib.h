#ifndef LUA_LIB_H
#define LUA_LIB_H

#include "dxm/platform.h"

#ifdef LUA_AS_DLL
	#if  defined ( LUA_EXPORT )
	#define LUA_DECL XE_DLL_EXPORT
	#else
	#define LUA_DECL XE_DLL_IMPORT
	#endif // LUA_EXPORT
#else
	#define LUA_DECL
#endif

#endif // LUA_LIB_H
