if(DXM_TARGET_WIN32)
	find_path( pthread_INCLUDE_DIR pthread/pthread.h
			${DXM_CMAKE_INSTALL_DIRECTORY}/inc
			)
	
	find_library( pthread_LIBRARIES pthreadVCE2
			${DXM_CMAKE_INSTALL_DIRECTORY}/lib/${DXM_CMAKE_COMPILER}/oslibs
			)
endif()