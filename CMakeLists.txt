cmake_minimum_required(VERSION 2.6)

project(batajelo)

#Variable
set(batajelo_VERSION_MAJOR 0)
set(batajelo_VERSION_MINOR 0)
set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH output_lib)

if(${CMAKE_BUILD_TYPE} MATCHES "debug")
	message("debug")
	add_definitions(-g3 -W -Wall -pedantic)
	set(DEFINE_DEBUG 1)
else(${CMAKE_BUILD_TYPE} MATCHES "debug")
	message("release")
	add_definitions(-O3)
	set(DEFINE_DEBUG 0)
endif(${CMAKE_BUILD_TYPE} MATCHES "debug")
configure_file("${PROJECT_SOURCE_DIR}/src/constant.hpp.in" "${PROJECT_SOURCE_DIR}/src/constant.hpp")

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
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(Boost 1.48.0 COMPONENTS system date_time unit_test_framework REQUIRED)
find_package(mysql REQUIRED)
find_package(Threads REQUIRED)


include_directories(${Boost_INCLUDE_DIR})
include_directories(${MYSQL_INCLUDE_DIR})
include_directories("src/")

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${MYSQL_LIBRARY_DIRS})


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
set_target_properties(database PROPERTIES COMPILE_FLAGS ${MYSQL_CXXFLAGS})

target_link_libraries(
	database
	logging
	config
	${MYSQL_LIBRARIES}
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
	
	add_executable(${lib}_main  EXCLUDE_FROM_ALL ${module_src})

	target_link_libraries(
		${lib}_main
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

	add_executable(${lib}_test  EXCLUDE_FROM_ALL ${test_src})

	target_link_libraries(
		${lib}_test
		${lib}
		)


endforeach(test_src)
