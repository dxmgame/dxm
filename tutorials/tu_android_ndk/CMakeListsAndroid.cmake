
# 加载配置;
include(${CMAKE_CURRENT_LIST_DIR}/config.cmake)

# 设置项目;
SET(DXM_PROJECT_CURRENT_MODULE ${DXM_CONFIG_MODULE}) # 设置项目; 为什么不直接用DXM_CONFIG_MODULE? DXM_CONFIG_MODULE会每次加载配置的时候都会重置.

# 添加9层目录，只为加载dxm_cmake;
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/..)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

# 包含dxm_cmake
include(dxm_cmake.project)

# 构建;
dxm_builder_android_build_module(${DXM_PROJECT_CURRENT_MODULE} ${CMAKE_CURRENT_LIST_DIR})

