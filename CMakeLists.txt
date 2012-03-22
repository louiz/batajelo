cmake_minimum_required(VERSION 2.6)

project(batajelo)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

add_subdirectory(src/network)

add_executable(batajelo src/main.cpp)

target_link_libraries(
	batajelo
	server
)
