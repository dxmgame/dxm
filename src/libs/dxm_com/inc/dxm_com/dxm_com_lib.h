#ifndef DXM_COM_LIB_H
#define DXM_COM_LIB_H

#include "dxm/platform.h"

#ifdef DXM_COM_AS_DLL
	#if  defined ( DXM_COM_EXPORT )
	#define DXM_COM_DECL XE_DLL_EXPORT
	#else
	#define DXM_COM_DECL XE_DLL_IMPORT
	#endif // DXM_COM_EXPORT
#else
	#define DXM_COM_DECL
#endif

#endif // DXM_COM_LIB_H
