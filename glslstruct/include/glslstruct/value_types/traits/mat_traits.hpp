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

#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

#include <glslstruct/types.hpp>
#include <glslstruct/value_types/data/mat_data.hpp>

namespace glslstruct {
	template<class T, size_t C, size_t R, glm::qualifier Q>
	struct mat_traits<glm::mat<C, R, T, Q>> {
		using value_type = T;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t columns = C;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t rows = R;

		static mat_data get_data(const glm::mat<C, R, T>& value, MajorType major) {
			return mat_data(value, major);
		}
	};

	template<class T, size_t C, size_t R>
	struct mat_traits<mstd::mat<C, R, T>> {
		using value_type = T;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t columns = C;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t rows = R;

		static mat_data get_data(const mstd::mat<C, R, T>& value, MajorType major) {
			return mat_data(value, major);
		}
	};

	template<class T>
	using glsl_mat_value_type = typename mat_traits<T>::value_type;

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const size_t glsl_mat_columns = mat_traits<T>::columns;

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const size_t glsl_mat_rows = mat_traits<T>::rows;

	static_assert(utils::is_glsl_scalar_v<int>);
	static_assert(utils::is_glsl_vec_v<glm::vec2>);
	static_assert(utils::is_glsl_mat_v<glm::mat2>);
}

#endif
