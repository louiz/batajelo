cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


##########################
# Project definition
##########################

project(batajelo)

set(batajelo_VERSION_MAJOR 0)
set(batajelo_VERSION_MINOR 1)

set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH output_lib)


##########################
# Project compile flags
##########################

# This flag is very important for the fixmath library, if one client is
# compiled without it, its world representation may not be identical with
# the other’s one.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFIXMATH_SIN_LUT")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -pedantic -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra")

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

find_package(Boost 1.48.0 COMPONENTS serialization system date_time unit_test_framework iostreams locale REQUIRED)
find_package(Cryptopp REQUIRED)
find_package(YamlCpp REQUIRED)

if(BUILD_CLIENT)
  find_package(SFML 2 REQUIRED system window graphics audio)
endif()

##########################
# Includes and Linkage
##########################

include_directories("src/")

include_directories(${Boost_INCLUDE_DIR})
include_directories(${CRYPTO++_INCLUDE_DIR})
include_directories(${YAML_CPP_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${CRYPTO++_LIBRARIES})

##########################
# Targets
##########################

if(BUILD_CLIENT)
  add_executable(batajelo_client src/main/batajelo_client.cpp)
  target_link_libraries(batajelo_client
    server
    gui
    )

  add_executable(minimap_generator src/main/minimap_generator.cpp)
  target_link_libraries(minimap_generator
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
## Mpreal
#
file(GLOB source_mpreal
  src/mpreal/mpreal.cpp
  src/mpreal/mpreal.hpp
  )

add_library(mpreal STATIC ${source_mpreal})

target_link_libraries(mpreal
  fixmath
  )

#
## fixmath
#
file(GLOB source_fixmath
  src/fixmath/*.c
  src/fixmath/*.cpp
  )

add_library(fixmath STATIC ${source_fixmath})

target_link_libraries(fixmath
  logging
  )


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
## Mod
#
file(GLOB source_mod
  src/mod/mod.cpp
  )

add_library(mod STATIC ${source_mod})

target_link_libraries(mod
  ${YAML_CPP_LIBRARIES}
  world
  )

#
## Client mod
#
file(GLOB source_clientmod
  src/mod/mod.cpp
  src/mod/client_mod.cpp
  )

add_library(clientmod STATIC ${source_clientmod})

target_link_libraries(clientmod
  ${YAML_CPP_LIBRARIES}
  world
  gui
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
    clientmod
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
  game
  serialization
  mpreal
  mod
  utils
  ${Boost_LIBRARIES}
  )

#
## ClientWorld
#
file(GLOB source_client_world
  src/world/client_world/*.[hc]pp
  )

add_library(client_world STATIC ${source_client_world})

target_Link_libraries(client_world
  world
  gui
  )

#
## Serialization
#
file(GLOB source_serialization
  src/serialization/*.[hc]pp
  )

add_library(serialization STATIC ${source_serialization})

target_Link_libraries(serialization
  logging
  world
  ${Boost_LIBRARIES}
  )

#
## Network server
#

file(GLOB source_server
  src/network/command_handler.*
  src/network/remote_client_base.*
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

target_link_libraries(server
  logging
  config
  game
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
  src/game/*.[ch]pp
  )

add_library(game STATIC ${source_game})

target_link_libraries(game
  serialization
  client
  logging
  config
  client_world
  )

#
## Game client
#
add_library(game_client STATIC ${source_game})

target_link_libraries(game_client
  game
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
      )
  endif(TARGET ${lib})
endforeach(test_src)
