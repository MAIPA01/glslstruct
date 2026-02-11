cmake_minimum_required(VERSION 3.24)
project(glslstruct VERSION 1.1.0 LANGUAGES CXX)

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
option(BUILD_GLSL_STRUCT_COVERAGE "Enable coverage reporting" OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glslstruct)

if (BUILD_GLSL_STRUCT_TESTS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if (BUILD_GLSL_STRUCT_COVERAGE)
    #set(PROFRAW_MAIN "glslstruct.profraw")
    set(PROFRAW_TESTS "glslstruct-tests.profraw")
    set(PROFDATA_FILE "coverage.profdata")

    #add_custom_target(run_main_program
        #COMMAND LLVM_PROFILE_FILE=${glslstruct_BINARY_DIR}/${PROFRAW_MAIN} ./my_program
        #COMMENT "Running main program to generate ${PROFRAW_MAIN}"
        #DEPENDS glslstruct
        #VERBATIM
    #)

    add_custom_target(run_tests
        COMMAND set LLVM_PROFILE_FILE=${glslstruct-tests_BINARY_DIR}/${PROFRAW_TESTS}
        COMMAND "${glslstruct-tests_BINARY_DIR}/glslstruct-tests.exe"
        COMMENT "Running tests to generate ${PROFRAW_TESTS}"
        DEPENDS glslstruct-tests
        VERBATIM
    )

    add_custom_target(merge_coverage_data
        COMMAND llvm-profdata merge -sparse -output=${CMAKE_CURRENT_BINARY_DIR}/${PROFDATA_FILE} ${glslstruct-tests_BINARY_DIR}/${PROFRAW_TESTS} #${CMAKE_BINARY_DIR}/${PROFRAW_MAIN}
        COMMENT "Merging coverage data into ${PROFDATA_FILE}"
        DEPENDS run_tests #run_main_program
        VERBATIM
    )

    add_custom_target(generate_coverage_report
        COMMAND llvm-cov show ${glslstruct-tests_BINARY_DIR}/glslstruct-tests.exe -instr-profile=${CMAKE_CURRENT_BINARY_DIR}/${PROFDATA_FILE} -format=html -output-dir=${CMAKE_CURRENT_BINARY_DIR}/coverage_report
        COMMENT "Generating HTML coverage report in ${CMAKE_CURRENT_BINARY_DIR}/coverage_report"
        DEPENDS merge_coverage_data
        VERBATIM
    )
endif()