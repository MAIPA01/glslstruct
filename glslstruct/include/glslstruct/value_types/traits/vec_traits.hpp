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

#include <glslstruct/value_types/data/vec_data.hpp>

namespace glslstruct {
	template<class>
	struct vec_traits {
		static_assert(false, "You need to define conversion for this type");
	};

	template<class T, size_t L>
	struct vec_traits<glm::vec<L, T>> {
		using value_type = T;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t length = L;

		static _GLSL_STRUCT_CONSTEXPR20 vec_data get_data(const glm::vec<L, T>& value) {
			return vec_data(value);
		}
	};

	template<class T, size_t N>
	struct vec_traits<mstd::vec<N, T>> {
		using value_type = T;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t length = N;

		static _GLSL_STRUCT_CONSTEXPR20 vec_data get_data(const mstd::vec<N, T>& value) {
			return vec_data(value);
		}
	};

	template<class T>
	using glsl_vec_value_type = vec_traits<T>::value_type;

	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const size_t glsl_vec_length = vec_traits<T>::length;
}

#endif