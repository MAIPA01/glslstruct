/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_VARIABLE_HPP_
	#define _GLSL_STRUCT_VARIABLE_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/types.hpp>

namespace glslstruct {
	namespace utils {
		/**
		 * @brief container for standard variables
		 * @ingroup utils
		 */
		struct standard_variable {};

		/**
		 * @brief container for layout variables
		 * @ingroup utils
		 * @tparam Layout layout type
		 */
		template<class Layout>
		struct layout_variable {
			/// @brief layout value
			const Layout layout;

			/// @brief constructor with layout value
			explicit layout_variable(const Layout& layout) : layout(layout) {}
		};
	} // namespace utils

		/**
		 * @brief container for variables for easier initialization of layouts
		 * @ingroup glslstruct
		 * @tparam T variable type
		 * @tparam num number of elements in array (default is 0. if it is 0 then it is not array but a single value)
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_simple_or_layout T, size_t Num>
		#else
	template<class T, size_t Num, std::enable_if_t<utils::is_glsl_simple_or_layout_v<T>, bool> >
		#endif
	struct glsl_variable
		: public std::conditional_t<utils::is_glsl_layout_v<T>, utils::layout_variable<T>, utils::standard_variable> {
	public:
		/// @brief var type
		using var_type									  = T;
		/// @brief size of array
		static _GLSL_STRUCT_CONSTEXPR17 size_t array_size = Num;
		/// @brief value indicating if it is array or not
		static _GLSL_STRUCT_CONSTEXPR17 bool is_array	  = array_size > 0;
		/// @brief value indicating if it is layout variable or not
		static _GLSL_STRUCT_CONSTEXPR17 bool is_layout	  = utils::is_glsl_layout_v<T>;

		#pragma region VARIABLES
		/// @brief variable name
		const std::string varName;
		#pragma endregion

		#pragma region NORMAL_CONSTRUCTOR
		/// @brief standard constructor with variable name
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type																		  = var_type,
		  std::enable_if_t<utils::is_glsl_simple_v<Type> && std::is_same_v<Type, var_type>, bool> = true>
		#endif
		explicit _GLSL_STRUCT_CONSTEXPR20 glsl_variable(
		  const std::string_view name
		) noexcept _GLSL_STRUCT_REQUIRES(utils::is_glsl_simple_v<T>)
			: varName(name) {
		}

		#pragma endregion

		#pragma region LAYOUT_CONSTRUCTOR
		/// @brief standard constructor with variable name and layout
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type																		  = var_type,
		  std::enable_if_t<utils::is_glsl_layout_v<Type> && std::is_same_v<Type, var_type>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 glsl_variable(const std::string_view name,
		  const T& layout) noexcept _GLSL_STRUCT_REQUIRES(utils::is_glsl_layout_v<T>)
			: utils::layout_variable<T>(layout), varName(name) {
		}

		#pragma endregion
	};
} // namespace glslstruct

	#endif
#endif