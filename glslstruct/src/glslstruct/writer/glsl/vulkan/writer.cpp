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

std::string glsl_vulkan_writer::_get_layout_data(const size_t set, const size_t binding, const std::string_view stdType) {
		if (!stdType.empty()) { return fmt::format("{}, set = {}, binding = {}", stdType, set, binding); }
	return fmt::format("set = {}, binding = {}", set, binding);
}

glsl_vulkan_writer::glsl_vulkan_writer() = default;

void glsl_vulkan_writer::append_struct(const std::string_view name, const struct_type& structType) {
	_writer.append_struct(name, structType);
}

void glsl_vulkan_writer::append_struct(const struct_type& structType) { _writer.append_struct(structType); }

void glsl_vulkan_writer::append_uniform_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std140_layout& layout) {
	_writer.append_uniform_buffer(_get_layout_data(set, binding), name, layout.get_top_level_variables(), varName);
}

void glsl_vulkan_writer::append_uniform_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std140_struct& structValue) {
	append_uniform_buffer(set, binding, name, varName, structValue.get_layout());
}

void glsl_vulkan_writer::append_shader_storage_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std140_layout& layout, const std::string_view qualifiers) {
	_writer.append_shader_storage_buffer(_get_layout_data(set, binding, "std140"), name, layout.get_top_level_variables(),
	  varName, qualifiers);
}

void glsl_vulkan_writer::append_shader_storage_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const std430_layout& layout, const std::string_view qualifiers) {
	_writer.append_shader_storage_buffer(_get_layout_data(set, binding, "std430"), name, layout.get_top_level_variables(),
	  varName, qualifiers);
}

void glsl_vulkan_writer::append_shader_storage_buffer(const size_t set, const size_t binding, const std::string_view name,
  const std::string_view varName, const scalar_layout& layout, const std::string_view qualifiers) {
	_writer.append_shader_storage_buffer(_get_layout_data(set, binding, "scalar"), name, layout.get_top_level_variables(),
	  varName, qualifiers);
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

const std::string& glsl_vulkan_writer::to_string() const noexcept { return _writer.to_string(); }

#endif