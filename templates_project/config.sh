#! /bin/bash

# -- 用户手动配置

# -------------------------------
# -- IOS
# -------------------------------

# -------------------------------
# -- Tools
# -------------------------------

# -- 系统自动配置

# -------------------------------
# -- IOS
# -------------------------------

# -------------------------------
# -- DxM
# -------------------------------
DXM_FRAMEWORK=$(pwd)/$(dirname $0)
DXM_TOOLS=${DXM_FRAMEWORK}/dxm/tools
DXM_INSTALL=${DXM_FRAMEWORK}/install
DXM_CMAKE=${DXM_FRAMEWORK}/dxm/cmake

# -- 导出所有变量到临时文件
echo $DXM_FRAMEWORK >var_dxm_framework.txt
echo $DXM_TOOLS >var_dxm_tools.txt
echo $DXM_INSTALL >var_DXM_INSTALL.txt
echo $DXM_CMAKE >var_dxm_cmake.txt

chmod 777 ${DXM_INSTALL}/inc