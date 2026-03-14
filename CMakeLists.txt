cmake_minimum_required(VERSION 3.30)
project(glslstruct VERSION 1.3.0 LANGUAGES CXX)

# variable for building shared library
option(BUILD_SHARED_LIBS "Build shared library" OFF)
option(GLSL_STRUCT_BUILD_TESTS "Build glsl struct tests" OFF)
option(GLSL_STRUCT_BUILD_COVERAGE "Enable coverage reporting" OFF)
option(GLSL_STRUCT_BUILD_DOCUMENTATIONS "Build glsl documentation" OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/coverage_report.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/clang_tidy.cmake)
if (GLSL_STRUCT_BUILD_DOCUMENTATION)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glslstruct)

if (GLSL_STRUCT_BUILD_TESTS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if (GLSL_STRUCT_BUILD_COVERAGE)
    setup_coverage_report(
		TEST_PROJECT glslstruct-tests
		FORMAT html
		OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/coverage_report
		CLEAN_TEMP
	)
endif()