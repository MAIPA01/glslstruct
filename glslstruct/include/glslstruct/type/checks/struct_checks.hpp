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

		#include <glslstruct/type/checks/layouts_checks.hpp>

namespace glslstruct {
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_layout Layout>
		#else
	template<class Layout, std::enable_if_t<utils::is_glsl_layout_v<Layout>, bool> = true>
		#endif
	class base_struct;

	/**
	 * @brief std140 struct representation
	 * @ingroup glslstruct
	 */
	using std140_struct = base_struct<std140_layout>;
	/**
	 * @brief std430 struct representation
	 * @ingroup glslstruct
	 */
	using std430_struct = base_struct<std430_layout>;
	/**
	 * @brief scalar struct representation
	 * @ingroup glslstruct
	 */
	using scalar_struct = base_struct<scalar_layout>;

	namespace utils {
		#pragma region IS_STRUCT

		/**
		 * @brief helper struct to check if type is glsl struct
		 * @ingroup utils
		 * @tparam S type which is glsl struct type
		 */
		template<class S>
		struct is_glsl_struct : std::false_type {};

		template<class L>
		struct is_glsl_struct<base_struct<L> > : std::true_type {};

		/**
		 * @brief check if type is glsl struct
		 * @ingroup utils
		 * @tparam S type which is glsl struct type
		 */
		template<class S>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_struct_v = is_glsl_struct<S>::value;

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief check if type is glsl struct
		 * @ingroup utils
		 * @tparam S type which is glsl struct type
		 */
		template<class S> concept glsl_struct = is_glsl_struct_v<S>;
		#endif
		#pragma endregion

		#pragma region IS_STRUCTS_VECTOR
		/**
		 * @brief check if type is array of glsl structs
		 * @ingroup utils
		 */
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_structs_array_v = is_array_of_v<is_glsl_struct, T>;

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief check if type is array of glsl structs
		 * @ingroup utils
		 */
		template<class T> concept glsl_structs_array = is_glsl_structs_array_v<T>;
		#endif
		#pragma endregion

		#pragma region IS_LAYOUT_STRUCT

		/**
		 * @brief helper struct to check if type is glsl struct with required layout
		 * @ingroup utils
		 * @tparam S type which is glsl struct with required layout
		 * @tparam Layout required glsl layout type
		 */
		template<class S, class Layout>
		struct is_glsl_layout_struct : std::false_type {};

		template<class L, class Layout>
		struct is_glsl_layout_struct<base_struct<Layout>, L> : std::bool_constant<std::is_same_v<Layout, L> > {};

		/**
		 * @brief check if type is glsl struct with required layout
		 * @ingroup utils
		 * @tparam S type which is glsl struct with required
		 * @tparam Layout required glsl layout type
		 */
		template<class S, class Layout>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_layout_struct_v = is_glsl_layout_struct<S, Layout>::value;

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief check if type is glsl struct with required layout
		 * @ingroup utils
		 * @tparam S type which is glsl struct with required
		 * @tparam Layout required glsl layout type
		 */
		template<class S, class Layout> concept glsl_layout_struct = is_glsl_layout_struct_v<S, Layout>;
		#endif
		#pragma endregion

		#pragma region IS_LAYOUT_STRUCTS_VECTOR
		/**
		 * @brief check if type is array of glsl structs with required layout
		 * @ingroup utils
		 * @tparam VS type which is array of glsl structs with required layout
		 * @tparam Layout required glsl layout type
		 */
		template<class VS, class Layout>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_layout_structs_array_v = is_array_of_v<is_glsl_layout_struct, VS, Layout>;

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief check if type is array of glsl structs with required layout
		 * @ingroup utils
		 * @tparam VS type which is array of glsl structs with required layout
		 * @tparam Layout required glsl layout type
		 */
		template<class VS, class Layout> concept glsl_layout_structs_array = is_glsl_layout_structs_array_v<VS, Layout>;
		#endif
		#pragma endregion

		#pragma region IS_SIMPLE_OR_LAYOUT_STRUCT
		/**
		 * @brief check if type is glsl struct with required layout or simple type
		 * @ingroup utils
		 * @tparam T type which is glsl struct with required layout or simple type
		 * @tparam Layout required glsl layout type
		 */
		template<class T, class Layout>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_simple_or_layout_struct_v =
		  is_glsl_simple_v<T> || is_glsl_layout_struct_v<T, Layout>;

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief check if type is glsl struct with required layout or simple type
		 * @ingroup utils
		 * @tparam T type which is glsl struct with required layout or simple type
		 * @tparam Layout required glsl layout type
		 */
		template<class T, class Layout> concept glsl_simple_or_layout_struct = is_glsl_simple_or_layout_struct_v<T, Layout>;
		#endif
		#pragma endregion

		#pragma region IS_SIMPLE_OR_STRUCT
		/**
		 * @brief check if type is glsl struct or simple type
		 * @ingroup utils
		 * @tparam T type which is glsl struct or simple type
		 */
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_simple_or_struct_v = is_glsl_simple_v<T> || is_glsl_struct_v<T>;

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief check if type is glsl struct or simple type
		 * @ingroup utils
		 * @tparam T type which is glsl struct or simple type
		 */
		template<class T> concept glsl_simple_or_struct = is_glsl_simple_or_struct_v<T>;
		#endif
		#pragma endregion
	} // namespace utils
} // namespace glslstruct
	#endif
#endif