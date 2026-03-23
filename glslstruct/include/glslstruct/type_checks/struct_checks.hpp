/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_STRUCT_CHECKS_HPP_
	#define _GLSL_STRUCT_STRUCT_CHECKS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/type_checks/layouts_checks.hpp>

namespace glslstruct {
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_layout Layout>
		#else
	template<class Layout, std::enable_if_t<utils::is_glsl_layout_v<Layout>, bool> = true>
		#endif
	class base_struct;

	namespace utils {
		#pragma region IS_STRUCT

		template<class S>
		struct is_glsl_struct : std::false_type {};

		template<class L>
		struct is_glsl_struct<base_struct<L> > : std::true_type {};

		template<class S>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_struct_v = is_glsl_struct<S>::value;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class S> concept glsl_struct = is_glsl_struct_v<S>;
		#endif
		#pragma endregion

		#pragma region IS_STRUCTS_VECTOR
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_structs_vector_v = is_vector_of_v<is_glsl_struct, T>;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class T> concept glsl_structs_vector = is_glsl_structs_vector_v<T>;
		#endif
		#pragma endregion

		#pragma region IS_LAYOUT_STRUCT

		template<class S, class L>
		struct is_glsl_layout_struct : std::false_type {};

		template<class L, class Layout>
		struct is_glsl_layout_struct<base_struct<Layout>, L> : std::bool_constant<std::is_same_v<Layout, L> > {};

		template<class S, class Layout>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_layout_struct_v = is_glsl_layout_struct<S, Layout>::value;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class S, class Layout> concept glsl_layout_struct = is_glsl_layout_struct_v<S, Layout>;
		#endif
		#pragma endregion

		#pragma region IS_LAYOUT_STRUCTS_VECTOR
		template<class VS, class Layout>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_layout_structs_vector_v =
		  is_vector_of_v<is_glsl_layout_struct, VS, Layout>;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class VS, class Layout> concept glsl_layout_structs_vector = is_glsl_layout_structs_vector_v<VS, Layout>;
		#endif
		#pragma endregion

		#pragma region IS_SIMPLE_OR_LAYOUT_STRUCT
		template<class T, class Layout>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_simple_or_layout_struct_v =
		  is_glsl_simple_v<T> || is_glsl_layout_struct_v<T, Layout>;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class T, class Layout> concept glsl_simple_or_layout_struct = is_glsl_simple_or_layout_struct_v<T, Layout>;
		#endif
		#pragma endregion

		#pragma region IS_SIMPLE_OR_STRUCT
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_simple_or_struct_v = is_glsl_simple_v<T> || is_glsl_struct_v<T>;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class T> concept glsl_simple_or_struct = is_glsl_simple_or_struct_v<T>;
		#endif
		#pragma endregion
	} // namespace utils
} // namespace glslstruct
	#endif
#endif