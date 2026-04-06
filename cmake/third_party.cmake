# for CPM
set(CPM_USE_LOCAL_PACKAGES OFF)

if(NOT CPM_SOURCE_CACHE)
    # Storage location
    set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/third_party)
endif()

# Set download location
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")

# download CPM.cmake
# Expand relative path. This is important if the provided path contains a tilde (~)
get_filename_component(CPM_DOWNLOAD_LOCATION ${CPM_DOWNLOAD_LOCATION} ABSOLUTE)
if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
	file(DOWNLOAD
		https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake
        ${CPM_DOWNLOAD_LOCATION}
	)
endif()
message(STATUS "Include CPM.cmake from ${CPM_DOWNLOAD_LOCATION}")
include(${CPM_DOWNLOAD_LOCATION})

if(NOT ${GLSL_STRUCT_USE_EXTERNAL_MSTD})
    CPMAddPackage(
            URI "gh:maipa01/mstd#v1.5.1"
            OPTIONS "MSTD_ENABLE_CXX20 ${GLSL_STRUCT_ENABLE_CXX20}"
                    "MSTD_ENABLE_ENUMS_MACROS ON"
                    "MSTD_DISABLE_ASSERT_ON_RELEASE ${GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE}"
    )
else()
    set(MSTD_ENABLE_CXX20 ${GLSL_STRUCT_ENABLE_CXX20})
    set(MSTD_ENABLE_ENUMS_MACROS ON)
endif()

if(NOT ${GLSL_STRUCT_DISABLE_PARSER})
    if(NOT ${GLSL_STRUCT_USE_EXTERNAL_PCRE2CPP})
        CPMAddPackage(
                URI "gh:maipa01/pcre2cpp#v1.2.3"
                OPTIONS "PCRE2CPP_ENABLE_CXX20 ${GLSL_STRUCT_ENABLE_CXX20}"
                        "PCRE2CPP_DISABLE_ASSERT_ON_RELEASE ${GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE}"
                        "PCRE2CPP_USE_EXTERNAL_MSTD ON"
        )
    endif()
endif()

if (NOT ${GLSL_STRUCT_USE_EXTERNAL_GLM})
    CPMAddPackage("gh:g-truc/glm#1.0.3")
endif()

target_compile_definitions(glm PRIVATE GLM_ENABLE_EXPERIMENTAL)

if (GLSL_STRUCT_BUILD_TESTS OR GLSL_STRUCT_BUILD_COVERAGE)
    CPMAddPackage("gh:google/googletest#v1.17.0")
endif()