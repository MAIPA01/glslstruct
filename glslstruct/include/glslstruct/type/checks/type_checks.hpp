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
	/**
	 * @brief base_type container handle
	 * @ingroup glslstruct
	 */
	using base_type_handle = std::shared_ptr<base_type>;
	/**
	 * @brief type container handle
	 * @ingroup glslstruct
	 * @tparam Derived derived type
	 */
	template<class Derived>
	using type_handle		 = std::shared_ptr<type<Derived> >;
	/**
	 * @brief scalar_type container handle
	 * @ingroup glslstruct
	 */
	using scalar_type_handle = std::shared_ptr<scalar_type>;
	/**
	 * @brief vec_type container handle
	 * @ingroup glslstruct
	 */
	using vec_type_handle	 = std::shared_ptr<vec_type>;
	/**
	 * @brief mat_type container handle
	 * @ingroup glslstruct
	 */
	using mat_type_handle	 = std::shared_ptr<mat_type>;
	/**
	 * @brief array_type container handle
	 * @ingroup glslstruct
	 */
	using array_type_handle	 = std::shared_ptr<array_type>;
	/**
	 * @brief struct_type container handle
	 * @ingroup glslstruct
	 */
	using struct_type_handle = std::shared_ptr<struct_type>;
		#pragma endregion

	/**
	 * @brief check if T is glsl type (not base_type container but derived from base_type container)
	 * @ingroup glslstruct
	 * @tparam T type which should be a derived from base_type but not base_type
	 */
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_type_v =
	  std::is_base_of_v<base_type, mstd::remove_cvref_t<T> > && !std::is_same_v<base_type, mstd::remove_cvref_t<T> >;

	/**
	 * @brief check if T is base_type container
	 * @ingroup glslstruct
	 * @tparam T type which should be a base_type
	 */
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_base_type_v = std::is_same_v<base_type, mstd::remove_cvref_t<T> >;

	/**
	 * @brief check if T is glsl type (not base_type container but derived from base_type container)
	 * @ingroup glslstruct
	 * @tparam T type which should be a derived from base_type but not base_type
	 */
	template<class T>
	struct is_glsl_type : std::bool_constant<is_glsl_type_v<T> > {};

	/**
	 * @brief check if T is base_type container
	 * @ingroup glslstruct
	 * @tparam T type which should be a base_type
	 */
	template<class T>
	struct is_glsl_base_type : std::bool_constant<is_glsl_base_type_v<T> > {};

		#if _GLSL_STRUCT_HAS_CXX20
	/**
	 * @brief check if T is glsl type (not base_type container but derived from base_type container)
	 * @ingroup glslstruct
	 * @tparam T type which should be a derived from base_type but not base_type
	 */
	template<class T> concept glsl_type		 = is_glsl_type_v<T>;

	/**
	 * @brief check if T is base_type container
	 * @ingroup glslstruct
	 * @tparam T type which should be a base_type
	 */
	template<class T> concept glsl_base_type = is_glsl_base_type_v<T>;
		#endif
} // namespace glslstruct

	#endif
#endif