/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/value_types/base_type.hpp>

namespace glslstruct {
	class value_data {
	private:
		friend struct std::hash<value_data>;

		base_type_handle _type;
		size_t _offset;
		size_t _size;
		size_t _padding;

	public:
		value_data(size_t offset, const base_type_handle& type, size_t size, size_t padding) noexcept;
		value_data(const value_data& other) noexcept = default;
		value_data(value_data&& other) noexcept;
		~value_data() noexcept = default;

		value_data& operator=(const value_data& other) noexcept = default;
		value_data& operator=(value_data&& other) noexcept;

		void change_type(const base_type_handle& newType) noexcept;
		void set_padding(size_t padding) noexcept;

		[[nodiscard]] const base_type_handle& get_type() const noexcept;
		[[nodiscard]] size_t get_offset() const noexcept;
		[[nodiscard]] size_t get_total_size() const noexcept;
		[[nodiscard]] size_t get_size() const noexcept;
		[[nodiscard]] size_t get_padding() const noexcept;

		[[nodiscard]] bool operator==(const value_data& other) const noexcept;
		[[nodiscard]] bool operator!=(const value_data& other) const noexcept _GLSL_STRUCT_DEFAULT20;
	};
}

template<>
struct std::hash<glslstruct::value_data> {
	[[nodiscard]] size_t operator()(const glslstruct::value_data& value) const noexcept;
};