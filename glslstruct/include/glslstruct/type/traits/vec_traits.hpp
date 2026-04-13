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

		#include <glslstruct/type/data/vec_data.hpp>

namespace glslstruct {
		#pragma region DEFAULT_TRAITS

	/**
	 * @brief vec_traits for glm::vec values
	 * @ingroup glslstruct
	 */
	template<class T, glm::length_t L, glm::qualifier Q>
	struct vec_traits<glm::vec<L, T, Q> > {
		/// @brief returns length of glm::vec
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_length() noexcept { return L; }

		/// @brief returns ValueType of glm::vec
		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept { return glslstruct::get_value_type<T>(); }

		/// @brief returns vec_data of glm::vec
		static vec_data get_data(const glm::vec<L, T, Q>& value) { return vec_data(value); }

		/// @brief returns glm::vec value from vec_data
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

	/**
	 * @brief vec_traits for mstd::vec values
	 * @ingroup glslstruct
	 */
	template<class T, size_t N>
	struct vec_traits<mstd::vec<N, T> > {
		/// @brief returns length of mstd::vec
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_length() noexcept { return N; }

		/// @brief returns ValueType of mstd::vec
		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept { return glslstruct::get_value_type<T>(); }

		/// @brief returns vec_data of mstd::vec
		static vec_data get_data(const mstd::vec<N, T>& value) { return vec_data(value); }

		/// @brief returns mstd::vec value from vec_data
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
		/// @brief returns length of T based on vec_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline size_t get_vec_length() noexcept {
		return vec_traits<T>::get_length();
	}

		/// @brief returns ValueType of T based on vec_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline ValueType get_vec_value_type() noexcept {
		return vec_traits<T>::get_value_type();
	}

		/// @brief returns vec_data of T based on vec_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline vec_data get_vec_data(const T& value) {
		return vec_traits<T>::get_data(value);
	}

		/// @brief returns vec value of type T from vec_data based on vec_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_vec T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
	static inline T get_vec_value(const vec_data& data) {
		return vec_traits<T>::get_value(data);
	}

	/// @brief returns glsl vec type string
	std::string vec_to_string(ValueType valueType, size_t length);

		/// @brief returns glsl vec type string from type T based on vec_traits
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