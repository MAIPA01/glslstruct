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

namespace glslstruct {
		#pragma region DEFAULT_TRAITS

	template<class T, size_t L, glm::qualifier Q>
	struct vec_traits<glm::vec<L, T, Q> > {
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_length() noexcept { return L; }

		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept { return get_value_type<T>(); }

		static vec_data get_data(const glm::vec<L, T, Q>& value) { return vec_data(value); }

		static glm::vec<L, T, Q> get_value(const vec_data& data) {
			const std::vector<std::byte>& bytes = data.data();

				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					const size_t maxSize = std::min(bytes.size(), sizeof(int) * L);

					glm::vec<L, int, Q> value;
					std::copy_n(bytes.begin(), maxSize, reinterpret_cast<std::byte*>(glm::value_ptr(value)));
					return value > 0;
				}
				else {
					const size_t maxSize = std::min(bytes.size(), sizeof(T) * L);

					glm::vec<L, T, Q> value;
					std::copy_n(bytes.begin(), maxSize, reinterpret_cast<std::byte*>(glm::value_ptr(value)));
					return value;
				}
		}
	};

	template<class T, size_t N>
	struct vec_traits<mstd::vec<N, T> > {
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_length() noexcept { return N; }

		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept { return get_value_type<T>(); }

		static vec_data get_data(const mstd::vec<N, T>& value) { return vec_data(value); }

		static mstd::vec<N, T> get_value(const vec_data& data) {
			const std::vector<std::byte>& bytes = data.data();

				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					const size_t maxSize = std::min(bytes.size(), sizeof(int) * N);

					mstd::vec<N, int> iValue;
					std::copy_n(bytes.begin(), maxSize, reinterpret_cast<std::byte*>(static_cast<int*>(iValue)));

					mstd::vec<N, T> value;
						for (size_t i = 0; i < N; ++i) { value[i] = iValue[i] > 0; }
					return value;
				}
				else {
					const size_t maxSize = std::min(bytes.size(), sizeof(T) * N);

					mstd::vec<N, T> value;
					std::copy_n(bytes.begin(), maxSize, reinterpret_cast<std::byte*>(static_cast<T*>(value)));
					return value;
				}
		}
	};

		#pragma endregion

		#pragma region FUNCTIONS
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline _GLSL_STRUCT_CONSTEXPR17 ValueType get_vec_value_type() noexcept {
		return vec_traits<T>::get_value_type();
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline _GLSL_STRUCT_CONSTEXPR17 size_t get_vec_length() noexcept {
		return vec_traits<T>::get_length();
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline vec_data get_vec_data(const T& value) {
		return vec_traits<T>::get_data(value);
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline T get_vec_value(const vec_data& data) {
		return vec_traits<T>::get_value(data);
	}

	static inline std::string vec_to_string(const ValueType valueType, const size_t length) {
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
	template<utils::glsl_vec T>
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