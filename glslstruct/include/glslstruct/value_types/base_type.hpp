/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/value_types/visitors/type_hash_visitor.hpp>

namespace glslstruct {
	class base_type;

	using base_type_handle = std::shared_ptr<base_type>;

	namespace utils {
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_type_v = std::is_base_of_v<base_type, T> && !std::is_same_v<base_type, T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool is_glsl_base_type_v = std::is_same_v<base_type, T>;

#if _GLSL_STRUCT_HAS_CXX20
		template<class T>
		concept glsl_type = is_glsl_type_v<T>;

		template<class T>
		concept glsl_base_type = is_glsl_base_type_v<T>;
#endif
	}

	class base_type {
	private:
		friend struct std::hash<base_type>;

		size_t _size;

	public:
		base_type(size_t size) noexcept;
		_GLSL_STRUCT_CONSTEXPR20 base_type(const base_type& other) noexcept = default;
		_GLSL_STRUCT_CONSTEXPR20 base_type(base_type&& other) noexcept = default;
		virtual _GLSL_STRUCT_CONSTEXPR20 ~base_type() noexcept = default;

		base_type& operator=(const base_type& other) noexcept = default;
		base_type& operator=(base_type&& other) noexcept = default;

		virtual void accept(base_type_visitor* const visitor) const = 0;

		[[nodiscard]] virtual bool operator==(const base_type& other) const noexcept;
		[[nodiscard]] bool operator!=(const base_type& other) const noexcept;

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, )
		friend _GLSL_STRUCT_CONSTEXPR20 bool operator==(const base_type& lhs, const T& rhs) noexcept;
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, )
		friend _GLSL_STRUCT_CONSTEXPR20 bool operator!=(const base_type& lhs, const T& rhs) noexcept;

		[[nodiscard]] size_t get_size() const noexcept;

		[[nodiscard]] virtual std::string to_string() const noexcept = 0;
	};

	[[nodiscard]] size_t sizeof_type(const base_type_handle& type) noexcept;
	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	[[nodiscard]] size_t sizeof_type(const std::shared_ptr<T>& type) noexcept {
		return type->get_size();
	}

	[[nodiscard]] std::string to_string(const base_type_handle& type) noexcept;
	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	[[nodiscard]] std::string to_string(const std::shared_ptr<T>& type) noexcept {
		return type->to_string();
	}

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 bool operator==(const base_type& lhs, const T& rhs) noexcept {
		return rhs == lhs;
	}
	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 bool operator!=(const base_type& lhs, const T& rhs) noexcept {
		return rhs != lhs;
	}
}

template<>
struct std::hash<glslstruct::base_type> {
	[[nodiscard]] size_t operator()(const glslstruct::base_type& value) noexcept;
};