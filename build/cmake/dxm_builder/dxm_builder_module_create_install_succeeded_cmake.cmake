# 
# 编译成功的MD5
# 
macro(_dxm_builder_module_create_install_succeeded_cmake)	
	_dxm_module_gen_save_install_md5_code(${DXM_BUILDER_MODULE} DXM_TEMP_CODE)
	file(WRITE ${CMAKE_BINARY_DIR}/dxm_cmake_install_succeeded.cmake ${DXM_TEMP_CODE})
endmacro()