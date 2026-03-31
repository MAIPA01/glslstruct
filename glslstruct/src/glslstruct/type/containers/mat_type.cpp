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

	#include <glslstruct/type/containers/mat_type.hpp>
	#include <pch.hpp>

using namespace glslstruct;

mat_type::mat_type(const ValueType type, const size_t cols, const size_t rows, const size_t size) noexcept
	: base_type(size), _cols(cols), _rows(rows), _type(type) {}

mat_type::mat_type(const mat_type& other) noexcept			  = default;
mat_type::mat_type(mat_type&& other) noexcept				  = default;
mat_type::~mat_type() noexcept								  = default;

mat_type& mat_type::operator=(const mat_type& other) noexcept = default;
mat_type& mat_type::operator=(mat_type&& other) noexcept	  = default;

ValueType mat_type::get_type() const noexcept { return _type; }

size_t mat_type::get_rows() const noexcept { return _rows; }

size_t mat_type::get_cols() const noexcept { return _cols; }

size_t mat_type::get_vec_length() const noexcept { return _rows; }

size_t mat_type::get_array_count() const noexcept { return _cols; }

std::string mat_type::to_string() const noexcept { return mat_to_string(_type, _cols, _rows); }

bool glslstruct::operator==(const mat_type& lhs, const mat_type& rhs) noexcept {
	return lhs._type == rhs._type && lhs._cols == rhs._cols && lhs._rows == rhs._rows;
}

	#if _GLSL_STRUCT_HAS_CXX20
bool glslstruct::operator!=(const mat_type& lhs, const mat_type& rhs) noexcept = default;
	#else
bool glslstruct::operator!=(const mat_type& lhs, const mat_type& rhs) noexcept { return !(lhs == rhs); }
	#endif

size_t std::hash<mat_type>::operator()(const mat_type& value) const noexcept {
	size_t seed = static_cast<size_t>(value._type);
	mstd::hash_append(seed, value._cols, value._rows);
	return seed;
}

#endif