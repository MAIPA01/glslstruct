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

	#include <glslstruct/writer/glsl/vulkan/writer.hpp>
	#include <pch.hpp>

using namespace glslstruct;

std::string glsl_vulkan_writer::_get_layout(const size_t set, const size_t binding, const std::string_view stdType) {
	std::string layout = "layout (";
		if (!stdType.empty()) { layout = fmt::format("{}{}, ", layout, stdType); }
	return fmt::format("{}set = {}, binding = {})", layout, set, binding);
}

void glsl_vulkan_writer::_append_struct_body(const std::string_view structBody) {
		if (!_result.empty()) { _result = fmt::format("{}\n\n", _result); }
	_result = fmt::format("{}{}", _result, structBody);
}

std::string glsl_vulkan_writer::_get_struct_body(const std::string_view type, const std::string_view name,
  const bool canHaveVariableSizeArray, const mstd::ordered_map<std::string, var_data>& variables) {
	std::string structBody = fmt::format("{} {} {{\n", type, name);
	size_t i			   = 0;
		for (const auto& [varName, varData] : variables) {
			// Check substruct
			varData.get_type()->accept(*this);
			// Add variable
			structBody = fmt::format("{}    {};\n", structBody,
			  utils::get_glsl_variable_string(varName, varData.get_type(), canHaveVariableSizeArray && i == variables.size() - 1,
				_uniqueStructs));

			// Increment variable index
			++i;
		}
	return fmt::format("{}}}", structBody);
}

void glsl_vulkan_writer::_append_buffer_body(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std::string_view bufferType, const bool canHaveVariableSizeArray,
  const mstd::ordered_map<std::string, var_data>& variables, const std::string_view stdType) {
	glsl_struct_assert(!_contains_name(name), "Structure or SSBO or UBO with name: '{}' already exist", name);

	// Add buffer definition
	_append_struct_body(fmt::format("{} {} {};", _get_layout(set, binding, stdType),
	  _get_struct_body(bufferType, name, canHaveVariableSizeArray, variables), varName));

	// Add name
	_uniqueNames.insert(name.data());
}

void glsl_vulkan_writer::_append_shader_storage_buffer_body(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std::string_view stdType, const mstd::ordered_map<std::string, var_data>& variables,
  const std::string_view qualifiers) {
	std::string bufferType = "buffer";
		if (!qualifiers.empty()) { bufferType = fmt::format("{} {}", qualifiers, bufferType); }
	_append_buffer_body(set, binding, name, varName, bufferType, true, variables, stdType);
}

bool glsl_vulkan_writer::_contains_name(const std::string_view name) {
	#if _GLSL_STRUCT_HAS_CXX20
	return _uniqueNames.contains(name.data());
	#else
	return _uniqueNames.find(name.data()) != _uniqueNames.end();
	#endif
}

glsl_vulkan_writer::glsl_vulkan_writer() = default;

void glsl_vulkan_writer::append_struct(const std::string_view name, const struct_type& structType) {
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

void glsl_vulkan_writer::append_struct(const struct_type& structType) {
	glsl_struct_assert(!_uniqueStructs.contains(structType), "Structure with given layout already exists with name: '{}'",
	  _uniqueStructs.at(structType));

	// Create struct name
	const std::string name = fmt::format("Struct_{}", _structIdx);
	++_structIdx;

	append_struct(name, structType);
}

void glsl_vulkan_writer::append_uniform_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std140_layout& layout) {
	_append_buffer_body(set, binding, name, varName, "uniform", false, layout.get_top_level_variables());
}

void glsl_vulkan_writer::append_uniform_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std140_struct& structValue) {
	append_uniform_buffer(set, binding, name, varName, structValue.get_layout());
}

void glsl_vulkan_writer::append_shader_storage_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std140_layout& layout, const std::string_view qualifiers) {
	_append_shader_storage_buffer_body(set, binding, name, varName, "std140", layout.get_top_level_variables(), qualifiers);
}

void glsl_vulkan_writer::append_shader_storage_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std430_layout& layout, const std::string_view qualifiers) {
	_append_shader_storage_buffer_body(set, binding, name, varName, "std430", layout.get_top_level_variables(), qualifiers);
}

void glsl_vulkan_writer::append_shader_storage_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const scalar_layout& layout, const std::string_view qualifiers) {
	_append_shader_storage_buffer_body(set, binding, name, varName, "scalar", layout.get_top_level_variables(), qualifiers);
}

void glsl_vulkan_writer::append_shader_storage_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std140_struct& structValue, const std::string_view qualifiers) {
	append_shader_storage_buffer(set, binding, name, varName, structValue.get_layout(), qualifiers);
}

void glsl_vulkan_writer::append_shader_storage_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std430_struct& structValue, const std::string_view qualifiers) {
	append_shader_storage_buffer(set, binding, name, varName, structValue.get_layout(), qualifiers);
}

void glsl_vulkan_writer::append_shader_storage_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const scalar_struct& structValue, const std::string_view qualifiers) {
	append_shader_storage_buffer(set, binding, name, varName, structValue.get_layout(), qualifiers);
}

const std::string& glsl_vulkan_writer::to_string() const noexcept { return _result; }

#endif