# 
# dxm_cmake.cmake
# dxm框架用的cmake主脚本
# DXM_CMAKE_PROJECT_DIRECTORY
if(NOT DXM_CMAKE_PROJECT_DIRECTORY)
	SET(DXM_CMAKE_PROJECT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endif()

if(NOT DXM_CMAKE_COMPANY_NAME)
	set(DXM_CMAKE_COMPANY_NAME "zhangdong")
endif()

if(NOT DXM_CMAKE_INSTALL_DIRECTORY)
SET( DXM_CMAKE_INSTALL_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/install )
IF(WIN32)
	STRING(REPLACE "/" "\\" DXM_CMAKE_INSTALL_DIRECTORY_WIN32 ${DXM_CMAKE_INSTALL_DIRECTORY})
ENDIF()
ENDIF()

# 
# 加载库框架; 如果已经是库目录则无需加载;
# 
SET(ENGINE_LIB dxm)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${ENGINE_LIB}/dxm_cmake.project.cmake)
	INCLUDE(${CMAKE_CURRENT_LIST_DIR}/${ENGINE_LIB}/dxm_cmake.project.cmake)
endif()

# 
# dxm-cmake 下的 dxm_modules ;
# 
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/dxm_cmake.cmake)
	INCLUDE(${CMAKE_CURRENT_LIST_DIR}/dxm_cmake.cmake)
endif()

# 
# dxm-cmake 下的 dxm_modules ;
# 
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/dxm_modules.cmake)
	INCLUDE(${CMAKE_CURRENT_LIST_DIR}/dxm_modules.cmake)
endif()

# 
# 全局头文件路径
# 
dxm_cmake_add_includes(${CMAKE_CURRENT_LIST_DIR}/inc)


# 
# 当前项目目录下，读取cmake结束;
# 
if(${DXM_CMAKE_PROJECT_DIRECTORY} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
	dxm_register_regist_module_finish()
endif()