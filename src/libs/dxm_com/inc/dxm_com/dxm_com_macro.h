#ifndef __DXM_COM_MACRO_H__
#define __DXM_COM_MACRO_H__

#include "dxm_com_lib.h"
#include <typeinfo>

// ###################################################
/*

PLUGIN_EXPORT(plugin_name){
	PLUGIN_EXPORT_COMPONENT(component_name, class_component)
	PLUGIN_EXPORT_COMPONENT(component_name, class_component)
	PLUGIN_EXPORT_COMPONENT(component_name, class_component)
}
*/

// ----------------------------
#define  _COMPONENT_HEADER_BEGIN						\
protected:												\
	virtual void* QueryInterface( InterfaceID const& interfaceID )	\
	{													\
		__COMPONENT_HEADER_INHERIT(IComponent)			\

// ----------------------------						
#define  __COMPONENT_HEADER_INHERIT(interface)			\
	if( strcmp(interfaceID.name(), typeid(interface).name()) == 0 )				\
	{													\
		return static_cast<interface*>(this);			\
	}

// ----------------------------
#define  _COMPONENT_HEADER_END return NULL;}	\
public:

// ----------------------------
#define _COMPONENT_HEADER_INHERIT1(interface1)									\
	_COMPONENT_HEADER_BEGIN														\
		__COMPONENT_HEADER_INHERIT(interface1)									\
	_COMPONENT_HEADER_END

#define _COMPONENT_HEADER_INHERIT2(interface1, interface2)						\
	_COMPONENT_HEADER_BEGIN														\
		__COMPONENT_HEADER_INHERIT(interface1)									\
		__COMPONENT_HEADER_INHERIT(interface2)									\
	_COMPONENT_HEADER_END

#define _COMPONENT_HEADER_INHERIT3(interface1, interface2, interface3)			\
	_COMPONENT_HEADER_BEGIN														\
		__COMPONENT_HEADER_INHERIT(interface1)									\
		__COMPONENT_HEADER_INHERIT(interface2)									\
		__COMPONENT_HEADER_INHERIT(interface3)									\
	_COMPONENT_HEADER_END

#define _COMPONENT_HEADER_INHERIT4(interface1, interface2, interface3, interface4)\
	_COMPONENT_HEADER_BEGIN														\
		__COMPONENT_HEADER_INHERIT(interface1)									\
		__COMPONENT_HEADER_INHERIT(interface2)									\
		__COMPONENT_HEADER_INHERIT(interface3)									\
		__COMPONENT_HEADER_INHERIT(interface4)									\
	_COMPONENT_HEADER_END

#define _COMPONENT_HEADER_INHERIT5(interface1, interface2, interface3, interface4, interface5)\
	_COMPONENT_HEADER_BEGIN															\
		__COMPONENT_HEADER_INHERIT(interface1)										\
		__COMPONENT_HEADER_INHERIT(interface2)										\
		__COMPONENT_HEADER_INHERIT(interface3)										\
		__COMPONENT_HEADER_INHERIT(interface4)										\
		__COMPONENT_HEADER_INHERIT(interface5)										\
	_COMPONENT_HEADER_END

/*
 *	声明后支持QueryInterface的转换，否则转换出为空的;
 */
#define COMPONENT_RTTI1(interface1) _COMPONENT_HEADER_INHERIT1(interface1)
#define COMPONENT_RTTI2(interface1, interface2) _COMPONENT_HEADER_INHERIT2(interface1, interface2)
#define COMPONENT_RTTI3(interface1, interface2, interface3) _COMPONENT_HEADER_INHERIT3(interface1, interface2, interface3)
#define COMPONENT_RTTI4(interface1, interface2, interface3, interface4) _COMPONENT_HEADER_INHERIT4(interface1, interface2, interface3, interface4)
#define COMPONENT_RTTI5(interface1, interface2, interface3, interface4, interface5) _COMPONENT_HEADER_INHERIT5(interface1, interface2, interface3, interface4, interface5)
// ###################################################

// ----------------------------------------------
#define REGISTER_COMPONENT(component_name,class_component)  sComponentService->AddFactory<class_component>( component_name );
#define REGISTER_COMPONENT_WITHOUT_NAME(class_component)  sComponentService->AddFactory<class_component>( component_name );
#define UNREGISTER_COMPONENT(component_name) sComponentService->Remove(component_name);
#define UNREGISTER_COMPONENT_WITHOUT_NAME(class_component) sComponentService->Remove<class_component>();


// ###################################################

#ifdef DXM_COM_AS_DLL // dynamic plug-in export

#if (DXM_TARGET_PLATFORM == DXM_PLATFORM_WIN32)
#define _PLUGIN_DLL_DECL _declspec ( dllexport )
#else
#define _PLUGIN_DLL_DECL
#endif // DXM_PLATFORM_WIN32

#define PLUGIN_EXPORT(plugin_name)		\
extern "C" {								\
	_PLUGIN_DLL_DECL void  DllPlugin(boost::shared_ptr<CDynamicLib>&);\
};											\
extern "C" void  DllPlugin(boost::shared_ptr<CDynamicLib>& dyn_lib_ptr)

#define PLUGIN_EXPORT_COMPONENT(component_name,class_component)	 sComponentService->AddFactory<class_component>( component_name, dyn_lib_ptr );

#else // static plug-in export

#define _PLUGIN_EXPORT_CLASS_NAME(plugin_name)   PluginExport##plugin_name

#define _PLUGIN_EXPORT_CLASS_INSTANCE_NAME(plugin_name)   PluginExportInstance##plugin_name

#define PLUGIN_EXPORT(plugin_name)                        \
class _PLUGIN_EXPORT_CLASS_NAME(plugin_name)                      \
{                                                   \
public:                                             \
	_PLUGIN_EXPORT_CLASS_NAME(plugin_name)();                     \
} _PLUGIN_EXPORT_CLASS_INSTANCE_NAME(plugin_name);                \
_PLUGIN_EXPORT_CLASS_NAME(plugin_name)::_PLUGIN_EXPORT_CLASS_NAME(plugin_name)()

#define PLUGIN_EXPORT_COMPONENT(component_name,class_component)	 sComponentService->AddFactory<class_component>( component_name );

#endif

#endif //__DXM_COM_MACRO_H__
