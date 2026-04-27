/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_ARRAY_TYPE_HPP_
	#define _GLSL_STRUCT_ARRAY_TYPE_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with defined GLSL_STRUCT_DISABLE_TYPES!"
);
	#else

		#include <glslstruct/type/containers/type.hpp>
		#include <glslstruct/var_data/var_data.hpp>

namespace glslstruct {
	/**
	 * @brief array type container
	 * @ingroup glsl_types
	 */
	class _GLSL_STRUCT_EXPORT array_type : public type<array_type> {
	private:
		friend struct std::hash<array_type>;

		using base_type		   = type;

		/// @brief handle to array elem type
		base_type_handle _type = nullptr;
		/// @brief count of array elements
		size_t _count		   = 0;

	public:
		/// @brief constructs scalar array type
		array_type(ValueType type, size_t scalarSize, size_t count, size_t size) noexcept;
		/// @brief constructs vec array type
		array_type(ValueType type, size_t length, size_t vecSize, size_t count, size_t size) noexcept;
		/// @brief constructs mat array type
		array_type(ValueType type, size_t cols, size_t rows, size_t matSize, size_t count, size_t size) noexcept;
		/// @brief constructs struct array type
		array_type(const mstd::ordered_map<std::string, var_data>& values, size_t structSize, size_t count, size_t size) noexcept;
		/// @brief constructs array with given elem type
		array_type(const base_type_handle& type, size_t count, size_t size) noexcept;
		/// @brief constructs array with given elem type
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_type T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 array_type(const std::shared_ptr<T>& type, const size_t count, const size_t size) noexcept
			: base_type(size), _type(std::dynamic_pointer_cast<glslstruct::base_type>(type)), _count(count) {
		}

		/// @brief default copy constructor
		array_type(const array_type& other) noexcept;
		/// @brief default move constructor
		array_type(array_type&& other) noexcept;
		/// @brief default destructor
		~array_type() noexcept override;

		/// @brief default copy assign operator
		array_type& operator=(const array_type& other) noexcept;
		/// @brief default move assign operator
		array_type& operator=(array_type&& other) noexcept;

		/// @brief accept function for type visitors
		#if _GLSL_STRUCT_HAS_CXX20
		template<type_visitor T>
		#else
		template<class T, std::enable_if_t<is_type_visitor_v<T>, bool> = true>
		#endif
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		/// @brief returns array elem type
		[[nodiscard]] const base_type_handle& get_type() const noexcept;

		/// @brief returns array elems count
		[[nodiscard]] size_t get_count() const noexcept;

		/// @brief converts type to string
		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const array_type& lhs, const array_type& rhs) noexcept;
		friend bool operator!=(const array_type& lhs, const array_type& rhs) noexcept;
	};

	/**
	 * @brief checks if two array types are equal
	 * @ingroup glsl_types
	 */
	[[nodiscard]] bool operator==(const array_type& lhs, const array_type& rhs) noexcept;
	/**
	 * @brief checks if two array types are not equal
	 * @ingroup glsl_types
	 */
	[[nodiscard]] bool operator!=(const array_type& lhs, const array_type& rhs) noexcept;
} // namespace glslstruct

/**
 * @brief std::hash overload for array_type
 * @ingroup glsl_types
 */
template<>
struct _GLSL_STRUCT_EXPORT std::hash<glslstruct::array_type> {
	[[nodiscard]] size_t operator()(const glslstruct::array_type& type) const noexcept;
};
	#endif
#endif