/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

namespace glslstruct {
	ENUM_CLASS_BASE(ValueType, uint8_t,
		(Other),
		(Bool),
		(Int),
		(Uint),
		(Float),
		(Double)
	)

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
	[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept {
		if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
			return ValueType::Bool;
		}
		else if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, int>) {
			return ValueType::Int;
		}
		else if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, unsigned int>) {
			return ValueType::Uint;
		}
		else if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, float>) {
			return ValueType::Float;
		}
		else if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, double>) {
			return ValueType::Double;
		}
		else {
			return ValueType::Other;
		}
	}

	[[nodiscard]] static inline _GLSL_STRUCT_CONSTEXPR20 std::string vec_type_to_string(ValueType type) noexcept {
		switch (type) {
		[[unlikely]] case ValueType::Other: return "other";
		case ValueType::Bool: return "bvec";
		case ValueType::Int: return "ivec";
		case ValueType::Uint: return "uvec";
		case ValueType::Float: return "vec";
		case ValueType::Double: return "dvec";
		[[unlikely]] default: return "UNKNOWN";
		}
	}

	[[nodiscard]] static inline _GLSL_STRUCT_CONSTEXPR20 std::string mat_type_to_string(ValueType type) noexcept {
		switch (type) {
		[[unlikely]] case ValueType::Other: return "other";
		case ValueType::Bool: return "bmat";
		case ValueType::Int: return "imat";
		case ValueType::Uint: return "umat";
		case ValueType::Float: return "mat";
		case ValueType::Double: return "dmat";
		[[unlikely]] default: return "UNKNOWN";
		}
	}

	[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t get_value_type_size(ValueType type) {
		switch (type) {
		[[unlikely]] default:
		[[unlikely]] case ValueType::Other:
			return 0;
		case ValueType::Bool:
		case ValueType::Int:
			return sizeof(int);
		case ValueType::Uint:
			return sizeof(unsigned int);
		case ValueType::Float:
			return sizeof(float);
		case ValueType::Double:
			return sizeof(double);
		}
	}
}
#endif