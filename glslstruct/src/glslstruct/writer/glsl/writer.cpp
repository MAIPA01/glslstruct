/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with defined GLSL_STRUCT_DISABLE_TYPES!"
);
#else

	#include <glslstruct/writer/glsl/writer.hpp>
	#include <pch.hpp>

using namespace glslstruct::utils;

glsl_var_type_getter::glsl_var_type_getter(const mstd::ordered_map<struct_type, std::string>& structsNames)
	: _structsNames(structsNames) {}

const std::string& glsl_var_type_getter::get_result() const noexcept { return _result; }

std::string glslstruct::utils::get_glsl_var_type_string(const base_type_handle& varType,
  const mstd::ordered_map<struct_type, std::string>& structsNames) {
	glsl_var_type_getter varTypeGetter(structsNames);
	varType->accept(varTypeGetter);
	return varTypeGetter.get_result();
}

const std::string& glsl_array_count_getter::get_result() const noexcept { return _result; }

std::string glslstruct::utils::get_glsl_array_count_string(const base_type_handle& varType) {
	glsl_array_count_getter arrayCountGetter;
	varType->accept(arrayCountGetter);
	return arrayCountGetter.get_result();
}

std::string glslstruct::utils::get_glsl_variable_string(const std::string_view name, const base_type_handle& varType,
  const mstd::ordered_map<struct_type, std::string>& structsNames) {
	return fmt::format("{} {}{}", get_glsl_var_type_string(varType, structsNames), name, get_glsl_array_count_string(varType));
}

#endif