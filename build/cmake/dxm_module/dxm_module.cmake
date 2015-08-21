
include(dxm_module_definitions)
include(dxm_module_includes)
include(dxm_module_libraries)
include(dxm_module_link)
include(dxm_module_md5)
include(dxm_module_setup)

# 
# 新建Module
# 
# _dxm_cmake_create_module
MACRO( _dxm_module_create MODULE MODULE_DIRECTORY )

	# 初始化;
	_dxm_module_init(${MODULE} ${MODULE_DIRECTORY} ${ARGN})

	# 修正类型;
	_dxm_module_fix_type(${MODULE})
ENDMACRO()

# 
# 模块初始化;
# 
MACRO( _dxm_module_init MODULE MODULE_DIRECTORY )

	string(TOUPPER ${MODULE} DXM_MODULE_${MODULE}_UPPER)
	SET(DXM_MODULE_${MODULE}_TYPE STATIC) # STATIC SHARED APPLICATION
	SET(DXM_MODULE_${MODULE}_PRECOMPILED false)
	SET(DXM_MODULE_${MODULE}_APP_NAME 游戏)
	SET(DXM_MODULE_${MODULE}_VERSION_CODE 1)
	SET(DXM_MODULE_${MODULE}_VERSION_STRING 1.0.1)
	SET(DXM_MODULE_${MODULE}_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
	SET(DXM_MODULE_${MODULE}_IPHONEOS_DEPLOYMENT_TARGET 5.0)
	SET(DXM_MODULE_${MODULE}_ANDROID_NAME android_name)
	SET(DXM_MODULE_${MODULE} ${MODULE})
	SET(DXM_MODULE_${MODULE}_DIRECTORY ${MODULE_DIRECTORY})
	SET(DXM_MODULE_${MODULE}_CONFIG_PATH ${DXM_MODULE_${MODULE}_DIRECTORY}/config.cmake)


	# 该模块所包含的库; 注册时引入;
	SET(LIBRARY_LIST ${ARGN})
	FOREACH(LIBRARY ${LIBRARY_LIST})		
		SET(DXM_MODULE_${LIBRARY}_MODULE ${MODULE})		
		LIST(APPEND DXM_MODULE_${MODULE}_LIBRARY_LIST ${LIBRARY})
	ENDFOREACH()

	# 加载配置并执行模块初始化;
	IF(EXISTS ${DXM_MODULE_${MODULE}_CONFIG_PATH})
		INCLUDE(${DXM_MODULE_${MODULE}_CONFIG_PATH})
		dxm_config_module_init(${MODULE}) # 初始化各种变量;
		_dxm_module_try_set_dirty(${MODULE})
	ENDIF()

ENDMACRO()

# 
# 在调用config.cmake的初始化之后调用;
# 
MACRO( _dxm_module_fix_type MODULE )

	# ANDROID与IOS特殊做;
	IF(DXM_CMAKE_PLATFORM_ANDROID)
		IF(DXM_MODULE_${MODULE}_ANDROID_TYPE)
			SET(DXM_MODULE_${MODULE}_TYPE ${DXM_MODULE_${MODULE}_ANDROID_TYPE})
		ELSE()
			SET(DXM_MODULE_${MODULE}_TYPE STATIC)
		ENDIF()
	ELSEIF(DXM_CMAKE_PLATFORM_IOS)
		IF(DXM_MODULE_${MODULE}_IOS_TYPE)
			SET(DXM_MODULE_${MODULE}_TYPE ${DXM_MODULE_${MODULE}_IOS_TYPE})
		ELSE()
			SET(DXM_MODULE_${MODULE}_TYPE STATIC)
		ENDIF()
	ENDIF()
ENDMACRO()
