/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_TYPE_VISITOR_CONCEPT_HPP_
	#define _GLSL_STRUCT_TYPE_VISITOR_CONCEPT_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1!"
);
	#else

		#include <glslstruct/types.hpp>

/**
 * @defgroup type_visitors Glsl type visitors
 * @ingroup glslstruct
 * @brief glsl type visitors
 * @details sample type visitor:
 * @code{.cpp}
 * class sample_type_visitor {
 *     void visit(const scalar_type&);
 *     void visit(const vec_type&);
 *     void visit(const mat_type&);
 *     void visit(const struct_type&);
 *     void visit(const array_type&);
 * };
 * @endcode
 */

namespace glslstruct {
		/**
		 * @var is_type_visitor_v
		 * @brief bool which is true if type T is type visitor
		 * @ingroup type_visitors
		 * @tparam T type to check if it is type visitor
		 */

		/**
		 * @struct is_type_visitor
		 * @brief struct with bool_constant which is true if type T is type visitor
		 * @ingroup type_visitors
		 * @tparam T type to check if it is type visitor
		 */

		#if _GLSL_STRUCT_HAS_CXX20
	/**
	 * @brief glsl type visitor concept
	 * @ingroup type_visitors
	 */
	template<class T>
	concept type_visitor = requires (T& visitor) {
		{ visitor.visit(std::declval<const scalar_type&>()) } -> std::same_as<void>;
		{ visitor.visit(std::declval<const vec_type&>()) } -> std::same_as<void>;
		{ visitor.visit(std::declval<const mat_type&>()) } -> std::same_as<void>;
		{ visitor.visit(std::declval<const struct_type&>()) } -> std::same_as<void>;
		{ visitor.visit(std::declval<const array_type&>()) } -> std::same_as<void>;
	};

	template<class T>
	static _GLSL_STRUCT_EXPORT _GLSL_STRUCT_CONSTEXPR17 bool is_type_visitor_v = type_visitor<T>;

	template<class T>
	struct _GLSL_STRUCT_EXPORT is_type_visitor : std::bool_constant<is_type_visitor_v<T> > {};
		#else
	template<class T, class = void>
	struct _GLSL_STRUCT_EXPORT is_type_visitor : std::false_type {};

	template<class T>
	struct _GLSL_STRUCT_EXPORT is_type_visitor<T,
	  std::void_t<std::enable_if_t<std::is_same_v<void, decltype(std::declval<T&>().visit(std::declval<const scalar_type&>()))> >,
		std::enable_if_t<std::is_same_v<void, decltype(std::declval<T&>().visit(std::declval<const vec_type&>()))> >,
		std::enable_if_t<std::is_same_v<void, decltype(std::declval<T&>().visit(std::declval<const mat_type&>()))> >,
		std::enable_if_t<std::is_same_v<void, decltype(std::declval<T&>().visit(std::declval<const struct_type&>()))> >,
		std::enable_if_t<std::is_same_v<void, decltype(std::declval<T&>().visit(std::declval<const array_type&>()))> > > >
		: std::true_type {};

	template<class T>
	static _GLSL_STRUCT_EXPORT _GLSL_STRUCT_CONSTEXPR17 bool is_type_visitor_v = is_type_visitor<T>::value;
		#endif
} // namespace glslstruct
	#endif
#endif