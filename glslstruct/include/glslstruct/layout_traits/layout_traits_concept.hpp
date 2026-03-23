/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_LAYOUT_TRAITS_CONCEPT_HPP_
	#define _GLSL_STRUCT_LAYOUT_TRAITS_CONCEPT_HPP_

	#include "glslstruct/utils/MajorType.hpp"
	#include "glslstruct/utils/ValueType.hpp"


	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

// add custom additional Data

namespace glslstruct {
		// get_scalar_alignment(valueType)
		// get_vec_alignment(valueType, size_t length)
		// get_mat_alignment(valueType, size_t columns, size_t rows, majorType)
		// get_array_alignment(elemBaseAlignment)
		// get_struct_alignment(structBaseAlignment)

		#if _GLSL_STRUCT_HAS_CXX20
	template<class T>
	concept layout_traits = requires {
		{ T::get_scalar_alignment(std::declval<ValueType>()) } -> std::same_as<size_t>;
		{ T::get_vec_alignment(std::declval<ValueType>(), std::declval<size_t>()) } -> std::same_as<size_t>;
		{
			T::get_mat_alignment(std::declval<ValueType>(), std::declval<size_t>(), std::declval<size_t>())
		} -> std::same_as<size_t>;
		{ T::get_array_alignment(std::declval<size_t>()) } -> std::same_as<size_t>;
		{ T::get_struct_alignment(std::declval<size_t>()) } -> std::same_as<size_t>;
	};

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_layout_traits_v = layout_traits<T>;

	template<class T>
	struct is_layout_traits : std::bool_constant<is_layout_traits_v<T> > {};
		#else
	template<class T, class = void>
	struct is_layout_traits : std::false_type {};

	template<class T>
	struct is_layout_traits<T,
	  std::void_t<std::enable_if_t<std::is_same_v<size_t, decltype(T::get_scalar_alignment(std::declval<ValueType>()))> >,
		std::enable_if_t<std::is_same_v<size_t,
		  decltype(T::get_vec_alignment(std::declval<ValueType>(), std::declval<size_t>()))> >,
		std::enable_if_t<std::is_same_v<size_t,
		  decltype(T::get_mat_alignment(std::declval<ValueType>(), std::declval<size_t>(), std::declval<size_t>()))> >,
		std::enable_if_t<std::is_same_v<size_t, decltype(T::get_array_alignment(std::declval<size_t>()))> >,
		std::enable_if_t<std::is_same_v<size_t, decltype(T::get_struct_alignment(std::declval<size_t>()))> > > >
		: std::true_type {};

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_layout_traits_v = is_layout_traits<T>::value;
		#endif

} // namespace glslstruct

	#endif
#endif