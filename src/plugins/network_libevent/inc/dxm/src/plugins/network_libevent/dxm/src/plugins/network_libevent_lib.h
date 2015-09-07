#ifndef _LIB_H
#define _LIB_H

#include "dxm/platform.h"

#ifdef _AS_DLL
	#if  defined ( _EXPORT )
	#define _DECL DXM_DLL_EXPORT
	#else
	#define _DECL DXM_DLL_IMPORT
	#endif // _EXPORT
#else
	#define _DECL
#endif

#endif // _LIB_H
