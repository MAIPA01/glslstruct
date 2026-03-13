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

CPMAddPackage(
    URI "gh:maipa01/mstd#v1.4.1"
    OPTIONS "MSTD_ENABLE_CXX20 ${GLSL_STRUCT_ENABLE_CXX20}"
)

CPMAddPackage("gh:g-truc/glm#1.0.3")

CPMAddPackage("gh:fmtlib/fmt#12.1.0")

if (GLSL_STRUCT_BUILD_TESTS)
    CPMAddPackage("gh:google/googletest#v1.17.0")
endif()