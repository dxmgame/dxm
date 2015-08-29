# 
# 模块安装并保存MD5
# 
MACRO(dxm_module_setup MODULE)
	# 脏模块安装
	_dxm_module_try_set_dirty(${MODULE})
	if( DXM_MODULE_${MODULE}_DIRTY )
		_dxm_module_setup_with_platform(${MODULE})
	endif()
ENDMACRO()
# 
# 指定目录安装
# 
function(_dxm_module_setup_with_platform MODULE)	
	# 不同平台下的安装文件
	IF(DXM_CMAKE_PLATFORM_WIN32)
		SET(SETUP_FILE_NAME "setup.win32")
	ELSEIF(DXM_CMAKE_PLATFORM_WIN64)
		SET(SETUP_FILE_NAME "setup.win64")
	ELSEIF(DXM_CMAKE_PLATFORM_UNIX)
		SET(SETUP_FILE_NAME "setup.unix")
	ELSEIF(DXM_CMAKE_PLATFORM_IOS)
		SET(SETUP_FILE_NAME "setup.ios")
	ELSEIF(DXM_CMAKE_PLATFORM_ANDROID_MK)
		SET(SETUP_FILE_NAME "setup.android.mk")
	ELSEIF(DXM_CMAKE_PLATFORM_ANDROID)
		SET(SETUP_FILE_NAME "setup.android")
	ENDIF()
	# 正式运行安装文件
	if( EXISTS ${DXM_MODULE_${MODULE}_DIRECTORY} )
		MESSAGE(STATUS 
		dxm_file_bash( ${SETUP_FILE_NAME} ${DXM_MODULE_${MODULE}_DIRECTORY}/build ))
		dxm_file_bash( ${SETUP_FILE_NAME} ${DXM_MODULE_${MODULE}_DIRECTORY}/build )
	endif()
endfunction()

