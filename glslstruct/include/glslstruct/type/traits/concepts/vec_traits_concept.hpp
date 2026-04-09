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

		#include <glslstruct/utils/array_traits.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
	class vec_data;

	/**
	 * @brief Contains all static functions needed for layout and struct classes to interpret given vec type as glsl vec value.
	 * @ingroup glslstruct
	 * @tparam T vec type for which user want to define own glsl converter
	 * @details static functions that need to be declared:
	 * @code{.cpp}
	 *	size_t get_length();            // -> returns length of vector
	 *	ValueType get_value_type();     // -> returns scalar type of vector
	 *	vec_data get_data(const T&);    // -> returns bytes data in vec_data container
	 *	                                //    (preferable is to use already declared type conversions
	 *	                                //     glm::vec or mstd::vec)
	 *	T get_value(const vec_data&);   // -> returns value read from vec_data container
	 * @endcode
	 */
	template<class T>
	struct vec_traits {};

	namespace utils {
		#pragma region CHECKS
		#pragma region IS_VEC
			/**
			 * @var is_glsl_vec_v
			 * @brief Bool value which is true if type can be converted to glsl vec type
			 * @ingroup utils
			 * @tparam T type for which converter to glsl vec type should be defined
			 */

			/**
			 * @struct is_glsl_vec
			 * @brief struct with bool_constant which is true if type can be converted to glsl vec type
			 * @ingroup utils
			 * @tparam T type for which converter to glsl vec type should be defined
			 */

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief Concept defining which type can be converted to glsl vec type
		 * @ingroup utils
		 * @tparam T type for which converter to glsl vec type should be defined
		 */
		template<class T>
		concept glsl_vec = requires {
			{ vec_traits<T>::get_length() } -> std::same_as<size_t>;
			{ vec_traits<T>::get_value_type() } -> std::same_as<ValueType>;
			{ vec_traits<T>::get_data(std::declval<const T&>()) } -> std::same_as<vec_data>;
			{ vec_traits<T>::get_value(std::declval<const vec_data&>()) } -> std::same_as<T>;
		};

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_vec_v = glsl_vec<T>;

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
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_vec_v = is_glsl_vec<T>::value;
		#endif
		#pragma endregion

		#pragma region IS_VECS_ARRAY
		/**
		 * @brief Bool value which is true if type V is array of types that passes is_glsl_vec test
		 * @ingroup utils
		 * @tparam V type that is checked
		 */
		template<class V>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_vecs_array_v = is_array_of_v<is_glsl_vec, V>;

		/**
		 * @brief Bool value which is true if type V is static size array of types that passes is_glsl_vec test
		 * @ingroup utils
		 * @tparam V type that is checked
		 */
		template<class V>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_vecs_static_size_array_v = is_static_size_array_of_v<is_glsl_vec, V>;

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief Concept which is true if type V is array of types that passes is_glsl_vec test
		 * @ingroup utils
		 * @tparam V type that is checked
		 */
		template<class V> concept glsl_vecs_array			  = is_glsl_vecs_array_v<V>;

		/**
		 * @brief Concept which is true if type V is static size array of types that passes is_glsl_vec test
		 * @ingroup utils
		 * @tparam V type that is checked
		 */
		template<class V> concept glsl_vecs_static_size_array = is_glsl_vecs_static_size_array_v<V>;
		#endif
		#pragma endregion
		#pragma endregion
	} // namespace utils
} // namespace glslstruct

	#endif
#endif