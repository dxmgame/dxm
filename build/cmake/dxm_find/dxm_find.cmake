
# 
# dxm_find_add_definitions
# dxm_find_add_includes
# dxm_find_add_libraries
# 

# 
# 项目宏支持
# 
# dxm_find_add_definitions(MODULE -DDEFINITION1 -DDEFINITION2)
MACRO(dxm_find_add_definitions MODULE)
	FOREACH( TEMP ${ARGN} )
		# 唯一性检测;
		SET(CHECK_UNIQUE TRUE)
		FOREACH(TEMP_VALUE ${DXM_FIND_${MODULE}_DEFINITIONS})
			IF(${TEMP_VALUE} STREQUAL ${TEMP})
				SET(CHECK_UNIQUE FALSE)
				BREAK()
			ENDIF()	
		ENDFOREACH()
		IF(CHECK_UNIQUE)
			LIST( APPEND DXM_FIND_${MODULE}_DEFINITIONS ${TEMP} )
		ENDIF()
	ENDFOREACH()	
ENDMACRO()

# 
# 添加头文件
# 
# dxm_find_add_includes(MODULE INCLUDE1 INCLUDE2)
MACRO(dxm_find_add_includes MODULE)

	FOREACH( TEMP ${ARGN} )
		# 唯一性检测;
		SET(CHECK_UNIQUE TRUE)
		FOREACH(TEMP_VALUE ${DXM_FIND_${MODULE}_INCLUDE_DIR})
			IF(${TEMP_VALUE} STREQUAL ${TEMP})
				SET(CHECK_UNIQUE FALSE)
				BREAK()
			ENDIF()	
		ENDFOREACH()
		IF(CHECK_UNIQUE)
			LIST( APPEND DXM_FIND_${MODULE}_INCLUDE_DIR ${TEMP} )
		ENDIF()
	ENDFOREACH()
ENDMACRO()

# 
# 添加库文件
# 
# dxm_find_add_libraries(MODULE LIBRARIES1 LIBRARIES)
MACRO(dxm_find_add_libraries MODULE)

	# 调整ios的包含;
	LIST(APPEND DXM_FIND_${MODULE}_LIBRARIES ${ARGN})	

ENDMACRO()


