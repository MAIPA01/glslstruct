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

	#include <glslstruct/type/traits/scalar_traits.hpp>

	#include <glslstruct/type/data/scalar_data.hpp>
	#include <pch.hpp>

using namespace glslstruct;

scalar_data::scalar_data(const std::vector<std::byte>& data) : _data(data) {}

scalar_data::scalar_data(const bool value) : scalar_data(static_cast<int>(value)) {}

scalar_data::scalar_data(const int value) : scalar_data(_get_value_data(value)) {}

scalar_data::scalar_data(const unsigned int value) : scalar_data(_get_value_data(value)) {}

scalar_data::scalar_data(const float value) : scalar_data(_get_value_data(value)) {}

scalar_data::scalar_data(const double value) : scalar_data(_get_value_data(value)) {}

scalar_data::scalar_data(const scalar_data& other)				  = default;
scalar_data::scalar_data(scalar_data&& other) noexcept			  = default;

scalar_data::~scalar_data()										  = default;

scalar_data& scalar_data::operator=(const scalar_data& other)	  = default;
scalar_data& scalar_data::operator=(scalar_data&& other) noexcept = default;

const std::vector<std::byte>& scalar_data::data() const noexcept { return _data; }

#endif