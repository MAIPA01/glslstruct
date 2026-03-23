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

		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
	struct std140_layout_traits {
		static _GLSL_STRUCT_CONSTEXPR17 size_t ceil_to_nearest_multiple(size_t valueToRoundUp, size_t multipleValue) noexcept {
			size_t modulo = valueToRoundUp % multipleValue;
				if (modulo != 0) { valueToRoundUp += multipleValue - modulo; }
			return valueToRoundUp;
		}

		static _GLSL_STRUCT_CONSTEXPR17 size_t get_scalar_alignment(ValueType valueType) noexcept {
			return get_value_type_size(valueType);
		}

		static _GLSL_STRUCT_CONSTEXPR17 size_t get_vec_alignment(ValueType valueType, size_t length) noexcept {
				if (length == 3) { return get_value_type_size(valueType) * 4; }
			return get_value_type_size(valueType) * length;
		}

		static _GLSL_STRUCT_CONSTEXPR17 size_t get_mat_alignment(ValueType valueType, size_t, size_t rows) noexcept {
			return ceil_to_nearest_multiple(get_value_type_size(valueType) * rows, 16);
		}

		static _GLSL_STRUCT_CONSTEXPR17 size_t get_array_alignment(size_t elemBaseAlignment) noexcept {
			return ceil_to_nearest_multiple(elemBaseAlignment, 16);
		}

		static _GLSL_STRUCT_CONSTEXPR17 size_t get_struct_alignment(size_t baseAlignment) noexcept {
			return ceil_to_nearest_multiple(baseAlignment, 16);
		}
	};
} // namespace glslstruct

	#endif
#endif
