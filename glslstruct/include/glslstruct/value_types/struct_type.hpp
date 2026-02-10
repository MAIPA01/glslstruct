/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/value_data.hpp>
#include <glslstruct/value_types/value_type.hpp>

namespace glslstruct {
	class struct_type;

	using struct_type_handle = std::shared_ptr<struct_type>;

	class struct_type : public value_type<struct_type> {
	private:
		friend struct std::hash<struct_type>;

		using _base_class = value_type<struct_type>;

		std::unordered_map<std::string, value_data> _values;

	public:
		struct_type(const std::unordered_map<std::string, value_data>& values, size_t size) noexcept;
		struct_type(const struct_type& other) noexcept = default;
		struct_type(struct_type&& other) noexcept;
		virtual ~struct_type() noexcept = default;

		struct_type& operator=(const struct_type& other) noexcept = default;
		struct_type& operator=(struct_type&& other) noexcept;

		void accept(base_type_visitor* const visitor) const override;

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
	[[nodiscard]] size_t operator()(const glslstruct::struct_type& value) noexcept;
};