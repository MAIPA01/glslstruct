/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_VALUE_TYPE_HPP_
	#define _GLSL_STRUCT_VALUE_TYPE_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

namespace glslstruct {
	// clang-format off
	/**
	 * @brief Value Type Enum
	 * @ingroup glslstruct
	 */
	_GLSL_STRUCT_EXPORT ENUM_CLASS_BASE(ValueType, uint8_t, (Bool), (Int), (Uint), (Float), (Double))
		// clang-format on

		#if _GLSL_STRUCT_HAS_CXX20
	  /**
	   * @brief Converts type to ValueType Enum value
	   * @ingroup glslstruct
	   * @tparam T type of scalar to convert to ValueType enum (only works for { bool, int, unsigned int, float, double })
	   */
	  template<class T>
		#else
	  /**
	   * @brief Converts type to ValueType Enum value
	   * @ingroup glslstruct
	   * @tparam T type of scalar to convert to ValueType enum (only works for { bool, int, unsigned int, float, double })
	   */
	  template<class T, std::enable_if_t<mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double>, bool> = true>
		#endif
	  [[nodiscard]] static _GLSL_STRUCT_EXPORT _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept _GLSL_STRUCT_REQUIRES(
		(mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double>)
	  ) {
			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) { return ValueType::Bool; }
			else if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, int>) { return ValueType::Int; }
			else if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, unsigned int>) { return ValueType::Uint; }
			else if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, float>) { return ValueType::Float; }
			else if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, double>) { return ValueType::Double; }
			else {
				static_assert(false, "Unknown value type!");
				return ValueType::Bool; // for clang-tidy error
			}
	}

	/**
	 * @brief Returns glsl type size of scalar
	 * @ingroup glslstruct
	 * @param type scalar value type
	 */
	[[nodiscard]] _GLSL_STRUCT_EXPORT size_t get_value_type_size(ValueType type);
} // namespace glslstruct
	#endif
#endif