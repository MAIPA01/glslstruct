cmake_minimum_required(VERSION 3.30)
project(glslstruct VERSION 1.4.7 LANGUAGES CXX)

# variable for building shared library
option(GLSL_STRUCT_BUILD_TESTS 				"Build glsl struct tests" 		${PROJECT_IS_TOP_LEVEL})
option(GLSL_STRUCT_BUILD_COVERAGE 			"Enable coverage reporting" 	${PROJECT_IS_TOP_LEVEL})
option(GLSL_STRUCT_BUILD_DOCUMENTATION	 	"Build glsl documentation" 		${PROJECT_IS_TOP_LEVEL})

option(GLSL_STRUCT_ENABLE_CLANG_TIDY 		"Enables clang-tidy checks" 	${PROJECT_IS_TOP_LEVEL})

option(GLSL_STRUCT_INSTALL 					"Enables installation of this project"						${PROJECT_IS_TOP_LEVEL})
option(GLSL_STRUCT_INSTALL_TEST				"This is only to test if installation of glslstruct works" 	OFF)

option(GLSL_STRUCT_MSTD_EXTERNAL 			"Uses users own mstd library (tested and compatible with: 1.5.5)"		OFF)
option(GLSL_STRUCT_GLM_EXTERNAL 			"Uses users own glm library (tested and compatible with: 1.0.3)"		OFF)
option(GLSL_STRUCT_PCRE2CPP_EXTERNAL        "Uses users own pcre2cpp library (tested and compatible with: 1.2.9)" 	OFF)

option(GLSL_STRUCT_ENABLE_CXX20 				"Enables C++20 features" 											OFF)
option(GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE 	"Disables assert on release builds" 								OFF)
option(GLSL_STRUCT_DISABLE_TYPES 				"Disables type classes" 											OFF)
option(GLSL_STRUCT_ENABLE_TYPE_CHECKS 			"Enables type checking (GLSL_STRUCT_DISABLE_TYPES should be OFF)"	OFF)
option(GLSL_STRUCT_DISABLE_PARSER 				"Disables parsers functions"										OFF)

message(STATUS "GLSL_STRUCT_ENABLE_CXX20: ${GLSL_STRUCT_ENABLE_CXX20}")
message(STATUS "GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE: ${GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE}")
message(STATUS "GLSL_STRUCT_DISABLE_TYPES: ${GLSL_STRUCT_DISABLE_TYPES}")
message(STATUS "GLSL_STRUCT_ENABLE_TYPE_CHECKS: ${GLSL_STRUCT_ENABLE_TYPE_CHECKS}")
message(STATUS "GLSL_STRUCT_DISABLE_PARSER: ${GLSL_STRUCT_DISABLE_PARSER}")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)

if (GLSL_STRUCT_ENABLE_CLANG_TIDY)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/clang_tidy.cmake)
endif()

if (GLSL_STRUCT_BUILD_DOCUMENTATION)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake)
endif()

if(NOT GLSL_STRUCT_INSTALL_TEST)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glslstruct)
endif()

if (GLSL_STRUCT_BUILD_TESTS OR GLSL_STRUCT_BUILD_COVERAGE)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if (GLSL_STRUCT_BUILD_COVERAGE)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/coverage_report.cmake)

    setup_coverage_report(
		TEST_TARGET glslstruct-tests
		FORMAT html
		OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/coverage_report
		CLEAN_TEMP
	)
endif()