/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

#include <glslstruct/types.hpp>
#include <glslstruct/value_types/traits/scalar_traits.hpp>
#include <glslstruct/value_types/traits/vec_traits.hpp>
#include <glslstruct/value_types/traits/mat_traits.hpp>

namespace glslstruct::utils {
#pragma region IS_VECTOR_OF
	template<template<class, class...> class Test, class V, class... Args>
	struct is_vector_of : std::false_type {};

	template<template<class, class...> class Test, class T, class... Args>
	struct is_vector_of<Test, std::vector<T>, Args...> : Test<T, Args...> {};

	template<template<class, class...> class Test, class V, class... Args>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_vector_of_v = is_vector_of<Test, V, Args...>::value;
#pragma endregion

#pragma region IS_SCALAR
	template<class T, class = void>
	struct is_glsl_scalar : std::false_type {};

	template<class T>
	struct is_glsl_scalar<T, std::void_t<decltype(scalar_traits<T>::get_data(std::declval<T>()))>> : std::true_type {};

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR20 const bool is_glsl_scalar_v = is_glsl_scalar<T>::value;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T> concept glsl_scalar = is_glsl_scalar_v<T>;
#endif
#pragma endregion

#pragma region IS_SCALARS_VECTOR
	template<class V>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_scalars_vector_v = is_vector_of_v<is_glsl_scalar, V>;

#if _GLSL_STRUCT_HAS_CXX20
	template<class V> concept glsl_scalars_vector = is_glsl_scalars_vector_v<V>;
#endif
#pragma endregion

#pragma region IS_VEC
	template<typename T, typename = void>
	struct is_glsl_vec : std::false_type {};

	template<typename T>
	struct is_glsl_vec<T, std::void_t<
		typename vec_traits<T>::value_type,
		decltype(vec_traits<T>::length),
		decltype(vec_traits<T>::get_data(std::declval<T>()))
	>> : std::bool_constant<(is_glsl_scalar_v<glsl_vec_value_type<T>> && mstd::is_in_range_v<glsl_vec_length<T>, 2, 4>)> {};

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_vec_v = is_glsl_vec<T>::value;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T> concept glsl_vec = is_glsl_vec_v<T>;
#endif
#pragma endregion

#pragma region IS_VECS_VECTOR
	template<class V>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_vecs_vector_v = is_vector_of_v<is_glsl_vec, V>;

#if _GLSL_STRUCT_HAS_CXX20
	template<class V> concept glsl_vecs_vector = is_glsl_vecs_vector_v<V>;
#endif
#pragma endregion

#pragma region IS_MAT
	template<typename T, typename = void>
	struct is_glsl_mat : std::false_type {};

	template<typename T>
	struct is_glsl_mat<T, std::void_t<
		typename mat_traits<T>::value_type,
		decltype(mat_traits<T>::columns),
		decltype(mat_traits<T>::rows),
		decltype(mat_traits<T>::get_data(std::declval<T>()))
	>> : std::bool_constant<(is_glsl_scalar_v<T> && mstd::is_in_range_v<glsl_mat_columns<T>, 2, 4> && mstd::is_in_range_v<glsl_mat_rows<T>, 2, 4>)> {};

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_mat_v = is_glsl_mat<T>::value;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T> concept glsl_mat = is_glsl_mat_v<T>;
#endif
#pragma endregion

#pragma region IS_MATS_VECTOR
	template<class V>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_mats_vector_v = is_vector_of_v<is_glsl_mat, V>;

#if _GLSL_STRUCT_HAS_CXX20
	template<class V> concept glsl_mats_vector = is_glsl_mats_vector_v<V>;
#endif
#pragma endregion

#pragma region IS_SIMPLE
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_simple_v = is_glsl_scalar_v<T> || is_glsl_vec_v<T> || is_glsl_mat_v<T>;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T> concept glsl_simple = is_glsl_simple_v<T>;
#endif
#pragma endregion
}
#endif