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
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC glm mstd fmt)

# Konfiguracja PCH
set(PCH_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp")
if(EXISTS ${PCH_HEADER})
    target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_HEADER})
endif()

# Optional compiler flags
set(PROJECT_EXPORT ${PROJECT_NAME}_EXPORT)
if(MSVC)
	target_compile_options(${PROJECT_NAME} PUBLIC /Zc:preprocessor)
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
target_compile_options(${PROJECT_NAME} PUBLIC
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Release>:/GL>
        $<$<CONFIG:Debug>:/RTC1>
        /Zc:preprocessor
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
    >
)

target_link_options(${PROJECT_NAME} PUBLIC
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:/LTCG>
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-flto>
        -static
        $<$<CONFIG:Release>:-fuse-ld=lld>
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-flto>
        -static
    >
)

if (GLSL_STRUCT_ENABLE_CXX20)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLSL_STRUCT_ENABLE_CXX20=1)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLSL_STRUCT_ENABLE_CXX20=0)
endif()