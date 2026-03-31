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

	#include <glslstruct/type/data/mat_data.hpp>
	#include <pch.hpp>

using namespace glslstruct;

mat_data::mat_data(const std::vector<vec_data>& data) : _data(data) {}

mat_data::mat_data(const mat_data& other)				 = default;
mat_data::mat_data(mat_data&& other) noexcept			 = default;

mat_data::~mat_data()									 = default;

mat_data& mat_data::operator=(const mat_data& other)	 = default;
mat_data& mat_data::operator=(mat_data&& other) noexcept = default;

const std::vector<vec_data>& mat_data::data() const noexcept { return _data; }

#endif