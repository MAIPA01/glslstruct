/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_TYPE_CHECKS_HPP_
	#define _GLSL_STRUCT_TYPE_CHECKS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1!"
);
	#else

namespace glslstruct {
		#pragma region TYPES
	class base_type;
	template<class Derived>
	class type;
	class scalar_type;
	class vec_type;
	class mat_type;
	class array_type;
	class struct_type;
		#pragma endregion

		#pragma region TYPES_HANDLES
	using base_type_handle = std::shared_ptr<base_type>;
	template<class Derived>
	using type_handle		 = std::shared_ptr<type<Derived> >;
	using scalar_type_handle = std::shared_ptr<scalar_type>;
	using vec_type_handle	 = std::shared_ptr<vec_type>;
	using mat_type_handle	 = std::shared_ptr<mat_type>;
	using array_type_handle	 = std::shared_ptr<array_type>;
	using struct_type_handle = std::shared_ptr<struct_type>;
		#pragma endregion

	namespace utils {
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_type_v = std::is_base_of_v<base_type, T> && !std::is_same_v<base_type, T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_base_type_v = std::is_same_v<base_type, T>;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class T> concept glsl_type		 = is_glsl_type_v<T>;

		template<class T> concept glsl_base_type = is_glsl_base_type_v<T>;
		#endif
	} // namespace utils
} // namespace glslstruct

	#endif
#endif