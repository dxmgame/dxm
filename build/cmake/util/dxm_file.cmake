# 
# mkdir 创建文件夹
# 
function(dxm_file_mkdir DIR)
	FILE( MAKE_DIRECTORY ${DIR} )
endfunction()

#
# cp 拷贝文件或者文件夹
#    	文件夹拷文件夹
#			文件拷文件夹
#     文件拷文件
# 
function(dxm_file_cp SOURCE DEST)

	IF(IS_ABSOLUTE ${DEST})
		set(DEST_TEMP ${DEST})
	ELSE()
		set(DEST_TEMP ${CMAKE_CURRENT_SOURCE_DIR}/${DEST})
	ENDIF()
	
	if(IS_ABSOLUTE ${SOURCE})
		set(SOURCE_TEMP ${SOURCE})
	else()
		set(SOURCE_TEMP ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE})
	endif()
	
	if(IS_DIRECTORY ${SOURCE_TEMP})
		# 文件夹拷文件夹
		if( NOT (EXISTS ${DEST_TEMP}) OR IS_DIRECTORY ${DEST_TEMP})
			file( COPY ${SOURCE_TEMP} DESTINATION ${DEST_TEMP} PATTERN *.svn EXCLUDE )
		else()
			message("FATAL_ERROR: cp " ${SOURCE} " " ${DEST} " XX文件夹复制失败，目标非文件夹")
		endif()
	else()
		if( EXISTS ${SOURCE_TEMP})
			# 文件拷贝到文件夹或者文件拷贝到某文件夹并改名
			if( (EXISTS ${DEST_TEMP}) AND (IS_DIRECTORY ${DEST_TEMP}))
				file( COPY ${SOURCE} DESTINATION ${DEST_TEMP} PATTERN *.svn EXCLUDE )
			else()
				if(${DEST_TEMP} MATCHES ".*/$")
					file( COPY ${SOURCE} DESTINATION ${DEST_TEMP} PATTERN *.svn EXCLUDE )
				else()
					get_filename_component( DEST_DIR ${DEST_TEMP} PATH)
					get_filename_component( DEST_NAME ${DEST_TEMP} NAME)
					get_filename_component( SOURCE_NAME ${SOURCE} NAME)
					file( COPY ${SOURCE} DESTINATION ${DEST_DIR} PATTERN *.svn EXCLUDE )
					if( (NOT DEST_NAME) OR (NOT ${SOURCE_NAME} STREQUAL ${DEST_NAME}) )
						dxm_file_mv( ${DEST_DIR}/${SOURCE_NAME} ${DEST_TEMP})
					endif()
				endif()
			endif()
		else()
			# GLOB 拷贝到文件夹
			if( EXISTS ${DEST_TEMP} )
				if( NOT (IS_DIRECTORY ${DEST_TEMP}) )
					message("FATAL_ERROR: cp " ${SOURCE} " " ${DEST} " GLOB FILES CAN'T COPY 2 NOT DIRECTORY")
				endif()
			endif()

			file( GLOB FILES ${SOURCE_TEMP})
	
			foreach( ONE_FILE ${FILES})
				get_filename_component( ONE_FILE_NAME ${ONE_FILE} NAME)
				if( DEST_TEMP )
					if(IS_DIRECTORY ${ONE_FILE})
						if(${DEST_TEMP} MATCHES ".*/$")
							dxm_file_cp( ${ONE_FILE}  ${DEST_TEMP})
						else()
							dxm_file_cp( ${ONE_FILE}  ${DEST_TEMP}/)
						endif()
					else()
						if(${DEST_TEMP} MATCHES ".*/$")
							dxm_file_cp( ${ONE_FILE}  ${DEST_TEMP}${ONE_FILE_NAME})
						else()
							dxm_file_cp( ${ONE_FILE}  ${DEST_TEMP}/${ONE_FILE_NAME})
						endif()
					endif()
				endif()
				
			endforeach()
		endif()
	endif()
endfunction()

