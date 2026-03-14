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
#include <glslstruct/value_types/types/array_type.hpp>
#include <glslstruct/value_types.hpp>

using namespace glslstruct;

size_t array_type::_calculate_array_size(const base_type_handle& type, size_t count) noexcept {
	size_t typeSize = type->get_size();
	static _GLSL_STRUCT_CONSTEXPR17 const size_t elem_alignment = 16;
	if (typeSize % elem_alignment != 0) {
		typeSize += (elem_alignment - (typeSize % elem_alignment));
	}
	return typeSize * count;
}

array_type::array_type(ValueType type, size_t count) noexcept 
	: array_type(std::make_shared<scalar_type>(type), count) {}
array_type::array_type(ValueType type, size_t length, size_t count) noexcept
	: array_type(std::make_shared<vec_type>(type, length), count) {}
array_type::array_type(ValueType type, size_t cols, size_t rows, MajorType major, size_t count) noexcept
	: array_type(std::make_shared<mat_type>(type, cols, rows, major), count) { }
array_type::array_type(ValueType type, size_t cols, size_t rows, size_t count) noexcept
	: array_type(type, cols, rows, MajorType::Column, count) {}
array_type::array_type(const std::unordered_map<std::string, value_data>& values, size_t size, size_t count) noexcept 
	: array_type(std::make_shared<struct_type>(values, size), count) { }
array_type::array_type(const base_type_handle& type, size_t count) noexcept 
	: value_type(_calculate_array_size(type, count)), _type(type), _count(count) {}
array_type::array_type(const array_type& other) noexcept = default;
array_type::array_type(array_type&& other) noexcept 
	: value_type(other), _type(std::exchange(other._type, nullptr)),
	_count(std::exchange(other._count, 0)) {}
array_type::~array_type() noexcept = default;

array_type& array_type::operator=(const array_type& other) noexcept {
	value_type::operator=(other);
	_type = other._type;
	_count = other._count;
	return *this;
}
array_type& array_type::operator=(array_type&& other) noexcept {
	value_type::operator=(other);
	_type = std::exchange(other._type, nullptr);
	_count = std::exchange(other._count, 0);
	return *this;
}

const base_type_handle& array_type::get_type() const noexcept {
	return _type;
}
size_t array_type::get_count() const noexcept {
	return _count;
}

std::string array_type::to_string() const noexcept {
	return fmt::format("{}[{}]", _type->to_string(), _count);
}

bool glslstruct::operator==(const array_type& lhs, const array_type& rhs) noexcept {
	return *lhs._type == *rhs._type && lhs._count == rhs._count;
}
bool glslstruct::operator!=(const array_type& lhs, const array_type& rhs) noexcept {
	return !(lhs == rhs);
}

size_t std::hash<array_type>::operator()(const array_type& type) const noexcept {
	return mstd::hash_combine(type._count, *type._type);
}

#endif