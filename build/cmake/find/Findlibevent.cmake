# -*- cmake -*-

# - Find libevent

MESSAGE(STATUS ">>> find libevent")

# 查询头文件
IF (UNIX)
  SET(libevent_COMPONENTS event_core event_pthreads)
ELSE(UNIX)
  SET(libevent_COMPONENTS libevent)
ENDIF (UNIX)

FIND_PATH(libevent_INCLUDE_DIR libevent/event2/event.h ${DXM_CMAKE_INSTALL_DIRECTORY}/inc)

# 设置已经找到
SET(libevent_FOUND TRUE)

IF ( NOT libevent_INCLUDE_DIR)
	SET(libevent_FOUND FALSE)
	return()
ENDIF()

# 查找库
FOREACH(COMPONENT ${libevent_COMPONENTS})
  
	IF(UNIX)
	
		# 查找库
		find_library( ${COMPONENT}_LIBRARY_RELEASE ${COMPONENT} 
			${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}
			)
	ELSE()

		# 在库根目录查找库
		find_library( ${COMPONENT}_LIBRARY_RELEASE ${COMPONENT}
				${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/libevent
				)
	ENDIF()

	IF ( NOT ${COMPONENT}_LIBRARY_DEBUG AND NOT ${COMPONENT}_LIBRARY_RELEASE)
		SET(libevent_FOUND FALSE)
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
		LIST(APPEND libevent_LIBRARIES ${${COMPONENT}_LIBRARY})
	ENDIF()
	
ENDFOREACH(COMPONENT)

IF( libevent_LIBRARIES )
	set(libevent_INCLUDE_DIR ${libevent_INCLUDE_DIR} CACHE FILEPATH "libevent include directory")
	SET(libevent_FOUND ON CACHE INTERNAL "Whether the libevent_LIBRARIES found")
	set(libevent_LIBRARIES ${libevent_LIBRARIES} CACHE FILEPATH "The libevent library")
	MARK_AS_ADVANCED( ${libevent_LIBRARIES} )
ENDIF()
