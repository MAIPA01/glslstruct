/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

	#include <glslstruct/pch.hpp>
	#include <glslstruct/utils/ValueType.hpp>

using namespace glslstruct;

_GLSL_STRUCT_EXPORT size_t glslstruct::get_value_type_size(const ValueType type) {
	_GLSL_STRUCT_CONSTEXPR17 size_t int_size	= 4;
	_GLSL_STRUCT_CONSTEXPR17 size_t double_size = 8;

		switch (type) {
		[[unlikely]] default:
			return 0;
		case ValueType::Bool:
		case ValueType::Int:
		case ValueType::Uint:
		case ValueType::Float:	return int_size;	// sizeof(int) == sizeof(unsigned int) == sizeof(float); bool -> int
		case ValueType::Double: return double_size; // sizeof(double);
		}
}

#endif