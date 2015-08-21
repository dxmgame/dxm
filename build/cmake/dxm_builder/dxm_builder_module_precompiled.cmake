##################################################################
# Support macro to use a precompiled header
# Usage:
#   use_precompiled_header(CURRENT_MODULE HEADER_FILE SRC_FILE)
##################################################################

# 
# 框架下的预编译环境
# 
macro(_dxm_builder_module_use_precompiled)

	if(DXM_CMAKE_PLATFORM_WIN32 OR DXM_CMAKE_PLATFORM_WIN64)
		_dxm_builder_module_use_precompiled_win(${DXM_MODULE_${DXM_BUILDER_MODULE}_DIRECTORY}/src/precompiled.h ${DXM_MODULE_${DXM_BUILDER_MODULE}_DIRECTORY}/src/precompiled.cpp)
	elseif(DXM_CMAKE_PLATFORM_IOS)
		_dxm_builder_module_use_precompiled_win_ios(${DXM_MODULE_${DXM_BUILDER_MODULE}_DIRECTORY}/src.ios/cpp/Prefix.pch)
	endif()
endmacro()
# 
# 
# 
macro(_dxm_builder_module_use_precompiled_win HEADER_FILE SRC_FILE)
	get_filename_component(HEADER ${HEADER_FILE} NAME)
	if(DXM_CMAKE_PLATFORM_WIN32 OR DXM_CMAKE_PLATFORM_WIN64)
		add_definitions(/Yu"${HEADER}")
		set_source_files_properties(${SRC_FILE} PPROPERTIES COMPILE_FLAGS /Yc"${HEADER}")
	else()
		MESSAGE(FATAL_ERROR "only DXM_CMAKE_PLATFORM_WIN32 OR DXM_CMAKE_PLATFORM_WIN64 can run 'use_precompiled_header'")
	endif()
endmacro()
# 
# 
# 
macro(_dxm_builder_module_use_precompiled_win_ios PCH_FILE)
	if(DXM_CMAKE_PLATFORM_IOS)                   
        set_target_properties(
            ${CURRENT_MODULE} 
            PROPERTIES
            XCODE_ATTRIBUTE_GCC_PREFIX_HEADER ${PCH_FILE}
            XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
	else()
		MESSAGE(FATAL_ERROR "only DXM_CMAKE_PLATFORM_IOS can run 'dxm_builder_module_use_precompiled_win_ios'")
    endif()
endmacro()
