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
#include <glslstruct/value_types/types/struct_type.hpp>

using namespace glslstruct;

struct_type::struct_type(const std::unordered_map<std::string, value_data>& values, size_t size) noexcept 
	: value_type(size), _values(values) {}
struct_type::struct_type(const struct_type& other) noexcept = default;
struct_type::struct_type(struct_type&& other) noexcept 
	: value_type(other), _values(std::exchange(other._values, {})) {}
struct_type::~struct_type() noexcept = default;

struct_type& struct_type::operator=(const struct_type& other) noexcept {
	value_type::operator=(other);
	_values = other._values;
	return *this;
}
struct_type& struct_type::operator=(struct_type&& other) noexcept {
	value_type::operator=(other);
	_values = std::exchange(other._values, {});
	return *this;
}

const std::unordered_map<std::string, value_data>& struct_type::get_values() const noexcept {
	return _values;
}

std::string struct_type::to_string() const noexcept {
	return "struct";
}

bool glslstruct::operator==(const struct_type& lhs, const struct_type& rhs) noexcept {
	return lhs._values == rhs._values;
}
bool glslstruct::operator!=(const struct_type& lhs, const struct_type& rhs) noexcept {
	return !(lhs == rhs);
}

size_t std::hash<struct_type>::operator()(const struct_type& type) const noexcept {
	size_t seed = 0;
	for (const auto& [name, data] : type._values) {
		mstd::hash_append(seed, name, data);
	}
	return seed;
}

#endif