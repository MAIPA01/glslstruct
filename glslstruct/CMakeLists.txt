# source files
set(PROJECT_SOURCES ${PROJECT_NAME}_SOURCES)
file(GLOB_RECURSE ${PROJECT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# header files
set(PROJECT_HEADERS ${PROJECT_NAME}_HEADERS)
file(GLOB_RECURSE ${PROJECT_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

# make library
add_library(${PROJECT_NAME} ${${PROJECT_LIBRARY_TYPE}} ${${PROJECT_HEADERS}} ${${PROJECT_SOURCES}})

disable_clang_tidy_for_file(${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp)
enable_clang_tidy(${PROJECT_NAME})

# set library alias
set(PROJECT_NAMESPACE ${PROJECT_NAME}::)
set(PROJECT_ALIAS ${PROJECT_NAMESPACE}${PROJECT_NAME})
add_library(${PROJECT_ALIAS} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC $<IF:$<BOOL:${GLSL_STRUCT_ENABLE_CXX20}>,cxx_std_20,cxx_std_17>)

# set headers folder
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PUBLIC    $<$<NOT:$<BOOL:${GLSL_STRUCT_DISABLE_GLM}>>:glm>
                                                mstd
)

# Konfiguracja PCH
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp")

# Optional compiler flags
set(PROJECT_EXPORT _GLSL_STRUCT_EXPORT)
if(MSVC)
	if(BUILD_SHARED_LIBS)
        target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_EXPORT}=__declspec(dllexport))
        target_compile_definitions(${PROJECT_NAME} INTERFACE ${PROJECT_EXPORT}=__declspec(dllimport))
    endif()
else()
	if(BUILD_SHARED_LIBS)
        target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_EXPORT}=__attribute__((visibility("default"))))
    endif()
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE  $<$<BOOL:${GLSL_STRUCT_ENABLE_CXX20}>:GLSL_STRUCT_ENABLE_CXX20>
                                                    $<$<BOOL:${GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE}>:GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE>
                                                    $<$<BOOL:${GLSL_STRUCT_DISABLE_TYPES}>:GLSL_STRUCT_DISABLE_TYPES>
                                                    $<$<BOOL:${GLSL_STRUCT_ENABLE_TYPE_CHECKS}>:GLSL_STRUCT_ENABLE_TYPE_CHECKS>
)

# Optimizations
target_compile_options(${PROJECT_NAME} PRIVATE
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:
            /O2
            /GL
            /fp:fast
        >
        $<$<CONFIG:Debug>:
            /RTC1
            /Zi
        >
        /Zc:preprocessor
        /Zc:__cplusplus
        /W4
        /WX
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:
            -O3
            -flto
        >
        -Wall
        -Wextra
        -Werror
        -fno-strict-aliasing
        $<$<BOOL:${BUILD_GLSL_STRUCT_COVERAGE}>:
            --coverage
            -fcoverage-mcdc
            -fprofile-instr-generate
            -fcoverage-mapping
        >
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:
            -O3
            -flto
        >
        -Wall
        -Wextra
        -Werror
        -fno-strict-aliasing
        $<$<BOOL:${BUILD_GLSL_STRUCT_COVERAGE}>:
            --coverage
            -fcoverage-mcdc
            -fprofile-instr-generate
            -fcoverage-mapping
        >
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
        -static
        $<$<BOOL:${BUILD_GLSL_STRUCT_COVERAGE}>:-fprofile-instr-generate>
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-flto>
        -static
        $<$<BOOL:${BUILD_GLSL_STRUCT_COVERAGE}>:-fprofile-instr-generate>
    >
)