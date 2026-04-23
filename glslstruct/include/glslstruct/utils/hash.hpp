/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_HASH_HPP_
	#define _GLSL_STRUCT_HASH_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/libs.hpp>

namespace glslstruct::utils {
		#if _GLSL_STRUCT_HAS_CXX20
	/**
	 * @brief Checks if type has defined std::hash
	 * @ingroup utils
	 * @tparam T type
	 */
	template<class T> concept hashable = requires { typename std::hash<T>; };

	/**
	 * @brief Checks if type has defined std::hash
	 * @ingroup utils
	 * @tparam T type
	 */
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_hashable_v = hashable<T>;

	/**
	 * @brief Checks if type has defined std::hash
	 * @ingroup utils
	 * @tparam T type
	 */
	template<class T>
	struct is_hashable : std::bool_constant<is_hashable_v<T> > {};
		#else
	/**
	 * @brief Checks if type has defined std::hash
	 * @ingroup utils
	 * @tparam T type
	 */
	template<class T, class = void>
	struct is_hashable : std::false_type {};

	/**
	 * @brief Checks if type has defined std::hash
	 * @ingroup utils
	 * @tparam T type
	 */
	template<class T>
	struct is_hashable<T, std::void_t<std::hash<T> > > : std::true_type {};

	/**
	 * @brief Checks if type has defined std::hash
	 * @ingroup utils
	 * @tparam T type
	 */
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_hashable_v = is_hashable<T>::value;
		#endif
} // namespace glslstruct::utils

	#endif
#endif
