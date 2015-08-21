
# 
# dxm_cmake.cmake
# dxm框架用的cmake主脚本
# DXM_CMAKE_ROOT_DIRECTORY
# dxm_cmake.cmake放在最外面的原因是因为这里才能获得 DXM_CMAKE_ROOT_DIRECTORY
IF(NOT DXM_CMAKE_PROJECT_DIRECTORY)
	SET(DXM_CMAKE_PROJECT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
ENDIF()

SET( DXM_CMAKE_ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
SET( DXM_CMAKE_TEMPLATE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/templates )

# 
# 加载DxM框架
# 
INCLUDE(${DXM_CMAKE_ROOT_DIRECTORY}/build/cmake/dxm_cmake_init.cmake)

# 
# 全局变量打印
# 
MACRO( dump_var var)
	MESSAGE(STATUS ">>> [environment] ${var}:${${var}}")
ENDMACRO()

FUNCTION( dump_string var)
	MESSAGE(STATUS ">>> [environment] ${var}")
ENDFUNCTION()

FUNCTION( _dump_newline)
	MESSAGE(STATUS " ")
ENDFUNCTION()

_dump_newline()
dump_string(" ------------- PLATFORM -------------")
dump_var(DXM_CMAKE_PLATFORM)
dump_var(DXM_CMAKE_COMPILER)
dump_string(" ------------- ENVIRONMENT DIRECTORY -------------")
dump_var(DXM_CMAKE_PROJECT_DIRECTORY)
dump_var(DXM_CMAKE_ROOT_DIRECTORY)
dump_var(DXM_CMAKE_CORE_DIRECTORY)
dump_var(DXM_CMAKE_TOOL_DIRECTORY)
_dump_newline()