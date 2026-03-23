/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_LAYOUT_CHECKS_HPP_
	#define _GLSL_STRUCT_LAYOUT_CHECKS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/layout_traits/layout_traits_concept.hpp>
		#include <glslstruct/layout_traits/scalar_layout_traits.hpp>
		#include <glslstruct/layout_traits/std140_layout_traits.hpp>
		#include <glslstruct/layout_traits/std430_layout_traits.hpp>
		#include <glslstruct/type_checks/simple_checks.hpp>

namespace glslstruct {
		#if _GLSL_STRUCT_HAS_CXX20
	template<layout_traits T>
		#else
	template<class T, std::enable_if_t<is_layout_traits_v<T>, bool> = true>
		#endif
	class base_layout;

	using std140_layout = base_layout<std140_layout_traits>;
	using std430_layout = base_layout<std430_layout_traits>;
	using scalar_layout = base_layout<scalar_layout_traits>;

	namespace utils {
		#pragma region IS_LAYOUT

		template<class T>
		struct is_glsl_layout : std::false_type {};

		template<class T>
		struct is_glsl_layout<base_layout<T> > : std::true_type {};

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_layout_v = is_glsl_layout<T>::value;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class T> concept glsl_layout = is_glsl_layout_v<T>;
		#endif
		#pragma endregion

		#pragma region IS_SIMPLE_OR_LAYOUT
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_simple_or_layout_v = is_glsl_simple_v<T> || is_glsl_layout_v<T>;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class T> concept glsl_simple_or_layout = is_glsl_simple_or_layout_v<T>;
		#endif
		#pragma endregion
	} // namespace utils
} // namespace glslstruct
	#endif
#endif