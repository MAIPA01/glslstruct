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

#include <glslstruct/libs.hpp>

namespace glslstruct {
	#pragma region ENUMS
	enum class ValueType : uint8_t;
	enum class MajorType : uint8_t;
	#pragma endregion

	#if !GLSL_STRUCT_DISABLE_TYPES
	#pragma region TYPES
	class base_type;
	template<class Derived>
	class value_type;
	class scalar_type;
	class vec_type;
	class mat_type;
	class array_type;
	class struct_type;
	#pragma endregion

	#pragma region TYPES_HANDLES
	using base_type_handle = std::shared_ptr<base_type>;
	template<class Derived>
	using value_type_handle = std::shared_ptr<value_type<Derived>>;
	using scalar_type_handle = std::shared_ptr<scalar_type>;
	using vec_type_handle = std::shared_ptr<vec_type>;
	using mat_type_handle = std::shared_ptr<mat_type>;
	using array_type_handle = std::shared_ptr<array_type>;
	using struct_type_handle = std::shared_ptr<struct_type>;
	#pragma endregion
	#endif

	#pragma region TYPE_DATAS
	class scalar_data;
	class vec_data;
	class mat_data;
	#pragma endregion

	#pragma region TYPE_TRAITS
	template<class>
	struct scalar_traits;

	template<class>
	struct vec_traits;

	template<class>
	struct mat_traits;
	#pragma endregion
}

#include <glslstruct/value_types/checks/type_checks.hpp>
#include <glslstruct/value_types/checks/value_checks.hpp>

namespace glslstruct {
	class base_layout;
	class std140_layout;
	class std430_layout;
}

#include <glslstruct/value_types/checks/layouts_checks.hpp>

namespace glslstruct {
	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(_Layout, utils::glsl_layout, utils::is_glsl_layout_v<_Layout>, = true)
	class base_struct;
}

#include <glslstruct/value_types/checks/struct_checks.hpp>

namespace glslstruct {
	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_simple_or_layout, utils::is_glsl_simple_or_layout_v<T>, = true, size_t num = 0)
	struct glsl_variable;
	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_simple_or_struct, utils::is_glsl_simple_or_struct_v<T>, = true, size_t num = 0)
	struct glsl_value;
}

#pragma region SOME_ADDITIONAL_GLM_TYPES
namespace glm {
	using bmat2x2 = mat<2, 2, bool, packed_highp>;
	using bmat2x3 = mat<2, 3, bool, packed_highp>;
	using bmat2x4 = mat<2, 4, bool, packed_highp>;
	using bmat3x2 = mat<3, 2, bool, packed_highp>;
	using bmat3x3 = mat<3, 3, bool, packed_highp>;
	using bmat3x4 = mat<3, 4, bool, packed_highp>;
	using bmat4x2 = mat<4, 2, bool, packed_highp>;
	using bmat4x3 = mat<4, 3, bool, packed_highp>;
	using bmat4x4 = mat<4, 4, bool, packed_highp>;

	using bmat2 = bmat2x2;
	using bmat3 = bmat3x3;
	using bmat4 = bmat4x4;

	using umat2 = umat2x2;
	using umat3 = umat3x3;
	using umat4 = umat4x4;

	using imat2 = imat2x2;
	using imat3 = imat3x3;
	using imat4 = imat4x4;
}
#pragma endregion

#endif