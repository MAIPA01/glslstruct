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
#include <glslstruct/value_types/types/mat_type.hpp>

using namespace glslstruct;

size_t mat_type::_calculate_mat_size(ValueType type, size_t cols, size_t rows, MajorType major) noexcept {
	const size_t arrayCount = major == MajorType::Column ? cols : rows;
	size_t vecSize = (major == MajorType::Column ? rows : cols) * get_value_type_size(type);

	static _GLSL_STRUCT_CONSTEXPR17 const size_t vec_alignment = 16;
	if (vecSize % vec_alignment != 0) {
		vecSize += vec_alignment - (vecSize % vec_alignment);
	}
	return vecSize * arrayCount;
}

mat_type::mat_type(ValueType type, size_t cols, size_t rows, MajorType major) noexcept
	: value_type(_calculate_mat_size(type, cols, rows, major)),
	_cols(cols), _rows(rows), _type(type), _major(major) {}
mat_type::mat_type(const mat_type& other) noexcept = default;
mat_type::mat_type(mat_type&& other) noexcept = default;
mat_type::~mat_type() noexcept = default;

mat_type& mat_type::operator=(const mat_type& other) noexcept = default;
mat_type& mat_type::operator=(mat_type&& other) noexcept = default;

ValueType mat_type::get_type() const noexcept {
	return _type;
}
size_t mat_type::get_rows() const noexcept {
	return _rows;
}
size_t mat_type::get_cols() const noexcept {
	return _cols;
}
size_t mat_type::get_vec_length() const noexcept {
	switch (_major) {
		default:
		case MajorType::Column : return _rows;
		case MajorType::Row : return _cols;
	}
}
size_t mat_type::get_array_count() const noexcept {
	switch (_major) {
		default:
		case MajorType::Column : return _cols;
		case MajorType::Row : return _rows;
	}
}

MajorType mat_type::get_major_type() const noexcept {
	return _major;
}

std::string mat_type::to_string() const noexcept {
	return fmt::format("{}{}", mat_type_to_string(_type),
		_cols == _rows ? fmt::format("{}", _cols) : fmt::format("{}x{}", _cols, _rows));
}

bool glslstruct::operator==(const mat_type& lhs, const mat_type& rhs) noexcept {
	return lhs._type == rhs._type && lhs._cols == rhs._cols && lhs._rows == rhs._rows;
}
bool glslstruct::operator!=(const mat_type& lhs, const mat_type& rhs) noexcept {
	return !(lhs == rhs);
}

size_t std::hash<mat_type>::operator()(const mat_type& value) const noexcept {
	size_t seed = static_cast<size_t>(value._type);
	mstd::hash_append(seed, value._cols, value._rows);
	return seed;
}

#endif