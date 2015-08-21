# 设置cmake版本要求;
cmake_minimum_required(VERSION 2.8.0)

# 加载配置;
include(${CMAKE_CURRENT_LIST_DIR}/config.cmake)
SET(DXM_PROJECT_CURRENT_MODULE ${DXM_CONFIG_MODULE}) # 设置项目; 为什么不直接用DXM_CONFIG_MODULE? DXM_CONFIG_MODULE会每次加载配置的时候都会重置.
project(${DXM_PROJECT_CURRENT_MODULE})

# 加载dxm核心模块;
include(${CMAKE_CURRENT_LIST_DIR}/dxm.cmake)


