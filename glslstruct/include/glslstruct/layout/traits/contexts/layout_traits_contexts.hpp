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
		size_t maxAlignment = 0;

		_GLSL_STRUCT_CONSTEXPR17 void update_max_alignment(const size_t currentAlignment) noexcept {
			maxAlignment = std::max(maxAlignment, currentAlignment);
		}
	};

	/**
	 * @brief layout context which contains alignment of last added struct
	 * @ingroup glslstruct
	 */
	struct struct_added_layout_context {
		size_t structAlignment = 0;

		static _GLSL_STRUCT_CONSTEXPR17 size_t ceil_to_nearest_multiple(size_t valueToRoundUp,
		  const size_t multipleValue) noexcept {
				if (const size_t modulo = valueToRoundUp % multipleValue; modulo != 0) {
					valueToRoundUp += multipleValue - modulo;
				}
			return valueToRoundUp;
		}
	};
} // namespace glslstruct

	#endif
#endif