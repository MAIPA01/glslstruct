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

#include <pch.hpp>
#include <glslstruct/type/traits/mat_traits.hpp>

using namespace glslstruct;

std::string glslstruct::mat_to_string(const ValueType valueType, const size_t columns, const size_t rows) {
	std::string sizeStr = columns == rows ? std::to_string(columns) : fmt::format("{}x{}", columns, rows);

	switch (valueType) {
		[[unlikely]] default:
		return "UNKNOWN";
	case ValueType::Bool:	return fmt::format("bmat{}", sizeStr);
	case ValueType::Int:	return fmt::format("imat{}", sizeStr);
	case ValueType::Uint:	return fmt::format("umat{}", sizeStr);
	case ValueType::Float:	return fmt::format("mat{}", sizeStr);
	case ValueType::Double: return fmt::format("dmat{}", sizeStr);
	}
}

#endif