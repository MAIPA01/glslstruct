/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_STRUCT_TYPE_HPP_
	#define _GLSL_STRUCT_STRUCT_TYPE_HPP_

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
	 * @brief struct type container
	 * @ingroup glsl_types
	 */
	class _GLSL_STRUCT_EXPORT struct_type : public type<struct_type> {
	private:
		friend struct std::hash<struct_type>;

		using base_type = type;

		/// @brief map with all values of struct
		mstd::ordered_map<std::string, var_data> _variables;

	public:
		/// @brief constructor with struct values and type size
		struct_type(const mstd::ordered_map<std::string, var_data>& values, size_t size) noexcept;
		/// @brief default copy constructor
		struct_type(const struct_type& other) noexcept;
		/// @brief default move constructor
		struct_type(struct_type&& other) noexcept;
		/// @brief default destructor
		~struct_type() noexcept override;

		/// @brief default copy assign operator
		struct_type& operator=(const struct_type& other) noexcept;
		/// @brief default move assign operator
		struct_type& operator=(struct_type&& other) noexcept;

		/// @brief accept function for type visitors
		#if _GLSL_STRUCT_HAS_CXX20
		template<type_visitor T>
		#else
		template<class T, std::enable_if_t<is_type_visitor_v<T>, bool> = true>
		#endif
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		/// @brief returns true if struct type contains variable of given name
		[[nodiscard]] bool contains(std::string_view name) const noexcept;

		/// @brief returns map with all values
		[[nodiscard]] const mstd::ordered_map<std::string, var_data>& get_variables() const noexcept;

		/// @brief returns map with all top level values
		[[nodiscard]] mstd::ordered_map<std::string, var_data> get_top_level_variables() const noexcept;

		/// @brief converts type to string
		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const struct_type& lhs, const struct_type& rhs) noexcept;
		friend bool operator!=(const struct_type& lhs, const struct_type& rhs) noexcept;
	};

	/**
	 * @brief checks if two struct types are equal
	 * @ingroup glsl_types
	 */
	[[nodiscard]] bool operator==(const struct_type& lhs, const struct_type& rhs) noexcept;
	/**
	 * @brief checks if two struct types are not equal
	 * @ingroup glsl_types
	 */
	[[nodiscard]] bool operator!=(const struct_type& lhs, const struct_type& rhs) noexcept;
} // namespace glslstruct

/**
 * @brief std::hash overload for struct_type
 * @ingroup glsl_types
 */
template<>
struct _GLSL_STRUCT_EXPORT std::hash<glslstruct::struct_type> {
	[[nodiscard]] size_t operator()(const glslstruct::struct_type& type) const noexcept;
};
	#endif
#endif