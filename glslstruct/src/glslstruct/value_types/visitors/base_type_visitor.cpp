/*
* glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1!");
#else

#include <pch.hpp>
#include <glslstruct/value_types/visitors/base_type_visitor.hpp>

using namespace glslstruct;

_GLSL_STRUCT_CONSTEXPR20 base_type_visitor::base_type_visitor() noexcept = default;
_GLSL_STRUCT_CONSTEXPR20 base_type_visitor::~base_type_visitor() noexcept = default;

#endif