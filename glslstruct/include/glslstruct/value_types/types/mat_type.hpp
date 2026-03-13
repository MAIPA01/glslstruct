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

#include <glslstruct/value_types/value_type.hpp>

namespace glslstruct {
	class mat_type;

	using mat_type_handle = std::shared_ptr<mat_type>;

	class mat_type : public value_type<mat_type> {
	private:
		friend struct std::hash<mat_type>;

		using _base_class = value_type<mat_type>;

		size_t _cols = 0;
		size_t _rows = 0;
		ValueType _type = ValueType::Other;
		bool _columnMajor = false;

		[[nodiscard]] static size_t _calculate_mat_size(ValueType type, size_t cols, size_t rows, bool columnMajor = true) noexcept;

	public:
		mat_type(ValueType type, size_t cols, size_t rows, bool columnMajor = true) noexcept;
		_GLSL_STRUCT_CONSTEXPR20 mat_type(const mat_type& other) noexcept = default;
		_GLSL_STRUCT_CONSTEXPR20 mat_type(mat_type&& other) noexcept = default;
		virtual _GLSL_STRUCT_CONSTEXPR20 ~mat_type() noexcept = default;

		mat_type& operator=(const mat_type& other) noexcept = default;
		mat_type& operator=(mat_type&& other) noexcept = default;

		void accept(base_type_visitor* const visitor) const override;

		[[nodiscard]] ValueType get_type() const noexcept;
		[[nodiscard]] size_t get_rows() const noexcept;
		[[nodiscard]] size_t get_cols() const noexcept;
		[[nodiscard]] size_t get_vec_length() const noexcept;
		[[nodiscard]] size_t get_array_count() const noexcept;

		[[nodiscard]] bool is_column_major() const noexcept;

		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const mat_type& lhs, const mat_type& rhs) noexcept;
		friend bool operator!=(const mat_type& lhs, const mat_type& rhs) noexcept;
	};

	[[nodiscard]] bool operator==(const mat_type& lhs, const mat_type& rhs) noexcept;
	[[nodiscard]] bool operator!=(const mat_type& lhs, const mat_type& rhs) noexcept;
}

template<>
struct std::hash<glslstruct::mat_type> {
	[[nodiscard]] size_t operator()(const glslstruct::mat_type& value) noexcept;
};
#endif