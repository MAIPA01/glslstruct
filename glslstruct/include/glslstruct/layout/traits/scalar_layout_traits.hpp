/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_SCALAR_LAYOUT_TRAITS_HPP_
	#define _GLSL_STRUCT_SCALAR_LAYOUT_TRAITS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/layout/traits/contexts/layout_traits_contexts.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
	/**
	 * @brief Scalar Layout additional context
	 * @ingroup layout_traits
	 */
	struct scalar_layout_context : max_alignment_layout_context {};

	/**
	 * @brief Scalar Layout traits
	 * @ingroup layout_traits
	 */
	struct scalar_layout_traits {
		/// @brief scalar layout context type
		using context_type = scalar_layout_context;

		/// @brief scalar layout scalar alignment
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_scalar_alignment(const ValueType valueType, context_type&) noexcept {
			return get_value_type_size(valueType);
		}

		/// @brief scalar layout vec alignment
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_vec_alignment(const ValueType valueType, const size_t,
		  context_type& ctx) noexcept {
			return get_scalar_alignment(valueType, ctx);
		}

		/// @brief scalar layout array alignment
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_array_alignment(const size_t elemBaseAlignment, context_type&) noexcept {
			return elemBaseAlignment;
		}

		/// @brief scalar layout struct alignment
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_struct_alignment(const context_type& ctx) noexcept { return ctx.maxAlignment; }

		/// @brief scalar layout after_add action
		static _GLSL_STRUCT_CONSTEXPR17 void after_add(size_t&, const size_t, const size_t alignment,
		  context_type& ctx) noexcept {
			ctx.update_max_alignment(alignment);
		}
	};
} // namespace glslstruct

	#endif
#endif
