cmake_minimum_required(VERSION 3.24)
project(glslstruct VERSION 1.0 LANGUAGES CXX)

option(GLSL_STRUCT_ENABLE_CXX20 OFF)

if (GLSL_STRUCT_ENABLE_CXX20)
	set(CMAKE_CXX_STANDARD 20)
else()
	set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# variable for building shared library
option(BUILD_SHARED_LIBS "Build shared library" OFF)
option(BUILD_GLSL_STRUCT_TESTS "Build glsl struct tests" OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glslstruct)

if (BUILD_GLSL_STRUCT_TESTS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glslstruct-tests)
endif()