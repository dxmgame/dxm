@echo off 

rem -- 用户手动配置

rem -------------------------------
rem -- ANDROID
rem -------------------------------
set JAVA_HOME=C:\Program Files\Java\jdk1.7.0_40
set ECLIPSE_HOME=E:\adt-bundle-windows-x86\eclipse
set ANDROID_SDK_HOME=E:\adt-bundle-windows-x86\sdk
set ANDROID_NDK_HOME=E:\android-ndk-r10e
set ANTBIN=%ECLIPSE_HOME%\plugins\org.apache.ant_1.9.2.v201404171502\bin
set DXM_CMAKE_V3=true

rem -------------------------------
rem -- Tools
rem -------------------------------
set TEXTURE_PACKER_PATH=E:\TexturePacker\bin

rem -------------------------------
rem -- DXM 变量
rem -------------------------------
set DXM_PROJECT_PATH=%~dp0
set DXM_PATH=%DXM_PROJECT_PATH%\dxm

rem -- 系统自动配置
rem -------------------------------
rem -- ANDROID
rem -------------------------------
set ANDROID_TOOLS=%ANDROID_SDK_HOME%\tools
set ANDROID_SDK_PLATFORM_TOOLS=%ANDROID_SDK_HOME%;%ANDROID_SDK_HOME%\platform-tools
set ANDROID_SDK_TOOLS=%ANDROID_SDK_HOME%\tools
set NDK_MODULE_PATH=%~dp0;%ANDROID_NDK_HOME%\sources\;
set NDK_MODULE_PATH=%NDK_MODULE_PATH:\=/%
set PATH=%PATH%;%JAVA_HOME%\bin;%ANTBIN%;%ANDROID_NDK_HOME%;%ANDROID_SDK_PLATFORM_TOOLS%;%ANDROID_SDK_TOOLS%;%DXM_PATH%\build\tools\win
set CLASSPATH=.;%JAVA_HOME%\lib
set ANDROID_HOME=%ANDROID_SDK_HOME%
set ANDROID_NDK=%ANDROID_NDK_HOME%

rem -------------------------------
rem -- DXM 变量
rem -------------------------------
set DXM_INSTALL=%DXM_PROJECT_PATH%\install
set DXM_TOOLS=%DXM_PATH%\build\tools\win
set DXM_CMAKE=%DXM_PATH%\build\cmake

rem -------------------------------
rem -- DXM Compiler
rem -------------------------------
if defined VS140COMNTOOLS (
	SET DXM_COMPILER_TOOLS="%VS140COMNTOOLS%"
	SET DXM_COMPILER=vc14
	if !DXM_CMAKE_V3!==true (
		SET DXM_GENERATOR_X86="Visual Studio 14 2015"
		SET DXM_GENERATOR_X64="Visual Studio 14 2015 Win64"	
	) else (
		SET DXM_GENERATOR_X86="Visual Studio 14"
		SET DXM_GENERATOR_X64="Visual Studio 14 Win64"		
	)
) else if defined VS120COMNTOOLS (
	SET DXM_COMPILER_TOOLS="%VS120COMNTOOLS%"
	SET DXM_COMPILER=vc12
	if !DXM_CMAKE_V3!==true (
		SET DXM_GENERATOR_X86="Visual Studio 12 2013"
		SET DXM_GENERATOR_X64="Visual Studio 12 2013 Win64"	
	) else (
		SET DXM_GENERATOR_X86="Visual Studio 12"
		SET DXM_GENERATOR_X64="Visual Studio 12 Win64"		
	)
) else if defined VS110COMNTOOLS (
	SET DXM_COMPILER_TOOLS="%VS110COMNTOOLS%"
	SET DXM_COMPILER=vc11
	if !DXM_CMAKE_V3!==true (
		SET DXM_GENERATOR_X86="Visual Studio 11 2012"
		SET DXM_GENERATOR_X64="Visual Studio 11 2012 Win64"	
	) else (
		SET DXM_GENERATOR_X86="Visual Studio 11"
		SET DXM_GENERATOR_X64="Visual Studio 11 Win64"		
	)
) else if defined VS100COMNTOOLS (
	SET DXM_COMPILER_TOOLS="%VS100COMNTOOLS%"
	SET DXM_COMPILER=vc10
	if !DXM_CMAKE_V3!==true (
		SET DXM_GENERATOR_X86="Visual Studio 10 2010"
		SET DXM_GENERATOR_X64="Visual Studio 10 2010 Win64"	
	) else (
		SET DXM_GENERATOR_X86="Visual Studio 10"
		SET DXM_GENERATOR_X64="Visual Studio 10 Win64"		
	)
)
