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
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1!"
);
#else

	#include <glslstruct/value_types/types/vec_type.hpp>
	#include <pch.hpp>

using namespace glslstruct;

vec_type::vec_type(ValueType type, size_t length) noexcept
	: value_type(get_value_type_size(type) * length), _length(length), _type(type) {}

vec_type::vec_type(const vec_type& other) noexcept			  = default;
vec_type::vec_type(vec_type&& other) noexcept				  = default;
vec_type::~vec_type() noexcept								  = default;

vec_type& vec_type::operator=(const vec_type& other) noexcept = default;
vec_type& vec_type::operator=(vec_type&& other) noexcept	  = default;

ValueType vec_type::get_type() const noexcept { return _type; }

size_t vec_type::get_length() const noexcept { return _length; }

std::string vec_type::to_string() const noexcept { return fmt::format("{}{}", vec_type_to_string(_type), _length); }

bool glslstruct::operator==(const vec_type& lhs, const vec_type& rhs) noexcept {
	return lhs._type == rhs._type && lhs._length == rhs._length;
}

bool glslstruct::operator!=(const vec_type& lhs, const vec_type& rhs) noexcept { return !(lhs == rhs); }

size_t std::hash<vec_type>::operator()(const vec_type& value) const noexcept {
	size_t seed = static_cast<size_t>(value._type);
	mstd::hash_append(seed, value._length);
	return seed;
}

#endif