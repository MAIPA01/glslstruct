/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_SCALAR_TYPE_HPP_
	#define _GLSL_STRUCT_SCALAR_TYPE_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with defined GLSL_STRUCT_DISABLE_TYPES!"
);
	#else

		#include <glslstruct/type/containers/type.hpp>

namespace glslstruct {
	/**
	 * @brief scalar type container
	 * @ingroup glsl_types
	 */
	class scalar_type : public type<scalar_type> {
	private:
		friend struct std::hash<scalar_type>;

		using base_type = type;

		/// @brief value scalar type
		ValueType _type;

	public:
		/// @brief constructor with scalar type and type size
		explicit scalar_type(ValueType type, size_t size) noexcept;
		/// @brief default copy constructor
		scalar_type(const scalar_type& other) noexcept;
		/// @brief default move constructor
		scalar_type(scalar_type&& other) noexcept;
		/// @brief default destructor
		~scalar_type() noexcept override;

		/// @brief default copy assign operator
		scalar_type& operator=(const scalar_type& other) noexcept;
		/// @brief default move assign operator
		scalar_type& operator=(scalar_type&& other) noexcept;

		/// @brief accept function for type visitors
		#if _GLSL_STRUCT_HAS_CXX20
		template<type_visitor T>
		#else
		template<class T, std::enable_if_t<is_type_visitor_v<T>, bool> = true>
		#endif
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		/// @brief returns scalar type
		[[nodiscard]] ValueType get_type() const noexcept;

		/// @brief converts type to string
		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const scalar_type& lhs, const scalar_type& rhs) noexcept;
		friend bool operator!=(const scalar_type& lhs, const scalar_type& rhs) noexcept;
	};

	/**
	 * @brief checks if two scalar types are equal
	 * @ingroup glsl_types
	 */
	[[nodiscard]] bool operator==(const scalar_type& lhs, const scalar_type& rhs) noexcept;
	/**
	 * @brief checks if two scalar types are not equal
	 * @ingroup glsl_types
	 */
	[[nodiscard]] bool operator!=(const scalar_type& lhs, const scalar_type& rhs) noexcept;
} // namespace glslstruct

/**
 * @brief std::hash overload for scalar_type
 * @ingroup glsl_types
 */
template<>
struct std::hash<glslstruct::scalar_type> {
	[[nodiscard]] size_t operator()(const glslstruct::scalar_type& value) const noexcept;
};
	#endif
#endif