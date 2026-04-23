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

	#include <glslstruct/layout/traits/contexts/contexts.hpp>
	#include <glslstruct/pch.hpp>

using namespace glslstruct;

void max_alignment_layout_context::update_max_alignment(const size_t currentAlignment) noexcept {
	maxAlignment = std::max(maxAlignment, currentAlignment);
}

size_t std::hash<max_alignment_layout_context>::operator()(const max_alignment_layout_context& ctx) const noexcept {
	return std::hash<size_t> {}(ctx.maxAlignment);
}

size_t std::hash<struct_added_layout_context>::operator()(const struct_added_layout_context&) const noexcept { return 0; }

#endif
