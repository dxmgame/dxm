if(DXM_TARGET_WIN32)
	find_path( vld_INCLUDE_DIR vld/vld.h
			${DXM_CMAKE_INSTALL_DIRECTORY}/inc
			)
	
	find_library( vld_LIBRARIES vld
			${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/oslibs
			)
endif()