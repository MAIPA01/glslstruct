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

		#include <glslstruct/type_containers/type.hpp>
		#include <glslstruct/var_data/var_data.hpp>

namespace glslstruct {
	class struct_type : public type<struct_type> {
	private:
		friend struct std::hash<struct_type>;

		using base_type = type;

		std::unordered_map<std::string, var_data> _values;

	public:
		struct_type(const std::unordered_map<std::string, var_data>& values, size_t size) noexcept;
		struct_type(const struct_type& other) noexcept;
		struct_type(struct_type&& other) noexcept;
		~struct_type() noexcept override;

		struct_type& operator=(const struct_type& other) noexcept;
		struct_type& operator=(struct_type&& other) noexcept;

		#if _GLSL_STRUCT_HAS_CXX20
		template<type_visitor T>
		#else
		template<class T, std::enable_if_t<is_type_visitor_v<T>, bool> = true>
		#endif
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		[[nodiscard]] const std::unordered_map<std::string, var_data>& get_values() const noexcept;

		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const struct_type& lhs, const struct_type& rhs) noexcept;
		friend bool operator!=(const struct_type& lhs, const struct_type& rhs) noexcept;
	};

	[[nodiscard]] bool operator==(const struct_type& lhs, const struct_type& rhs) noexcept;
	[[nodiscard]] bool operator!=(const struct_type& lhs, const struct_type& rhs) noexcept;
} // namespace glslstruct

template<>
struct std::hash<glslstruct::struct_type> {
	[[nodiscard]] size_t operator()(const glslstruct::struct_type& type) const noexcept;
};
	#endif
#endif