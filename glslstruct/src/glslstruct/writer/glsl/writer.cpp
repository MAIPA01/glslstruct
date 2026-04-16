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

	#include <glslstruct/pch.hpp>
	#include <glslstruct/writer/glsl/writer.hpp>

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

glsl_array_count_getter::glsl_array_count_getter(const bool canBeVariableSize) noexcept : _canBeVariableSize(canBeVariableSize) {}

const std::string& glsl_array_count_getter::get_result() const noexcept { return _result; }

std::string glslstruct::utils::get_glsl_array_count_string(const base_type_handle& varType, const bool canBeVariableSize) {
	glsl_array_count_getter arrayCountGetter(canBeVariableSize);
	varType->accept(arrayCountGetter);
	return arrayCountGetter.get_result();
}

std::string glslstruct::utils::get_glsl_variable_string(const std::string_view name, const base_type_handle& varType,
  const bool canBeVariableSize, const mstd::ordered_map<struct_type, std::string>& structsNames) {
	return fmt::format("{} {}{}", get_glsl_var_type_string(varType, structsNames), name,
	  get_glsl_array_count_string(varType, canBeVariableSize));
}

std::string glsl_writer::_get_layout(const std::string_view layoutData) { return fmt::format("layout ({})", layoutData); }

void glsl_writer::_append_struct_body(const std::string_view structBody) {
		if (!_result.empty()) { _result = fmt::format("{}\n\n", _result); }
	_result = fmt::format("{}{}", _result, structBody);
}

std::string glsl_writer::_get_struct_body(const std::string_view type, const std::string_view name,
  const bool canHaveVariableSizeArray, const mstd::ordered_map<std::string, var_data>& variables) {
	std::string structBody = fmt::format("{} {} {{\n", type, name);
	size_t i			   = 0;
		for (const auto& [varName, varData] : variables) {
			// Check substruct
			varData.get_type()->accept(*this);
			// Add variable
			structBody = fmt::format("{}    {};\n", structBody,
			  get_glsl_variable_string(varName, varData.get_type(), canHaveVariableSizeArray && i == variables.size() - 1,
				_uniqueStructs));

			// Increment variable index
			++i;
		}
	return fmt::format("{}}}", structBody);
}

void glsl_writer::_append_buffer_body(const std::string_view layoutData, const std::string_view name,
  const std::string_view bufferType, const bool canHaveVariableSizeArray,
  const mstd::ordered_map<std::string, var_data>& variables, const std::string_view varName) {
	glsl_struct_assert(!_contains_name(name), "Structure or SSBO or UBO with name: '{}' already exist", name);

	// Add buffer definition
	std::string structBody =
	  fmt::format("{} {}", _get_layout(layoutData), _get_struct_body(bufferType, name, canHaveVariableSizeArray, variables));
		if (!varName.empty()) { structBody = fmt::format("{} {};", structBody, varName); }
		else { structBody = fmt::format("{};", structBody); }
	_append_struct_body(structBody);

	// Add name
	_uniqueNames.insert(name.data());
}

void glsl_writer::_append_shader_storage_buffer_body(const std::string_view layoutData, const std::string_view name,
  const mstd::ordered_map<std::string, var_data>& variables, const std::string_view varName, const std::string_view qualifiers) {
		if (!qualifiers.empty()) {
			_append_buffer_body(layoutData, name, fmt::format("{} {}", qualifiers, "buffer"), true, variables, varName);
		}
		else { _append_buffer_body(layoutData, name, "buffer", true, variables, varName); }
}

bool glsl_writer::_contains_name(const std::string_view name) {
	#if _GLSL_STRUCT_HAS_CXX20
	return _uniqueNames.contains(name.data());
	#else
	return _uniqueNames.find(name.data()) != _uniqueNames.end();
	#endif
}

glsl_writer::glsl_writer() = default;

void glsl_writer::append_struct(const std::string_view name, const struct_type& structType) {
	glsl_struct_assert(!_contains_name(name), "Structure or SSBO or UBO with name: '{}' already exist", name);
	glsl_struct_assert(!_uniqueStructs.contains(structType),
	  "Structure with given layout already exists with different name: '{}' (yours: '{}')", _uniqueStructs.at(structType), name);

	const std::string structBody =
	  fmt::format("{};", _get_struct_body("struct", name, false, structType.get_top_level_variables()));

	// Add struct
	_uniqueStructs.emplace_back(structType, name.data());

	// Add name
	_uniqueNames.insert(name.data());

	// Add struct definition
	_append_struct_body(structBody);
}

void glsl_writer::append_struct(const struct_type& structType) {
	glsl_struct_assert(!_uniqueStructs.contains(structType), "Structure with given layout already exists with name: '{}'",
	  _uniqueStructs.at(structType));

	// Create struct name
	const std::string name = fmt::format("Struct_{}", _structIdx);
	++_structIdx;

	append_struct(name, structType);
}

void glsl_writer::append_uniform_buffer(const std::string_view layoutData, const std::string_view name,
  const mstd::ordered_map<std::string, var_data>& variables, const std::string_view varName) {
	_append_buffer_body(layoutData, name, "uniform", false, variables, varName);
}

void glsl_writer::append_shader_storage_buffer(const std::string_view layoutData, const std::string_view name,
  const mstd::ordered_map<std::string, var_data>& variables, const std::string_view varName, const std::string_view qualifiers) {
	_append_shader_storage_buffer_body(layoutData, name, variables, varName, qualifiers);
}

const std::string& glsl_writer::to_string() const noexcept { return _result; }

#endif