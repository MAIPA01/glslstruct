/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_VEC_TRAITS_CONCEPT_HPP
	#define _GLSL_STRUCT_VEC_TRAITS_CONCEPT_HPP

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/utils/is_vector_of.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
	class vec_data;

	template<class>
	struct vec_traits;

	namespace utils {
		#pragma region CHECKS
		#pragma region IS_VEC
		#if _GLSL_STRUCT_HAS_CXX20
		template<class T>
		concept glsl_vec = requires {
			{ vec_traits<T>::get_length() } -> std::same_as<size_t>;
			{ vec_traits<T>::get_value_type() } -> std::same_as<ValueType>;
			{ vec_traits<T>::get_data(std::declval<const T&>()) } -> std::same_as<vec_data>;
			{ vec_traits<T>::get_value(std::declval<const vec_data&>()) } -> std::same_as<T>;
		};

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_vec_v = glsl_vec<T>;

		template<class T>
		struct is_glsl_vec : std::bool_constant<is_glsl_vec_v<T> > {};

		#else
		template<typename T, typename = void>
		struct is_glsl_vec : std::false_type {};

		template<class T>
		struct is_glsl_vec<T,
		  std::void_t<std::enable_if_t<std::is_same_v<size_t, decltype(vec_traits<T>::get_length())> >,
			std::enable_if_t<std::is_same_v<ValueType, decltype(vec_traits<T>::get_value_type())> >,
			std::enable_if_t<std::is_same_v<vec_data, decltype(vec_traits<T>::get_data(std::declval<const T&>()))> >,
			std::enable_if_t<std::is_same_v<T, decltype(vec_traits<T>::get_value(std::declval<const vec_data&>()))> > > >
			: std::true_type {};

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_vec_v = is_glsl_vec<T>::value;
		#endif
		#pragma endregion

		#pragma region IS_VECS_VECTOR
		template<class V>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_vecs_vector_v = is_vector_of_v<is_glsl_vec, V>;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class V> concept glsl_vecs_vector = is_glsl_vecs_vector_v<V>;
		#endif
		#pragma endregion
		#pragma endregion
	} // namespace utils
} // namespace glslstruct

	#endif
#endif