cmake_minimum_required(VERSION 2.6)

project(batajelo)
set(batajelo_VERSION_MAJOR 0)
set(batajelo_VERSION_MINOR 0)

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

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

add_subdirectory(src/network)

add_executable(batajelo src/main.cpp)

target_link_libraries(
	batajelo
	server
)
