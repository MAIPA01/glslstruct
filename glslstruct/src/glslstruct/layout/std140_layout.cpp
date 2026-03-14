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
#include <glslstruct/layout/std140_layout.hpp>

using namespace glslstruct;

std140_layout::std140_layout() noexcept = default;
std140_layout::std140_layout(const std140_layout& other) noexcept = default;
std140_layout::std140_layout(std140_layout&& other) noexcept = default;
std140_layout::~std140_layout() noexcept = default;

std140_layout& std140_layout::operator=(const std140_layout& other) noexcept = default;
std140_layout& std140_layout::operator=(std140_layout&& other) noexcept = default;

size_t std140_layout::add(const std::string_view& name, const std140_layout& structLayout) {
	return _add_struct(name, structLayout.base_alignment(), structLayout._currentOffset, structLayout._values);
}

std::vector<size_t> std140_layout::add(const std::string_view& name, const std140_layout& structLayout, size_t count) {
	return _add_struct_array(name, structLayout.base_alignment(), structLayout._currentOffset,
		structLayout._values, count);
}

#if _GLSL_STRUCT_HAS_CXX20
bool std140_layout::operator==(const std140_layout& other) const noexcept = default;

bool std140_layout::operator!=(const std140_layout& other) const noexcept = default;
#else
bool std140_layout::operator==(const std140_layout& other) const noexcept {
	return base_layout::operator==(other);
}

bool std140_layout::operator!=(const std140_layout& other) const noexcept {
	return base_layout::operator!=(other);
}
#endif

size_t std::hash<std140_layout>::operator()(const std140_layout& std140Layout) const noexcept {
	return std::hash<base_layout>()(*static_cast<const base_layout*>(&std140Layout));
}

#endif