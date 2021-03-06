
# 
# Android平台编译独立
# 
# - DXM_CMAKE_PROJECT_DIRECTORY
# - DXM_CMAKE_ROOT_DIRECTORY
# - DXM_CMAKE_CORE_DIRECTORY
# - DXM_CMAKE_TOOL_DIRECTORY
# - DXM_CMAKE_INSTALL_DIRECTORY
# - DXM_CMAKE_COMPILER
# - DXM_CMAKE_PLATFORM: WIN32 WIN64 UNIX ANDROID IOS
#     - DXM_CMAKE_PLATFORM_WIN32
#     - DXM_CMAKE_PLATFORM_WIN64
#     - DXM_CMAKE_PLATFORM_UNIX
#     - DXM_CMAKE_PLATFORM_ANDROID
#     - DXM_CMAKE_PLATFORM_IOS    

MACRO(_dxm_environment_init)
	_dxm_environment_init_cmake()
	_dxm_environment_init_platform(DXM_CMAKE_PLATFORM)
	_dxm_environment_init_compiler()
ENDMACRO()

# 
# CMake CMake初始化;
# 
MACRO(_dxm_environment_init_cmake)

	# 跨平台设置
	set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/output )
	set( LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/output )

ENDMACRO()

# 
# CMake 确定平台;
# 
MACRO(_dxm_environment_init_platform)
	
	if(DXM_CMAKE_PLATFORM)
		if(${DXM_CMAKE_PLATFORM} STREQUAL WIN32)
			set(DXM_CMAKE_PLATFORM_WIN32 true)
		elseif(${DXM_CMAKE_PLATFORM} STREQUAL WIN64)
			set(DXM_CMAKE_PLATFORM_WIN64 true)
		elseif(${DXM_CMAKE_PLATFORM} STREQUAL UNIX)
			set(DXM_CMAKE_PLATFORM_UNIX true)
		elseif(${DXM_CMAKE_PLATFORM} STREQUAL IOS)
			set(DXM_CMAKE_PLATFORM_IOS true)
		elseif(${DXM_CMAKE_PLATFORM} STREQUAL ANDROID)
			set(DXM_CMAKE_PLATFORM_ANDROID true)	
			if(NOT CMAKE_GENERATOR)
				set(DXM_CMAKE_PLATFORM_ANDROID_MK true)
			endif()
		endif()
	else()
		set(DXM_CMAKE_PLATFORM_WIN32 true)
	endif()

ENDMACRO()

