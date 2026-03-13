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
#include <glslstruct/value_types/visitors/type_hash_visitor.hpp>
#include <glslstruct/value_types/types/scalar_type.hpp>
#include <glslstruct/value_types/types/vec_type.hpp>
#include <glslstruct/value_types/types/mat_type.hpp>
#include <glslstruct/value_types/types/struct_type.hpp>
#include <glslstruct/value_types/types/array_type.hpp>

using namespace glslstruct;

_GLSL_STRUCT_CONSTEXPR20 type_hash_visitor::type_hash_visitor() noexcept = default;
_GLSL_STRUCT_CONSTEXPR20 type_hash_visitor::~type_hash_visitor() noexcept override = default;

void type_hash_visitor::visit(const scalar_type& value) {
	_result = std::hash<scalar_type>()(value);
}
void type_hash_visitor::visit(const vec_type& value) {
	_result = std::hash<vec_type>()(value);
}
void type_hash_visitor::visit(const mat_type& value) {
	_result = std::hash<mat_type>()(value);
}
void type_hash_visitor::visit(const struct_type& value) {
	_result = std::hash<struct_type>()(value);
}
void type_hash_visitor::visit(const array_type& value) {
	_result = std::hash<array_type>()(value);
}

size_t type_hash_visitor::result() const noexcept {
	return _result;
}

#endif