#ifndef LIB_LIB_H
#define LIB_LIB_H

#include "dxm/platform.h"

#ifdef LIB_AS_DLL
	#if  defined ( LIB_EXPORT )
	#define LIB_DECL DXM_DLL_EXPORT
	#else
	#define LIB_DECL DXM_DLL_IMPORT
	#endif // LIB_EXPORT
#else
	#define LIB_DECL
#endif

#endif // LIB_LIB_H
