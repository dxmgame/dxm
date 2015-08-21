# -*- cmake -*-

# - Find libcocos2dx

MESSAGE(STATUS ">>> find libcocos2dx")

# opengl32.lib
if(DXM_TARGET_IOS)
SET(libcocos2dx_COMPONENTS libcocos2dx libCocosDenshion libextensions libcurl libwebp)
else()
SET(libcocos2dx_COMPONENTS libBox2D libchipmunk libcocos2d libCocosDenshion libExtensions glew32 libcurl_imp libiconv libzlib)
endif()

# 查询头文件
FIND_PATH(libcocos2dx_INCLUDE_DIR libcocos2dx/cocos2d.h ${DXM_CMAKE_INSTALL_DIRECTORY}/inc)

# 设置已经找到
SET(libcocos2dx_FOUND TRUE)

IF ( NOT libcocos2dx_INCLUDE_DIR)
	SET(libcocos2dx_FOUND FALSE)
	return()
ENDIF()

# 查找库
FOREACH(COMPONENT ${libcocos2dx_COMPONENTS})
  
  	set(TEMP_COMPONENT ${COMPONENT})
  	if(DXM_TARGET_IOS)
		if(${COMPONENT} MATCHES "lib.*")	
			string(SUBSTRING ${COMPONENT} 3 -1 TEMP_COMPONENT)
		endif()
	endif()
	# 在库根目录查找库
	find_library( ${COMPONENT}_LIBRARY_RELEASE ${TEMP_COMPONENT}
				PATHS ${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/libcocos2dx/release
				NO_DEFAULT_PATH
				)
				
	find_library( ${COMPONENT}_LIBRARY_DEBUG ${TEMP_COMPONENT}
				PATHS ${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/libcocos2dx/debug
				NO_DEFAULT_PATH
				)

	IF ( NOT ${COMPONENT}_LIBRARY_DEBUG AND NOT ${COMPONENT}_LIBRARY_RELEASE)
		SET(libcocos2dx_FOUND FALSE)
		return()
	ENDIF()
	
  IF (${COMPONENT}_LIBRARY_DEBUG AND ${COMPONENT}_LIBRARY_RELEASE)
    SET(${COMPONENT}_LIBRARY optimized ${${COMPONENT}_LIBRARY_RELEASE} debug ${${COMPONENT}_LIBRARY_DEBUG})
  ENDIF()
    
  # if only the release version was found, set the debug variable also to the release version
  IF (${COMPONENT}_LIBRARY_RELEASE AND NOT ${COMPONENT}_LIBRARY_DEBUG)
    SET(${COMPONENT}_LIBRARY_DEBUG ${${COMPONENT}_LIBRARY_RELEASE})
    SET(${COMPONENT}_LIBRARY       ${${COMPONENT}_LIBRARY_RELEASE})
  ENDIF()

  # if only the debug version was found, set the release variable also to the debug version
  IF (${COMPONENT}_LIBRARY_DEBUG AND NOT ${COMPONENT}_LIBRARY_RELEASE)
    SET(${COMPONENT}_LIBRARY_RELEASE ${${COMPONENT}_LIBRARY_DEBUG})
    SET(${COMPONENT}_LIBRARY         ${${COMPONENT}_LIBRARY_DEBUG})
  ENDIF()

	IF (${COMPONENT}_LIBRARY)
		LIST(APPEND libcocos2dx_LIBRARIES ${${COMPONENT}_LIBRARY})
	ENDIF()

	message(STATUS ">>> " ${COMPONENT}_LIBRARY_DEBUG  ":" ${${COMPONENT}_LIBRARY_DEBUG})
	message(STATUS ">>> " ${COMPONENT}_LIBRARY_RELEASE  ":" ${${COMPONENT}_LIBRARY_RELEASE})
	message(STATUS ">>> " ${COMPONENT}_LIBRARIES  ":" ${${COMPONENT}_LIBRARY})
	
ENDFOREACH(COMPONENT)

IF( libcocos2dx_LIBRARIES )
IF(WIN32)
	LIST(APPEND libcocos2dx_LIBRARIES opengl32)
ENDIF(WIN32)	
	set(libcocos2dx_INCLUDE_DIR ${libcocos2dx_INCLUDE_DIR} CACHE FILEPATH "libcocos2dx include directory")
	SET(libcocos2dx_FOUND ON CACHE INTERNAL "Whether the libcocos2dx_LIBRARIES found")
	set(libcocos2dx_LIBRARIES ${libcocos2dx_LIBRARIES} CACHE FILEPATH "The libcocos2dx library")
	MARK_AS_ADVANCED( ${libcocos2dx_LIBRARIES} )
ENDIF()