function(dxm_file_copy SOURCE DEST)
	dxm_file_cp(${SOURCE} ${DEST})
endfunction()
#
# mv 移动
# 
function(dxm_file_mv OLD_FILE NEW_FILE)
	file(RENAME ${OLD_FILE} ${NEW_FILE})
endfunction()

#
# rm 删除文件
# 
function(dxm_file_rm FILE)
	file(REMOVE ${FILE})
endfunction()

#
# rmdir 删除文件夹
#
function(dxm_file_rmdir DIR)
	file(REMOVE_RECURSE ${DIR})
endfunction()

#
# mvdir 移动文件夹
#
function(dxm_file_mvdir OLD_DIR NEW_DIR)
	file(RENAME ${OLD} ${NEW})
endfunction()

# 
# dxm_file_md5file 计算file的md5
#
function(dxm_file_md5file FILE_NAME MD5_VALUE)
	file(MD5 ${FILE_NAME} FILE_MD5_VALUE)
	set(NEW_STRING_TEMP ${${MD5_VALUE}}${FILE_MD5_VALUE})
	string(MD5 RESULT_VALUE ${NEW_STRING_TEMP})
	SET(${MD5_VALUE} ${RESULT_VALUE} PARENT_SCOPE)
	# MESSAGE(STATUS ${FILE_NAME}:)
	# MESSAGE(STATUS ${${MD5_VALUE}}-${FILE_MD5_VALUE}-${RESULT_VALUE})
endfunction()

