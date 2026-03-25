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

		#include <glslstruct/type_data/scalar_data.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
		#pragma region DEFAULT_TRAITS

	template<>
	struct scalar_traits<bool> {
		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept;

		static scalar_data get_data(bool value);

		static bool get_value(const scalar_data& data);
	};

	template<>
	struct scalar_traits<int> {
		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept;

		static scalar_data get_data(int value);

		static int get_value(const scalar_data& data);
	};

	template<>
	struct scalar_traits<unsigned int> {
		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept;

		static scalar_data get_data(unsigned int value);

		static unsigned int get_value(const scalar_data& data);
	};

	template<>
	struct scalar_traits<float> {
		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept;

		static scalar_data get_data(float value);

		static float get_value(const scalar_data& data);
	};

	template<>
	struct scalar_traits<double> {
		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept;

		static scalar_data get_data(double value);

		static double get_value(const scalar_data& data);
	};

		#pragma endregion

		#pragma region FUNCTIONS
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_scalar T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
	static inline _GLSL_STRUCT_CONSTEXPR17 ValueType get_scalar_value_type() noexcept {
		return scalar_traits<T>::get_value_type();
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_scalar T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
	static inline scalar_data get_scalar_data(const T& value) {
		return scalar_traits<T>::get_data(value);
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_scalar T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
	static inline T get_scalar_value(const scalar_data& data) {
		return scalar_traits<T>::get_value(data);
	}

	static inline std::string scalar_to_string(const ValueType valueType) {
			switch (valueType) {
			[[unlikely]] default:
				return "UNKNOWN";
			case ValueType::Bool:	return "bool";
			case ValueType::Int:	return "int";
			case ValueType::Uint:	return "uint";
			case ValueType::Float:	return "float";
			case ValueType::Double: return "double";
			}
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_scalar T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
	static inline std::string scalar_to_string() {
		return scalar_to_string(get_scalar_value_type<T>());
	}

		#pragma endregion
} // namespace glslstruct

	#endif
#endif