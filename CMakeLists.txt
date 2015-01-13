cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


##########################
# Project definition
##########################

project(batajelo)

set(batajelo_VERSION_MAJOR 0)
set(batajelo_VERSION_MINOR 1)

##########################
# Project compile flags
##########################

# This flag is very important for the fixmath library, if one client is
# compiled without it, its world representation may not be identical with
# the other’s one.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFIXMATH_SIN_LUT")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -pedantic -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pedantic -Wall -Wextra")

configure_file("${PROJECT_SOURCE_DIR}/src/config.hpp.in" "${PROJECT_SOURCE_DIR}/src/config.hpp")

##########################
# Command line options
##########################

set(BUILD_SERVER true)
option(BUILD_SERVER "build the server" ${BUILD_SERVER})

set(BUILD_CLIENT true)
option(BUILD_CLIENT "build the server" ${BUILD_CLIENT})


##########################
# Libraries look up
##########################

find_package(Boost 1.48.0 COMPONENTS filesystem system iostreams locale serialization chrono REQUIRED)
find_package(Cryptopp REQUIRED)
find_package(YamlCpp REQUIRED)
find_package(Protobuf REQUIRED)

if(BUILD_CLIENT)
  find_package(SFML 2 REQUIRED system window graphics audio)
endif()

find_package(GTest)

##########################
# Includes and Linkage
##########################

include_directories("src/")

include_directories(${Boost_INCLUDE_DIR})
include_directories(${CRYPTO++_INCLUDE_DIR})
include_directories(${YAML_CPP_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})
if(GTEST_FOUND)
  include_directories(${GTEST_INCLUDE_DIRS})
endif()

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${CRYPTO++_LIBRARIES})

##########################
# Targets
##########################

if(BUILD_CLIENT)
  add_executable(batajelo_client src/main/batajelo_client.cpp)
  target_link_libraries(batajelo_client
    game_client
    client
    gui
    )
endif()

if(BUILD_SERVER)
  add_executable(batajelo_server src/main/batajelo_server.cpp)
  target_link_libraries(batajelo_server
    server
    )
endif(BUILD_SERVER)


#########################
# Project libraries
#########################

#
## Config
#
file(GLOB source_config
  src/config/config.[hc]pp
  )

add_library(config STATIC ${source_config})

target_link_libraries(config
  utils
  )

#
## Logging
#
file(GLOB source_logging
  src/logging/logging.[hc]pp
  )

add_library(logging STATIC ${source_logging})

target_link_libraries(logging
  config
  ${Boost_LIBRARIES}
  )

#
## fixmath
#
file(GLOB source_fixmath
  src/fixmath/*.c
  src/fixmath/*.cpp
  )

add_library(fixmath STATIC ${source_fixmath})

#
## Utils
#
file(GLOB source_utils
  src/utils/*.[hc]pp
  )

add_library(utils STATIC ${source_utils})

target_link_libraries(
  utils
  ${Boost_LIBRARIES}
  )

#
## Gui
#
if(BUILD_CLIENT)
  file(GLOB source_gui
    src/gui/*.[hc]pp
    src/gui/camera/*.[hc]pp
    src/gui/screen/*.[hc]pp
    src/gui/hud/*.[hc]pp
    src/gui/sprites/*.[hc]pp
    )

  add_library(gui STATIC ${source_gui})

  target_Link_libraries(gui
    logging
    config
    world
    utils
    ${SFML_LIBRARIES}
    ${Boost_LIBRARIES}
    )
endif()

#
## World
#
file(GLOB source_world
  src/world/*.[hc]pp
  )

add_library(world STATIC ${source_world})

target_Link_libraries(world
  logging
  serialization
  fixmath
  utils
  ${Boost_LIBRARIES}
  )

#
## Serialization
#
file(GLOB proto_files
  src/proto/*.proto
  )
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HDRS ${proto_files})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(serialization STATIC ${PROTO_SRC})
target_link_libraries(serialization ${PROTOBUF_LIBRARIES})


#
## Network server
#

file(GLOB source_server
  src/network/*.[hc]pp
  )

add_library(server STATIC ${source_server})

target_link_libraries(server
  logging
  config
  game
  world
  ${Boost_LIBRARIES}
  )

#
## Network client
#
file(GLOB source_client
  src/network/client.*
  src/network/game_client.*
  src/network/client_base.*
  src/network/command_handler.*
  src/network/transfer_receiver.*
  src/network/command.*
  src/network/timed_event.*
  src/network/timed_event_handler.*
  src/network/ping_handler.*
  )

add_library(client STATIC ${source_client})

target_link_libraries(client
  logging
  config
  )

add_library(network STATIC ${source_server} ${source_client})

#
## Game
#
file(GLOB source_game
  src/game/game.[ch]pp
  src/game/game_server.[ch]pp
  src/game/occupant.[ch]pp
  src/game/occupants_handler.[ch]pp
  src/game/remote_game_client.[ch]pp
  src/game/replay.[ch]pp
  src/game/selection.[ch]pp
  src/game/time.[ch]pp
  src/game/turn.[ch]pp
  src/game/turn_handler.[ch]pp
  )

add_library(game STATIC ${source_game})

target_link_libraries(game
  serialization
  network
  client
  logging
  config
  world
  )

#
## Game client
#
file(GLOB source_game_client
  src/game/game_client.[ch]pp
  )

add_library(game_client STATIC ${source_game_client})

target_link_libraries(game_client
  game
  gui
  )

#
## Menu client
#
add_library(menu_client STATIC ${source_game})

target_link_libraries(menu_client
  game
  )

#
## Server
#
if(BUILD_SERVER)
  add_library(game_server STATIC ${source_server})

  target_link_libraries(game_server
    logging
    config
    world
    ${Boost_LIBRARIES}
    )
endif()

####################
# Tests
####################

if(GTEST_FOUND)
  enable_testing()
  add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -V)

  file(GLOB tests_srcs
    tests/*test.cpp
    )

  foreach(test_src ${tests_srcs})
    get_filename_component(lib ${test_src} NAME_WE)

    string(REGEX REPLACE "(.*)_test$" "\\1" lib ${lib})

    if(TARGET ${lib})
      add_executable(${lib}_test  EXCLUDE_FROM_ALL ${test_src})

      target_link_libraries(${lib}_test
        ${lib}
        ${GTEST_BOTH_LIBRARIES}
        )
      add_test(${lib}_test ${lib}_test)
      add_dependencies(check ${lib}_test)
    else()
      message("Library ${lib} does not exist, ${lib}_test.cpp will not be enabled.")
    endif()
  endforeach(test_src)
endif()
