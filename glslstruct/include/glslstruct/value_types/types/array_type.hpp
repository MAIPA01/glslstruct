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

#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1!");
#else

#include <glslstruct/value_types/data/value_data.hpp>
#include <glslstruct/value_types/types/value_type.hpp>
#include <glslstruct/value_types/ValueType.hpp>
#include <glslstruct/value_types/MajorType.hpp>

namespace glslstruct {
	class array_type : public value_type<array_type> {
	private:
		friend struct std::hash<array_type>;

		base_type_handle _type = nullptr;
		size_t _count = 0;

		[[nodiscard]] static size_t _calculate_array_size(const base_type_handle& type, size_t count) noexcept;

	public:
		array_type(ValueType type, size_t count) noexcept;
		array_type(ValueType type, size_t length, size_t count) noexcept;
		array_type(ValueType type, size_t cols, size_t rows, MajorType major, size_t count) noexcept;
		array_type(ValueType type, size_t cols, size_t rows, size_t count) noexcept;
		array_type(const std::unordered_map<std::string, value_data>& values, size_t size, size_t count) noexcept;
		array_type(const base_type_handle& type, size_t count) noexcept;
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR20 array_type(const std::shared_ptr<T>& type, size_t count) noexcept 
			: array_type(std::dynamic_pointer_cast<base_type>(type), count) {}
		array_type(const array_type& other) noexcept;
		array_type(array_type&& other) noexcept;
		~array_type() noexcept override;

		array_type& operator=(const array_type& other) noexcept;
		array_type& operator=(array_type&& other) noexcept;

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, type_visitor, is_type_visitor_v<T>, = true)
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		[[nodiscard]] const base_type_handle& get_type() const noexcept;
		[[nodiscard]] size_t get_count() const noexcept;

		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const array_type& lhs, const array_type& rhs) noexcept;
		friend bool operator!=(const array_type& lhs, const array_type& rhs) noexcept;
	};

	[[nodiscard]] bool operator==(const array_type& lhs, const array_type& rhs) noexcept;
	[[nodiscard]] bool operator!=(const array_type& lhs, const array_type& rhs) noexcept;
}

template<>
struct std::hash<glslstruct::array_type> {
	[[nodiscard]] size_t operator()(const glslstruct::array_type& type) const noexcept;
};
#endif