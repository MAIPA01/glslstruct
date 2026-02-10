project(glslstruct-tests VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(GLSL_STRUCT_ENABLE_CXX20 ON CACHE INTERNAL "" FORCE)

# source files
set(PROJECT_TEST_SOURCES ${PROJECT_NAME}_TEST_SOURCES)
file(GLOB_RECURSE ${PROJECT_TEST_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

enable_testing()

# make exec
add_executable(${PROJECT_NAME} ${${PROJECT_TEST_SOURCES}})
add_test(${PROJECT_NAME}_TEST ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE glslstruct gtest_main)

# Konfiguracja PCH
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp")

# Optimizations
target_compile_options(${PROJECT_NAME} PRIVATE
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Release>:/GL>
        $<$<CONFIG:Release>:/fp:fast>
        $<$<CONFIG:Debug>:/RTC1>
        $<$<CONFIG:Debug>:/Zi>
        /Zc:preprocessor
        /Zc:__cplusplus
        /W4
        /WX
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
        -Wall
        -Wextra
        -Werror
        -fno-strict-aliasing
        --coverage 
        -fprofile-arcs 
        -ftest-coverage
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
        -Wall
        -Wextra
        -Werror
        -fno-strict-aliasing
        --coverage 
        -fprofile-arcs 
        -ftest-coverage
    >
)

target_link_options(${PROJECT_NAME} PRIVATE
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:/LTCG>
        $<$<CONFIG:Debug>:/DEBUG>
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-flto>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-fuse-ld=lld>
        -static
        --coverage
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-flto>
        -static
        --coverage
    >
)

# 2. Definicja targetu "coverage"
add_custom_target(coverage
    COMMENT "Generowanie raportu pokrycia kodu..."
    DEPENDS ${PROJECT_NAME}
)

file(TO_NATIVE_PATH "${glslstruct_SOURCE_DIR}\\glslstruct" coverages_sources_path)
file(TO_NATIVE_PATH "${glslstruct_BINARY_DIR}" coverages_modules_path)

if(MSVC)
    # --- Œcie¿ka dla Windows (OpenCppCoverage) ---
    find_program(OPENCPP_EXE OpenCppCoverage)
    
    if(OPENCPP_EXE)
        add_custom_command(TARGET coverage POST_BUILD
            COMMAND ${OPENCPP_EXE} 
                    --sources "${coverages_sources_path}\\*"
                    --modules "${coverages_modules_path}\\*"
                    --export_type=html:coverage_report 
                    -- "${CMAKE_CTEST_COMMAND}" -C $<CONFIG>
                    --verbose
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Running OpenCppCoverage for MSVC..."
            VERBATIM
        )
        message(STATUS "Founded OpenCppCovergae ${OPENCPP_EXE}")
    else()
        message(WARNING "OpenCppCoverage was not founded! Install it to generate report on Windows!")
    endif()

else()
    # --- Œcie¿ka dla Linux/macOS (Gcovr) ---
    find_program(GCOVR_EXE gcovr)
    
    if(GCOVR_EXE)
        add_custom_command(TARGET coverage POST_BUILD
            # Najpierw uruchamiamy testy
            COMMAND ${CMAKE_CTEST_COMMAND}
            # Potem generujemy raport HTML (szczegó³owy)
            COMMAND ${GCOVR_EXE} -r ${coverages_sources_path} 
                    --html-details 
                    -o coverage_report.html
                    --filter ${coverages_sources_path}
                    --exclude-unreachable-branches
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating report with Gcovr for GCC/Clang..."
            VERBATIM
        )
        message(STATUS "Founded gcovr ${GCOVR_EXE}")
    else()
        message(WARNING "Gcovr was not founded! Install it using: pip install gcovr")
    endif()
endif()