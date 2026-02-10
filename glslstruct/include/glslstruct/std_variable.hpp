/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/templates.hpp>

namespace glslstruct {
	struct standard_variable {};

	template<class _Offset>
	struct offset_variable {
		const _Offset struct_offsets;

		offset_variable(const _Offset& offsets) : struct_offsets(offsets) {}
	};

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_simple_or_offset_value, utils::is_glsl_simple_or_offset_value_v<T>, , size_t num)
	struct std_variable : public std::conditional_t<
		utils::is_glsl_offset_v<T>,
		offset_variable<T>,
		standard_variable> {
	public:
		using var_type = T;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t array_size = num;
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_offsets = utils::is_glsl_offset_v<T>;

#pragma region VARIABLES
		const std::string var_name;
#pragma endregion

#pragma region NORMAL_CONSTRUCTOR
		_GLSL_STRUCT_ENABLE_IF_TEMPLATE(type = var_type, (utils::is_glsl_simple_value_v<type> && std::is_same_v<type, var_type>))
		_GLSL_STRUCT_CONSTEXPR20 std_variable(const std::string& name) noexcept
			_GLSL_STRUCT_REQUIRES(utils::is_glsl_simple_value_v<T>)
			: var_name(name) {}
#pragma endregion

#pragma region OFFSETS_CONSTRUCTOR
		_GLSL_STRUCT_ENABLE_IF_TEMPLATE(type = var_type, (utils::is_glsl_offset_v<type> && std::is_same_v<type, var_type>))
		_GLSL_STRUCT_CONSTEXPR20 std_variable(const std::string& name, const T& offsets) noexcept 
			_GLSL_STRUCT_REQUIRES(utils::is_glsl_offset_v<T>)
			: offset_variable<T>(offsets), var_name(name) {}
	};
}