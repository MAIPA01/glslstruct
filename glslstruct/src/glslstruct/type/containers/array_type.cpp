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
	#include <glslstruct/type/containers/array_type.hpp>
	#include <glslstruct/type/containers/mat_type.hpp>
	#include <glslstruct/type/containers/scalar_type.hpp>
	#include <glslstruct/type/containers/struct_type.hpp>
	#include <glslstruct/type/containers/vec_type.hpp>

using namespace glslstruct;

array_type::array_type(const ValueType type, const size_t scalarSize, const size_t count, const size_t size) noexcept
	: array_type(std::make_shared<scalar_type>(type, scalarSize), count, size) {}

array_type::array_type(const ValueType type, const size_t length, const size_t vecSize, const size_t count,
  const size_t size) noexcept
	: array_type(std::make_shared<vec_type>(type, length, vecSize), count, size) {}

array_type::array_type(const ValueType type, const size_t cols, const size_t rows, const size_t matSize, const size_t count,
  const size_t size) noexcept
	: array_type(std::make_shared<mat_type>(type, cols, rows, matSize), count, size) {}

array_type::array_type(const mstd::ordered_map<std::string, var_data>& values, const size_t structSize, const size_t count,
  const size_t size) noexcept
	: array_type(std::make_shared<struct_type>(values, structSize), count, size) {}

array_type::array_type(const base_type_handle& type, const size_t count, const size_t size) noexcept
	: base_type(size), _type(type), _count(count) {}

array_type::array_type(const array_type& other) noexcept			= default;

array_type::array_type(array_type&& other) noexcept					= default;

array_type::~array_type() noexcept									= default;

array_type& array_type::operator=(const array_type& other) noexcept = default;

array_type& array_type::operator=(array_type&& other) noexcept		= default;

const base_type_handle& array_type::get_type() const noexcept { return _type; }

size_t array_type::get_count() const noexcept { return _count; }

std::string array_type::to_string() const noexcept { return fmt::format("{}[{}]", _type->to_string(), _count); }

_GLSL_STRUCT_EXPORT bool glslstruct::operator==(const array_type& lhs, const array_type& rhs) noexcept {
	return *lhs._type == *rhs._type && lhs._count == rhs._count;
}

	#if _GLSL_STRUCT_HAS_CXX20
_GLSL_STRUCT_EXPORT bool glslstruct::operator!=(const array_type& lhs, const array_type& rhs) noexcept = default;
	#else
_GLSL_STRUCT_EXPORT bool glslstruct::operator!=(const array_type& lhs, const array_type& rhs) noexcept { return !(lhs == rhs); }
	#endif

size_t std::hash<array_type>::operator()(const array_type& type) const noexcept {
	return mstd::hash_combine(type._count, *type._type);
}

#endif