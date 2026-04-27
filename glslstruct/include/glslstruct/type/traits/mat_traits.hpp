/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_MAT_TRAITS_HPP_
	#define _GLSL_STRUCT_MAT_TRAITS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/type/data/mat_data.hpp>

namespace glslstruct {
		#pragma region DEFAULT_TRAITS

	/**
	 * @brief mat_traits for glm::mat values
	 * @ingroup glslstruct
	 */
	template<class T, glm::length_t C, glm::length_t R, glm::qualifier Q>
	struct mat_traits<glm::mat<C, R, T, Q> > {
		/// @brief returns number of columns of glm::mat
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_columns() noexcept { return C; }

		/// @brief returns number of rows of glm::mat
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_rows() noexcept { return R; }

		/// @brief returns ValueType of glm::mat
		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept { return glslstruct::get_value_type<T>(); }

		/// @brief returns mat_data of glm::mat
		static mat_data get_data(const glm::mat<C, R, T, Q>& value) { return mat_data(value); }

		/// @brief returns glm::mat from mat_data
		static glm::mat<C, R, T, Q> get_value(const mat_data& data) {
			const std::vector<vec_data>& vecs = data.data();

			glm::mat<C, R, T, Q> value;
				for (size_t i = 0; i < vecs.size() && i < C; ++i) {
					value[i] = vecs[i].get<typename glm::mat<C, R, T, Q>::col_type>();
				}
			return value;
		}
	};

	/**
	 * @brief mat_traits for mstd::mat values
	 * @ingroup glslstruct
	 */
	template<class T, size_t C, size_t R>
	struct mat_traits<mstd::mat<C, R, T> > {
		/// @brief returns number of columns of mstd::mat
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_columns() noexcept { return C; }

		/// @brief returns number of rows of mstd::mat
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_rows() noexcept { return R; }

		/// @brief returns ValueType of mstd::mat
		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept { return glslstruct::get_value_type<T>(); }

		/// @brief returns mat_data of mstd::mat
		static mat_data get_data(const mstd::mat<C, R, T>& value) { return mat_data(value); }

		/// @brief returns mstd::mat from mat_data
		static mstd::mat<C, R, T> get_value(const mat_data& data) {
			const std::vector<vec_data>& vecs = data.data();

			mstd::mat<C, R, T> value;
				for (size_t i = 0; i < vecs.size() && i < C; ++i) {
					value[i] = vecs[i].get<typename mstd::mat<C, R, T>::column_type>();
				}
			return value;
		}
	};

		#pragma endregion

		#pragma region FUNCTIONS
		/// @brief returns number of columns of T based on mat_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static _GLSL_STRUCT_CONSTEXPR17 size_t get_mat_columns() noexcept {
		return mat_traits<T>::get_columns();
	}

		/// @brief returns number of rows of T based on mat_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static _GLSL_STRUCT_CONSTEXPR17 size_t get_mat_rows() noexcept {
		return mat_traits<T>::get_rows();
	}

		/// @brief returns ValueType of T based on mat_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static _GLSL_STRUCT_CONSTEXPR17 ValueType get_mat_value_type() noexcept {
		return mat_traits<T>::get_value_type();
	}

		/// @brief returns mat_data of T based on mat_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static mat_data get_mat_data(const T& value) {
		return mat_traits<T>::get_data(value);
	}

		/// @brief returns mat value of type T from mat_data based on mat_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static T get_mat_value(const mat_data& data) {
		return mat_traits<T>::get_value(data);
	}

	/// @brief returns glsl mat type string
	_GLSL_STRUCT_EXPORT std::string mat_to_string(ValueType valueType, size_t columns, size_t rows);

		/// @brief returns glsl mat type string from type T based on mat_traits
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static std::string mat_to_string() {
		return mat_to_string(get_mat_value_type<T>(), get_mat_columns<T>(), get_mat_rows<T>());
	}

		#pragma endregion
} // namespace glslstruct

	#endif
#endif