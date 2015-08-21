# 
# 
# 
MACRO(_dxm_builder_module_add_definitions)

	dxm_current_module_definitions_common()

	IF(DXM_MODULE_${DXM_BUILDER_MODULE}_TYPE STREQUAL "SHARED")
		dxm_current_module_definitions_shared()
	ENDIF()

	IF(DXM_MODULE_${DXM_BUILDER_MODULE}_TYPE STREQUAL "SHARED" OR DXM_MODULE_${DXM_BUILDER_MODULE}_TYPE STREQUAL "STATIC")
		dxm_current_module_definitions_lib()
	ENDIF()

	# 包含头文件
	ADD_DEFINITIONS(${DXM_MODULE_${DXM_BUILDER_MODULE}_DEFINITIONS})
ENDMACRO()
# 
# 
# 
MACRO(dxm_current_module_definitions_common)
		# 平台宏定义
	IF(DXM_CMAKE_PLATFORM_IOS)		
		dxm_module_add_definitions(${DXM_BUILDER_MODULE} -DTARGET_OS_IPHONE )
		dxm_module_add_definitions(${DXM_BUILDER_MODULE} -DCC_TARGET_OS_IPHONE )
	ELSEIF(DXM_CMAKE_PLATFORM_WIN32 OR DXM_CMAKE_PLATFORM_WIN64)
		dxm_module_add_definitions(${DXM_BUILDER_MODULE} -we4715 )
		dxm_module_add_definitions(${DXM_BUILDER_MODULE} -Zm200 )
	ENDIF()
ENDMACRO()
# 
# 
# 
MACRO(dxm_current_module_definitions_lib)
	dxm_module_add_definitions(${DXM_BUILDER_MODULE} -D${DXM_MODULE_${DXM_BUILDER_MODULE}_UPPER}_EXPORT )
ENDMACRO()
# 
# 
# 
MACRO(dxm_current_module_definitions_shared)
	dxm_module_add_definitions(${DXM_BUILDER_MODULE} -D${DXM_MODULE_${DXM_BUILDER_MODULE}_UPPER}_AS_DLL )
ENDMACRO()
