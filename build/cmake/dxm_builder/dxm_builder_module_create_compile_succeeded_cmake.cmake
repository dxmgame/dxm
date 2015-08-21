# 
# 编译成功的MD5
# 
macro(_dxm_builder_module_create_compile_succeeded_cmake)	
	_dxm_module_gen_save_compile_md5_code(${DXM_BUILDER_MODULE} DXM_TEMP_CODE)
	file(WRITE ${CMAKE_BINARY_DIR}/dxm_cmake_compile_succeeded.cmake ${DXM_TEMP_CODE})
endmacro()

