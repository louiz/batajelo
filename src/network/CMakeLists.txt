cmake_minimum_required(VERSION 2.6)

include(FindBoost)
find_package(
	Boost 
	1.49.0
	REQUIRED system
)

find_package(Threads)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#à modifier c’est nimp
file(
	GLOB
	source_server
	../logging/logging*
	../config/config*
	remote_client*
	command_handler*
	server.cpp
	server.h
)

add_library(server STATIC ${source_server})
add_executable(test_server EXCLUDE_FROM_ALL main.cpp)

target_link_libraries(
	server
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(
	test_server
	server
)
