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

#if _GLSL_STRUCT_HAS_TYPES
#include <glslstruct/value_types/types/base_type.hpp>
#endif

namespace glslstruct {
	class value_data {
	private:
		friend struct std::hash<value_data>;

#if _GLSL_STRUCT_HAS_TYPES
		base_type_handle _type;
#endif
		size_t _offset;
		size_t _size;
		size_t _padding;

	public:
#if _GLSL_STRUCT_HAS_TYPES
		value_data(size_t offset, const base_type_handle& type, size_t size, size_t padding) noexcept;
#else
		value_data(size_t offset, size_t size, size_t padding) noexcept;
#endif
		value_data(const value_data& other) noexcept;
		value_data(value_data&& other) noexcept;
		~value_data() noexcept;

		value_data& operator=(const value_data& other) noexcept;
		value_data& operator=(value_data&& other) noexcept;

#if _GLSL_STRUCT_HAS_TYPES
		void change_type(const base_type_handle& newType) noexcept;
#endif
		void set_padding(size_t padding) noexcept;

#if _GLSL_STRUCT_HAS_TYPES
		[[nodiscard]] const base_type_handle& get_type() const noexcept;
#endif
		[[nodiscard]] size_t get_offset() const noexcept;
		[[nodiscard]] size_t get_total_size() const noexcept;
		[[nodiscard]] size_t get_size() const noexcept;
		[[nodiscard]] size_t get_padding() const noexcept;

		[[nodiscard]] bool operator==(const value_data& other) const noexcept;
		[[nodiscard]] bool operator!=(const value_data& other) const noexcept;
	};
}

template<>
struct std::hash<glslstruct::value_data> {
	[[nodiscard]] size_t operator()(const glslstruct::value_data& value) const noexcept;
};
#endif