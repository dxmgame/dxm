# 
# 源码管理
# 
# 整理标准源码目录
# 
MACRO( _dxm_builder_module_add_sources )

	# 清理缓存
	_internal_source_clear()

	# 跨平台源码
	_internal_src_import_from_dir( inc .h .hpp .hxx )
	_internal_src_import_from_dir( src .h .hpp .hxx .c .cpp .cxx .cc )
	
	# 独立平台源码
	if(DXM_CMAKE_PLATFORM_WIN32 OR DXM_CMAKE_PLATFORM_WIN64)
		_internal_src_import_from_dir( src.win32/cpp .h .hpp .hxx .c .cpp .cxx .cc )
		_internal_src_import_from_dir( package .pkg .bat )
		_internal_src_import_from_dir( proto .proto )
		_internal_src_import_from_dir( tbl .tbl )	
		_internal_src_import_from_dir( lua .lua )
	elseif(DXM_CMAKE_PLATFORM_UNIX)
		_internal_src_import_from_dir( src.unix/cpp .h .hpp .hxx .c .cpp .cxx .cc )
	elseif(DXM_CMAKE_PLATFORM_IOS)
		_internal_src_import_from_dir( src.ios/cpp .h .hpp .hxx .c .cpp .cxx .cc .m .mm .a )
	elseif(DXM_CMAKE_PLATFORM_ANDROID)
		_internal_src_import_from_dir( src.android/cpp .h .hpp .hxx .c .cpp .cxx .cc )
	endif()
	_internal_source_to_group()
ENDMACRO()

# 
# 整理源码目录结构并保存起来
#	${FILE_DIR} 需要添加的文件夹（递归）
# 	${ARGN} 为后缀名列表
# 
MACRO( _internal_src_import_from_dir SUB_DIR )
	
	# 组合绝对路径
	SET(FULL_DIR ${DXM_MODULE_${DXM_BUILDER_MODULE}_DIRECTORY}/${SUB_DIR})		
	# 检测本路径是否为文件夹
	if( (EXISTS ${FULL_DIR}) AND (IS_DIRECTORY ${FULL_DIR}) )
		
		# 增加源码路径列表
		LIST(APPEND DXM_SRC_SUB_DIRECTORIES ${SUB_DIR})
		
		# 源码添加到具体路径中
		FOREACH(TEMP_FILE_EXTENSION ${ARGN})
			FILE(GLOB TEMP_FILES  "${FULL_DIR}/*${TEMP_FILE_EXTENSION}")
			FOREACH( TEMP_FILE ${TEMP_FILES} )				
				# 增加源码到路径源码列表
				LIST(APPEND DXM_SRC_SUB_DIRECTORY_FILES_${SUB_DIR} ${TEMP_FILE})
				LIST(APPEND DXM_MODULE_${DXM_BUILDER_MODULE}_SOURCES ${TEMP_FILE})
			ENDFOREACH()
		ENDFOREACH()
	
		# 遍历子文件夹
		file(GLOB TEMP_FILES ${FULL_DIR}/*)
		foreach(TEMP_FILE ${TEMP_FILES})
			if( (EXISTS ${TEMP_FILE}) AND (IS_DIRECTORY ${TEMP_FILE}) )
				get_filename_component( TEMP_DIR ${TEMP_FILE} NAME )
				_internal_src_import_from_dir("${SUB_DIR}/${TEMP_DIR}" ${ARGN})
			endif()
		endforeach()
	endif()
ENDMACRO()

# 
# 清理缓存
# 
MACRO( _internal_source_clear )

	# 遍历源码目录
	FOREACH(TEMP_SRC_DIRECTORY ${DXM_SRC_SUB_DIRECTORIES})
		UNSET(DXM_SRC_SUB_DIRECTORY_FILES_${TEMP_SRC_DIRECTORY})
	ENDFOREACH()
	UNSET(DXM_SRC_SUB_DIRECTORIES)
	UNSET(DXM_MODULE_${DXM_BUILDER_MODULE}_SOURCES)
ENDMACRO()

# 
# 源码管理，添加到SOURCE_GROUP
# 
MACRO( _internal_source_to_group )

	# 遍历源码目录，把各自目录下的源码进行分组
	FOREACH(TEMP_SRC_DIRECTORY ${DXM_SRC_SUB_DIRECTORIES})
		dxm_module_add_includes(${DXM_BUILDER_MODULE} ${DXM_MODULE_${DXM_BUILDER_MODULE}_DIRECTORY}/${TEMP_SRC_DIRECTORY})
	
		FOREACH(TEMP_SRC_DIRECTORY_FILE ${DXM_SRC_SUB_DIRECTORY_FILES_${TEMP_SRC_DIRECTORY}})
			STRING(REPLACE "/" "\\" TEMP_SRC_DIRECTORY ${TEMP_SRC_DIRECTORY})
			if(CMAKE_GENERATOR)
				SOURCE_GROUP(${TEMP_SRC_DIRECTORY} FILES ${TEMP_SRC_DIRECTORY_FILE})
			endif()
		ENDFOREACH()
	ENDFOREACH()
ENDMACRO()