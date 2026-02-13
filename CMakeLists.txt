cmake_minimum_required(VERSION 3.24)

# variable for building shared library
option(BUILD_SHARED_LIBS "Build shared library" OFF)
option(BUILD_GLSL_STRUCT_TESTS "Build glsl struct tests" OFF)
option(BUILD_GLSL_STRUCT_COVERAGE "Enable coverage reporting" OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/coverage_report.cmake)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glslstruct)

if (BUILD_GLSL_STRUCT_TESTS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if (BUILD_GLSL_STRUCT_COVERAGE)
    setup_coverage_report(
		TEST_PROJECT glslstruct-tests
		FORMAT html
		OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/coverage_report
		CLEAN_TEMP
	)
endif()