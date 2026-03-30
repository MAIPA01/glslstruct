/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_TYPES_HPP_
	#define _GLSL_STRUCT_TYPES_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/type_checks/struct_checks.hpp>
#if _GLSL_STRUCT_HAS_TYPES
		#include <glslstruct/type_checks/type_checks.hpp>
#endif

namespace glslstruct {
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_simple_or_layout T, size_t num = 0>
		#else
	template<class T, size_t num = 0, std::enable_if_t<utils::is_glsl_simple_or_layout_v<T>, bool> = true>
		#endif
	struct glsl_variable;

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_simple_or_struct T, size_t num = 0>
		#else
	template<class T, size_t num = 0, std::enable_if_t<utils::is_glsl_simple_or_struct_v<T>, bool> = true>
		#endif
	struct glsl_value;
} // namespace glslstruct

		#ifndef GLSL_STRUCT_DISABLE_GLM
			#pragma region SOME_ADDITIONAL_GLM_TYPES

namespace glm {
	using bmat2x2 = mat<2, 2, bool, packed_highp>;
	using bmat2x3 = mat<2, 3, bool, packed_highp>;
	using bmat2x4 = mat<2, 4, bool, packed_highp>;
	using bmat3x2 = mat<3, 2, bool, packed_highp>;
	using bmat3x3 = mat<3, 3, bool, packed_highp>;
	using bmat3x4 = mat<3, 4, bool, packed_highp>;
	using bmat4x2 = mat<4, 2, bool, packed_highp>;
	using bmat4x3 = mat<4, 3, bool, packed_highp>;
	using bmat4x4 = mat<4, 4, bool, packed_highp>;

	using bmat2	  = bmat2x2;
	using bmat3	  = bmat3x3;
	using bmat4	  = bmat4x4;

	using umat2	  = umat2x2;
	using umat3	  = umat3x3;
	using umat4	  = umat4x4;

	using imat2	  = imat2x2;
	using imat3	  = imat3x3;
	using imat4	  = imat4x4;
} // namespace glm

			#pragma endregion
		#endif

	#endif
#endif