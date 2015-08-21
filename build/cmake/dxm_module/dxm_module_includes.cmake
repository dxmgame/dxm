
# 
# 添加头文件
# 
# dxm_module_add_includes(MODULE INCLUDE1 INCLUDE2)
MACRO(dxm_module_add_includes MODULE)

	# MESSAGE(STATUS "-------------dxm_module_add_includes ${MODULE} ${ARGN}")
	FOREACH( TEMP ${ARGN} )
		# 唯一性检测;
		SET(CHECK_UNIQUE TRUE)
		FOREACH(TEMP_VALUE ${DXM_MODULE_${MODULE}_INCLUDES})
			IF(${TEMP_VALUE} STREQUAL ${TEMP})
				SET(CHECK_UNIQUE FALSE)
				BREAK()
			ENDIF()	
		ENDFOREACH()
		IF(CHECK_UNIQUE)
			LIST( APPEND DXM_MODULE_${MODULE}_INCLUDES ${TEMP} )
		ENDIF()
	ENDFOREACH()
ENDMACRO()
