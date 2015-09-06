
SET(DXM_CONFIG_TEMPLATE prebuit)
SET(DXM_CONFIG_MODULE libevent) 


# 
# 初始化工程, 配置基本变量;
# 
MACRO(dxm_config_module_init MODULE)
	
	#
	# 1. 基本属性配置;
	#
	SET(DXM_MODULE_${MODULE}_ANDROID_NAME "cocos2d_static")
	SET(DXM_MODULE_${MODULE}_APP_NAME ${MODULE}) 

	# 模块类型变量; app(APPLICATION); lib(STATIC, SHARED);
	SET(DXM_MODULE_${MODULE}_TYPE PREBUILT) # default  pc mac
	SET(DXM_MODULE_${MODULE}_ANDROID_TYPE STATIC)
	SET(DXM_MODULE_${MODULE}_IOS_TYPE STATIC) # can only build static library on ios

	# 模块版本;
	SET(DXM_MODULE_${MODULE}_VERSION_CODE "1")
	SET(DXM_MODULE_${MODULE}_VERSION_STRING "1.0.1")

	# IOS设置;
	SET(DXM_MODULE_${MODULE}_IOS_GUI_IDENTIFIER "com.${DXM_CMAKE_COMPANY}.${MODULE}")
	SET(DXM_MODULE_${MODULE}_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
	SET(DXM_MODULE_${MODULE}_IPHONEOS_DEPLOYMENT_TARGET 5.0)

	# 是否使用预编译头
	SET(DXM_MODULE_${MODULE}_PRECOMPILED true)
	SET(DXM_MODULE_${MODULE}_PREBUILT true)

	# 
	# 2. 计算md5;
	# dxm_module_add_default_md5(${MODULE})
	# 	额外计算默认目录的md5，其中包括src,inc,proto,src.android/cpp,src.ios/cpp,src.win/cpp，src.unix/cpp
	# dxm_module_add_directory_md5(${MODULE} patch)
	# 	额外计算工程当前某目录下的MD5为模块MD5
	dxm_module_add_file_md5(${MODULE} libevent-2.0.21-stable.tar.gz)
	dxm_module_add_directory_md5(${MODULE} patch)
	# 
	# 3. 导入模块;
	# 
	# dxm_module_link_library(${MODULE} lib FALSE)
	
	# dxm_module_link_library(${MODULE} lib false)
	# dxm_module_link_library(${MODULE} foo false)
	# dxm_module_link_library(${MODULE} dxm_util false)
	# dxm_module_link_library(${MODULE} dxm_com false)
	# dxm_module_link_library(${MODULE} lua false)
	# dxm_module_link_library(${MODULE} tolua false)
	# dxm_module_link_library(${MODULE} gtest false)
	# dxm_module_link_library(${MODULE} pthread false)
	# dxm_module_link_library(${MODULE} dl false)
	# dxm_module_link_library(${MODULE} socket false)
	# dxm_module_link_library(${MODULE} xml2 false)
	# dxm_module_link_library(${MODULE} z false)
	# dxm_module_link_library(${MODULE} inet false)
	# dxm_module_link_library(${MODULE} vld false)


ENDMACRO()


# 
# 导出模块的library和头文件目录;
# 
MACRO(dxm_config_find_module MODULE)

	#  dxm_find_module(<module>
	#    [PACKAGE <package>]
	#    [COMPONENTS <component...>]
	#    [HEADERS <path>])

	SET(LIBRARY_LIST ${ARGN})
#	FOREACH(TEMP_LIBRARY_NAME ${LIBRARY_LIST})
#		IF(${TEMP_LIBRARY_NAME} STREQUAL pthread)	
#		ELSEIF(${TEMP_LIBRARY_NAME} STREQUAL pthread)
#		ENDIF()
#	ENDFOREACH()

	IF(DXM_CMAKE_PLATFORM_WIN32)
		dxm_find_module(${MODULE} PACKAGE libevent COMPONENTS libevent HEADERS libevent/event2/event.h)
		
		# dxm_find_module(${MODULE} PACKAGE external HEADERS external/json/document.h)	
		# dxm_find_module(${MODULE} PACKAGE extensions HEADERS extensions/cocos-ext.h)	
		# dxm_find_module(${MODULE} PACKAGE GL HEADERS GL/glew.h)	
		# dxm_find_module(${MODULE} PACKAGE chipmunk HEADERS chipmunk/chipmunk.h)	
		# dxm_find_module(${MODULE} PACKAGE freetype2 HEADERS freetype2/freetype.h)	
		# dxm_find_module(${MODULE} PACKAGE glfw3 HEADERS glfw3/glfw3.h)			
		# dxm_find_module(${MODULE} PACKAGE curl HEADERS curl/curl/curl.h)			
	ELSEIF(DXM_CMAKE_PLATFORM_UNIX)	
		dxm_find_module(${MODULE} PACKAGE libevent COMPONENTS event_core event_pthreads HEADERS libevent/event2/event.h)
	ELSEIF(DXM_CMAKE_PLATFORM_ANDORID)
	ELSEIF(DXM_CMAKE_PLATFORM_IOS)
	ELSE()
		MESSAGE(FATAL "Load ${MODULE} is not match!")
	ENDIF()

ENDMACRO()


# 
# 准备构建项目;
# 
MACRO(dxm_config_module_pre_build)

	# 添加target前的事件
	# dxm_module_add_definitions(<module> -DDEFINITION1 -DDEFINITION2)
	# dxm_module_add_includes(<module> <include...>)
	# 
	# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows")
	# 

ENDMACRO()

# 
# 构建项目结束，准备链接;
# 
MACRO(dxm_config_module_post_build)

	# 
	# 添加target后的事件
	# dxm_module_add_libraries(<module> <librariy...>)
	# 

ENDMACRO()
