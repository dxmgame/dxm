SET(rapidxml_FOUND TRUE)

find_path(rapidxml_INCLUDE_DIR rapidxml/rapidxml.hpp
${DXM_CMAKE_INSTALL_DIRECTORY}/inc
)
		
IF (NOT rapidxml_INCLUDE_DIR)
	SET(rapidxml_FOUND FALSE)
	return()
ENDIF()

IF( rapidxml_INCLUDE_DIR )
	set(rapidxml_INCLUDE_DIR ${rapidxml_INCLUDE_DIR} CACHE FILEPATH "rapidxml include directory")
	SET(rapidxml_FOUND ON CACHE INTERNAL "Whether the rapidxml_INCLUDE_DIR found")
	MARK_AS_ADVANCED( ${rapidxml_INCLUDE_DIR} )
ENDIF()