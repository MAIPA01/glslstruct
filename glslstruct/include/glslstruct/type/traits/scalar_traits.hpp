/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_SCALAR_TRAITS_HPP_
	#define _GLSL_STRUCT_SCALAR_TRAITS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/type/data/scalar_data.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
		#pragma region DEFAULT_TRAITS

	/**
	 * @brief scalar_traits for bool values
	 * @ingroup glslstruct
	 */
	template<>
	struct _GLSL_STRUCT_EXPORT scalar_traits<bool> {
		/// @brief returns ValueType of bool
		static ValueType get_value_type() noexcept;

		/// @brief returns scalar_data of bool value
		static scalar_data get_data(bool value);

		/// @brief returns bool value from scalar_data
		static bool get_value(const scalar_data& data);
	};

	/**
	 * @brief scalar_traits for int values
	 * @ingroup glslstruct
	 */
	template<>
	struct _GLSL_STRUCT_EXPORT scalar_traits<int> {
		/// @brief returns ValueType of int
		static ValueType get_value_type() noexcept;

		/// @brief returns scalar_data of int value
		static scalar_data get_data(int value);

		/// @brief returns int value from scalar_data
		static int get_value(const scalar_data& data);
	};

	/**
	 * @brief scalar_traits for unsigned int values
	 * @ingroup glslstruct
	 */
	template<>
	struct _GLSL_STRUCT_EXPORT scalar_traits<unsigned int> {
		/// @brief returns ValueType of unsigned int
		static ValueType get_value_type() noexcept;

		/// @brief returns scalar_data of unsigned int value
		static scalar_data get_data(unsigned int value);

		/// @brief returns unsigned int value from scalar_data
		static unsigned int get_value(const scalar_data& data);
	};

	/**
	 * @brief scalar_traits for float values
	 * @ingroup glslstruct
	 */
	template<>
	struct _GLSL_STRUCT_EXPORT scalar_traits<float> {
		/// @brief returns ValueType of float
		static ValueType get_value_type() noexcept;

		/// @brief returns scalar_data of float value
		static scalar_data get_data(float value);

		/// @brief returns float value from scalar_data
		static float get_value(const scalar_data& data);
	};

	/**
	 * @brief scalar_traits for double values
	 * @ingroup glslstruct
	 */
	template<>
	struct _GLSL_STRUCT_EXPORT scalar_traits<double> {
		/// @brief returns ValueType of double
		static ValueType get_value_type() noexcept;

		/// @brief returns scalar_data of double value
		static scalar_data get_data(double value);

		/// @brief returns double value from scalar_data
		static double get_value(const scalar_data& data);
	};

		#pragma endregion

		#pragma region FUNCTIONS
		/// @brief returns ValueType of T based on scalar_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_scalar T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
	static _GLSL_STRUCT_EXPORT inline ValueType get_scalar_value_type() noexcept {
		return scalar_traits<T>::get_value_type();
	}

		/// @brief returns scalar_data of T based on scalar_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_scalar T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
	static _GLSL_STRUCT_EXPORT inline scalar_data get_scalar_data(const T& value) {
		return scalar_traits<T>::get_data(value);
	}

		/// @brief returns value of type T from scalar_data based on scalar_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_scalar T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
	static _GLSL_STRUCT_EXPORT inline T get_scalar_value(const scalar_data& data) {
		return scalar_traits<T>::get_value(data);
	}

	/// @brief returns glsl scalar type string
	_GLSL_STRUCT_EXPORT std::string scalar_to_string(ValueType valueType);

		/// @brief returns glsl scalar type string from type T based on scalar_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_scalar T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
	static _GLSL_STRUCT_EXPORT inline std::string scalar_to_string() {
		return scalar_to_string(get_scalar_value_type<T>());
	}

		#pragma endregion
} // namespace glslstruct

	#endif
#endif