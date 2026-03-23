/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

	#include <../../../../include/glslstruct/type_data/vec_data.hpp>
	#include <pch.hpp>

using namespace glslstruct;

vec_data::vec_data(const vec_data& other)				 = default;
vec_data::vec_data(vec_data&& other) noexcept			 = default;

vec_data::~vec_data()									 = default;

vec_data& vec_data::operator=(const vec_data& other)	 = default;
vec_data& vec_data::operator=(vec_data&& other) noexcept = default;

const std::vector<std::byte>& vec_data::data() const noexcept { return _data; }

#endif