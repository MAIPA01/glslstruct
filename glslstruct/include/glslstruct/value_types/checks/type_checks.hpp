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
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1!");
#else

#include <glslstruct/types.hpp>

namespace glslstruct::utils {
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_type_v = std::is_base_of_v<base_type, T> && !std::is_same_v<base_type, T>;

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_base_type_v = std::is_same_v<base_type, T>;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T>
	concept glsl_type = is_glsl_type_v<T>;

	template<class T>
	concept glsl_base_type = is_glsl_base_type_v<T>;
#endif
}

#endif