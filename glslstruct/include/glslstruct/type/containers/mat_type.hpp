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

		#include <glslstruct/type/containers/type.hpp>

namespace glslstruct {
	/**
	 * @brief mat type container
	 * @ingroup glsl_types
	 */
	class mat_type : public type<mat_type> {
	private:
		friend struct std::hash<mat_type>;

		using base_type = type;

		/// @brief number of columns
		size_t _cols;
		/// @brief number of rows
		size_t _rows;
		/// @brief scalar type of mat
		ValueType _type;

	public:
		/// @brief constructor with scalar type, number of columns and rows of mat and size of type
		mat_type(ValueType type, size_t cols, size_t rows, size_t size) noexcept;
		/// @brief default copy constructor
		mat_type(const mat_type& other) noexcept;
		/// @brief default move constructor
		mat_type(mat_type&& other) noexcept;
		/// @brief default destructor
		~mat_type() noexcept override;

		/// @brief default copy assign operator
		mat_type& operator=(const mat_type& other) noexcept;
		/// @brief default move assign operator
		mat_type& operator=(mat_type&& other) noexcept;

		/// @brief accept function for type visitors
		#if _GLSL_STRUCT_HAS_CXX20
		template<type_visitor T>
		#else
		template<class T, std::enable_if_t<is_type_visitor_v<T>, bool> = true>
		#endif
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		/// @brief returns scalar type of mat
		[[nodiscard]] ValueType get_type() const noexcept;
		/// @brief returns rows number
		[[nodiscard]] size_t get_rows() const noexcept;
		/// @brief returns columns number
		[[nodiscard]] size_t get_cols() const noexcept;
		/// @brief returns column vector length (rows)
		[[nodiscard]] size_t get_vec_length() const noexcept;
		/// @brief returns column vectors number (columns)
		[[nodiscard]] size_t get_array_count() const noexcept;

		/// @brief converts type to string
		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const mat_type& lhs, const mat_type& rhs) noexcept;
		friend bool operator!=(const mat_type& lhs, const mat_type& rhs) noexcept;
	};

	/**
	 * @brief checks if two mats types are equal
	 * @ingroup glsl_types
	 */
	[[nodiscard]] bool operator==(const mat_type& lhs, const mat_type& rhs) noexcept;
	/**
	 * @brief checks if two mats types are not equal
	 * @ingroup glsl_types
	 */
	[[nodiscard]] bool operator!=(const mat_type& lhs, const mat_type& rhs) noexcept;
} // namespace glslstruct

/**
 * @brief std::hash overload for mat_type
 * @ingroup glsl_types
 */
template<>
struct std::hash<glslstruct::mat_type> {
	[[nodiscard]] size_t operator()(const glslstruct::mat_type& value) const noexcept;
};
	#endif
#endif