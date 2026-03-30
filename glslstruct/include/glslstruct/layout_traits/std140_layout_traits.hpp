/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_STD140_LAYOUT_TRAITS_HPP_
	#define _GLSL_STRUCT_STD140_LAYOUT_TRAITS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/layout_traits_contexts/layout_traits_contexts.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
	struct std140_layout_context : max_alignment_layout_context,
								   struct_added_layout_context {};

	struct std140_layout_traits {
		using context_type = std140_layout_context;

		static _GLSL_STRUCT_CONSTEXPR17 size_t get_scalar_alignment(const ValueType valueType, context_type&) noexcept {
			return get_value_type_size(valueType);
		}

		static _GLSL_STRUCT_CONSTEXPR17 size_t get_vec_alignment(const ValueType valueType, const size_t length,
		  context_type&) noexcept {
				if (length == 3) { return get_value_type_size(valueType) * 4; }
			return get_value_type_size(valueType) * length;
		}

		static _GLSL_STRUCT_CONSTEXPR17 size_t get_array_alignment(const size_t elemBaseAlignment, context_type&) noexcept {
			return context_type::ceil_to_nearest_multiple(elemBaseAlignment, 16);
		}

		static _GLSL_STRUCT_CONSTEXPR17 size_t get_struct_alignment(const context_type& ctx) noexcept {
			return context_type::ceil_to_nearest_multiple(ctx.maxAlignment, 16);
		}

		static _GLSL_STRUCT_CONSTEXPR17 void before_add(size_t& currentOffset, context_type& ctx) noexcept {
				if (ctx.structAlignment != 0) {
					currentOffset		= context_type::ceil_to_nearest_multiple(currentOffset, ctx.structAlignment);
					ctx.structAlignment = 0;
				}
		}

		static _GLSL_STRUCT_CONSTEXPR17 void after_add(size_t&, const size_t, const size_t alignment,
		  context_type& ctx) noexcept {
			ctx.update_max_alignment(alignment);
		}

		static _GLSL_STRUCT_CONSTEXPR17 void after_add_struct(size_t& currentOffset, const size_t size, const size_t alignment,
		  context_type& ctx) noexcept {
			ctx.structAlignment = alignment;
			after_add(currentOffset, size, alignment, ctx);
		}
	};
} // namespace glslstruct

	#endif
#endif
