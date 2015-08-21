#ifndef TOLUA_LIB_H
#define TOLUA_LIB_H

#include "dxm/platform.h"

#ifdef TOLUA_AS_DLL
	#if  defined ( TOLUA_EXPORT )
	#define TOLUA_DECL XE_DLL_EXPORT
	#else
	#define TOLUA_DECL XE_DLL_IMPORT
	#endif // TOLUA_EXPORT
#else
	#define TOLUA_DECL
#endif

#endif // TOLUA_LIB_H
