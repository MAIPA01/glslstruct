project(glslstruct VERSION 1.1.1 LANGUAGES CXX)

option(GLSL_STRUCT_ENABLE_CXX20 OFF)

if (GLSL_STRUCT_ENABLE_CXX20)
	set(CMAKE_CXX_STANDARD 20)
else()
	set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# source files
set(PROJECT_SOURCES ${PROJECT_NAME}_SOURCES)
file(GLOB_RECURSE ${PROJECT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# header files
set(PROJECT_HEADERS ${PROJECT_NAME}_HEADERS)
file(GLOB_RECURSE ${PROJECT_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

# make library
add_library(${PROJECT_NAME} ${${PROJECT_LIBRARY_TYPE}} ${${PROJECT_HEADERS}} ${${PROJECT_SOURCES}})

# set library alias
set(PROJECT_NAMESPACE ${PROJECT_NAME}::)
set(PROJECT_ALIAS ${PROJECT_NAMESPACE}${PROJECT_NAME})
add_library(${PROJECT_ALIAS} ALIAS ${PROJECT_NAME})

# set headers folder
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include 
                                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PUBLIC glm mstd fmt)

# Konfiguracja PCH
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp")

# Optional compiler flags
set(PROJECT_EXPORT ${PROJECT_NAME}_EXPORT)
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
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-fuse-ld=lld>
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

if (GLSL_STRUCT_ENABLE_CXX20)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLSL_STRUCT_ENABLE_CXX20=1)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLSL_STRUCT_ENABLE_CXX20=0)
endif()