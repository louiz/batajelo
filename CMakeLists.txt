cmake_minimum_required(VERSION 2.6)

project(batajelo)

#Variable
set(batajelo_VERSION_MAJOR 0)
set(batajelo_VERSION_MINOR 1)
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



if(NOT DEFINED BUILD_SERVER)
	set(BUILD_SERVER true CACHE BOOL "build the server")
	message("You can make BUILD_SERVER to false in CMakeCache.txt, if you only want to build the client")
endif(NOT DEFINED BUILD_SERVER)

if(NOT DEFINED BUILD_CLIENT)
	set(BUILD_CLIENT true CACHE BOOL "build the client")
	message("You can make BUILD_CLIENT to false in CMakeCache.txt, if you only want to build the server")
endif(NOT DEFINED BUILD_CLIENT)


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
find_package(Boost 1.47.0 COMPONENTS serialization system date_time unit_test_framework REQUIRED)
find_package(Threads REQUIRED)



include_directories(${Boost_INCLUDE_DIR})
include_directories("src/")

link_directories(${Boost_LIBRARY_DIRS})

if(BUILD_CLIENT)
	find_package(SFML 2 REQUIRED system window graphics audio)
	find_package(SFGUI REQUIRED)
	include_directories(${SFML_INCLUDE_DIR})
	link_directories(${SFML_LIBRARIES})

	include_directories(${SFGUI_INCLUDE_DIR})
	link_directories(${SFGUI_LIBRARY})
endif(BUILD_CLIENT)

if(BUILD_SERVER)
	find_package(mysql REQUIRED)
	include_directories(${MYSQL_INCLUDE_DIR})
	link_directories(${MYSQL_LIB_DIR})
endif(BUILD_SERVER)

#Principal build

if(BUILD_CLIENT)
	add_executable(batajelo_client src/main/batajelo_client.cpp)
	target_link_libraries(
		batajelo_client
		server
		)
endif(BUILD_CLIENT)

if(BUILD_SERVER)
	add_executable(batajelo_server src/main/batajelo_server.cpp)
	target_link_libraries(
		batajelo_server
		server
		)
endif(BUILD_SERVER)



#Library build

file(
	GLOB
	source_logging
	src/logging/logging.*
)
add_library(logging STATIC ${source_logging})

target_link_libraries(
	logging
	config
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


if(BUILD_CLIENT)
file(
	GLOB
	source_gui
	src/gui/*.[hc]pp
	src/gui/menu/*.[hc]pp
	src/gui/camera/*.[hc]pp
    	)
add_library(gui STATIC ${source_gui})

target_Link_libraries(
	gui
	logging
	config
	world
	${SFML_LIBRARIES}
	${SFGUI_LIBRARY}
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	)
endif(BUILD_CLIENT)

file(
    GLOB
    source_world
    src/world/*.[hc]pp
)
add_library(world STATIC ${source_world})

target_Link_libraries(
	world
	game
	logging
	serialization
	${SFML_LIBRARIES}
	${SFGUI_LIBRARY}
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)

file(
    GLOB
    source_serialization
    src/serialization/*.[hc]pp
)
add_library(serialization STATIC ${source_serialization})

target_Link_libraries(
	serialization
	logging
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)


	file(
		GLOB
		source_client
		src/network/client.*
		src/network/game_client.*
		src/network/interface_client.*
		src/network/command_handler.*
		src/network/transfer_receiver.*
		src/network/command.*
		src/network/timed_event.*
		src/network/timed_event_handler.*
		src/network/ping_handler.*
		)
	add_library(client STATIC ${source_client})

	add_library(network STATIC ${source_server} ${source_client})

	target_link_libraries(
		client
		logging
		config
		)

	file(
		GLOB
		source_game
		src/game/*.[ch]pp
		)
	add_library(game STATIC ${source_game})

	target_link_libraries(
		game
		serialization
		client
		logging
		config
		gui
		world
		)

	add_library(game_client STATIC ${source_game})

	target_link_libraries(
		game_client
		game
		)

	add_library(menu_client STATIC ${source_game})
	target_link_libraries(
		menu_client
		game
		)

if(BUILD_SERVER)

	file(
		GLOB
		source_server
		src/network/command_handler.*
		src/network/interface_remote_client.*
		src/network/remote_client.*
		src/network/remote_game_client.*
		src/network/server.*
		src/network/game_server.*
		src/network/transfer_sender.*
		src/network/command.*
		src/network/timed_event.*
		src/network/timed_event_handler.*
		src/network/ping_handler.*
		)
	add_library(server STATIC ${source_server})
	target_link_libraries(
		server
		logging
		config
		game
		database
		${SFML_LIBRARIES}
		${SFGUI_LIBRARY}
		${Boost_LIBRARIES}
		${CMAKE_THREAD_LIBS_INIT}
		)

	add_library(game_server STATIC ${source_server})
	target_link_libraries(
		game_server
		logging
		config
		game
		database
		${SFML_LIBRARIES}
		${SFGUI_LIBRARY}
		${Boost_LIBRARIES}
		${CMAKE_THREAD_LIBS_INIT}
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

endif(BUILD_SERVER)

#Module build
file(
	GLOB
	modules_srcs
	tests/*main.cpp
)

foreach(module_src ${modules_srcs})

	get_filename_component(lib ${module_src} NAME_WE)
	string(REGEX REPLACE "(.*)_main$" "\\1" lib ${lib})

	if(TARGET ${lib})
	add_executable(${lib}_main  EXCLUDE_FROM_ALL ${module_src})

	target_link_libraries(
		${lib}_main
		${lib}
		)
	endif(TARGET ${lib})
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

	if(TARGET ${lib})
		add_executable(${lib}_test  EXCLUDE_FROM_ALL ${test_src})

		target_link_libraries(
			${lib}_test
			${lib}
			)
	endif(TARGET ${lib})

endforeach(test_src)
