# - Find mysqlclient
# Find the native MySQL includes and library
#
#  MYSQL_INCLUDE_DIR  - where to find mysql.h, etc.
#  MYSQL_LIBRARY_DIRS - where to find libraries.
#  MYSQL_LIBRARIES    - List of libraries when using MySQL.
#  MYSQL_CFLAGS       - List of c flags for mysql
#  MYSQL_FOUND        - True if MySQL found.

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	if(((NOT MYSQL_INCLUDE_DIR) OR (NOT MYSQL_LIBRARY_DIRS) OR (NOT MYSQL_LIBRARIES) OR (NOT MYSQL_CFLAGS)) AND (mysql_FIND_REQUIRED))
		execute_process(COMMAND "mysql_config" "--help" OUTPUT_VARIABLE MYSQL_CONFIG)
	elseif(((NOT MYSQL_INCLUDE_DIR) OR (NOT MYSQL_LIBRARY_DIRS) OR (NOT MYSQL_LIBRARIES) OR (NOT MYSQL_CFLAGS)) AND (mysql_FIND_REQUIRED))
		set(MYSQL_CONFIG TRUE)
	endif(((NOT MYSQL_INCLUDE_DIR) OR (NOT MYSQL_LIBRARY_DIRS) OR (NOT MYSQL_LIBRARIES) OR (NOT MYSQL_CFLAGS)) AND (mysql_FIND_REQUIRED))

	if(MYSQL_CONFIG)
		if(NOT MYSQL_INCLUDE_DIR)
			execute_process(COMMAND "mysql_config" "--include" OUTPUT_VARIABLE MYSQL_INCLUDE_DIR)
			string(REGEX REPLACE "^-I(.*[^\n]).*" "\\1" MYSQL_INCLUDE_DIR ${MYSQL_INCLUDE_DIR})
		endif(NOT MYSQL_INCLUDE_DIR)
		
		if(NOT MYSQL_LIBRARY_DIRS)
			execute_process(COMMAND "mysql_config" "--libs" OUTPUT_VARIABLE LIBS_AND_DIR)
			string(REGEX REPLACE "^-L([^-]*).*" "\\1" MYSQL_LIBRARY_DIRS ${LIBS_AND_DIR})
		endif(NOT MYSQL_LIBRARY_DIRS)
		
		if(NOT MYSQL_LIBRARIES)
			execute_process(COMMAND "mysql_config" "--libs" OUTPUT_VARIABLE LIBS_AND_DIR)
			string(REGEX REPLACE "^-L[^-]*([^\n]*).*" "\\1" MYSQL_LIBRARIES ${LIBS_AND_DIR})
		endif(NOT MYSQL_LIBRARIES)

		if(NOT MYSQL_CFLAGS)
			execute_process(COMMAND "mysql_config" "--cflags" OUTPUT_VARIABLE MYSQL_CFLAGS)
			string(REGEX REPLACE "^-I[^-]*(.*[^\n]).*" "\\1" MYSQL_CFLAGS ${MYSQL_CFLAGS})
		endif(NOT MYSQL_CFLAGS)

		separate_arguments(MYSQL_LIBRARIES)
		separate_arguments(MYSQL_CFLAGS)

		# message(${MYSQL_INCLUDE_DIR})
		# message(${MYSQL_LIBRARY_DIRS})
		# message(${MYSQL_LIBRARIES})
		# message(${MYSQL_CFLAGS})

	else(MYSQL_CONFIG)
		if(mysql_FIND_REQUIRED)
			message(FATAL_ERROR "mysql_config not found !")
		elseif(mysql_FIND_REQUIRED)
			message("mysql_config not found !")
		endif(mysql_FIND_REQUIRED)
	endif(MYSQL_CONFIG)

else(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	if(NOT MYSQL_INCLUDE_DIR)
		message("plz, add to CMakeCache.txt: MYSQL_INCLUDE_DIR:FILEPATH=/rep/to/includedir")
	endif(NOT MYSQL_INCLUDE_DIR)

	if(NOT MYSQL_LIBRARY_DIRS)
		message("plz, add to CMakeCache.txt: MYSQL_LIBRARY_DIRS:FILEPATH=/rep/to/libdir")
	endif(NOT MYSQL_LIBRARY_DIRS)

	set(MYSQL_LIBRARIES mysqlclient_r mysqlclient pthread z m rt	ssl	crypto dl)
	set(MYSQL_CFLAGS "-fPIC -pipe -fstack-protector --param=ssp-buffer-size=4 -D_FORTIFY_SOURCE=2 -fno-strict-aliasing -DBIG_JOINS=1 -fomit-frame-pointer -g")

endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY_DIRS)
  set(MYSQL_FOUND TRUE)
else(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY_DIRS)
  set(MYSQL_FOUND FALSE)
endif(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY_DIRS)

IF (MYSQL_FOUND)
  MESSAGE(STATUS "Found MySQL: ${MYSQL_LIBRARY_DIRS}")
ELSE (MYSQL_FOUND)
  IF (mysql_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could NOT find MySQL library")
  ENDIF (mysql_FIND_REQUIRED)
ENDIF (MYSQL_FOUND)

MARK_AS_ADVANCED(
	MYSQL_LIBRARIES
	MYSQL_INCLUDE_DIR
	MYSQL_CFLAGS
	MYSQL_LIBRARY_DIRS
	)