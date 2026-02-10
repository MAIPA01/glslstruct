/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/value_types/visitors/base_type_visitor.hpp>

namespace glslstruct {
	class type_hash_visitor : public base_type_visitor {
	private:
		size_t _result = 0;

	public:
		_GLSL_STRUCT_CONSTEXPR20 type_hash_visitor() noexcept = default;
		virtual _GLSL_STRUCT_CONSTEXPR20 ~type_hash_visitor() noexcept = default;

		void visit(const scalar_type& value) override;
		void visit(const vec_type& value) override;
		void visit(const mat_type& value) override;
		void visit(const struct_type& value) override;
		void visit(const array_type& value) override;

		[[nodiscard]] size_t result() const noexcept;
	};
}