/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_VALUE_CHECKS_HPP_
	#define _GLSL_STRUCT_VALUE_CHECKS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/type_traits/mat_traits.hpp>
		#include <glslstruct/type_traits/scalar_traits.hpp>
		#include <glslstruct/type_traits/vec_traits.hpp>

namespace glslstruct::utils {
		#pragma region IS_SIMPLE
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_simple_v = is_glsl_scalar_v<T> || is_glsl_vec_v<T> || is_glsl_mat_v<T>;

		#if _GLSL_STRUCT_HAS_CXX20
	template<class T> concept glsl_simple = is_glsl_simple_v<T>;
		#endif
		#pragma endregion
} // namespace glslstruct::utils

	#endif
#endif