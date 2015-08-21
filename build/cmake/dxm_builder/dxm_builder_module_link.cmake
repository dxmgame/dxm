
# 
# DxM框架用的 dxm_builder_module_link_all
# 
macro(_dxm_builder_module_link_all)
	FOREACH(DCMAIMIAL_TEMP_MODULE ${DXM_MODULE_${DXM_BUILDER_MODULE}_IMPORT_MODULE_LIST})
		_dxm_builder_module_link_includes_and_libraries(${DCMAIMIAL_TEMP_MODULE} ${DXM_MODULE_${DXM_BUILDER_MODULE}_${DCMAIMIAL_TEMP_MODULE}_IMPORT_LIBRARY_LIST})
	ENDFOREACH()
endmacro()
# 
# DxM框架用的 _dxm_builder_module_link_includes_and_libraries
# 
macro(_dxm_builder_module_link_includes_and_libraries MODULE)
	
	# 查看是否存在模块;
	IF(NOT DXM_MODULE_${MODULE})
		MESSAGE(FATAL_ERROR "${MODULE} Not Found")
		RETURN()
	ENDIF()

	# 库列表; 缺省参数没有值则表示模块名就是库名;
	SET(LIBRARY_LIST ${ARGN})
	LIST(LENGTH LIBRARY_LIST LEN)
	IF(${LEN} EQUAL 0)
		LIST(APPEND LIBRARY_LIST ${MODULE})
	ENDIF()	

	# 查找库;
	INCLUDE(${DXM_MODULE_${MODULE}_CONFIG_PATH})
	dxm_config_find_module(${MODULE} ${LIBRARY_LIST})

	# 查找到了就可以添加头文件目录和库路径;	
	# 导入库XXX_AS_DLL宏
	string(TOUPPER ${MODULE} MODULE_UPPER)
	IF(${DXM_MODULE_${MODULE}_TYPE} STREQUAL "SHARED")
		dxm_module_add_definitions( ${DXM_BUILDER_MODULE} -D${DXM_MODULE_${MODULE}_UPPER}_AS_DLL )
	ENDIF()
	# 添加头文件路径与库
	dxm_module_add_definitions(${DXM_BUILDER_MODULE} ${DXM_FIND_${MODULE}_DEFINITIONS})
	dxm_module_add_includes(${DXM_BUILDER_MODULE} ${DXM_FIND_${MODULE}_INCLUDE_DIR}) 
	dxm_module_add_libraries(${DXM_BUILDER_MODULE} ${DXM_FIND_${MODULE}_LIBRARIES})

	MESSAGE(STATUS ">>> [${DXM_BUILDER_MODULE}] import MODULE(${MODULE}) includes: ${DXM_FIND_${MODULE}_INCLUDE_DIR}")
	MESSAGE(STATUS ">>> [${DXM_BUILDER_MODULE}] import MODULE(${MODULE}) libraries: ${DXM_FIND_${MODULE}_LIBRARIES}")

endmacro()
