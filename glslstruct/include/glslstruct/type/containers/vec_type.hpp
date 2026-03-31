/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_VEC_TYPE_HPP_
	#define _GLSL_STRUCT_VEC_TYPE_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with defined GLSL_STRUCT_DISABLE_TYPES!"
);
	#else

		#include <glslstruct/type/containers/type.hpp>

namespace glslstruct {
	/**
	 * @brief vec type container
	 * @ingroup glsl_types
	 */
	class vec_type : public type<vec_type> {
	private:
		friend struct std::hash<vec_type>;

		using base_type = type;

		/// @brief length of vec
		size_t _length;
		/// @brief scalar type of vec
		ValueType _type;

	public:
		/// @brief constructor with scalar type and length of vec and size of type
		vec_type(ValueType type, size_t length, size_t size) noexcept;
		/// @brief default copy constructor
		vec_type(const vec_type& other) noexcept;
		/// @brief default move constructor
		vec_type(vec_type&& other) noexcept;
		/// @brief default destructor
		~vec_type() noexcept override;

		/// @brief default copy assign operator
		vec_type& operator=(const vec_type& other) noexcept;
		/// @brief default move assign operator
		vec_type& operator=(vec_type&& other) noexcept;

		/// @brief accept function for type visitors
		#if _GLSL_STRUCT_HAS_CXX20
		template<type_visitor T>
		#else
		template<class T, std::enable_if_t<is_type_visitor_v<T>, bool> = true>
		#endif
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		/// @brief returns scalar type of vec
		[[nodiscard]] ValueType get_type() const noexcept;
		/// @brief returns length of vec
		[[nodiscard]] size_t get_length() const noexcept;

		/// @brief converts type to string
		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const vec_type& lhs, const vec_type& rhs) noexcept;
		friend bool operator!=(const vec_type& lhs, const vec_type& rhs) noexcept;
	};

	/**
	 * @brief checks if two vec types are equal
	 * @ingroup glsl_types
	 */
	[[nodiscard]] bool operator==(const vec_type& lhs, const vec_type& rhs) noexcept;
	/**
	 * @brief checks if two vec types are not equal
	 * @ingroup glsl_types
	 */
	[[nodiscard]] bool operator!=(const vec_type& lhs, const vec_type& rhs) noexcept;
} // namespace glslstruct

/**
 * @brief std::hash overload for vec_type
 * @ingroup glsl_types
 */
template<>
struct std::hash<glslstruct::vec_type> {
	[[nodiscard]] size_t operator()(const glslstruct::vec_type& value) const noexcept;
};
	#endif
#endif