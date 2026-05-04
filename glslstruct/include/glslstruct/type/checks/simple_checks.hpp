/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_VALUE_CHECKS_HPP_
	#define _GLSL_STRUCT_VALUE_CHECKS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/type/traits/mat_traits.hpp>
		#include <glslstruct/type/traits/scalar_traits.hpp>
		#include <glslstruct/type/traits/vec_traits.hpp>

namespace glslstruct {
		#pragma region IS_SIMPLE
	/**
	 * @brief check if type is glsl simple (is of scalar or vec or mat type)
	 * @ingroup glslstruct
	 * @tparam T type which is glsl simple type
	 */
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_simple_v = is_glsl_scalar_v<T> || is_glsl_vec_v<T> || is_glsl_mat_v<T>;

	/**
	 * @brief check if type is glsl simple (is of scalar or vec or mat type)
	 * @ingroup glslstruct
	 * @tparam T type which is glsl simple type
	 */
	template<class T>
	struct is_glsl_simple : std::bool_constant<is_glsl_simple_v<T>> {};

		#if _GLSL_STRUCT_HAS_CXX20
	/**
	 * @brief check if type is glsl simple (is of scalar or vec or mat type)
	 * @ingroup glslstruct
	 * @tparam T type which is glsl simple type
	 */
	template<class T> concept glsl_simple = is_glsl_simple_v<T>;
		#endif

		#pragma endregion

	namespace utils {
		#pragma region IS_SIMPLE_ARRAY
		/**
		 * @brief check if type is array of glsl simple (scalar or vec or mat type)
		 * @ingroup utils
		 */
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_simples_array_v =
		  is_glsl_scalars_array_v<T> || is_glsl_vecs_array_v<T> || is_glsl_mats_array_v<T>;

		/**
		 * @brief check if type is static size array of glsl simple (scalar or vec or mat type)
		 * @ingroup utils
		 */
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_simples_static_size_array_v =
		  is_glsl_scalars_static_size_array_v<T> || is_glsl_vecs_static_size_array_v<T> || is_glsl_mats_static_size_array_v<T>;

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @brief check if type is array of glsl simple (scalar or vec or mat type)
		 * @ingroup utils
		 */
		template<class T> concept glsl_simples_array			 = is_glsl_simples_array_v<T>;

		/**
		 * @brief check if type is static size array of glsl simple (scalar or vec or mat type)
		 * @ingroup utils
		 */
		template<class T> concept glsl_simples_static_size_array = is_glsl_simples_static_size_array_v<T>;
		#endif
		#pragma endregion
	} // namespace utils
} // namespace glslstruct

	#endif
#endif