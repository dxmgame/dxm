@echo off

set ocd=%cd%
cd /d %~dp0

echo ##### 提示：读取配置文件 #####
if exist ..\config.bat call ..\config.bat
if exist ..\..\config.bat call ..\..\config.bat
if exist ..\..\..\config.bat call ..\..\..\config.bat
if exist ..\..\..\..\config.bat call ..\..\..\..\config.bat
if exist ..\..\..\..\..\config.bat call ..\..\..\..\..\config.bat
if exist ..\..\..\..\..\..\config.bat call ..\..\..\..\..\..\config.bat
if exist ..\..\..\..\..\..\..\config.bat call ..\..\..\..\..\..\..\config.bat

echo ##### 提示：变量配置 #####
set LIBEVENT_VERSION_NAME=libevent-2.0.21-stable
SET DXM_PREBUILT=%cd%\prebuilt
SET DXM_PLATFORM=win_x86

echo ##### 提示：解压 %LIBEVENT_VERSION_NAME% #####
if not exist %LIBEVENT_VERSION_NAME% (
	
	rem 解压操作
	%DXM_TOOLS%\7za.exe x -y %LIBEVENT_VERSION_NAME%.tar.gz
	%DXM_TOOLS%\7za.exe x -y %LIBEVENT_VERSION_NAME%.tar
	del %LIBEVENT_VERSION_NAME%.tar /Q
	
	rem 代码补丁
	xcopy /y /s patch\* %LIBEVENT_VERSION_NAME%\
	)

echo ##### 提示：编译 LibEvent #####	
cd %LIBEVENT_VERSION_NAME%

echo ##### 提示：编译 %LIBEVENT_VERSION_NAME% #####
nmake makefile.nmake
if %errorlevel% neq 0 goto :cmEnd

echo ##### 提示：安装 libevent #####

if not exist %DXM_PREBUILT%\lib\%DXM_PLATFORM%\libevent mkdir %DXM_PREBUILT%\lib\%DXM_PLATFORM%
if not exist %DXM_PREBUILT%\inc\libevent mkdir %DXM_PREBUILT%\inc\libevent
if not exist %DXM_PREBUILT%\inc\libevent\WIN32-Code mkdir %DXM_PREBUILT%\inc\libevent\WIN32-Code

copy libevent.lib %DXM_PREBUILT%\lib\%DXM_PLATFORM%\
copy libevent_core.lib %DXM_PREBUILT%\lib\%DXM_PLATFORM%\
copy libevent_extras.lib %DXM_PREBUILT%\lib\%DXM_PLATFORM%\

xcopy /y/s include\*.h %DXM_PREBUILT%\inc\libevent
xcopy /y/s WIN32-Code\*.h %DXM_PREBUILT%\inc\libevent

rem ---------------------
cd /d %~dp0

setlocal enabledelayedexpansion
call :GET_PATH_NAME %cd%
set project=%PATH_NAME%

if not exist  proj.win32 md proj.win32
cd proj.win32

echo #####提示：开始构建#####
cmake -DDXM_CMAKE_PLATFORM=WIN32 ..
echo %errorlevel%
if %errorlevel% neq 0 goto :cmEnd
cmake -DDXM_CMAKE_PLATFORM=WIN32 ..
echo %errorlevel%
if %errorlevel% neq 0 goto :cmEnd
echo #####提示：构建结束#####

echo #####提示：开始编译#####
rem BuildConsole.exe %project%.sln /prj=ALL_BUILD /Silent /OpenMonitor /Cfg="Debug|WIN32,Release|WIN32" 
rem BuildConsole.exe %project%.sln /prj=ALL_BUILD /Silent  /Cfg="Debug|WIN32,Release|WIN32" 
rem if %errorlevel% neq 0 goto :cmEnd
echo #####提示：编译结束#####

echo #####提示：开始安装#####
cmake -DBUILD_TYPE="Debug" -P cmake_install.cmake
if %errorlevel% neq 0 goto :cmEnd
cmake -DBUILD_TYPE="Release" -P cmake_install.cmake
if %errorlevel% neq 0 goto :cmEnd
echo #####提示：安装结束#####

goto cmDone
:cmEnd
echo setup failed
pause
exit

:cmDone
cmake -P dxm_cmake_compile_succeeded.cmake
cmake -P dxm_cmake_install_succeeded.cmake
cd /d %ocd%

goto :eof
:GET_PATH_NAME
set PATH_NAME=%~n1

:eof

@echo on