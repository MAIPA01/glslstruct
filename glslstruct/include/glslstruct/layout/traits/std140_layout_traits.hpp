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

		#include <glslstruct/layout/traits/contexts/layout_traits_contexts.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
	/**
	 * @brief Std140 Layout additional context
	 * @ingroup layout_traits
	 */
	struct _GLSL_STRUCT_EXPORT std140_layout_context : max_alignment_layout_context,
								   struct_added_layout_context {};

	/**
	 * @brief Std140 Layout traits
	 * @ingroup layout_traits
	 */
	struct _GLSL_STRUCT_EXPORT std140_layout_traits {
		/// @brief std140 layout context type
		using context_type = std140_layout_context;

		/// @brief std140 layout scalar alignment
		static size_t get_scalar_alignment(ValueType valueType, context_type& ctx) noexcept;

		/// @brief std140 layout vec alignment
		static size_t get_vec_alignment(ValueType valueType, size_t length, context_type& ctx) noexcept;

		/// @brief std140 layout array alignment
		static size_t get_array_alignment(size_t elemBaseAlignment, context_type& ctx) noexcept;

		/// @brief std140 layout struct alignment
		static size_t get_struct_alignment(const context_type& ctx) noexcept;

		/// @brief std140 layout before_add action
		static void before_add(size_t& currentOffset, context_type& ctx) noexcept;

		/// @brief std140 layout after_add action
		static void after_add(size_t& currentOffset, size_t size, size_t alignment, context_type& ctx) noexcept;

		/// @brief std140 layout after_add_struct action
		static void after_add_struct(size_t& currentOffset, size_t size, size_t alignment, context_type& ctx) noexcept;
	};
} // namespace glslstruct

	#endif
#endif
