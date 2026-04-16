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
	#include <glslstruct/layout/traits/scalar_layout_traits.hpp>

using namespace glslstruct;

size_t scalar_layout_traits::get_scalar_alignment(const ValueType valueType, context_type&) noexcept {
	return get_value_type_size(valueType);
}

size_t scalar_layout_traits::get_vec_alignment(const ValueType valueType, const size_t, context_type& ctx) noexcept {
	return get_scalar_alignment(valueType, ctx);
}

size_t scalar_layout_traits::get_array_alignment(const size_t elemBaseAlignment, context_type&) noexcept {
	return elemBaseAlignment;
}

size_t scalar_layout_traits::get_struct_alignment(const context_type& ctx) noexcept { return ctx.maxAlignment; }

void scalar_layout_traits::after_add(size_t&, const size_t, const size_t alignment, context_type& ctx) noexcept {
	ctx.update_max_alignment(alignment);
}

#endif