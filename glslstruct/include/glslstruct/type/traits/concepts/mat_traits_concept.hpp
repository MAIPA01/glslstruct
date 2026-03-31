/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_MAT_TRAITS_CONCEPT_HPP_
	#define _GLSL_STRUCT_MAT_TRAITS_CONCEPT_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/utils/is_vector_of.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
	class mat_data;

	/**
	 * @brief Contains all static functions needed for layout and struct classes to interpret given mat type as glsl mat value.
	 * @ingroup glslstruct
	 * @tparam T mat type for which user want to define own glsl converter
	 * @details static functions that need to be declared:
	 * @code{.cpp}
	 *	size_t get_columns();            // -> returns number of columns in matrix
	 *	size_t get_rows();               // -> returns number of rows in matrix
	 *	ValueType get_value_type();      // -> returns scalar type of matrix
	 *	mat_data get_data(const T&);     // -> returns bytes data in mat_data container
	 *	                                 //    (preferable is to use already declared type conversions
	 *	                                 //     glm::mat or mstd::mat)
	 *	T get_value(const mat_data&);    // -> returns value read from mat_data container
	 * @endcode
	 */
	template<class T>
	struct mat_traits {};

	namespace utils {
		#pragma region CHECKS
		#pragma region IS_MAT
		/**
			 * @var is_glsl_mat_v
			 * @brief Bool value which is true if type can be converted to glsl mat type
			 * @ingroup utils
			 * @tparam T type for which converter to glsl mat type should be defined
			 */

		/**
		 * @struct is_glsl_mat
		 * @brief struct with bool_constant which is true if type can be converted to glsl mat type
		 * @ingroup utils
		 * @tparam T type for which converter to glsl mat type should be defined
		 */

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief Concept defining which type can be converted to glsl mat type
		 * @ingroup utils
		 * @tparam T type for which converter to glsl mat type should be defined
		 */
		template<class T>
		concept glsl_mat = requires {
			{ mat_traits<T>::get_columns() } -> std::same_as<size_t>;
			{ mat_traits<T>::get_rows() } -> std::same_as<size_t>;
			{ mat_traits<T>::get_value_type() } -> std::same_as<ValueType>;
			{ mat_traits<T>::get_data(std::declval<const T&>()) } -> std::same_as<mat_data>;
			{ mat_traits<T>::get_value(std::declval<const mat_data&>()) } -> std::same_as<T>;
		};

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_mat_v = glsl_mat<T>;

		template<typename T>
		struct is_glsl_mat : std::bool_constant<is_glsl_mat_v<T> > {};

		#else
		template<typename T, typename = void>
		struct is_glsl_mat : std::false_type {};

		template<typename T>
		struct is_glsl_mat<T,
		  std::void_t<std::enable_if_t<std::is_same_v<size_t, decltype(mat_traits<T>::get_columns())> >,
			std::enable_if_t<std::is_same_v<size_t, decltype(mat_traits<T>::get_rows())> >,
			std::enable_if_t<std::is_same_v<ValueType, decltype(mat_traits<T>::get_value_type())> >,
			std::enable_if_t<std::is_same_v<mat_data, decltype(mat_traits<T>::get_data(std::declval<const T&>()))> >,
			std::enable_if_t<std::is_same_v<T, decltype(mat_traits<T>::get_value(std::declval<const mat_data&>()))> > > >
			: std::true_type {};

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_mat_v = is_glsl_mat<T>::value;
		#endif
		#pragma endregion

		#pragma region IS_MATS_VECTOR
		/**
		 * @brief Bool value which is true if type V is std::vector of types that passes is_glsl_mat test
		 * @ingroup utils
		 * @tparam V type that is checked
		 */
		template<class V>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_mats_vector_v = is_vector_of_v<is_glsl_mat, V>;

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief Concept which is true if type V is std::vector of types that passes is_glsl_mat test
		 * @ingroup utils
		 * @tparam V type that is checked
		 */
		template<class V> concept glsl_mats_vector = is_glsl_mats_vector_v<V>;
		#endif
		#pragma endregion
		#pragma endregion
	} // namespace utils
} // namespace glslstruct

	#endif
#endif