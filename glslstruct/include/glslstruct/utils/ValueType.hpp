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
	ENUM_CLASS_BASE(ValueType, uint8_t, (Bool), (Int), (Uint), (Float), (Double))
		// clang-format on

		#if _GLSL_STRUCT_HAS_CXX20
	template<class T>
		#else
	template<class T, std::enable_if_t<mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double>, bool> = true>
		#endif
	[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 ValueType
	  get_value_type() noexcept _GLSL_STRUCT_REQUIRES((mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double>)) {
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

	[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t get_value_type_size(ValueType type) {
		_GLSL_STRUCT_CONSTEXPR17 size_t intSize	   = 4;
		_GLSL_STRUCT_CONSTEXPR17 size_t doubleSize = 8;

			switch (type) {
			[[unlikely]] default:
				return 0;
			case ValueType::Bool:
			case ValueType::Int:
			case ValueType::Uint:
			case ValueType::Float:	return intSize;	   // sizeof(int) == sizeof(unsigned int) == sizeof(float); bool -> int
			case ValueType::Double: return doubleSize; // sizeof(double);
			}
	}
} // namespace glslstruct
	#endif
#endif