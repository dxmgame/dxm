
# 
# 设置版本的兼容性以避免不必要的警告
# 
if( COMMAND CMAKE_POLICY )
	CMAKE_POLICY( SET CMP0003 NEW )
	CMAKE_POLICY( SET CMP0006 OLD )
	CMAKE_POLICY( SET CMP0011 OLD )
	CMAKE_POLICY( SET CMP0053 OLD )
	CMAKE_POLICY( SET CMP0054 OLD )
endif()

# 
# dxm.cmake
# DxM的cmake主脚本，提供给框架使用者调用
# 
# 
# 配置CMake变量
# 
SET( DXM_CMAKE_CORE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
SET( DXM_CMAKE_TOOL_DIRECTORY  ${DXM_CMAKE_ROOT_DIRECTORY}/build/tools )

# 
# 加载CMake源码目录函数：提供init.cmake初始化对应目录下的内容; 这样做这个文件就干净多了
# 
MACRO(_dxm_add_cmake_directory cmake_directory)	
	LIST(APPEND CMAKE_MODULE_PATH ${DXM_CMAKE_CORE_DIRECTORY}/${cmake_directory})
	IF(EXISTS ${DXM_CMAKE_CORE_DIRECTORY}/${cmake_directory}/init.cmake)
		INCLUDE(${DXM_CMAKE_CORE_DIRECTORY}/${cmake_directory}/init.cmake)
	ENDIF()
ENDMACRO()

#
# 添加全局include
#
MACRO(dxm_cmake_add_includes)
	LIST(APPEND DXM_CMAKE_INCLUDES ${ARGN})
ENDMACRO()
# 
# 加载CMake源码目录
# 
_dxm_add_cmake_directory(dxm_module)
_dxm_add_cmake_directory(dxm_register)
_dxm_add_cmake_directory(dxm_builder)
_dxm_add_cmake_directory(dxm_find)
_dxm_add_cmake_directory(util)
_dxm_add_cmake_directory(find)
_dxm_add_cmake_directory(launcher)
_dxm_add_cmake_directory(toolchain)
# 
# 初始化环境;
# 
INCLUDE(${DXM_CMAKE_CORE_DIRECTORY}/dxm_environment.cmake)


