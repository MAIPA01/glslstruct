/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_SCALAR_TRAITS_CONCEPT_HPP_
	#define _GLSL_STRUCT_SCALAR_TRAITS_CONCEPT_HPP_

	#include "glslstruct/type_traits/scalar_traits.hpp"


	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/utils/is_vector_of.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
	class scalar_data;

	template<class>
	struct scalar_traits;

	namespace utils {
		#pragma region CHECKS

		#pragma region IS_SCALAR
		#if _GLSL_STRUCT_HAS_CXX20
		template<class T>
		concept glsl_scalar = requires {
			{ scalar_traits<T>::get_value_type() } -> std::same_as<ValueType>;
			{ scalar_traits<T>::get_data(std::declval<const T&>()) } -> std::same_as<scalar_data>;
			{ scalar_traits<T>::get_value(std::declval<const scalar_data&>()) } -> std::same_as<T>;
		};

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_scalar_v = glsl_scalar<T>;

		template<class T>
		struct is_glsl_scalar : std::bool_constant<is_glsl_scalar_v<T> > {};
		#else
		template<class T, class = void>
		struct is_glsl_scalar : std::false_type {};

		template<class T>
		struct is_glsl_scalar<T,
		  std::void_t<std::enable_if_t<std::is_same_v<ValueType, decltype(scalar_traits<T>::get_value_type())> >,
			std::enable_if_t<std::is_same_v<scalar_data, decltype(scalar_traits<T>::get_data(std::declval<const T&>()))> >,
			std::enable_if_t<std::is_same_v<T, decltype(scalar_traits<T>::get_value(std::declval<const scalar_data&>()))> > > >
			: std::true_type {};

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_scalar_v = is_glsl_scalar<T>::value;
		#endif
		#pragma endregion

		#pragma region IS_SCALARS_VECTOR
		template<class V>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_scalars_vector_v = is_vector_of_v<is_glsl_scalar, V>;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class V> concept glsl_scalars_vector = is_glsl_scalars_vector_v<V>;
		#endif
		#pragma endregion
		#pragma endregion
	} // namespace utils
} // namespace glslstruct

	#endif
#endif