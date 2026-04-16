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

	#include <glslstruct/pch.hpp>
	#include <glslstruct/layout/traits/std430_layout_traits.hpp>
	#include <glslstruct/utils/functions.hpp>

using namespace glslstruct;

size_t std430_layout_traits::get_scalar_alignment(const ValueType valueType, context_type&) noexcept {
	return get_value_type_size(valueType);
}

size_t std430_layout_traits::get_vec_alignment(const ValueType valueType, const size_t length, context_type&) noexcept {
		if (length == 3) { return get_value_type_size(valueType) * 4; }
	return get_value_type_size(valueType) * length;
}

size_t std430_layout_traits::get_array_alignment(const size_t elemBaseAlignment, context_type&) noexcept {
	return elemBaseAlignment;
}

size_t std430_layout_traits::get_struct_alignment(const context_type& ctx) noexcept { return ctx.maxAlignment; }

void std430_layout_traits::before_add(size_t& currentOffset, context_type& ctx) noexcept {
		if (ctx.structAlignment != 0) {
			currentOffset		= ceil_to_nearest_multiple(currentOffset, ctx.structAlignment);
			ctx.structAlignment = 0;
		}
}

void std430_layout_traits::after_add(size_t&, const size_t, const size_t alignment, context_type& ctx) noexcept {
	ctx.update_max_alignment(alignment);
}

void std430_layout_traits::after_add_struct(size_t& currentOffset, const size_t size, const size_t alignment,
  context_type& ctx) noexcept {
	ctx.structAlignment = alignment;
	after_add(currentOffset, size, alignment, ctx);
}

#endif