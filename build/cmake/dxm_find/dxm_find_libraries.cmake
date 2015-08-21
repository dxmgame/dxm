
# 
# 添加库文件
# 
# dxm_find_add_libraries(MODULE LIBRARIES1 LIBRARIES)
MACRO(dxm_find_add_libraries MODULE)

	# 调整ios的包含;
	LIST(APPEND DXM_FIND_${MODULE}_LIBRARIES ${ARGN})	

ENDMACRO()


