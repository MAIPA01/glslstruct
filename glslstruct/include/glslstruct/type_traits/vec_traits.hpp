/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_VEC_TRAITS_HPP
	#define _GLSL_STRUCT_VEC_TRAITS_HPP

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/type_data/vec_data.hpp>
		#include <glslstruct/utils/is_vector_of.hpp>
		#include <glslstruct/utils/ValueType.hpp>

namespace glslstruct {
	template<class>
	struct vec_traits;

		#pragma region DEFAULT_TRAITS

	template<class T, size_t L, glm::qualifier Q>
	struct vec_traits<glm::vec<L, T, Q> > {
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_length() noexcept { return L; }

		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept { return get_value_type<T>(); }

		static vec_data get_data(const glm::vec<L, T, Q>& value) { return vec_data(value); }
	};

	template<class T, size_t N>
	struct vec_traits<mstd::vec<N, T> > {
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_length() noexcept { return N; }

		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept { return get_value_type<T>(); }

		static vec_data get_data(const mstd::vec<N, T>& value) { return vec_data(value); }
	};

		#pragma endregion

	namespace utils {
		#pragma region CHECKS
		#pragma region IS_VEC
		#if _GLSL_STRUCT_HAS_CXX20
		template<class T>
		concept glsl_vec = requires {
			{ vec_traits<T>::get_length() } -> std::same_as<size_t>;
			{ vec_traits<T>::get_value_type() } -> std::same_as<ValueType>;
			{ vec_traits<T>::get_data(std::declval<const T&>()) } -> std::same_as<vec_data>;
		};

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_vec_v = glsl_vec<T>;

		template<class T>
		struct is_glsl_vec : std::bool_constant<is_glsl_vec_v<T> > {};

		#else
		template<typename T, typename = void>
		struct is_glsl_vec : std::false_type {};

		template<class T>
		struct is_glsl_vec<T,
		  std::void_t<std::enable_if_t<std::is_same_v<size_t, decltype(vec_traits<T>::get_length())> >,
			std::enable_if_t<std::is_same_v<ValueType, decltype(vec_traits<T>::get_value_type())> >,
			std::enable_if_t<std::is_same_v<vec_data, decltype(vec_traits<T>::get_data(std::declval<const T&>()))> > > >
			: std::true_type {};

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_vec_v = is_glsl_vec<T>::value;
		#endif
		#pragma endregion

		#pragma region IS_VECS_VECTOR
		template<class V>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_vecs_vector_v = is_vector_of_v<is_glsl_vec, V>;

		#if _GLSL_STRUCT_HAS_CXX20
		template<class V> concept glsl_vecs_vector = is_glsl_vecs_vector_v<V>;
		#endif
		#pragma endregion
		#pragma endregion
	} // namespace utils

		#pragma region FUNCTIONS
		#if _GLSL_STRUCT_HAS_CXX20
	template<glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline _GLSL_STRUCT_CONSTEXPR17 ValueType get_vec_value_type() noexcept {
		return vec_traits<T>::get_value_type();
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline _GLSL_STRUCT_CONSTEXPR17 size_t get_vec_length() noexcept {
		return vec_traits<T>::get_length();
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline vec_data get_vec_data(const T& value) {
		return vec_traits<T>::get_data(value);
	}

	static inline std::string vec_to_string(ValueType valueType, size_t length) {
			switch (valueType) {
			[[unlikely]] default:
				return "UNKNOWN";
			case ValueType::Bool:	return fmt::format("bvec{}", length);
			case ValueType::Int:	return fmt::format("ivec{}", length);
			case ValueType::Uint:	return fmt::format("uvec{}", length);
			case ValueType::Float:	return fmt::format("vec{}", length);
			case ValueType::Double: return fmt::format("dvec{}", length);
			}
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline std::string vec_to_string() {
		return vec_to_string(get_vec_value_type<T>(), get_vec_length<T>());
	}

		#pragma endregion
} // namespace glslstruct

	#endif
#endif