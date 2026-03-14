/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1!");
#else

#include <glslstruct/value_types/data/value_data.hpp>
#include <glslstruct/value_types/types/value_type.hpp>

namespace glslstruct {
	class struct_type : public value_type<struct_type> {
	private:
		friend struct std::hash<struct_type>;

		std::unordered_map<std::string, value_data> _values;

	public:
		struct_type(const std::unordered_map<std::string, value_data>& values, size_t size) noexcept;
		struct_type(const struct_type& other) noexcept;
		struct_type(struct_type&& other) noexcept;
		~struct_type() noexcept override;

		struct_type& operator=(const struct_type& other) noexcept;
		struct_type& operator=(struct_type&& other) noexcept;

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, type_visitor, is_type_visitor_v<T>, = true)
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		[[nodiscard]] const std::unordered_map<std::string, value_data>& get_values() const noexcept;

		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const struct_type& lhs, const struct_type& rhs) noexcept;
		friend bool operator!=(const struct_type& lhs, const struct_type& rhs) noexcept;
	};

	[[nodiscard]] bool operator==(const struct_type& lhs, const struct_type& rhs) noexcept;
	[[nodiscard]] bool operator!=(const struct_type& lhs, const struct_type& rhs) noexcept;
}

template<>
struct std::hash<glslstruct::struct_type> {
	[[nodiscard]] size_t operator()(const glslstruct::struct_type& type) const noexcept;
};
#endif