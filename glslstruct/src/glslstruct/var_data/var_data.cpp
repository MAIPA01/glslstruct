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

#if _GLSL_STRUCT_HAS_TYPES
	#include <glslstruct/type/containers/base_type.hpp>
#endif

	#include <glslstruct/var_data/var_data.hpp>
	#include <pch.hpp>

using namespace glslstruct;

	#if _GLSL_STRUCT_HAS_TYPES
var_data::var_data(const size_t offset, const base_type_handle& type, const size_t padding) noexcept
	: _type(type), _offset(offset), _padding(padding) {}
	#else
var_data::var_data(const size_t offset, const size_t size, const size_t padding) noexcept
	: _size(size), _offset(offset), _padding(padding) {}
	#endif
var_data::var_data(const var_data& other) noexcept			  = default;
var_data::var_data(var_data&& other) noexcept				  = default;

var_data::~var_data() noexcept								  = default;

var_data& var_data::operator=(const var_data& other) noexcept = default;
var_data& var_data::operator=(var_data&& other) noexcept	  = default;

void var_data::set_padding(const size_t padding) noexcept { _padding = padding; }

	#if _GLSL_STRUCT_HAS_TYPES
[[nodiscard]] const base_type_handle& var_data::get_type() const noexcept { return _type; }
	#endif
[[nodiscard]] size_t var_data::get_offset() const noexcept { return _offset; }

[[nodiscard]] size_t var_data::get_total_size() const noexcept { return get_size() + _padding; }

[[nodiscard]] size_t var_data::get_size() const noexcept {
	#if _GLSL_STRUCT_HAS_TYPES
	return _type->get_size();
	#else
	return _size;
	#endif
}

[[nodiscard]] size_t var_data::get_padding() const noexcept { return _padding; }

bool var_data::operator==(const var_data& other) const noexcept {
	return _offset == other._offset &&
	#if _GLSL_STRUCT_HAS_TYPES
		   *_type == *other._type &&
	#else
		   _size == other._size &&
	#endif
		   _padding == other._padding;
}
	#if _GLSL_STRUCT_HAS_CXX20
bool var_data::operator!=(const var_data& other) const noexcept = default;
	#else
bool var_data::operator!=(const var_data& other) const noexcept { return !(*this == other); }
	#endif

size_t std::hash<var_data>::operator()(const var_data& value) const noexcept {
	return mstd::hash_combine(value._offset,
	#if _GLSL_STRUCT_HAS_TYPES
	  *value._type,
	#else
	  value._size,
	#endif
	  value._padding);
}

#endif