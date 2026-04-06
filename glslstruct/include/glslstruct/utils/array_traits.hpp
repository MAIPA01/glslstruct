/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_IS_VECTOR_OF_HPP_
	#define _GLSL_STRUCT_IS_VECTOR_OF_HPP_

	#include "array_traits.hpp"


	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/libs.hpp>

namespace glslstruct::utils {
	/**
	 * @brief Array Traits
	 * @ingroup utils
	 * @tparam T array type
	 */
	template<class T>
	struct array_traits {};

	/**
	 * @brief Array Traits overload for std::vector
	 * @ingroup utils
	 * @tparam T value type
	 */
	template<class T>
	struct array_traits<std::vector<T> > {
		/// @brief value type
		using value_type = T;

		/// @brief pointer to vector data
		static _GLSL_STRUCT_CONSTEXPR17 const T* data(const std::vector<T>& vec) { return vec.data(); }

		/// @brief size of vector
		static _GLSL_STRUCT_CONSTEXPR17 size_t size(const std::vector<T>& vec) { return vec.size(); }
	};

	/**
	 * @brief Array Traits overload for std::array
	 * @ingroup utils
	 * @tparam T value type
	 * @tparam N array size
	 */
	template<class T, size_t N>
	struct array_traits<std::array<T, N> > {
		/// @brief value type
		using value_type = T;

		/// @brief pointer to array data
		static _GLSL_STRUCT_CONSTEXPR17 const T* data(const std::array<T, N>& array) { return array.data(); }

		/// @brief array size
		static _GLSL_STRUCT_CONSTEXPR17 size_t size(const std::array<T, N>& array) { return array.size(); }
	};

	/**
	 * @brief Array Traits overload for c-style array
	 * @ingroup utils
	 * @tparam T value type
	 * @tparam N array size
	 */
	template<class T, size_t N>
	struct array_traits<T[N]> {
		/// @brief value type
		using value_type = T;

		/// @brief pointer to c-style array data
		static _GLSL_STRUCT_CONSTEXPR17 const T* data(const T (&array)[N]) { return array; }

		/// @brief c-style array size
		static _GLSL_STRUCT_CONSTEXPR17 size_t size(const T (&)[N]) { return N; }
	};

	/**
	 * @brief Array Traits overload for pointer to c-style array
	 * @ingroup utils
	 * @tparam T value type
	 * @tparam N array size
	 */
	template<class T, size_t N>
	struct array_traits<T(*)[N]> {
		/// @brief value type
		using value_type = T;

		/// @brief pointer to c-style array data
		static _GLSL_STRUCT_CONSTEXPR17 const T* data(const T (*array)[N]) { return *array; }

		/// @brief c-style array size
		static _GLSL_STRUCT_CONSTEXPR17 size_t size(const T (*)[N]) { return N; }
	};

		#if _GLSL_STRUCT_HAS_CXX20
	/**
	 * @brief Array Traits overload for std::span
	 * @ingroup utils
	 * @tparam T value type
	 */
	template<class T>
	struct array_traits<std::span<T> > {
		/// @brief value type
		using value_type = T;

		/// @brief pointer to span data
		static _GLSL_STRUCT_CONSTEXPR17 const T* data(const std::span<const T> span) { return span.data(); }

		/// @brief span size
		static _GLSL_STRUCT_CONSTEXPR17 size_t size(const std::span<const T> span) { return span.size(); }
	};
		#endif

	/**
	 * @var bool is_array_v
	 * @brief checks if T has array traits defined
	 * @ingroup utils
	 */

		#if _GLSL_STRUCT_HAS_CXX20
	/**
	 * @brief Array concept
	 * @ingroup utils
	 */
	template<class T>
	concept array = requires (const T& value) {
		typename array_traits<T>::value_type;
		{ array_traits<T>::data(value) } -> std::same_as<const typename array_traits<T>::value_type*>;
		{ array_traits<T>::size(value) } -> std::same_as<size_t>;
	};

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_array_v = array<T>;
		#else
	/**
	 * @brief Is Array Test Struct
	 * @ingroup utils
	 */
	template<class T, class = void>
	struct is_array : std::false_type {};

	template<class T>
	struct is_array<T, std::void_t<typename array_traits<T>::value_type,
						 std::enable_if_t<std::is_same_v<decltype(array_traits<T>::data(std::declval<const T&>())),
						   const typename array_traits<T>::value_type*> >,
						 std::enable_if_t<std::is_same_v<decltype(array_traits<T>::size(std::declval<const T&>())), size_t> > > >
		: std::true_type {};

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_array_v = is_array<T>::value;
		#endif

	/**
	 * @brief get value_type of array
	 * @ingroup utils
	 */
	#if _GLSL_STRUCT_HAS_CXX20
	template<array T>
	#else
	template<class T, std::enable_if_t<is_array_v<T>, bool> = true>
	#endif
	using array_value_type_t = typename array_traits<T>::value_type;

	/**
	 * @brief get pointer to data of array
	 * @ingroup utils
	 */
	#if _GLSL_STRUCT_HAS_CXX20
	template<array T>
	#else
	template<class T, std::enable_if_t<is_array_v<T>, bool> = true>
	#endif
	[[nodiscard]] static inline _GLSL_STRUCT_CONSTEXPR17 const array_value_type_t<T>* get_array_data(const T& value) {
		return array_traits<T>::data(value);
	}

	/**
	 * @brief get size of array
	 * @ingroup utils
	 */
	#if _GLSL_STRUCT_HAS_CXX20
	template<array T>
	#else
	template<class T, std::enable_if_t<is_array_v<T>, bool> = true>
	#endif
	[[nodiscard]] static inline _GLSL_STRUCT_CONSTEXPR17 size_t get_array_size(const T& value) {
		return array_traits<T>::size(value);
	}

	/**
	 * @brief helper struct for is_array_of_v check
	 * @tparam Test test template struct with static value bool
	 * @tparam T type to checks
	 * @tparam isArray check if T is array type
	 * @tparam Args aditional Test arguments
	 */
	template<template<class, class...> class Test, class T, bool isArray = false, class... Args>
	struct is_array_of : std::false_type {};

	template<template<class, class...> class Test, class T, class... Args>
	struct is_array_of<Test, T, true, Args...> : Test<array_value_type_t<T>, Args...> {};

	/**
	 * @brief checks if value type is array of type and if type passes given test template struct
	 * @ingroup utils
	 */
	template<template<class, class...> class Test, class T, class... Args>
	static _GLSL_STRUCT_CONSTEXPR17 bool is_array_of_v = is_array_of<Test, T, is_array_v<T>, Args...>::value;
} // namespace glslstruct::utils

	#endif
#endif
