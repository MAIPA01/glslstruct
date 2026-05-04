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

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/utils/array_traits.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
	class scalar_data;

	/**
	 * @brief Contains all static functions needed for layout and struct classes to interpret given scalar type as glsl scalar value.
	 * @ingroup glslstruct
	 * @tparam T scalar type for which user want to define own glsl converter
	 * @details static functions that need to be declared:
	 * @code{.cpp}
	 *	ValueType get_value_type();         // -> returns scalar type
	 *	scalar_data get_data(const T&);     // -> returns bytes data in scalar_data container
	 *	                                    //    (preferable is to use already declared type conversions
	 *	                                    //     bool, int, unsigned int, float or double)
	 *	T get_value(const scalar_data&);    // -> returns value read from scalar_data container
	 * @endcode
	 */
	template<class T>
	struct _GLSL_STRUCT_EXPORT scalar_traits {};

		#pragma region CHECKS
		#pragma region IS_SCALAR
		/**
		 * @var is_glsl_scalar_v
		 * @brief Bool value which is true if type can be converted to glsl scalar type
		 * @ingroup glslstruct
		 * @tparam T type for which converter to glsl scalar type should be defined
		 */

		/**
		 * @struct is_glsl_scalar
		 * @brief struct with bool_constant which is true if type can be converted to glsl scalar type
		 * @ingroup glslstruct
		 * @tparam T type for which converter to glsl scalar type should be defined
		 */

		#if _GLSL_STRUCT_HAS_CXX20
	/**
	 * @brief Concept defining which type can be converted to glsl scalar type
	 * @ingroup glslstruct
	 * @tparam T type for which converter to glsl scalar type should be defined
	 */
	template<class T>
	concept glsl_scalar = requires {
		{ scalar_traits<T>::get_value_type() } -> std::convertible_to<ValueType>;
		{ scalar_traits<T>::get_data(std::declval<const T&>()) } -> std::convertible_to<scalar_data>;
		{ scalar_traits<T>::get_value(std::declval<const scalar_data&>()) } -> std::convertible_to<T>;
	};

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_scalar_v = glsl_scalar<T>;

	template<class T>
	struct is_glsl_scalar : std::bool_constant<is_glsl_scalar_v<T> > {};
		#else
	template<class T, class = void>
	struct is_glsl_scalar : std::false_type {};

	template<class T>
	struct is_glsl_scalar<T,
	  std::void_t<std::enable_if_t<std::is_convertible_v<decltype(scalar_traits<T>::get_value_type()), ValueType> >,
		std::enable_if_t<std::is_convertible_v<decltype(scalar_traits<T>::get_data(std::declval<const T&>())), scalar_data> >,
		std::enable_if_t<std::is_convertible_v<decltype(scalar_traits<T>::get_value(std::declval<const scalar_data&>())), T> > > >
		: std::true_type {};

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_scalar_v = is_glsl_scalar<T>::value;
		#endif
		#pragma endregion

	namespace utils {
		#pragma region IS_SCALARS_ARRAY
		/**
		 * @brief Bool value which is true if type V is array of types that passes is_glsl_scalar test
		 * @ingroup utils
		 * @tparam V type that is checked
		 */
		template<class V>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_scalars_array_v = is_array_of_v<is_glsl_scalar, V>;

		/**
		 * @brief Bool value which is true if type V is static size array of types that passes is_glsl_scalar test
		 * @ingroup utils
		 * @tparam V type that is checked
		 */
		template<class V>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_scalars_static_size_array_v = is_static_size_array_of_v<is_glsl_scalar, V>;

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief Concept which is true if type V is array of types that passes is_glsl_scalar test
		 * @ingroup utils
		 * @tparam V type that is checked
		 */
		template<class V> concept glsl_scalars_array			 = is_glsl_scalars_array_v<V>;

		/**
		 * @brief Concept which is true if type V is static size array of types that passes is_glsl_scalar test
		 * @ingroup utils
		 * @tparam V type that is checked
		 */
		template<class V> concept glsl_scalars_static_size_array = is_glsl_scalars_static_size_array_v<V>;
		#endif
		#pragma endregion
	} // namespace utils

		#pragma endregion
} // namespace glslstruct

	#endif
#endif