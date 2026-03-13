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

#if !_GLSL_STRUCT_HAS_TYPES && _GLSL_STRUCT_HAS_TYPE_CHECKS
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1 "\
		   "and when GLSL_STRUCT_ENABLE_TYPE_CHECKS is set to 1!");
#else

#include <glslstruct/value_types/scalar_type.hpp>
#include <glslstruct/value_types/vec_type.hpp>
#include <glslstruct/value_types/mat_type.hpp>
#include <glslstruct/value_types/array_type.hpp>
#include <glslstruct/value_types/struct_type.hpp>
#include <glslstruct/value_types/base_type.hpp>
#include <glslstruct/templates.hpp>

namespace glslstruct {
	template<class T, class GT>
	struct check_type_impl {
		static _GLSL_STRUCT_CONSTEXPR20 bool check(const std::shared_ptr<GT>&) noexcept {
			return false;
		}
	};

	template<class T>
	struct check_type_impl<T, scalar_type> {
		static _GLSL_STRUCT_CONSTEXPR20 bool check(const scalar_type_handle&) noexcept {
			return utils::is_glsl_scalar_v<T>;
		}
	};

	template<class T>
	struct check_type_impl<T, vec_type> {
		static _GLSL_STRUCT_CONSTEXPR20 bool check(const vec_type_handle& type_handle) noexcept {
			return utils::is_glsl_vec_v<T> &&
				get_value_type<typename T::value_type> == type_handle->get_type() &&
					T::length() == type_handle->get_length();
		}
	};

	template<class T, utils::glsl_type GT>
	inline _GLSL_STRUCT_CONSTEXPR20 bool check_type(const std::shared_ptr<GT>& type_handler) {
		if (std::is_same_v<GT, scalar_type> && utils::is_glsl_scalar_v<T>) {
			return true;
		}
		if (std::is_same_v<GT, vec_type> && utils::is_glsl_vec_v<T>) {
			return true;
		}
		if (std::is_same_v<GT, mat_type> && utils::is_glsl_mat_v<T>) {
			return true;
		}
		if (std::is_same_v<GT, array_type> && utils::is_glsl_array_v<T>) {
			return true;
		}
	}
}
#endif