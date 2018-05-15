cmake_minimum_required (VERSION 3.8.2)
project (logo-recognizer)

# enable C and C++14 language
enable_language (C CXX)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wno-sign-compare -g")

message(STATUS "Start of CMake execution for Logo Recognizer")

# location of additional cmake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    )

# detect operating system
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_definitions(-DSYSTEM_LINUX)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DSYSTEM_WINDOWS)
endif()

# detect host processor
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#-------------------------------------------------------------------------------
# EXECUTABLE SOURCES SETTING
#-------------------------------------------------------------------------------

set (RECOGNIZER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (RECOGNIZER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set (RECOGNIZER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
    )

add_executable (logo-recognizer ${RECOGNIZER_SOURCES})

target_include_directories (logo-recognizer PUBLIC
    ${RECOGNIZER_INCLUDE_DIR}
    )

# target to update git submodules
add_custom_target(
    update_submodules
    COMMAND git submodule init
    COMMAND git submodule update
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

#-------------------------------------------------------------------------------
# EXTERNAL LIBRARIES
#-------------------------------------------------------------------------------

find_package (OpenCV REQUIRED)

target_link_libraries (logo-recognizer ${OpenCV_LIBS})
target_link_libraries (logo-recognizer stdc++fs)