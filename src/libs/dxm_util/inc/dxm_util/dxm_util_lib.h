#ifndef DXM_UTIL_LIB_H
#define DXM_UTIL_LIB_H

#include "dxm/platform.h"

#ifdef DXM_UTIL_AS_DLL
	#if  defined ( DXM_UTIL_EXPORT )
	#define DXM_UTIL_DECL DXM_DLL_EXPORT
	#else
	#define DXM_UTIL_DECL DXM_DLL_IMPORT
	#endif // DXM_UTIL_EXPORT
#else
	#define DXM_UTIL_DECL
#endif

#define NS_UTIL_BEGIN                     namespace util {
#define NS_UTIL_END                       }

#endif // DXM_UTIL_LIB_H
