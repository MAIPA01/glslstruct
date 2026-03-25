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

		#include <glslstruct/type_containers/type.hpp>
		#include <glslstruct/var_data/var_data.hpp>

namespace glslstruct {
	class array_type : public type<array_type> {
	private:
		friend struct std::hash<array_type>;

		using base_type		   = type;

		base_type_handle _type = nullptr;
		size_t _count		   = 0;

	public:
		array_type(ValueType type, size_t scalarSize, size_t count, size_t size) noexcept;
		array_type(ValueType type, size_t length, size_t vecSize, size_t count, size_t size) noexcept;
		array_type(ValueType type, size_t cols, size_t rows, size_t matSize, size_t count, size_t size) noexcept;
		array_type(const std::unordered_map<std::string, var_data>& values, size_t structSize, size_t count,
		  size_t size) noexcept;
		array_type(const base_type_handle& type, size_t count, size_t size) noexcept;
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_type T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 array_type(const std::shared_ptr<T>& type, size_t count, size_t size) noexcept
			: array_type(std::dynamic_pointer_cast<base_type>(type), count, size) {
		}

		array_type(const array_type& other) noexcept;
		array_type(array_type&& other) noexcept;
		~array_type() noexcept override;

		array_type& operator=(const array_type& other) noexcept;
		array_type& operator=(array_type&& other) noexcept;

		#if _GLSL_STRUCT_HAS_CXX20
		template<type_visitor T>
		#else
		template<class T, std::enable_if_t<is_type_visitor_v<T>, bool> = true>
		#endif
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		[[nodiscard]] const base_type_handle& get_type() const noexcept;

		[[nodiscard]] size_t get_count() const noexcept;

		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const array_type& lhs, const array_type& rhs) noexcept;
		friend bool operator!=(const array_type& lhs, const array_type& rhs) noexcept;
	};

	[[nodiscard]] bool operator==(const array_type& lhs, const array_type& rhs) noexcept;
	[[nodiscard]] bool operator!=(const array_type& lhs, const array_type& rhs) noexcept;
} // namespace glslstruct

template<>
struct std::hash<glslstruct::array_type> {
	[[nodiscard]] size_t operator()(const glslstruct::array_type& type) const noexcept;
};
	#endif
#endif