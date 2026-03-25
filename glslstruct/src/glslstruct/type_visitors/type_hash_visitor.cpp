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

	#include <glslstruct/type_containers/array_type.hpp>
	#include <glslstruct/type_containers/mat_type.hpp>
	#include <glslstruct/type_containers/scalar_type.hpp>
	#include <glslstruct/type_containers/struct_type.hpp>
	#include <glslstruct/type_containers/vec_type.hpp>
	#include <glslstruct/type_visitors/type_hash_visitor.hpp>
	#include <pch.hpp>

using namespace glslstruct;

type_hash_visitor::type_hash_visitor() noexcept	 = default;
type_hash_visitor::~type_hash_visitor() noexcept = default;

void type_hash_visitor::visit(const scalar_type& value) { _result = std::hash<scalar_type>()(value); }

void type_hash_visitor::visit(const vec_type& value) { _result = std::hash<vec_type>()(value); }

void type_hash_visitor::visit(const mat_type& value) { _result = std::hash<mat_type>()(value); }

void type_hash_visitor::visit(const struct_type& value) { _result = std::hash<struct_type>()(value); }

void type_hash_visitor::visit(const array_type& value) { _result = std::hash<array_type>()(value); }

size_t type_hash_visitor::result() const noexcept { return _result; }

#endif