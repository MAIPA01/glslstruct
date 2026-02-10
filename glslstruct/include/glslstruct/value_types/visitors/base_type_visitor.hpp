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
#include <glslstruct/libraries.hpp>

namespace glslstruct {
	class scalar_type;
	class vec_type;
	class mat_type;
	class struct_type;
	class array_type;

	class base_type_visitor {
	public:
		_GLSL_STRUCT_CONSTEXPR20 base_type_visitor() noexcept = default;
		virtual _GLSL_STRUCT_CONSTEXPR20 ~base_type_visitor() noexcept = default;

		virtual void visit(const scalar_type& value) = 0;
		virtual void visit(const vec_type& value) = 0;
		virtual void visit(const mat_type& value) = 0;
		virtual void visit(const struct_type& value) = 0;
		virtual void visit(const array_type& value) = 0;
	};
}