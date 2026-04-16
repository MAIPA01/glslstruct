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
	#include <glslstruct/type/traits/vec_traits.hpp>

using namespace glslstruct;

_GLSL_STRUCT_EXPORT std::string glslstruct::vec_to_string(const ValueType valueType, const size_t length) {
		switch (valueType) {
		[[unlikely]] default:
			return "UNKNOWN";
		case ValueType::Bool:	return fmt::format("bvec{}", length);
		case ValueType::Int:	return fmt::format("ivec{}", length);
		case ValueType::Uint:	return fmt::format("uvec{}", length);
		case ValueType::Float:	return fmt::format("vec{}", length);
		case ValueType::Double: return fmt::format("dvec{}", length);
		}
}

#endif