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

#include <glslstruct/value_types/types/value_type.hpp>
#include <glslstruct/value_types/ValueType.hpp>

namespace glslstruct {
	class scalar_type : public value_type<scalar_type> {
	private:
		friend struct std::hash<scalar_type>;

		ValueType _type = ValueType::Other;

	public:
		explicit scalar_type(ValueType type) noexcept;
		scalar_type(const scalar_type& other) noexcept;
		scalar_type(scalar_type&& other) noexcept;
		~scalar_type() noexcept override;

		scalar_type& operator=(const scalar_type& other) noexcept;
		scalar_type& operator=(scalar_type&& other) noexcept;

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, type_visitor, is_type_visitor_v<T>, = true)
		void accept(T& visitor) const {
			visitor.visit(*this);
		}

		[[nodiscard]] ValueType get_type() const noexcept;

		[[nodiscard]] std::string to_string() const noexcept override;

		friend bool operator==(const scalar_type& lhs, const scalar_type& rhs) noexcept;
		friend bool operator!=(const scalar_type& lhs, const scalar_type& rhs) noexcept;
	};

	[[nodiscard]] bool operator==(const scalar_type& lhs, const scalar_type& rhs) noexcept;
	[[nodiscard]] bool operator!=(const scalar_type& lhs, const scalar_type& rhs) noexcept;
}

template<>
struct std::hash<glslstruct::scalar_type> {
	[[nodiscard]] size_t operator()(const glslstruct::scalar_type& value) const noexcept;
};
#endif