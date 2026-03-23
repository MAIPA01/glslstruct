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

		#include <glslstruct/type_containers/type.hpp>

namespace glslstruct {
	class vec_type : public type<vec_type> {
	private:
		friend struct std::hash<vec_type>;

		size_t _length;
		ValueType _type;

	public:
		vec_type(ValueType type, size_t length, size_t size) noexcept;
		vec_type(const vec_type& other) noexcept;
		vec_type(vec_type&& other) noexcept;
		~vec_type() noexcept override;

		vec_type& operator=(const vec_type& other) noexcept;
		vec_type& operator=(vec_type&& other) noexcept;

		#if _GLSL_STRUCT_HAS_CXX20
		template<type_visitor T>
		#else
		template<class T, std::enable_if_t<is_type_visitor_v<T>, bool> = true>
		#endif
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		[[nodiscard]] ValueType get_type() const noexcept;
		[[nodiscard]] size_t get_length() const noexcept;

		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const vec_type& lhs, const vec_type& rhs) noexcept;
		friend bool operator!=(const vec_type& lhs, const vec_type& rhs) noexcept;
	};

	[[nodiscard]] bool operator==(const vec_type& lhs, const vec_type& rhs) noexcept;
	[[nodiscard]] bool operator!=(const vec_type& lhs, const vec_type& rhs) noexcept;
} // namespace glslstruct

template<>
struct std::hash<glslstruct::vec_type> {
	[[nodiscard]] size_t operator()(const glslstruct::vec_type& value) const noexcept;
};
	#endif
#endif