# 
# CMake 确定编译器;
# 
MACRO(_dxm_environment_init_compiler)
	
	if(NOT CMAKE_GENERATOR)
		MESSAGE(STATUS "this is not CMakelist.txt (CMAKE_GENERATOR is null)!")
		return()
	endif()

	# 独立平台设置
	if(DXM_CMAKE_PLATFORM_WIN32 OR DXM_CMAKE_PLATFORM_WIN64)
		if(NOT WIN32)
			MESSAGE(FATAL_ERROR "DXM_CMAKE_PLATFORM_WIN32 need WIN32 platform!")
		endif()

		# 指定编译器名称
		if(CMAKE_GENERATOR STREQUAL "Visual Studio 7" OR CMAKE_GENERATOR STREQUAL "Visual Studio 7 .NET 2003")
			SET(DXM_CMAKE_COMPILER "win_x86")
		elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 8 2005")
			SET(DXM_CMAKE_COMPILER "win_x86")
		elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 9 2008")
			SET(DXM_CMAKE_COMPILER "win_x86")
		elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 10" OR CMAKE_GENERATOR STREQUAL "Visual Studio 10 2010")
			SET(DXM_CMAKE_COMPILER "win_x86")
		elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 10 Win64" OR CMAKE_GENERATOR STREQUAL "Visual Studio 10 2010")
			SET(DXM_CMAKE_COMPILER "win_x64")
		elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 11" OR CMAKE_GENERATOR STREQUAL "Visual Studio 11 2012")
			SET(DXM_CMAKE_COMPILER "win_x86")			
		elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 11 Win64" OR CMAKE_GENERATOR STREQUAL "Visual Studio 11 2012 Win64")
			SET(DXM_CMAKE_COMPILER "win_x64")
		elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 12" OR CMAKE_GENERATOR STREQUAL "Visual Studio 12 2013")
			SET(DXM_CMAKE_COMPILER "win_x86")			
		elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 12 Win64" OR CMAKE_GENERATOR STREQUAL "Visual Studio 12 2013 Win64")
			SET(DXM_CMAKE_COMPILER "win_x64")
		elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 14" OR CMAKE_GENERATOR STREQUAL "Visual Studio 14 2015")
			SET(DXM_CMAKE_COMPILER "win_x86")			
		elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 14 Win64" OR CMAKE_GENERATOR STREQUAL "Visual Studio 14 2015 Win64")
			SET(DXM_CMAKE_COMPILER "win_x64")						
		endif()

		# set(CMAKE_DEBUG_POSTFIX "d")
		SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1")
		# release "/MD /O2 /Ob2 /D NDEBUG"
		set(CMAKE_CXX_FLAGS_RELEASE "/MD /Zi /O2 /Ob1 /D NDEBUG")
		set(CMAKE_CXX_FLAGS_MINSIZEREL "/MD /O1 /Ob1 /D NDEBUG")
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /Zi /O2 /Ob1 /D NDEBUG")

		# 设置debug info
		SET(CMAKE_EXE_LINKER_FLAGS_RELEASE ${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}) 
		SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE ${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO})
		
		# 创建bin目录结构
		dxm_file_mkdir(${DXM_CMAKE_INSTALL_DIRECTORY}/bin/${DXM_CMAKE_COMPILER}/debug)
		dxm_file_mkdir(${DXM_CMAKE_INSTALL_DIRECTORY}/bin/${DXM_CMAKE_COMPILER}/release)
		
	elseif(DXM_CMAKE_PLATFORM_UNIX)
		if(NOT UNIX)
			MESSAGE(FATAL_ERROR "DXM_CMAKE_PLATFORM_UNIX need UNIX/LINUX platform!")
		endif()
		SET(DXM_CMAKE_COMPILER "unix")
	
		SET(CMAKE_BUILD_TYPE RELEASE)
		SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g")
		SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g")
		
		# 设置RPATH
		SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
		SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
		SET(CMAKE_INSTALL_RPATH ".")
		SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

		# 设置c++11
		include(CheckCXXCompilerFlag)
		CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
		CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
		if(COMPILER_SUPPORTS_CXX11)
		    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
		elseif(COMPILER_SUPPORTS_CXX0X)
		    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
		else()
		    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
		endif()
		
		# 创建bin目录结构
		dxm_file_mkdir(${DXM_CMAKE_INSTALL_DIRECTORY}/bin/${DXM_CMAKE_COMPILER})
		
	elseif(DXM_CMAKE_PLATFORM_IOS)
		if(NOT APPLE)
			MESSAGE(FATAL_ERROR "DXM_CMAKE_PLATFORM_IOS need APPLE platform!")
		endif()
		SET(DXM_CMAKE_COMPILER "ios")
	
		include(dxm_cmake_ios)
		
		# 创建bin目录结构
		dxm_file_mkdir(${DXM_CMAKE_INSTALL_DIRECTORY}/bin/${DXM_CMAKE_COMPILER}/debug)
		dxm_file_mkdir(${DXM_CMAKE_INSTALL_DIRECTORY}/bin/${DXM_CMAKE_COMPILER}/release)
	elseif(DXM_CMAKE_PLATFORM_ANDROID)

		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
		SET(DXM_CMAKE_COMPILER "android")
	endif()

	# 确认DXM_CMAKE_COMPILER
	if(NOT DXM_CMAKE_COMPILER)
		SET(DXM_CMAKE_COMPILER "unknown")
		MESSAGE(FATAL_ERROR "DXM_CMAKE_COMPILER is not existed!")
	endif()
ENDMACRO()


_dxm_environment_init()