/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#include <glslstruct/config.hpp>

#if _GLSL_STRUCT_HAS_TYPES

	#include <glslstruct/pch.hpp>
	#include <glslstruct/type/containers/base_type.hpp>
	#include <glslstruct/type/visitors/type_hash_visitor.hpp>

using namespace glslstruct;

base_type::base_type(const size_t size) noexcept : _size(size) {}

base_type::base_type(const base_type& other) noexcept			 = default;
base_type::base_type(base_type&& other) noexcept				 = default;
base_type::~base_type() noexcept								 = default;

base_type& base_type::operator=(const base_type& other) noexcept = default;
base_type& base_type::operator=(base_type&& other) noexcept		 = default;

bool base_type::operator==(const base_type& other) const noexcept { return _size == other._size; }

bool base_type::operator!=(const base_type& other) const noexcept
	#if _GLSL_STRUCT_HAS_CXX20
  = default;
	#else
{
	return !(*this == other);
}
	#endif

size_t base_type::get_size() const noexcept { return _size; }

_GLSL_STRUCT_EXPORT size_t glslstruct::sizeof_type(const base_type_handle& type) noexcept { return type->get_size(); }

_GLSL_STRUCT_EXPORT std::string glslstruct::to_string(const base_type_handle& type) noexcept { return type->to_string(); }

size_t std::hash<base_type>::operator()(const base_type& type) const noexcept {
	type_hash_visitor visitor;
	glslstruct::visit(visitor, type);

	size_t result = visitor.result();
	mstd::hash_append(result, type._size);
	return result;
}

#endif