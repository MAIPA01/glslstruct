# region SOURCE_FILES
set(PROJECT_SOURCES ${PROJECT_NAME}_SOURCES)
file(GLOB_RECURSE ${PROJECT_SOURCES} CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

if (GLSL_STRUCT_DISABLE_TYPES)
    list(FILTER ${PROJECT_SOURCES} EXCLUDE REGEX "^.*[\\\/\\\\]type[\\\/\\\\](containers|visitors)[\\\/\\\\].*\\.cpp$")
    list(FILTER ${PROJECT_SOURCES} EXCLUDE REGEX "^.*[\\\/\\\\]writer[\\\/\\\\].*\\.cpp$")
endif()

if (GLSL_STRUCT_DISABLE_PARSER)
    list(FILTER ${PROJECT_SOURCES} EXCLUDE REGEX "^.*[\\\/\\\\]parser[\\\/\\\\].*\\.cpp$")
endif()
# endregion

# region HEADER_FILES
set(PROJECT_HEADERS ${PROJECT_NAME}_HEADERS)

set(BUILD_PUBLIC_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BUILD_PRIVATE_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INSTALL_PUBLIC_HEADERS_DIR include)

file(GLOB_RECURSE ${PROJECT_HEADERS} CONFIGURE_DEPENDS ${BUILD_PUBLIC_HEADERS_DIR}/*.hpp)

if (GLSL_STRUCT_DISABLE_TYPES)
    list(FILTER ${PROJECT_HEADERS} EXCLUDE REGEX "^.*[\\\/\\\\]type[\\\/\\\\](containers|visitors)[\\\/\\\\].*\\.hpp$")
    list(FILTER ${PROJECT_HEADERS} EXCLUDE REGEX "^.*[\\\/\\\\]type[\\\/\\\\]checks[\\\/\\\\]type_checks\\.hpp$")
    list(FILTER ${PROJECT_HEADERS} EXCLUDE REGEX "^.*[\\\/\\\\]writer[\\\/\\\\].*\\.hpp$")
endif()

if (GLSL_STRUCT_DISABLE_PARSER)
    list(FILTER ${PROJECT_HEADERS} EXCLUDE REGEX "^.*[\\\/\\\\]parser[\\\/\\\\].*\\.hpp$")
endif()
# endregion

# region LIBRARY_SETUP
add_library(${PROJECT_NAME} ${${PROJECT_HEADERS}} ${${PROJECT_SOURCES}})

# set library alias
set(PROJECT_NAMESPACE ${PROJECT_NAME}::)
set(PROJECT_ALIAS ${PROJECT_NAMESPACE}${PROJECT_NAME})
add_library(${PROJECT_ALIAS} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
# endregion

# region CLANG_TIDY
if (GLSL_STRUCT_ENABLE_CLANG_TIDY)
    if(GLSL_STRUCT_ENABLE_CXX20)
        set(CLANG_TIDY_CXX_STANDARD c++20)
    else()
        set(CLANG_TIDY_CXX_STANDARD c++17)
    endif()

    disable_clang_tidy_for_file(${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp)
    enable_clang_tidy(
            TARGET ${PROJECT_NAME}
            CONFIG "${CMAKE_SOURCE_DIR}/.clang-tidy"
            CXX_STANDARD ${CLANG_TIDY_CXX_STANDARD}
            HEADER_FILTER "^${CMAKE_CURRENT_SOURCE_DIR}[\\\\\/\\\\].*"
            EXCLUDE_HEADER_FILTER "^${CMAKE_SOURCE_DIR}[\\\\\/\\\\]third_party[\\\\\/\\\\].*"
    )
endif()
# endregion

target_precompile_headers(${PROJECT_NAME} PRIVATE "$<BUILD_INTERFACE:${BUILD_PRIVATE_HEADERS_DIR}/${PROJECT_NAME}/pch.hpp>")

target_compile_features(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:$<IF:$<BOOL:${GLSL_STRUCT_ENABLE_CXX20}>,cxx_std_20,cxx_std_17>>)

# region LINK_LIBRARIES
if(NOT TARGET ${PROJECT_NAMESPACE}glm_imported)
    add_library(${PROJECT_NAMESPACE}glm_imported INTERFACE IMPORTED)
endif()

if(NOT TARGET ${PROJECT_NAMESPACE}mstd_imported)
    add_library(${PROJECT_NAMESPACE}mstd_imported INTERFACE IMPORTED)
endif()

if(NOT TARGET ${PROJECT_NAMESPACE}pcre2cpp_imported)
    add_library(${PROJECT_NAMESPACE}pcre2cpp_imported INTERFACE IMPORTED)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:
            glm::glm
            mstd::mstd
            $<$<NOT:$<BOOL:${GLSL_STRUCT_DISABLE_PARSER}>>:pcre2cpp>
        >
        $<INSTALL_INTERFACE:
            ${PROJECT_NAMESPACE}glm_imported
            ${PROJECT_NAMESPACE}mstd_imported
            ${PROJECT_NAMESPACE}pcre2cpp_imported
        >
)
# endregion

# set headers folder
target_include_directories(${PROJECT_NAME} PUBLIC   $<BUILD_INTERFACE:${BUILD_PUBLIC_HEADERS_DIR}>
                                           PRIVATE  $<BUILD_INTERFACE:${BUILD_PRIVATE_HEADERS_DIR}>
)

# region COMPILE_DEFINITIONS
target_compile_definitions(${PROJECT_NAME} PRIVATE  MSTD_ENABLE_ENUMS_MACROS
                                                    PCRE2CPP_DISABLE_UTF16
                                                    PCRE2CPP_DISABLE_UTF32
                                                    GLM_ENABLE_EXPERIMENTAL
                                                    GLM_FORCE_INTRINSICS
                                                    GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
                                                    $<BUILD_INTERFACE:
                                                        $<$<BOOL:${GLSL_STRUCT_ENABLE_CXX20}>:  GLSL_STRUCT_ENABLE_CXX20
                                                                                                MSTD_ENABLE_CXX20
                                                                                                PCRE2CPP_ENABLE_CXX20
                                                        >
                                                        $<$<BOOL:${GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE}>: GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE
                                                                                                            MSTD_DISABLE_ASSERT_ON_RELEASE
                                                                                                            PCRE2CPP_DISABLE_ASSERT_ON_RELEASE
                                                        >
                                                        $<$<BOOL:${GLSL_STRUCT_DISABLE_TYPES}>:GLSL_STRUCT_DISABLE_TYPES>
                                                        $<$<BOOL:${GLSL_STRUCT_ENABLE_TYPE_CHECKS}>:GLSL_STRUCT_ENABLE_TYPE_CHECKS>
                                                        $<$<BOOL:${GLSL_STRUCT_DISABLE_PARSER}>:GLSL_STRUCT_DISABLE_PARSER>
                                                        $<$<BOOL:${BUILD_SHARED_LIBS}>:GLSL_STRUCT_SHARED_LIBS>
                                                        GLSL_STRUCT_EXPORT
                                                    >
)
target_compile_definitions(${PROJECT_NAME} INTERFACE    MSTD_ENABLE_ENUMS_MACROS
                                                        PCRE2CPP_DISABLE_UTF16
                                                        PCRE2CPP_DISABLE_UTF32
                                                        GLM_ENABLE_EXPERIMENTAL
                                                        GLM_FORCE_INTRINSICS
                                                        GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
                                                        $<BUILD_INTERFACE:
                                                            $<$<BOOL:${GLSL_STRUCT_ENABLE_CXX20}>:  GLSL_STRUCT_ENABLE_CXX20
                                                                MSTD_ENABLE_CXX20
                                                                PCRE2CPP_ENABLE_CXX20
                                                            >
                                                            $<$<BOOL:${GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE}>: GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE
                                                                MSTD_DISABLE_ASSERT_ON_RELEASE
                                                                PCRE2CPP_DISABLE_ASSERT_ON_RELEASE
                                                            >
                                                            $<$<BOOL:${GLSL_STRUCT_DISABLE_TYPES}>:GLSL_STRUCT_DISABLE_TYPES>
                                                            $<$<BOOL:${GLSL_STRUCT_ENABLE_TYPE_CHECKS}>:GLSL_STRUCT_ENABLE_TYPE_CHECKS>
                                                            $<$<BOOL:${GLSL_STRUCT_DISABLE_PARSER}>:GLSL_STRUCT_DISABLE_PARSER>
                                                            $<$<BOOL:${BUILD_SHARED_LIBS}>:GLSL_STRUCT_SHARED_LIBS>
                                                        >
)
# endregion

# region COMPILE_OPTIONS
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
    >
    $<BUILD_INTERFACE:$<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>,$<BOOL:${GLSL_STRUCT_BUILD_COVERAGE}>>:
        --coverage
        -fcoverage-mcdc
        -fprofile-instr-generate
        -fcoverage-mapping
    >>
)
# endregion

# region LINK_OPTIONS
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
    >
    $<BUILD_INTERFACE:$<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>,$<BOOL:${GLSL_STRUCT_BUILD_COVERAGE}>>:
        --coverage
        -fprofile-instr-generate
    >>
)
# endregion

# region INSTALL
if(GLSL_STRUCT_INSTALL)
    # Installing header files
    install(DIRECTORY ${BUILD_PUBLIC_HEADERS_DIR}/${PROJECT_NAME}/
            DESTINATION ${INSTALL_PUBLIC_HEADERS_DIR}/${PROJECT_NAME}
            FILES_MATCHING PATTERN "*.hpp"
    )

    # Installing targets
    set(PROJECT_INSTALL_TARGETS ${PROJECT_NAME}Targets)
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION bin
            INCLUDES DESTINATION ${INSTALL_PUBLIC_HEADERS_DIR}
    )

    # Generate Targets.cmake
    install(EXPORT ${PROJECT_INSTALL_TARGETS}
            FILE ${PROJECT_INSTALL_TARGETS}.cmake
            NAMESPACE ${PROJECT_NAMESPACE}
            DESTINATION cmake
    )

    # Create Config.cmake
    include(CMakePackageConfigHelpers)

    set(PROJECT_INSTALL_CONFIG ${PROJECT_NAME}Config)
    configure_package_config_file(
            "${CMAKE_SOURCE_DIR}/cmake/${PROJECT_INSTALL_CONFIG}.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_CONFIG}.cmake"
            INSTALL_DESTINATION cmake
    )

    # Create Version.cmake
    set(PROJECT_INSTALL_VERSION ${PROJECT_NAME}Version)
    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_VERSION}.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY AnyNewerVersion
    )

    # Install config files
    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_CONFIG}.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_VERSION}.cmake"
            DESTINATION cmake
    )
endif()
# endregion