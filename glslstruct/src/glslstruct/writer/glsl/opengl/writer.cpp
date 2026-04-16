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

	#include <glslstruct/pch.hpp>
	#include <glslstruct/writer/glsl/opengl/writer.hpp>

using namespace glslstruct;

std::string glsl_opengl_writer::_get_layout_data(const size_t binding, const std::string_view stdType) {
	return fmt::format("{}, binding = {}", stdType, binding);
}

glsl_opengl_writer::glsl_opengl_writer() = default;

void glsl_opengl_writer::append_struct(const std::string_view name, const struct_type& structType) {
	_writer.append_struct(name, structType);
}

void glsl_opengl_writer::append_struct(const struct_type& structType) { _writer.append_struct(structType); }

void glsl_opengl_writer::append_uniform_buffer(const size_t binding, const std::string_view name, const std140_layout& layout) {
	_writer.append_uniform_buffer(_get_layout_data(binding, "std140"), name, layout.get_top_level_variables());
}

void glsl_opengl_writer::append_uniform_buffer(const size_t binding, const std::string_view name,
  const std140_struct& structValue) {
	append_uniform_buffer(binding, name, structValue.get_layout());
}

void glsl_opengl_writer::append_shader_storage_buffer(const size_t binding, const std::string_view name,
  const std140_layout& layout) {
	_writer.append_shader_storage_buffer(_get_layout_data(binding, "std140"), name, layout.get_top_level_variables());
}

void glsl_opengl_writer::append_shader_storage_buffer(const size_t binding, const std::string_view name,
  const std430_layout& layout) {
	_writer.append_shader_storage_buffer(_get_layout_data(binding, "std430"), name, layout.get_top_level_variables());
}

void glsl_opengl_writer::append_shader_storage_buffer(const size_t binding, const std::string_view name,
  const std140_struct& structValue) {
	append_shader_storage_buffer(binding, name, structValue.get_layout());
}

void glsl_opengl_writer::append_shader_storage_buffer(const size_t binding, const std::string_view name,
  const std430_struct& structValue) {
	append_shader_storage_buffer(binding, name, structValue.get_layout());
}

const std::string& glsl_opengl_writer::to_string() const noexcept { return _writer.to_string(); }

#endif