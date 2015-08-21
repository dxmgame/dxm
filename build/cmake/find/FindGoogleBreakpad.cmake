# -*- cmake -*-

# - Find Google BreakPad
# Find the Google BreakPad includes and library
# This module defines
#  BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR, where to find exception_handler.h, etc.
#  BREAKPAD_EXCEPTION_HANDLER_LIBRARIES, the libraries needed to use Google BreakPad.
#  BREAKPAD_EXCEPTION_HANDLER_FOUND, If false, do not try to use Google BreakPad.
# also defined, but not for general use are
#  BREAKPAD_EXCEPTION_HANDLER_LIBRARY, where to find the Google BreakPad library.

MESSAGE(STATUS "find google breakpad")
# 查询头文件
IF (UNIX)
  SET(GoogleBreakpad_COMPONENTS breakpad_client)
ELSE(UNIX)
  SET(GoogleBreakpad_COMPONENTS exception_handler crash_generation_client common)
ENDIF (UNIX)

FIND_PATH(GoogleBreakpad_INCLUDE_DIR google_breakpad/exception_handler.h 
  		${DXM_CMAKE_INSTALL_DIRECTORY}/inc)

# 设置已经找到
SET(GoogleBreakpad_FOUND TRUE)

IF ( NOT GoogleBreakpad_INCLUDE_DIR)
	SET(GoogleBreakpad_FOUND FALSE)
	return()
ENDIF()
	
# 查找库
FOREACH(COMPONENT ${GoogleBreakpad_COMPONENTS})
  
	IF(UNIX)
	
		# 查找库
		find_library( ${COMPONENT}_LIBRARY_RELEASE ${COMPONENT} 
			${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}
			)
	ELSE()

		# 在库根目录查找库
		find_library( ${COMPONENT}_LIBRARY_RELEASE ${COMPONENT}
				${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/google_breakpad/release
				)
				
		find_library( ${COMPONENT}_LIBRARY_DEBUG ${COMPONENT}
				${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/google_breakpad/debug
				)
	ENDIF()
	
	IF ( NOT ${COMPONENT}_LIBRARY_DEBUG AND NOT ${COMPONENT}_LIBRARY_RELEASE)
		SET(GoogleBreakpad_FOUND FALSE)
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
		LIST(APPEND GoogleBreakpad_LIBRARIES ${${COMPONENT}_LIBRARY})
	ENDIF()
	
#	MESSAGE(STATUS "find google breakpad " ${COMPONENT})
#	MESSAGE(STATUS "find google breakpad " ${${COMPONENT}_LIBRARY_RELEASE})
#	MESSAGE(STATUS "find google breakpad " ${${COMPONENT}_LIBRARY_DEBUG})
ENDFOREACH(COMPONENT)

IF( GoogleBreakpad_LIBRARIES )
	set(GoogleBreakpad_INCLUDE_DIR ${GoogleBreakpad_INCLUDE_DIR} CACHE FILEPATH "GoogleBreakpad include directory")
	SET(GoogleBreakpad_FOUND ON CACHE INTERNAL "Whether the GoogleBreakpad_LIBRARIES found")
	set(GoogleBreakpad_LIBRARIES ${GoogleBreakpad_LIBRARIES} CACHE FILEPATH "The GoogleBreakpad  library")
	MARK_AS_ADVANCED( ${GoogleBreakpad_LIBRARIES} )
ENDIF()
# MESSAGE(STATUS "find google breakpad " ${GoogleBreakpad_LIBRARIES})