# 
# dxm_file_md5dir 计算dir的md5
#
function(dxm_file_md5dir DIR_NAME MD5_VALUE)
	set(FULL_DIR ${DIR_NAME})
	# MESSAGE(STATUS ${FULL_DIR})
	# 检测本路径是否为文件夹
	if( (EXISTS ${FULL_DIR}) AND (IS_DIRECTORY ${FULL_DIR}) )
		# 遍历子文件夹
		file(GLOB TEMP_FILES ${FULL_DIR}/*)
		foreach(TEMP_FILE ${TEMP_FILES})
			if(IS_DIRECTORY ${TEMP_FILE})
				get_filename_component( TEMP_DIR ${TEMP_FILE} NAME )
				dxm_file_md5dir(${TEMP_FILE} ${MD5_VALUE})
			else()
				dxm_file_md5file(${TEMP_FILE} ${MD5_VALUE})
			endif()
		endforeach()
	endif()
	SET(${MD5_VALUE} ${${MD5_VALUE}} PARENT_SCOPE)
endfunction()

# tar( tar_file_name ...)
#
# 运行tar 解压tar包
#
function(internal_tar_win32)
	# 查找是否存在7za程序，没找到则退出程序
	set(TAR_PROGRAM ${DXM_CMAKE_TOOL_DIRECTORY}/win/7za.exe)
	if(EXISTS ${TAR_PROGRAM})
		MESSAGE(STATUS "# tar_program: " ${TAR_PROGRAM} " FOUND")
	else()
		MESSAGE(FATAL_ERROR "# tar_program: " ${TAR_PROGRAM} " NOT_FOUND")
	endif()
			
	foreach(var ${ARGN})
		if(IS_ABSOLUTE ${var})
			set(TAR_FILE ${var})
		else()
			set(TAR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${var})
		endif()
		
		get_filename_component(TAR_PATH ${TAR_FILE} PATH)
		message(STATUS "# tar " ${TAR_FILE} " ...")
		if( EXISTS ${TAR_FILE})
			
			# 执行7za程序
			execute_process(COMMAND ${TAR_PROGRAM} x -y ${TAR_FILE}
				OUTPUT_VARIABLE OUT 
				RESULT_VARIABLE RLT
				ERROR_VARIABLE ERR
				WORKING_DIRECTORY ${TAR_PATH}
				)
				
			# 打印结果
			if( NOT RLT )
				# message(STATUS "# tar "$TAR_TEMP_FILE" result:" ${OUT})
			else( )
				message(FATAL_ERROR "# TAR_PROGRAM: " ${TAR_PROGRAM} " NOT_FOUND")
			endif( )
			
			# 查找中间 .tar 文件，存储到 TAR_TEMP_FILE
			set(TAR_TEMP_FILE "")
			if(NOT (${TAR_FILE} MATCHES ".*tar$"))
				string(REGEX MATCH ".*[.]tar" TAR_TEMP_FILE ${TAR_FILE})
			endif()
			
			# message(STATUS "# TEMP_TAR_FILE: " ${TAR_TEMP_FILE})
		
			# 如果存在 TAR_TEMP_FILE 则继续解压
			if(EXISTS ${TAR_TEMP_FILE})
				execute_process(COMMAND ${TAR_PROGRAM} x -y ${TAR_TEMP_FILE}
					OUTPUT_VARIABLE OUT 
					RESULT_VARIABLE RLT
					ERROR_VARIABLE ERR
					WORKING_DIRECTORY ${TAR_PATH}
					)	
				
				if( NOT RLT )
					# message(STATUS "# tar " ${TAR_TEMP_FILE} " result:" ${OUT})
				else( )
					message(FATAL_ERROR "# TAR_PROGRAM: " ${TAR_TEMP_FILE} " NOT_FOUND")
				endif()
				
				# 删除临时文件
				file( REMOVE_RECURSE ${TAR_TEMP_FILE} )
			endif()
		else()
			message(FATAL_ERROR "# tar fail " ${TAR_FILE} ": NOT_EXISTS")
		endif()
	endforeach( var )
endfunction()

function(internal_tar_unix)
	set(TAR_PROGRAM /usr/bin/tar)
	if(EXISTS ${TAR_PROGRAM})
		MESSAGE(STATUS "# tar_program: " ${TAR_PROGRAM} " FOUND")
	else()
		set(TAR_PROGRAM /bin/tar)
		if(EXISTS ${TAR_PROGRAM})
		MESSAGE(STATUS "# tar_program: " ${TAR_PROGRAM} " FOUND")
		else()
		MESSAGE(FATAL_ERROR "# tar_program: " ${TAR_PROGRAM} " NOT_FOUND")
		endif()
	endif()
	
	foreach(var ${ARGN})
		
		if(IS_ABSOLUTE ${var})
			set(TAR_FILE ${var})
		else()
			set(TAR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${var})
		endif()
		
		get_filename_component(TAR_PATH ${TAR_FILE} PATH)
		
		MESSAGE(STATUS "# tar " ${TAR_FILE})
		
		if( EXISTS ${TAR_FILE})
			execute_process(COMMAND ${TAR_PROGRAM} xvf ${TAR_FILE}
				OUTPUT_VARIABLE OUT 
				RESULT_VARIABLE RLT
				ERROR_VARIABLE ERR
				WORKING_DIRECTORY ${TAR_PATH})
			
			if( NOT RLT )
					# message(STATUS "# tar " ${TAR_FILE} " result:" ${OUT})
			else( )
					message(FATAL_ERROR "# TAR_PROGRAM: " ${TAR_FILE} " FAILED")
			endif()
		endif()
	endforeach( var )
endfunction()

function(dxm_file_tar)
	MESSAGE(STATUS "################")
	MESSAGE(STATUS "# function tar  " ${ARGN})
	
	if(UNIX)
		internal_tar_unix(${ARGN})
	elseif(WIN32 OR ANDROID)
		internal_tar_win32(${ARGN})
	endif()
endfunction()

#
# my_bash：执行批处理命令
#
function(dxm_file_bash NAME WORK_DIR)
	if(WIN32 OR ANDROID)
		execute_process(COMMAND ${NAME}.bat WORKING_DIRECTORY ${WORK_DIR})
	elseif(UNIX)
		execute_process(COMMAND bash ${NAME}.sh WORKING_DIRECTORY ${WORK_DIR})
	endif()
endfunction()



