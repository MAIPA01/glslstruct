/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_GLSL_OPENGL_WRITER_HPP_
	#define _GLSL_STRUCT_GLSL_OPENGL_WRITER_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with defined GLSL_STRUCT_DISABLE_TYPES!"
);
	#else

		#include <glslstruct/struct/base_struct.hpp>

/**
 * @defgroup opengl_writer Opengl Writer
 * @ingroup glslstruct
 * @brief Converts glsl struct and layout to glsl code compatible with opengl
 * @details Example:
 * @code{.cpp}
 * // UBO
 * layout (std140, binding = 0) uniform CameraData {
 *     mat4 projection;
 *     mat4 view;
 *     vec3 eyePos;
 * };
 *
 * // SSBO
 * struct Particle {
 *     vec4 position;
 *     vec4 velocity;
 * };
 *
 * // here we can have std140 or std430 layout type
 * layout(std430, binding = 1) buffer ParticleBuffer {
 *     Particle particles[];
 * };
 * @endcode
 */

namespace glslstruct {
	/**
	 * @ingroup opengl_writer OpenGL glsl code writer
	 * @brief struct to opengl glsl writer
	 */
	class glsl_opengl_writer {
		/// @brief glsl writer instance
		utils::glsl_writer _writer;

		/// @brief returns layout data string
		[[nodiscard]] static std::string _get_layout_data(size_t binding, std::string_view stdType);

	public:
		/// @brief default constructor
		glsl_opengl_writer();

		/// @brief appends struct with given name
		void append_struct(std::string_view name, const struct_type& structType);

		/// @brief appends struct with generated name
		void append_struct(const struct_type& structType);

		/// @brief appends uniform buffer
		void append_uniform_buffer(size_t binding, std::string_view name, const std140_layout& layout);

		/// @brief appends uniform buffer
		void append_uniform_buffer(size_t binding, std::string_view name, const std140_struct& structValue);

		/// @brief appends shader storage buffer
		void append_shader_storage_buffer(size_t binding, std::string_view name, const std140_layout& layout);

		/// @brief appends shader storage buffer
		void append_shader_storage_buffer(size_t binding, std::string_view name, const std430_layout& layout);

		/// @brief appends shader storage buffer
		void append_shader_storage_buffer(size_t binding, std::string_view name, const std140_struct& structValue);

		/// @brief appends shader storage buffer
		void append_shader_storage_buffer(size_t binding, std::string_view name, const std430_struct& structValue);

		/// @brief returns result string
		[[nodiscard]] const std::string& to_string() const noexcept;
	};
} // namespace glslstruct

	#endif
#endif