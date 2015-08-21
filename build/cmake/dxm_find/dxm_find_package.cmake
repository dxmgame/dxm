

# 
#  dxm_find_module(<module>
#    [PACKAGE <package>]
#    [COMPONENTS <component...>])
# 

MACRO(dxm_find_package MODULE )

	SET(DXM_FIND_MODULE ${MODULE})
	_dxm_find_process_args(${ARGN})

	MESSAGE(STATUS ">> DXM_FIND_MODULE: ${DXM_FIND_MODULE}")
	MESSAGE(STATUS ">> DXM_FIND_ARG_PACKAGE: ${DXM_FIND_ARG_PACKAGE}")
	MESSAGE(STATUS ">> DXM_FIND_ARG_COMPONENTS: ${DXM_FIND_ARG_COMPONENTS}")
	MESSAGE(STATUS ">> find_package(${DXM_FIND_ARG_PACKAGE} COMPONENTS ${DXM_FIND_ARG_COMPONENTS})")

	find_package(${DXM_FIND_ARG_PACKAGE} COMPONENTS ${DXM_FIND_ARG_COMPONENTS})

	if(NOT ${DXM_FIND_ARG_PACKAGE}_FOUND)
		message(FATAL_ERROR ">>> [${DXM_BUILDER_MODULE}] find_${DXM_FIND_ARG_PACKAGE}: ${DXM_FIND_ARG_PACKAGE} NOT FOUND")
	endif()

	dxm_find_add_libraries(boost ${${DXM_FIND_ARG_PACKAGE}_LIBRARIES})
	dxm_find_add_includes(boost ${${DXM_FIND_ARG_PACKAGE}_INCLUDE_DIR})

endMACRO()


# 
# 处理参数;
# 
MACRO( _dxm_find_package_args_process )

	# MESSAGE(STATUS ">> ${ARGN}")
	set(_NO_WHERE)
	set(_CUR_DEST _NO_WHERE)
	set(_VAL_ARGS
		PACKAGE
		COMPONENTS
		)

	foreach(_ARG ${_VAL_ARGS})
		set(${_ARG})
	endforeach()

	foreach(_ELEMENT ${ARGN})
		list(FIND _VAL_ARGS "${_ELEMENT}" _VAL_ARG_FIND)
		list(FIND _BOOL_ARGS "${_ELEMENT}" _BOOL_ARG_FIND)
		if("${_VAL_ARG_FIND}" GREATER "-1")
			set(_CUR_DEST "${_ELEMENT}")
		elseif("${_BOOL_ARG_FIND}" GREATER "-1")
			set("${_ELEMENT}" ON)
			set(_CUR_DEST _NO_WHERE)
		else()
			list(APPEND ${_CUR_DEST} "${_ELEMENT}")
		endif()
	endforeach()

	if(PACKAGE)
		SET(DXM_FIND_ARG_PACKAGE ${PACKAGE})
	else()
		SET(DXM_FIND_ARG_PACKAGE ${DXM_FIND_MODULE})
	endif()

	if(COMPONENTS)
		SET(DXM_FIND_ARG_COMPONENTS ${COMPONENTS})
	else()
		SET(DXM_FIND_ARG_COMPONENTS ${DXM_FIND_MODULE})
	endif()
	
	if(HEADERS)
		SET(DXM_FIND_ARG_HEADERS ${HEADERS})
	else()
		SET(DXM_FIND_ARG_HEADERS ${DXM_FIND_MODULE}/${DXM_FIND_MODULE}_lib.h)
	endif()

ENDMACRO( _dxm_find_package_args_process )

