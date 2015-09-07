
# 加载核心cmake文件
# 添加9层目录，只为加载dxm_cmake;
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

# 包含dxm_cmake.project
include(dxm_cmake.project)

# 当前的模块目录;
set(DXM_CMAKE_CURRENT_TEMPLATE_DIRECTORY ${DXM_CMAKE_TEMPLATE_DIRECTORY}/${TEMPLATE_TYPE}/)
SET(DXM_CONFIG_MODULE_BE_CREATED ${MODULE_NAME})

macro(_dxm_template_create_module)

	# 转换路径为unix方式
	string(REPLACE "\\" "/" MODULE_DIRECTORY_BE_CREATED ${DXM_CMAKE_PROJECT_DIRECTORY})
	string(REPLACE "\\" "/" MODULE_RELATIVE_PATH ${DXM_CONFIG_MODULE_BE_CREATED})

	# 模块目录;
	set(MODULE_DIRECTORY_BE_CREATED ${MODULE_DIRECTORY_BE_CREATED}/${MODULE_RELATIVE_PATH})

	# 拆解路径并创建
	get_filename_component( MODULE_BE_CREATED ${MODULE_DIRECTORY_BE_CREATED} NAME)

	# 魔板中使用了PROJECT_NAME与PROJECT_NAME_UPPER
	SET(PROJECT_NAME ${MODULE_BE_CREATED})
	STRING(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

	# 添加模块文件包含;
	if(EXISTS ${DXM_CMAKE_CURRENT_TEMPLATE_DIRECTORY}/template.cmake)
		include(${DXM_CMAKE_CURRENT_TEMPLATE_DIRECTORY}/template.cmake)
	else()
		message(FATAL_ERROR "please check TEMPLATE_TYPE(${TEMPLATE_TYPE})?")
		return()
	endif()

	# 创建模板;这个函数在模板的template.cmake
	message(STATUS ">>> [MODULE_DIRECTORY_BE_CREATED] dxm_config_template_init: ${MODULE_BE_CREATED} ${MODULE_DIRECTORY_BE_CREATED}")
	dxm_config_template_init(${MODULE_BE_CREATED} ${MODULE_DIRECTORY_BE_CREATED})
endmacro()

#################################################################
# 
# 创建通用函数
# 
macro( dxm_template_make_sure_directory )
	if(EXISTS ${MODULE_DIRECTORY_BE_CREATED})
		message(FATAL_ERROR "create failed(${MODULE_DIRECTORY_BE_CREATED} is existed)" )
	else()
		file(MAKE_DIRECTORY ${MODULE_DIRECTORY_BE_CREATED})
	endif()
endmacro()

# 
# 拷贝目录
# 
macro( dxm_template_copy_directory )
	file( COPY ${DXM_CMAKE_CURRENT_TEMPLATE_DIRECTORY}	DESTINATION ${MODULE_DIRECTORY_BE_CREATED})
endmacro()

# 
# 创建config
# 
macro( dxm_template_create_config )

	# 模块列表生成
	SET(IGNORE_LIBRARY_NAME_LIST ${ARGN})
	
	SET(IMPORT_MODULE_TEXT "\n")
	FOREACH(MY_LIRARY ${DXM_REGISTER_LIBRARY_LIST})	
		# 判断当前模块是否被忽略注释（默认开启）
		SET(IS_IGNORE_MODULE false)
		FOREACH(IGNORE_MODULE_NAME ${IGNORE_LIBRARY_NAME_LIST})
			IF( ${MY_LIRARY} STREQUAL ${IGNORE_MODULE_NAME} )
				SET(IS_IGNORE_MODULE true)
				BREAK()
			ENDIF()
		ENDFOREACH()
		
		# crash_handler 特殊处理
		IF(${MY_LIRARY} STREQUAL "crash_handler")
			SET(IMPORT_MODULE_TEXT "${IMPORT_MODULE_TEXT}	IF(DXM_TARGET_WIN32 OR DXM_TARGET_WIN64)\n")
		ENDIF()
		
		# 进行开启或者关闭
		IF(IS_IGNORE_MODULE)
			SET(IMPORT_MODULE_TEXT "${IMPORT_MODULE_TEXT}	dxm_module_link_library(\${MODULE} ${MY_LIRARY} false)\n")
		ELSE()
			SET(IMPORT_MODULE_TEXT "${IMPORT_MODULE_TEXT}	# dxm_module_link_library(\${MODULE} ${MY_LIRARY} false)\n")
		ENDIF()
		
		# crash_handler 特殊处理
		IF(${MY_LIRARY} STREQUAL "crash_handler")
			SET(IMPORT_MODULE_TEXT "${IMPORT_MODULE_TEXT}	ENDIF()\n")
		ENDIF()
		
	ENDFOREACH()

	configure_file(
		${DXM_CMAKE_CURRENT_TEMPLATE_DIRECTORY}/config.cmake.in
		${MODULE_DIRECTORY_BE_CREATED}/config.cmake
		@ONLY
		)
endmacro()

# 正式运行
_dxm_template_create_module()