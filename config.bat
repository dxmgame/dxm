@echo off 

rem -- 用户手动配置

rem -------------------------------
rem -- ANDROID
rem -------------------------------
set JAVA_HOME=C:\Program Files\Java\jdk1.7.0_40
set ECLIPSE_HOME=E:\adt-bundle-windows-x86\eclipse
set ANDROID_SDK_HOME=E:\adt-bundle-windows-x86\sdk
rem set ANDROID_NDK_HOME=E:\android-ndk-r8d
set ANDROID_NDK_HOME=E:\android-ndk-r10e
set ANTBIN=%ECLIPSE_HOME%\plugins\org.apache.ant_1.9.2.v201404171502\bin
set DXM_CMAKE_V3=true

rem -------------------------------
rem -- Tools
rem -------------------------------
set TEXTURE_PACKER_PATH=E:\TexturePacker\bin
set ANDROID_HOME=%ANDROID_SDK_HOME%
set ANDROID_NDK=%ANDROID_NDK_HOME%

rem -- 系统自动配置

rem -------------------------------
rem -- ANDROID
rem -------------------------------
set ANDROID_TOOLS=%ANDROID_SDK_HOME%\tools
set ANDROID_SDK_PLATFORM_TOOLS=%ANDROID_SDK_HOME%;%ANDROID_SDK_HOME%\platform-tools
set ANDROID_SDK_TOOLS=%ANDROID_SDK_HOME%\tools
set MY_ROOT=%~dp0
set NDK_MODULE_PATH=%~dp0;%ANDROID_NDK_HOME%\sources\
set NDK_MODULE_PATH=%NDK_MODULE_PATH:\=/%
set PATH=%PATH%;%JAVA_HOME%\bin;%ANTBIN%;%ANDROID_NDK_HOME%;%ANDROID_SDK_PLATFORM_TOOLS%;%ANDROID_SDK_TOOLS%;%~dp0\build\tools\win
set CLASSPATH=.;%JAVA_HOME%\lib

rem -------------------------------
rem -- DxM
rem -------------------------------
set DXM_TOOLS=%~dp0\build\tools\win
set DXM_INSTALL=%~dp0\install
set DXM_CMAKE=%~dp0\build\cmake

if defined VS140COMNTOOLS (
	SET DXM_COMPILER_TOOLS=msvc-14.0
	SET DXM_COMPILER=vc14
	if !DXM_CMAKE_V3!==true (
		SET DXM_GENERATOR_X86="Visual Studio 14 2015"
		SET DXM_GENERATOR_X64="Visual Studio 14 2015 Win64"	
	) else (
		SET DXM_GENERATOR_X86="Visual Studio 14"
		SET DXM_GENERATOR_X64="Visual Studio 14 Win64"		
	)
) else if defined VS120COMNTOOLS (
	SET DXM_COMPILER_TOOLS=msvc-12.0
	SET DXM_COMPILER=vc12
	if !DXM_CMAKE_V3!==true (
		SET DXM_GENERATOR_X86="Visual Studio 12 2013"
		SET DXM_GENERATOR_X64="Visual Studio 12 2013 Win64"	
	) else (
		SET DXM_GENERATOR_X86="Visual Studio 12"
		SET DXM_GENERATOR_X64="Visual Studio 12 Win64"		
	)
) else if defined VS110COMNTOOLS (
	SET DXM_COMPILER_TOOLS=msvc-11.0
	SET DXM_COMPILER=vc11
	if !DXM_CMAKE_V3!==true (
		SET DXM_GENERATOR_X86="Visual Studio 11 2012"
		SET DXM_GENERATOR_X64="Visual Studio 11 2012 Win64"	
	) else (
		SET DXM_GENERATOR_X86="Visual Studio 11"
		SET DXM_GENERATOR_X64="Visual Studio 11 Win64"		
	)
) else if defined VS100COMNTOOLS (
	SET DXM_COMPILER_TOOLS=msvc-10.0
	SET DXM_COMPILER=vc10
	if !DXM_CMAKE_V3!==true (
		SET DXM_GENERATOR_X86="Visual Studio 10 2010"
		SET DXM_GENERATOR_X64="Visual Studio 10 2010 Win64"	
	) else (
		SET DXM_GENERATOR_X86="Visual Studio 10"
		SET DXM_GENERATOR_X64="Visual Studio 10 Win64"		
	)
)
