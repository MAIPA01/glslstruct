/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_STRUCT_ELEM_DATA_HPP_
	#define _GLSL_STRUCT_STRUCT_ELEM_DATA_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/types.hpp>

namespace glslstruct {
	class struct_elem_data {
	private:
		friend struct std::hash<struct_elem_data>;

		#if _GLSL_STRUCT_HAS_TYPES
		base_type_handle _type;
		#else
		size_t _size;
		#endif
		size_t _offset;
		size_t _padding;

	public:
		#if _GLSL_STRUCT_HAS_TYPES
		struct_elem_data(size_t offset, const base_type_handle& type, size_t padding) noexcept;
		#else
		struct_elem_data(size_t offset, size_t size, size_t padding) noexcept;
		#endif
		struct_elem_data(const struct_elem_data& other) noexcept;
		struct_elem_data(struct_elem_data&& other) noexcept;
		~struct_elem_data() noexcept;

		struct_elem_data& operator=(const struct_elem_data& other) noexcept;
		struct_elem_data& operator=(struct_elem_data&& other) noexcept;

		#if _GLSL_STRUCT_HAS_TYPES
		void set_type(const base_type_handle& newType) noexcept;
		#else
		void set_size(size_t size) noexcept;
		#endif
		void set_padding(size_t padding) noexcept;

		#if _GLSL_STRUCT_HAS_TYPES
		[[nodiscard]] const base_type_handle& get_type() const noexcept;
		#endif
		[[nodiscard]] size_t get_offset() const noexcept;
		[[nodiscard]] size_t get_total_size() const noexcept;
		[[nodiscard]] size_t get_size() const noexcept;
		[[nodiscard]] size_t get_padding() const noexcept;

		[[nodiscard]] bool operator==(const struct_elem_data& other) const noexcept;
		[[nodiscard]] bool operator!=(const struct_elem_data& other) const noexcept;
	};
} // namespace glslstruct

template<>
struct std::hash<glslstruct::struct_elem_data> {
	[[nodiscard]] size_t operator()(const glslstruct::struct_elem_data& value) const noexcept;
};
	#endif
#endif