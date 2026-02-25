# CMake Stuff
cmake_minimum_required(VERSION 3.28)
include("cmake/create_lib.cmake" PUBLIC)
set(LIB_PROJECT_NAME playground)

# C/C++ Stuff
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Vim Stuff
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project Base Stuff
project(antimony-${LIB_PROJECT_NAME}) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.bin)

# Submodules (The Foundation)
add_subdirectory(extern/sb-types)
add_subdirectory(extern/sb-logger)
add_subdirectory(extern/sb-event)

# Database project components
add_subdirectory("include")
add_subdirectory("src")

if(BUILD_EXAMPLES STREQUAL "yes")
  add_subdirectory(examples)
endif()
