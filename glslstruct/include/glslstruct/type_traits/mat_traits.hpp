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

		#include <glslstruct/type_data/mat_data.hpp>

namespace glslstruct {
	template<class>
	struct mat_traits;

		#pragma region DEFAULT_TRAITS

	template<class T, glm::length_t C, glm::length_t R, glm::qualifier Q>
	struct mat_traits<glm::mat<C, R, T, Q> > {
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_columns() noexcept { return C; }

		static _GLSL_STRUCT_CONSTEXPR17 size_t get_rows() noexcept { return R; }

		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept { return glslstruct::get_value_type<T>(); }

		static mat_data get_data(const glm::mat<C, R, T, Q>& value) { return mat_data(value); }

		static glm::mat<C, R, T, Q> get_value(const mat_data& data) {
			const std::vector<vec_data>& vecs = data.data();

			glm::mat<C, R, T, Q> value;
				for (size_t i = 0; i < vecs.size() && i < C; ++i) {
					value[i] = vecs[i].get<typename glm::mat<C, R, T, Q>::col_type>();
				}
			return value;
		}
	};

	template<class T, size_t C, size_t R>
	struct mat_traits<mstd::mat<C, R, T> > {
		static _GLSL_STRUCT_CONSTEXPR17 size_t get_columns() noexcept { return C; }

		static _GLSL_STRUCT_CONSTEXPR17 size_t get_rows() noexcept { return R; }

		static _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type() noexcept { return glslstruct::get_value_type<T>(); }

		static mat_data get_data(const mstd::mat<C, R, T>& value) { return mat_data(value); }

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
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static inline _GLSL_STRUCT_CONSTEXPR17 ValueType get_mat_value_type() noexcept {
		return mat_traits<T>::get_value_type();
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static inline _GLSL_STRUCT_CONSTEXPR17 size_t get_mat_columns() noexcept {
		return mat_traits<T>::get_columns();
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static inline _GLSL_STRUCT_CONSTEXPR17 size_t get_mat_rows() noexcept {
		return mat_traits<T>::get_rows();
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static inline mat_data get_mat_data(const T& value) {
		return mat_traits<T>::get_data(value);
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static inline T get_mat_value(const mat_data& data) {
		return mat_traits<T>::get_value(data);
	}

	static inline std::string mat_to_string(const ValueType valueType, const size_t columns, const size_t rows) {
		std::string sizeStr = columns == rows ? std::to_string(columns) : fmt::format("{}x{}", columns, rows);

			switch (valueType) {
			[[unlikely]] default:
				return "UNKNOWN";
			case ValueType::Bool:	return fmt::format("bmat{}", sizeStr);
			case ValueType::Int:	return fmt::format("imat{}", sizeStr);
			case ValueType::Uint:	return fmt::format("umat{}", sizeStr);
			case ValueType::Float:	return fmt::format("mat{}", sizeStr);
			case ValueType::Double: return fmt::format("dmat{}", sizeStr);
			}
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_mat T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
	static inline std::string mat_to_string() {
		return mat_to_string(get_mat_value_type<T>(), get_mat_columns<T>(), get_mat_rows<T>());
	}

		#pragma endregion
} // namespace glslstruct

	#endif
#endif