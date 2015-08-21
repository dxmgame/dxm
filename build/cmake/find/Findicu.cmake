if(UNIX)
	find_path( icu_INCLUDE_DIR icu/unicode/utf32.h
		${DXM_CMAKE_INSTALL_DIRECTORY}/inc
		# /usr/local/include
		# /usr/include
		)

	find_library( icuuc_LIBRARY_RELEASE icuuc
		${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}
		# /usr/local/lib
		# /usr/lib
		)

	find_library( icuin_LIBRARY_RELEASE icui18n
		${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}
		# /usr/local/lib
		# /usr/lib
		)

	IF ( NOT icu_INCLUDE_DIR OR 
		NOT icuuc_LIBRARY_RELEASE OR 
		NOT icuin_LIBRARY_RELEASE)
		SET(icu_FOUND FALSE)
		return()
	ENDIF()
	
	SET(icu_LIBRARIES ${icuuc_LIBRARY_RELEASE} ${icuin_LIBRARY_RELEASE})
else()
	find_path( icu_INCLUDE_DIR icu/unicode/utf32.h
		${DXM_CMAKE_INSTALL_DIRECTORY}/inc
		)

	find_library( icuuc_LIBRARY_RELEASE icuuc
		${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/icu/release
		)
	
	find_library( icuin_LIBRARY_RELEASE icuin
		${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/icu/release
		)

	find_library( icuuc_LIBRARY_DEBUG icuucd
			${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/icu/debug
			)

	find_library( icuin_LIBRARY_DEBUG icuind
			${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/icu/debug
			)

	IF ( NOT icu_INCLUDE_DIR OR 
		NOT icuuc_LIBRARY_RELEASE OR NOT icuuc_LIBRARY_DEBUG OR 
		NOT icuin_LIBRARY_RELEASE OR NOT icuin_LIBRARY_DEBUG)
		SET(icu_FOUND FALSE)
		return()
	ENDIF()

	SET(icu_LIBRARIES optimized ${icuuc_LIBRARY_RELEASE} debug ${icuuc_LIBRARY_DEBUG} 
		optimized ${icuin_LIBRARY_RELEASE} debug ${icuin_LIBRARY_DEBUG})
endif()

IF( icu_LIBRARIES )
	set(icu_INCLUDE_DIR ${icu_INCLUDE_DIR} CACHE FILEPATH "icu include directory")
	SET(icu_FOUND ON CACHE INTERNAL "Whether the icu_LIBRARIES found")
	set(icu_LIBRARIES ${icu_LIBRARIES} CACHE FILEPATH "The icu  library")
	MARK_AS_ADVANCED( ${icu_LIBRARIES} )
ENDIF()