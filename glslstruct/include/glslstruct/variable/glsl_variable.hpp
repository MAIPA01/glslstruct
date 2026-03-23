/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_VARIABLE_HPP_
	#define _GLSL_STRUCT_VARIABLE_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/types.hpp>

namespace glslstruct {
	struct standard_variable {};

	template<class Layout>
	struct layout_variable {
		const Layout layout;

		explicit layout_variable(const Layout& layout) : layout(layout) {}
	};

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_simple_or_layout T, size_t num>
		#else
	template<class T, size_t num, std::enable_if_t<utils::is_glsl_simple_or_layout_v<T>, bool> >
		#endif
	struct glsl_variable : public std::conditional_t<utils::is_glsl_layout_v<T>, layout_variable<T>, standard_variable> {
	public:
		using var_type											= T;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t array_size = num;
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_layout	= utils::is_glsl_layout_v<T>;

		#pragma region VARIABLES
		const std::string var_name;
		#pragma endregion

		#pragma region NORMAL_CONSTRUCTOR
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class type																		  = var_type,
		  std::enable_if_t<utils::is_glsl_simple_v<type> && std::is_same_v<type, var_type>, bool> = true>
		#endif
		explicit _GLSL_STRUCT_CONSTEXPR20 glsl_variable(
		  const std::string_view name
		) noexcept _GLSL_STRUCT_REQUIRES(utils::is_glsl_simple_v<T>)
			: var_name(name) {
		}

		#pragma endregion

		#pragma region OFFSETS_CONSTRUCTOR
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class type																		  = var_type,
		  std::enable_if_t<utils::is_glsl_layout_v<type> && std::is_same_v<type, var_type>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 glsl_variable(const std::string_view name,
		  const T& layout) noexcept _GLSL_STRUCT_REQUIRES(utils::is_glsl_layout_v<T>)
			: layout_variable<T>(layout), var_name(name) {
		}

		#pragma endregion
	};
} // namespace glslstruct

	#endif
#endif