cmake_minimum_required(VERSION 2.6)

project(batajelo)


#Variable
set(batajelo_VERSION_MAJOR 0)
set(batajelo_VERSION_MINOR 0)
set(EXECUTABLE_OUTPUT_PATH bin)


#Documentation doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file(
    "${PROJECT_SOURCE_DIR}/doxygen/Doxyfile.in"
    "${PROJECT_SOURCE_DIR}/doxygen/Doxyfile")
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doxygen
    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)


#Librairies to include
find_package(Boost 1.48.0	REQUIRED system date_time COMPONENTS unit_test_framework)
find_package(Threads)


include_directories(${Boost_INCLUDE_DIRS})
include_directories(${MYSQL_INCLUDE_DIRS})
include_directories("src/")


link_directories(${Boost_LIBRARY_DIRS})
link_directories(${MYSQL_LIBRARY_DIRS})

set(mysql_libs mysqlclient_r mysqlclient pthread z m rt	ssl	crypto dl)

set(mysql_cflags "-fPIC -pipe -fstack-protector --param=ssp-buffer-size=4 -D_FORTIFY_SOURCE=2 -fno-strict-aliasing -DBIG_JOINS=1 -fomit-frame-pointer -g")

#Principal build
add_executable(batajelo_server src/main/batajelo_server.cpp)
add_executable(batajelo_client src/main/batajelo_client.cpp)


target_link_libraries(
	batajelo_server
	server
)

target_link_libraries(
	batajelo_client
	server
)

#Library build
file(
	GLOB
	source_logging
	src/logging/logging.*
)
add_library(logging STATIC ${source_logging})

target_link_libraries(
	logging
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)

file(
	GLOB
	source_config
	src/config/config.*
)
add_library(config STATIC ${source_config})

target_link_libraries(
	config
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)

file(
	GLOB
	source_server
	src/network/remote_client.*
	src/network/command_handler.*
	src/network/server.*
)
add_library(server STATIC ${source_server})

target_link_libraries(
	server
	logging
	config
)


file(
	GLOB
	source_database
	src/database/*.[ch]pp
)
add_library(database STATIC ${source_database})
set_target_properties(database PROPERTIES COMPILE_FLAGS ${mysql_cflags})

target_link_libraries(
	database
	logging
	config
	${mysql_libs}
)

#Module build
file(
	GLOB
	modules_srcs
	tests/*main.cpp
)

foreach(module_src ${modules_srcs})

	get_filename_component(lib ${module_src} NAME_WE) 
	string(REGEX REPLACE "(.*)_main$" "\\1" lib ${lib})
	
	add_executable(module_${lib} EXCLUDE_FROM_ALL ${module_src})

	target_link_libraries(
		module_${lib}
		${lib}
		)

endforeach(module_src)

#Test
file(
	GLOB
	tests_srcs
	tests/*test.cpp
)

foreach(test_src ${tests_srcs})

	get_filename_component(lib ${test_src} NAME_WE) 
	string(REGEX REPLACE "(.*)_test$" "\\1" lib ${lib})

	add_executable(test_${lib} EXCLUDE_FROM_ALL ${test_src})

	target_link_libraries(
		test_${lib}
		${lib}
		)


endforeach(test_src)
