
set(DXM_CONFIG_TEMPLATE prebuilt)
SET(DXM_CONFIG_MODULE prebuilt) 


# 
# 初始化工程, 配置基本变量;
# 
MACRO(dxm_config_module_init MODULE)
	
	#
	# 1. 基本属性配置;
	#
	SET(DXM_MODULE_${MODULE}_ANDROID_NAME ${MODULE})
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
	dxm_module_add_directory_md5(${MODULE} prebuilt/inc)

	#
	# 3. 导入模块;
	#
	# dxm_module_link_library(${MODULE} lib FALSE)

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
	FOREACH(TEMP_LIBRARY_NAME ${LIBRARY_LIST})
		IF(${TEMP_LIBRARY_NAME} STREQUAL pthread)
			IF(WIN32)
				dxm_find_module(${MODULE} COMPONENTS pthreadVC2 HEADERS pthread/pthread.h)
			ELSEIF(UNIX)
				dxm_find_add_libraries(${MODULE} pthread)
			ENDIF()

		ELSEIF(${TEMP_LIBRARY_NAME} STREQUAL vld)
			IF(DXM_TARGET_WIN32 OR DXM_TARGET_WIN64)
				dxm_find_module(${MODULE} HEADERS vld/vld.h)
				dxm_find_add_definitions(${MODULE} -DUSE_VLD )		
			ENDIF()
		ELSEIF(${TEMP_LIBRARY_NAME} STREQUAL dl)
			IF(UNIX)
				dxm_find_add_libraries(${MODULE} dl)
			ENDIF()
		ELSEIF(${TEMP_LIBRARY_NAME} STREQUAL socket)
			IF(WIN32)
				dxm_find_add_libraries(${MODULE} ws2_32 wsock32)
			ENDIF()
		ELSEIF(${TEMP_LIBRARY_NAME} STREQUAL xml2)
			IF(UNIX)
				dxm_find_add_includes(${MODULE}/usr/include/libxml2)
				dxm_find_add_libraries(${MODULE} xml2)
			ENDIF()
		ELSEIF(${TEMP_LIBRARY_NAME} STREQUAL z)
			IF(UNIX)
				dxm_find_add_libraries(${MODULE} z)
			ENDIF()
		ELSEIF(${TEMP_LIBRARY_NAME} STREQUAL inet)
			IF(WIN32)
				dxm_find_add_libraries(${MODULE} wininet)
			ENDIF()
		ELSE()
			MESSAGE(FATAL "Load ${TEMP_LIBRARY_NAME} is not match pthread dl socket!")
		ENDIF()
	ENDFOREACH()

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



