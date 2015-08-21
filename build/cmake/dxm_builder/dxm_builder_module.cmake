
# 
# - DXM_BUILDER_MODULE: 当前模块
# 
INCLUDE(dxm_module)
INCLUDE(dxm_builder_module_includes)
INCLUDE(dxm_builder_module_definitions)
INCLUDE(dxm_builder_module_libraries)
INCLUDE(dxm_builder_module_sources)
INCLUDE(dxm_builder_module_precompiled)
INCLUDE(dxm_builder_module_install)
include(dxm_builder_module_setup)
include(dxm_builder_module_link)
INCLUDE(dxm_builder_module_create_compile_succeeded_cmake)
INCLUDE(dxm_builder_module_create_install_succeeded_cmake)

# 
# 创建当前模块;
# 
MACRO(_dxm_builder_module_create MODULE MODULE_DIRECTORY)	
	IF(NOT DXM_MODULE_${MODULE})		
		_dxm_module_create(${MODULE} ${MODULE_DIRECTORY})
		_dxm_module_link_inherit(${MODULE})
		_dxm_module_gen_link_list(${MODULE})		
	ENDIF()
	SET(DXM_BUILDER_MODULE ${MODULE})
	MESSAGE(STATUS "")
ENDMACRO()
# 
# 
# 
MACRO(_dxm_builder_module_pre_build)

	# 执行配置接口;
	INCLUDE(${DXM_MODULE_${DXM_BUILDER_MODULE}_CONFIG_PATH})
	dxm_config_module_pre_build()	
	if(EXISTS ${DXM_CMAKE_TEMPLATE_DIRECTORY}/${DXM_CONFIG_TEMPLATE}/template.cmake)
		INCLUDE(${DXM_CMAKE_TEMPLATE_DIRECTORY}/${DXM_CONFIG_TEMPLATE}/template.cmake)
		dxm_config_template_pre_build()
	endif()

	_dxm_builder_module_setup_dirty_link() # 递归设脏并对脏模块安装;
	_dxm_builder_module_link_all() # find并添加宏定义，库及头文件
	
	_dxm_builder_module_add_sources() # 导入源码
	_dxm_builder_module_add_includes() # 导入头文件目录
	_dxm_builder_module_add_definitions() # 导入宏定义

	_dxm_builder_module_create_compile_succeeded_cmake() 
	_dxm_builder_module_create_install_succeeded_cmake()
ENDMACRO()
# 
# 构建;
# 
MACRO(_dxm_builder_module_build)

	IF( NOT DXM_MODULE_${DXM_BUILDER_MODULE}_PREBUILT )
		IF( ${DXM_MODULE_${DXM_BUILDER_MODULE}_TYPE} STREQUAL APPLICATION)
			IF(DXM_CMAKE_PLATFORM_IOS)
				# 窗口程序为MACOSX_BUNDLE
				ADD_EXECUTABLE(${DXM_BUILDER_MODULE} MACOSX_BUNDLE ${DXM_MODULE_${DXM_BUILDER_MODULE}_SOURCES})
				# SET_MODULE_PROPERTIES( ${DXM_BUILDER_MODULE} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${DXM_PROJECT_TEMPLATES_DIRECTORY}/app_window/in/IosInfo.plist.in)
			ELSE()				
				ADD_EXECUTABLE(${DXM_BUILDER_MODULE} ${DXM_MODULE_${DXM_BUILDER_MODULE}_SOURCES})
			ENDIF()
			_dxm_builder_module_exe_copy_to_prebuilt()
		ELSEIF( ${DXM_MODULE_${DXM_BUILDER_MODULE}_TYPE} STREQUAL SHARED )
			ADD_LIBRARY(${DXM_BUILDER_MODULE} ${DXM_MODULE_${DXM_BUILDER_MODULE}_TYPE} ${DXM_MODULE_${DXM_BUILDER_MODULE}_SOURCES})
			_dxm_builder_module_lib_copy_to_prebuilt()
		ELSEIF( ${DXM_MODULE_${DXM_BUILDER_MODULE}_TYPE} STREQUAL STATIC )
			ADD_LIBRARY(${DXM_BUILDER_MODULE} ${DXM_MODULE_${DXM_BUILDER_MODULE}_TYPE} ${DXM_MODULE_${DXM_BUILDER_MODULE}_SOURCES})
			_dxm_builder_module_lib_copy_to_prebuilt()
		ENDIF()
	ENDIF()

	_dxm_builder_module_copy_to_install()
ENDMACRO()

# 
# 构建后;
# 
MACRO(_dxm_builder_module_post_build)

	# 执行配置接口;
	INCLUDE(${DXM_MODULE_${DXM_BUILDER_MODULE}_CONFIG_PATH})
	dxm_config_module_post_build()
	if(EXISTS ${DXM_CMAKE_TEMPLATE_DIRECTORY}/${DXM_CONFIG_TEMPLATE}/template.cmake)
		INCLUDE(${DXM_CMAKE_TEMPLATE_DIRECTORY}/${DXM_CONFIG_TEMPLATE}/template.cmake)
		dxm_config_template_post_build()
	endif()

	# 导入库;
	_dxm_builder_module_link_libraries()

	# 是否采用预编译头
	IF(DXM_MODULE_${DXM_BUILDER_MODULE}_PRECOMPILED)
		_dxm_builder_module_use_precompiled()
	ENDIF()
ENDMACRO()
