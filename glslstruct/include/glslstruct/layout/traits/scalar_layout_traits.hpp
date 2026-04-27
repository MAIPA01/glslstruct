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

		#include <glslstruct/layout/traits/contexts/contexts.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
	/**
	 * @brief Scalar Layout additional context
	 * @ingroup layout_traits
	 */
	struct _GLSL_STRUCT_EXPORT scalar_layout_context : max_alignment_layout_context {};

	/**
	 * @brief Scalar Layout traits
	 * @ingroup layout_traits
	 */
	struct _GLSL_STRUCT_EXPORT scalar_layout_traits {
		/// @brief scalar layout context type
		using context_type = scalar_layout_context;

		/// @brief scalar layout scalar alignment
		static size_t get_scalar_alignment(ValueType valueType, context_type& ctx) noexcept;

		/// @brief scalar layout vec alignment
		static size_t get_vec_alignment(ValueType valueType, size_t length, context_type& ctx) noexcept;

		/// @brief scalar layout array alignment
		static size_t get_array_alignment(size_t elemBaseAlignment, context_type& ctx) noexcept;

		/// @brief scalar layout struct alignment
		static size_t get_struct_alignment(const context_type& ctx) noexcept;

		/// @brief scalar layout after_add action
		static void after_add(size_t& currentOffset, size_t size, size_t alignment, context_type& ctx) noexcept;
	};
} // namespace glslstruct

/**
 * @brief std::hash specialization for glslstruct::scalar_layout_context
 * @ingroup glslstruct
 */
template<>
struct _GLSL_STRUCT_EXPORT std::hash<glslstruct::scalar_layout_context> {
	[[nodiscard]] size_t operator()(const glslstruct::scalar_layout_context& ctx) const noexcept;
};

	#endif
#endif
