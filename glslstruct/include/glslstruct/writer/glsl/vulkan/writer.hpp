/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_GLSL_VULKAN_WRITER_HPP_
	#define _GLSL_STRUCT_GLSL_VULKAN_WRITER_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with defined GLSL_STRUCT_DISABLE_TYPES!"
);
	#else

		#include <glslstruct/struct/base_struct.hpp>
		#include <glslstruct/writer/glsl/writer.hpp>

/**
 * @defgroup vulkan_writer Vulkan Writer
 * @ingroup glslstruct
 * @brief Converts glsl struct and layout to glsl code compatible with vulkan
 * @details Example:
 * @code{.cpp}
 * // UBO
 * layout (set = num, binding = num) uniform CameraData {
 *     mat4 projection;
 *     mat4 view;
 *     vec3 eyePos;
 * } ubo;
 *
 * // SSBO
 * struct Particle {
 *     vec4 position;
 *     vec4 velocity;
 * };
 *
 * // here we can have std140 or std430 or scalar layout type
 * // here we can also have qualifiers for SSBO
 * layout(scalar, set = 0, binding = 1) readonly buffer ParticleBuffer {
 *     Particle particles[];
 * } ssbo;
 * @endcode
 */

namespace glslstruct {
	/**
	 * @ingroup vulkan_writer Vulkan glsl code writer
	 * @brief struct to vulkan glsl writer
	 */
	class glsl_vulkan_writer {
		/// @brief glsl writer instance
		utils::glsl_writer _writer;

		/// @brief returns layout data string
		[[nodiscard]] static std::string _get_layout_data(size_t set, size_t binding, std::string_view stdType = "");

	public:
		/// @brief default constructor
		glsl_vulkan_writer();

		/// @brief appends struct with given name
		void append_struct(std::string_view name, const struct_type& structType);

		/// @brief appends struct with generated name
		void append_struct(const struct_type& structType);

		/// @brief appends uniform buffer
		void append_uniform_buffer(size_t set, size_t binding, std::string_view name, std::string_view varName,
		  const std140_layout& layout);

		/// @brief appends uniform buffer
		void append_uniform_buffer(size_t set, size_t binding, std::string_view name, std::string_view varName,
		  const std140_struct& structValue);

		/// @brief appends shader storage buffer
		void append_shader_storage_buffer(size_t set, size_t binding, std::string_view name, std::string_view varName,
		  const std140_layout& layout, std::string_view qualifiers = "");

		/// @brief appends shader storage buffer
		void append_shader_storage_buffer(size_t set, size_t binding, std::string_view name, std::string_view varName,
		  const std430_layout& layout, std::string_view qualifiers = "");

		/// @brief appends shader storage buffer
		void append_shader_storage_buffer(size_t set, size_t binding, std::string_view name, std::string_view varName,
		  const scalar_layout& layout, std::string_view qualifiers = "");

		/// @brief appends shader storage buffer
		void append_shader_storage_buffer(size_t set, size_t binding, std::string_view name, std::string_view varName,
		  const std140_struct& structValue, std::string_view qualifiers = "");

		/// @brief appends shader storage buffer
		void append_shader_storage_buffer(size_t set, size_t binding, std::string_view name, std::string_view varName,
		  const std430_struct& structValue, std::string_view qualifiers = "");

		/// @brief appends shader storage buffer
		void append_shader_storage_buffer(size_t set, size_t binding, std::string_view name, std::string_view varName,
		  const scalar_struct& structValue, std::string_view qualifiers = "");

		/// @brief returns result string
		[[nodiscard]] const std::string& to_string() const noexcept;
	};
} // namespace glslstruct

	#endif
#endif