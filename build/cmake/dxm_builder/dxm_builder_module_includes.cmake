# 
# 添加头文件
# 
MACRO(_dxm_builder_module_add_includes)
	INCLUDE_DIRECTORIES(${DXM_MODULE_${DXM_BUILDER_MODULE}_INCLUDES})
ENDMACRO()

MACRO(_dxm_builder_module_add_global_includes)
	# 包含全局头文件;
	dxm_module_add_includes( ${DXM_BUILDER_MODULE} ${DXM_CMAKE_INCLUDES} )	
ENDMACRO()




