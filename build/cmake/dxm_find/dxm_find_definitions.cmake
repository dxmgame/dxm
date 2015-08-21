

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