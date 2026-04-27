/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_LAYOUT_TRAITS_CONTEXTS_HPP_
	#define _GLSL_STRUCT_LAYOUT_TRAITS_CONTEXTS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

namespace glslstruct {
	/**
	 * @brief layout context which contains max alignment of all variables
	 * @ingroup glslstruct
	 */
	struct max_alignment_layout_context {
		/// @brief contains max alignment value
		size_t maxAlignment = 0;

		/// @brief updates max alignment value
		void update_max_alignment(size_t currentAlignment) noexcept;
	};

	/**
	 * @brief layout context which contains alignment of last added struct
	 * @ingroup glslstruct
	 */
	struct struct_added_layout_context {
		/// @brief contains last added struct alignment
		size_t structAlignment = 0;
	};
} // namespace glslstruct

/**
 * @brief std::hash specialization for glslstruct::max_alignment_layout_context
 * @ingroup glslstruct
 */
template<>
struct std::hash<glslstruct::max_alignment_layout_context> {
	[[nodiscard]] size_t operator()(const glslstruct::max_alignment_layout_context& ctx) const noexcept;
};

/**
 * @brief std::hash specialization for glslstruct::struct_added_layout_context
 * @ingroup glslstruct
 */
template<>
struct std::hash<glslstruct::struct_added_layout_context> {
	[[nodiscard]] size_t operator()(const glslstruct::struct_added_layout_context& ctx) const noexcept;
};

	#endif
#endif