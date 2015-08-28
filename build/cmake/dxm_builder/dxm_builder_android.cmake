# 
# dxm_builder_build_module: CMakeLists.txt中使用
# 
INCLUDE(dxm_module)
INCLUDE(dxm_builder_module)

# 
# 构建
# 
MACRO(dxm_builder_build_module MODULE MODULE_DIR)
	_dxm_builder_pre_build(${MODULE} ${MODULE_DIR})
	_dxm_builder_build()
	_dxm_builder_post_build()
ENDMACRO()

# 
# 准备构建
# 
MACRO(_dxm_builder_pre_build MODULE MODULE_DIR)

	# 创建模块
	_dxm_builder_module_create(${MODULE} ${MODULE_DIR})

	# 构建模块;
	_dxm_builder_module_pre_build()
	
	MESSAGE(STATUS ">>> [${DXM_BUILDER_MODULE}] build modules: ${DXM_MODULE_${DXM_BUILDER_MODULE}_IMPORT_MODULE_LIST}")
	MESSAGE(STATUS ">>> [${DXM_BUILDER_MODULE}] build includes: ${DXM_MODULE_${DXM_BUILDER_MODULE}_INCLUDES}")
	MESSAGE(STATUS ">>> [${DXM_BUILDER_MODULE}] build sources: ${DXM_MODULE_${DXM_BUILDER_MODULE}_SOURCES}")
	MESSAGE(STATUS ">>> [${DXM_BUILDER_MODULE}] build libraries: ${DXM_MODULE_${DXM_BUILDER_MODULE}_LIBRARIES}")
	MESSAGE(STATUS ">>> [${DXM_BUILDER_MODULE}] build definition: ${DXM_MODULE_${DXM_BUILDER_MODULE}_DEFINITIONS}")
ENDMACRO()

# 
# 开始构建
# 
MACRO(_dxm_builder_build)
	
	# 构建;
	_dxm_builder_module_build()

ENDMACRO()
# 
# 完成构建
# 
MACRO(_dxm_builder_post_build)

	# 构建后执行模块操作;
	_dxm_builder_module_post_build()

	MESSAGE(STATUS ">>> [${DXM_BUILDER_MODULE}] done")
	MESSAGE(STATUS "")
ENDMACRO()


