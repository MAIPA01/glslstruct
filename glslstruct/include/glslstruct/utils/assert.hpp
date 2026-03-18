/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

#include <mstd/mstd.hpp>

#if _DEBUG
#define glsl_struct_assert(expression, ...) mstd_stop_assert_base(expression, [](const std::string_view&) -> void {} __VA_OPT__(, ) __VA_ARGS__)
#elif !defined(GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE)
#define glsl_struct_assert(expression, ...) mstd_log_assert_base(expression, [](const std::string_view&) -> void {} __VA_OPT__(, ) __VA_ARGS__)
#else
#define glsl_struct_assert(expression, ...) mstd_empty_assert_base(expression, [](const std::string_view&) -> void {} __VA_OPT__(, ) __VA_ARGS__)
#endif

#endif