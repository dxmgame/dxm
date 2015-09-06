# 
# 计算模块MD5
# 
macro(dxm_module_add_default_md5 MODULE)
	SET(PROJECT_DIR ${DXM_MODULE_${MODULE}_DIRECTORY})
	if(EXISTS ${PROJECT_DIR}/inc)
		dxm_file_md5dir(${PROJECT_DIR}/inc DXM_MODULE_${MODULE}_MD5)
	endif()
	
	if(EXISTS ${PROJECT_DIR}/src)
		dxm_file_md5dir(${PROJECT_DIR}/src DXM_MODULE_${MODULE}_MD5)
	endif()		
	
	if(EXISTS ${PROJECT_DIR}/src.win32/cpp)
		dxm_file_md5dir(${PROJECT_DIR}/src.win32/cpp DXM_MODULE_${MODULE}_MD5)
	endif()		
	
	if(EXISTS ${PROJECT_DIR}/src.ios/cpp)
		dxm_file_md5dir(${PROJECT_DIR}/src.ios/cpp DXM_MODULE_${MODULE}_MD5)
	endif()		
	
	if(EXISTS ${PROJECT_DIR}/src.android/cpp)
		dxm_file_md5dir(${PROJECT_DIR}/src.android/cpp DXM_MODULE_${MODULE}_MD5)
	endif()			
	
	if(EXISTS ${PROJECT_DIR}/proto)
		dxm_file_md5dir(${PROJECT_DIR}/proto DXM_MODULE_${MODULE}_MD5)
	endif()
	
	if(EXISTS ${PROJECT_DIR}/tbl)
		dxm_file_md5dir(${PROJECT_DIR}/tbl DXM_MODULE_${MODULE}_MD5)
	endif()

	if(EXISTS ${PROJECT_DIR}/patch)
		dxm_file_md5dir(${PROJECT_DIR}/patch DXM_MODULE_${MODULE}_MD5)
	endif()
endmacro()

# 
# 计算工程下载文件夹MD5
# 
macro(dxm_module_add_directory_md5 MODULE DIRECTORY)

	if(IS_ABSOLUTE ${DIRECTORY})
		dxm_file_md5dir(${DIRECTORY} DXM_MODULE_${MODULE}_MD5)
	elseif(EXISTS ${DXM_MODULE_${MODULE}_DIRECTORY}/${DIRECTORY})
		dxm_file_md5dir(${DXM_MODULE_${MODULE}_DIRECTORY}/${DIRECTORY} DXM_MODULE_${MODULE}_MD5)
	endif()
endmacro()

macro(dxm_module_add_file_md5 MODULE FILE_PATH)
	if(IS_ABSOLUTE ${FILE_PATH})
		dxm_file_md5file(${FILE_PATH} DXM_MODULE_${MODULE}_MD5)
	elseif(EXISTS ${DXM_MODULE_${MODULE}_DIRECTORY}/${FILE_PATH})
		dxm_file_md5file(${DXM_MODULE_${MODULE}_DIRECTORY}/${FILE_PATH} DXM_MODULE_${MODULE}_MD5)
	endif()
endmacro()
# 
# 加载安装MD5
# 
macro(_dxm_module_load_setup_md5 MODULE)
	set(DXM_MODULE_${MODULE}_COMPILE_MD5 null)
	if(EXISTS ${DXM_MODULE_${MODULE}_DIRECTORY}/prebuilt/md5/${DXM_CMAKE_COMPILER}_${MODULE}_compile_md5.txt) 
		file(READ ${DXM_MODULE_${MODULE}_DIRECTORY}/prebuilt/md5/${DXM_CMAKE_COMPILER}_${MODULE}_compile_md5.txt DXM_MODULE_${MODULE}_COMPILE_MD5)
	endif()
	set(DXM_MODULE_${MODULE}_INSTALL_MD5 null)
	if(EXISTS ${DXM_CMAKE_INSTALL_DIRECTORY}/md5/${DXM_CMAKE_COMPILER}_${MODULE}_install_md5.txt) 
		file(READ ${DXM_CMAKE_INSTALL_DIRECTORY}/md5/${DXM_CMAKE_COMPILER}_${MODULE}_install_md5.txt DXM_MODULE_${MODULE}_INSTALL_MD5)
	endif()	
endmacro()

# 
# 保存安装MD5
# 
macro(_dxm_module_gen_save_compile_md5_code MODULE RETURN_CODE_VARIABLE)
	SET( ${RETURN_CODE_VARIABLE} 
		"file(WRITE ${DXM_MODULE_${MODULE}_DIRECTORY}/prebuilt/md5/${DXM_CMAKE_COMPILER}_${MODULE}_compile_md5.txt ${DXM_MODULE_${MODULE}_MD5})"
		)
endmacro()

macro(_dxm_module_gen_save_install_md5_code MODULE RETURN_CODE_VARIABLE)
	SET( ${RETURN_CODE_VARIABLE} 
		"file(WRITE ${DXM_CMAKE_INSTALL_DIRECTORY}/md5/${DXM_CMAKE_COMPILER}_${MODULE}_install_md5.txt ${DXM_MODULE_${MODULE}_MD5})"
		)
endmacro()

# 
# 清理安装MD5
# 
macro(_dxm_module_clear_setup_md5 MODULE)
	file(WRITE ${DXM_MODULE_${MODULE}_DIRECTORY}/prebuilt/md5/${DXM_CMAKE_COMPILER}_${MODULE}_compile_md5.txt null)
endmacro()
# 
# 检查是否改变
# 
macro(_dxm_module_try_set_dirty MODULE)

	_dxm_module_load_setup_md5(${MODULE})

	if(${DXM_MODULE_${MODULE}_MD5} MATCHES ${DXM_MODULE_${MODULE}_COMPILE_MD5})
		SET(DXM_MODULE_${MODULE}_DIRTY FALSE)
	else()
		SET(DXM_MODULE_${MODULE}_DIRTY TRUE)
	endif()

	if(NOT DXM_MODULE_${MODULE}_DIRTY)
		if(${DXM_MODULE_${MODULE}_MD5} MATCHES ${DXM_MODULE_${MODULE}_INSTALL_MD5})
			SET(DXM_MODULE_${MODULE}_DIRTY FALSE)
		else()
			SET(DXM_MODULE_${MODULE}_DIRTY TRUE)
		endif()
	endif()
	
	# MESSAGE(STATUS "_dxm_module_try_set_dirty ${MODULE} {${DXM_MODULE_${MODULE}_MD5}} {${DXM_MODULE_${MODULE}_COMPILE_MD5}} {${DXM_MODULE_${MODULE}_INSTALL_MD5}} ${DXM_MODULE_${MODULE}_DIRTY}")
endmacro()
