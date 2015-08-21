MESSAGE(STATUS "find libcurl")

# 查询头文件
FIND_PATH(libcurl_INCLUDE_DIR libcurl/curl.h ${DXM_CMAKE_INSTALL_DIRECTORY}/inc)

# 设置已经找到
SET(libcurl_FOUND TRUE)

IF ( NOT libcurl_INCLUDE_DIR)
	SET(libcurl_FOUND FALSE)
	return()
ENDIF()

# 查找库

# 在库根目录查找库
find_library( libcurl_LIBRARY_RELEASE libcurl_imp 
	PATHS ${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/libcurl/release
	NO_DEFAULT_PATH
	)
		
find_library( libcurl_LIBRARY_DEBUG libcurl_imp 
	PATHS ${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/libcurl/debug
	NO_DEFAULT_PATH
	)
	
IF ( NOT libcurl_LIBRARY_DEBUG AND NOT libcurl_LIBRARY_RELEASE)
	SET(libcurl_FOUND FALSE)
	return()
ENDIF()
	
IF ( libcurl_LIBRARY_DEBUG AND libcurl_LIBRARY_RELEASE)
	SET(libcurl_LIBRARY optimized ${libcurl_LIBRARY_RELEASE} debug ${libcurl_LIBRARY_DEBUG})
ENDIF()
    
# if only the release version was found, set the debug variable also to the release version
IF ( libcurl_LIBRARY_RELEASE AND NOT libcurl_LIBRARY_DEBUG)
	SET(libcurl_LIBRARY_DEBUG ${libcurl_LIBRARY_RELEASE})
	SET(libcurl_LIBRARY       ${libcurl_LIBRARY_RELEASE})
ENDIF()

# if only the debug version was found, set the release variable also to the debug version
IF (libcurl_LIBRARY_DEBUG AND NOT libcurl_LIBRARY_RELEASE)
	SET(libcurl_LIBRARY_RELEASE ${libcurl_LIBRARY_DEBUG})
	SET(libcurl_LIBRARY         ${libcurl_LIBRARY_DEBUG})
ENDIF()

IF (libcurl_LIBRARY)
	LIST(APPEND libcurl_LIBRARIES ${libcurl_LIBRARY})
ENDIF()

IF( libcurl_LIBRARIES )
	set(libcurl_INCLUDE_DIR ${libcurl_INCLUDE_DIR} CACHE FILEPATH "libcurl include directory")
	SET(libcurl_FOUND ON CACHE INTERNAL "Whether the libcurl_LIBRARIES found")
	set(libcurl_LIBRARIES ${libcurl_LIBRARIES} CACHE FILEPATH "The libcurl  library")
	MARK_AS_ADVANCED( ${libcurl_LIBRARIES} )
ENDIF()
