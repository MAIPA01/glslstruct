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
	template<class>
	struct mat_traits {
		static_assert(false, "You need to define conversion for this type");
	};

	template<class T, size_t C, size_t R>
	struct mat_traits<glm::mat<C, R, T>> {
		using value_type = T;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t columns = C;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t rows = R;

		static _GLSL_STRUCT_CONSTEXPR20 mat_data get_data(const glm::mat<C, R, T>& value) {
			return mat_data(value);
		}
	};

	template<class T, size_t C, size_t R>
	struct vec_traits<mstd::mat<C, R, T>> {
		using value_type = T;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t columns = C;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t rows = R;

		static _GLSL_STRUCT_CONSTEXPR20 mat_data get_data(const mstd::mat<C, R, T>& value) {
			return mat_data(value);
		}
	};

	template<class T>
	using glsl_mat_value_type = mat_traits<T>::value_type;

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const size_t glsl_mat_columns = mat_traits<T>::columns;

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const size_t glsl_mat_rows = mat_traits<T>::rows;
}

#endif
