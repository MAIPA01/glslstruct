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

#include <glslstruct/value_types/types/value_type.hpp>
#include <glslstruct/value_types/ValueType.hpp>
#include <glslstruct/value_types/MajorType.hpp>

namespace glslstruct {
	class mat_type : public value_type<mat_type> {
	private:
		friend struct std::hash<mat_type>;

		size_t _cols = 0;
		size_t _rows = 0;
		ValueType _type = ValueType::Other;
		MajorType _major = MajorType::Column;

		[[nodiscard]] static size_t _calculate_mat_size(ValueType type, size_t cols, size_t rows, MajorType major = MajorType::Column) noexcept;

	public:
		mat_type(ValueType type, size_t cols, size_t rows, MajorType major = MajorType::Column) noexcept;
		mat_type(const mat_type& other) noexcept;
		mat_type(mat_type&& other) noexcept;
		~mat_type() noexcept override;

		mat_type& operator=(const mat_type& other) noexcept;
		mat_type& operator=(mat_type&& other) noexcept;

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, type_visitor, is_type_visitor_v<T>, = true)
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		[[nodiscard]] ValueType get_type() const noexcept;
		[[nodiscard]] size_t get_rows() const noexcept;
		[[nodiscard]] size_t get_cols() const noexcept;
		[[nodiscard]] size_t get_vec_length() const noexcept;
		[[nodiscard]] size_t get_array_count() const noexcept;

		[[nodiscard]] MajorType get_major_type() const noexcept;

		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const mat_type& lhs, const mat_type& rhs) noexcept;
		friend bool operator!=(const mat_type& lhs, const mat_type& rhs) noexcept;
	};

	[[nodiscard]] bool operator==(const mat_type& lhs, const mat_type& rhs) noexcept;
	[[nodiscard]] bool operator!=(const mat_type& lhs, const mat_type& rhs) noexcept;
}

template<>
struct std::hash<glslstruct::mat_type> {
	[[nodiscard]] size_t operator()(const glslstruct::mat_type& value) const noexcept;
};
#endif