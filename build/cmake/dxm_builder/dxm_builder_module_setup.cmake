# - DXM_CMAKE_DIRTY_MODULE_LIST


MACRO(_dxm_builder_module_setup_dirty_link)
	
	# MESSAGE(STATUS ">>>>>>>>>>> _dxm_builder_module_setup_dirty_link ${DXM_MODULE_${DXM_BUILDER_MODULE}_IMPORT_MODULE_LIST}")
	FOREACH(TEMP_MODULE_XX ${DXM_MODULE_${DXM_BUILDER_MODULE}_IMPORT_MODULE_LIST})		
		_dxm_builder_module_check_link_dirty(${TEMP_MODULE_XX})
	ENDFOREACH()

	FOREACH(TEMP_MODULE ${DXM_CMAKE_CACHE_DIRTY_MODULE_LIST})
		MESSAGE(STATUS ">>> [${DXM_BUILDER_MODULE}] setup dirty module: ${TEMP_MODULE}")
	ENDFOREACH()

	FOREACH(TEMP_MODULE ${DXM_CMAKE_CACHE_DIRTY_MODULE_LIST})
		dxm_module_setup(${TEMP_MODULE})
	ENDFOREACH()
ENDMACRO()

# 
# 检查模块是否脏
# 
MACRO(_dxm_builder_module_check_link_dirty MODULE)

	SET(CHECK_${MODULE}_RETURN_DIRTY FALSE)

	if(DXM_MODULE_${MODULE}_DIRTY)		
		SET(CHECK_${MODULE}_RETURN_DIRTY TRUE)
	endif()

	FOREACH(TEMP_MODULE_YY ${DXM_MODULE_${MODULE}_IMPORT_MODULE_LIST})
		_dxm_builder_module_check_link_dirty_recursive(${MODULE} ${TEMP_MODULE_YY})		
	ENDFOREACH()

	if(CHECK_${MODULE}_RETURN_DIRTY)
		_dxm_builder_module_add_dirty_module(${MODULE})		
	endif()
ENDMACRO()

# 
# 递归检查模块
# 
MACRO(_dxm_builder_module_check_link_dirty_recursive MODULE MODULE_IMPORT)

	SET(CHECK_${MODULE_IMPORT}_RETURN_DIRTY FALSE)

	if(DXM_MODULE_${MODULE_IMPORT}_DIRTY)
		SET(CHECK_${MODULE_IMPORT}_RETURN_DIRTY TRUE)
	endif()

	FOREACH(DCMMCISMD_TEMP_MODULE ${DXM_MODULE_${MODULE_IMPORT}_IMPORT_MODULE_LIST})
		MESSAGE("_dxm_builder_module_check_link_dirty_recursive ${TEMP_MODULE}")
		_dxm_builder_module_check_link_dirty_recursive(${MODULE_IMPORT} ${TEMP_MODULE})
	ENDFOREACH()

	if(CHECK_${MODULE_IMPORT}_RETURN_DIRTY)
		_dxm_builder_module_add_dirty_module(${MODULE_IMPORT})
		SET(CHECK_${MODULE}_RETURN_DIRTY TRUE)
	endif()

ENDMACRO()

# 
# 添加脏的模块;
# 
MACRO(_dxm_builder_module_add_dirty_module MODULE)

	SET(CHECK_MODULE_UNIQUE TRUE)
	FOREACH(DCMADM_TEMP_MODULE ${DXM_CMAKE_CACHE_DIRTY_MODULE_LIST})
		IF(${DCMADM_TEMP_MODULE} STREQUAL ${MODULE})
			SET(CHECK_MODULE_UNIQUE FALSE)
			BREAK()
		ENDIF()	
	ENDFOREACH()

	IF(CHECK_MODULE_UNIQUE)
		LIST(APPEND DXM_CMAKE_CACHE_DIRTY_MODULE_LIST ${MODULE})
		_dxm_module_clear_setup_md5(${MODULE})
	ENDIF()
ENDMACRO()
