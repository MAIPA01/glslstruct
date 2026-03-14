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

#include "pch.hpp"
#include <glslstruct/layout/std430_layout.hpp>

using namespace glslstruct;

size_t std430_layout::_get_array_elem_base_alignment(size_t baseAlignment) const noexcept {
	return baseAlignment;
}

std430_layout::std430_layout() noexcept = default;
std430_layout::std430_layout(const std430_layout& other) noexcept = default;
std430_layout::std430_layout(std430_layout&& other) noexcept = default;
std430_layout::~std430_layout() noexcept = default;

std430_layout& std430_layout::operator=(const std430_layout& other) noexcept = default;
std430_layout& std430_layout::operator=(std430_layout&& other) noexcept = default;

size_t std430_layout::add(const std::string_view& name, const std430_layout& structLayout) {
	return _add_struct(name, structLayout.base_alignment(), structLayout._currentOffset, structLayout._values);
}

std::vector<size_t> std430_layout::add(const std::string_view& name, const std430_layout& structLayout, size_t count) {
	return _add_struct_array(name, structLayout.base_alignment(),
		structLayout._currentOffset, structLayout._values, count);
}

size_t std430_layout::base_alignment() const noexcept {
	return _maxAlignment;
}

#if _GLSL_STRUCT_HAS_CXX20
bool std430_layout::operator==(const std430_layout& other) const noexcept = default;
bool std430_layout::operator!=(const std430_layout& other) const noexcept = default;
#else
bool std430_layout::operator==(const std430_layout& other) const noexcept {
	return base_layout::operator==(other);
}
bool std430_layout::operator!=(const std430_layout& other) const noexcept {
	return base_layout::operator!=(other);
}
#endif

size_t std::hash<std430_layout>::operator()(const std430_layout& std430Layout) const noexcept {
	return std::hash<base_layout>()(*static_cast<const base_layout*>(&std430Layout));
}

#endif