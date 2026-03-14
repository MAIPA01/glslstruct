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

#include <pch.hpp>
#include <glslstruct/value_types/data/value_data.hpp>
#include <glslstruct/value_types.hpp>

using namespace glslstruct;

#if _GLSL_STRUCT_HAS_TYPES
value_data::value_data(size_t offset, const base_type_handle& type, size_t size, size_t padding) noexcept 
	: _type(type), _offset(offset), _size(size), _padding(padding) {}
#else
value_data::value_data(size_t offset, size_t size, size_t padding) noexcept
	: _offset(offset), _size(size), _padding(padding) {}
#endif
value_data::value_data(const value_data& other) noexcept = default;
value_data::value_data(value_data&& other) noexcept = default;

value_data::~value_data() noexcept = default;

value_data& value_data::operator=(const value_data& other) noexcept = default;
value_data& value_data::operator=(value_data&& other) noexcept = default;

#if _GLSL_STRUCT_HAS_TYPES
void value_data::change_type(const base_type_handle& newType) noexcept {
	_type = newType;
}
#endif
void value_data::set_padding(size_t padding) noexcept {
	_padding = padding;
}

#if _GLSL_STRUCT_HAS_TYPES
[[nodiscard]] const base_type_handle& value_data::get_type() const noexcept {
	return _type;
}
#endif
[[nodiscard]] size_t value_data::get_offset() const noexcept {
	return _offset;
}
[[nodiscard]] size_t value_data::get_total_size() const noexcept {
	return _size + _padding;
}
[[nodiscard]] size_t value_data::get_size() const noexcept {
	return _size;
}
[[nodiscard]] size_t value_data::get_padding() const noexcept {
	return _padding;
}

bool value_data::operator==(const value_data& other) const noexcept {
	return _offset == other._offset && *_type == *other._type && 
		_size == other._size && _padding == other._padding;
}
#if _GLSL_STRUCT_HAS_CXX20
bool value_data::operator!=(const value_data& other) const noexcept = default;
#else
bool value_data::operator!=(const value_data& other) const noexcept {
	return !(*this == other);
}
#endif

size_t std::hash<value_data>::operator()(const value_data& value) const noexcept {
	return mstd::hash_combine(value._offset, *value._type, 
		value._size, value._padding);
}

#endif