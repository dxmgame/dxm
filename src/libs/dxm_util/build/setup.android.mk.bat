REM ע��
@echo off

set ocd=%cd%
cd /d %~dp0
cd ..

set project=tu_android_ndk

if not exist proj.android md proj.android
cd proj.android

echo #####��ʾ����ʼ����#####
cmake -DDXM_CMAKE_PLATFORM=ANDROID -P ..\CMakeListsAndroid.cmake
rem cmake -DDXM_CMAKE_PLATFORM=ANDROID -P ..\CMakeListsAndroid.cmake
echo #####��ʾ����������#####

echo #####��ʾ����ʼ����#####
rem BuildConsole.exe %project%.sln /prj=ALL_BUILD  /Cfg="Debug|WIN32,Release|WIN32" 
echo #####��ʾ���������#####

echo #####��ʾ����ʼ��װ#####
rem cmake -DBUILD_TYPE="Debug" -P cmake_install.cmake
rem cmake -DBUILD_TYPE="Release" -P cmake_install.cmake
echo #####��ʾ����װ����#####

cd /d %ocd%

pause
@echo on
