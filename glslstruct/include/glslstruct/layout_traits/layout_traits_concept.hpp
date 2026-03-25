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
		// get_scalar_alignment(const ValueType) -> size_t
		// get_vec_alignment(const ValueType, const size_t length) -> size_t
		// get_array_alignment(const size_t elemBaseAlignment) -> size_t
		// get_struct_alignment(const size_t structBaseAlignment) -> size_t
		// get_struct_size(const size_t structBaseOffset) -> size_t

		#if _GLSL_STRUCT_HAS_CXX20
	template<class T>
	concept layout_traits = requires {
		{ T::get_scalar_alignment(std::declval<const ValueType>()) } -> std::same_as<size_t>;
		{ T::get_vec_alignment(std::declval<const ValueType>(), std::declval<const size_t>()) } -> std::same_as<size_t>;
		{ T::get_array_alignment(std::declval<const size_t>()) } -> std::same_as<size_t>;
		{ T::get_struct_alignment(std::declval<const size_t>()) } -> std::same_as<size_t>;
		{ T::get_struct_size(std::declval<const size_t>()) } -> std::same_as<size_t>;
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
	  std::void_t<std::enable_if_t<std::is_same_v<size_t, decltype(T::get_scalar_alignment(std::declval<const ValueType>()))> >,
		std::enable_if_t<std::is_same_v<size_t,
		  decltype(T::get_vec_alignment(std::declval<const ValueType>(), std::declval<const size_t>()))> >,
		std::enable_if_t<std::is_same_v<size_t, decltype(T::get_array_alignment(std::declval<const size_t>()))> >,
		std::enable_if_t<std::is_same_v<size_t, decltype(T::get_struct_alignment(std::declval<const size_t>()))> >,
		std::enable_if_t<std::is_same_v<size_t, decltype(T::get_struct_size(std::declval<const size_t>()))> > > >
		: std::true_type {};

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_layout_traits_v = is_layout_traits<T>::value;
		#endif

} // namespace glslstruct

	#endif
#endif