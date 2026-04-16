if(NOT GLSL_STRUCT_MSTD_EXTERNAL OR NOT TARGET mstd::mstd)
    set(DOWNLOAD_MSTD ON)
else()
    set(DOWNLOAD_MSTD OFF)
endif()

if(NOT GLSL_STRUCT_DISABLE_PARSER AND (NOT GLSL_STRUCT_PCRE2CPP_EXTERNAL OR NOT TARGET pcre2cpp::pcre2cpp))
    set(DOWNLOAD_PCRE2CPP ON)
else()
    set(DOWNLOAD_PCRE2CPP OFF)
endif()

if(NOT GLSL_STRUCT_GLM_EXTERNAL OR NOT TARGET glm::glm)
    set(DOWNLOAD_GLM ON)
else()
    set(DOWNLOAD_GLM OFF)
endif()

if((GLSL_STRUCT_BUILD_TESTS OR GLSL_STRUCT_BUILD_COVERAGE) AND NOT TARGET gtest_main)
    set(DOWNLOAD_GTEST ON)
else()
    set(DOWNLOAD_GTEST OFF)
endif()

if(DOWNLOAD_MSTD OR DOWNLOAD_PCRE2CPP OR DOWNLOAD_GLM OR DOWNLOAD_GTEST)
    include(${CMAKE_CURRENT_LIST_DIR}/get_cpm.cmake)
endif()

if(DOWNLOAD_MSTD)
    CPMAddPackage("gh:maipa01/mstd#v1.5.2")
endif()

if(DOWNLOAD_PCRE2CPP)
    CPMAddPackage(
            URI "gh:maipa01/pcre2cpp#v1.2.5"
            OPTIONS "PCRE2CPP_USE_EXTERNAL_MSTD ON"
    )
endif()

if (DOWNLOAD_GLM)
    CPMAddPackage("gh:g-truc/glm#1.0.3")
endif()

if (DOWNLOAD_GTEST)
    CPMAddPackage("gh:google/googletest#v1.17.0")
endif()