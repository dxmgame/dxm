REM 注释
@echo off

set ocd=%cd%
cd /d %~dp0
cd ..

if not exist proj.android md proj.android
cd proj.android

echo #####提示：开始构建#####
cmake -DDXM_CMAKE_PLATFORM=ANDROID -P ..\CMakeListsAndroid.cmake
rem cmake -DDXM_CMAKE_PLATFORM=ANDROID -P ..\CMakeListsAndroid.cmake
echo #####提示：构建结束#####

echo #####提示：开始编译#####
rem BuildConsole.exe %project%.sln /prj=ALL_BUILD  /Cfg="Debug|WIN32,Release|WIN32" 
call build.bat
echo #####提示：编译结束#####

echo #####提示：开始安装#####
rem cmake -DBUILD_TYPE="Debug" -P cmake_install.cmake
rem cmake -DBUILD_TYPE="Release" -P cmake_install.cmake
echo #####提示：安装结束#####

cd /d %ocd%

@echo on
