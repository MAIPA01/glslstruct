/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_MAT_TYPE_HPP_
	#define _GLSL_STRUCT_MAT_TYPE_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with defined GLSL_STRUCT_DISABLE_TYPES!"
);
	#else

		#include <glslstruct/type_containers/type.hpp>

namespace glslstruct {
	class mat_type : public type<mat_type> {
	private:
		friend struct std::hash<mat_type>;

		size_t _cols;
		size_t _rows;
		ValueType _type;

	public:
		mat_type(ValueType type, size_t cols, size_t rows, size_t size) noexcept;
		mat_type(const mat_type& other) noexcept;
		mat_type(mat_type&& other) noexcept;
		~mat_type() noexcept override;

		mat_type& operator=(const mat_type& other) noexcept;
		mat_type& operator=(mat_type&& other) noexcept;

		#if _GLSL_STRUCT_HAS_CXX20
		template<type_visitor T>
		#else
		template<class T, std::enable_if_t<is_type_visitor_v<T>, bool> = true>
		#endif
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		[[nodiscard]] ValueType get_type() const noexcept;
		[[nodiscard]] size_t get_rows() const noexcept;
		[[nodiscard]] size_t get_cols() const noexcept;
		[[nodiscard]] size_t get_vec_length() const noexcept;
		[[nodiscard]] size_t get_array_count() const noexcept;

		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const mat_type& lhs, const mat_type& rhs) noexcept;
		friend bool operator!=(const mat_type& lhs, const mat_type& rhs) noexcept;
	};

	[[nodiscard]] bool operator==(const mat_type& lhs, const mat_type& rhs) noexcept;
	[[nodiscard]] bool operator!=(const mat_type& lhs, const mat_type& rhs) noexcept;
} // namespace glslstruct

template<>
struct std::hash<glslstruct::mat_type> {
	[[nodiscard]] size_t operator()(const glslstruct::mat_type& value) const noexcept;
};
	#endif
#endif