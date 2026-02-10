/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/value_types/base_type.hpp>
#include <glslstruct/value_types/visitors/eq_type_visitor.hpp>
#include <glslstruct/value_types/ValueType.hpp>

namespace glslstruct {
	template<class _Derived>
	class value_type : public base_type {
	public:
		using base_type::operator!=;

		_GLSL_STRUCT_CONSTEXPR20 value_type(size_t size) noexcept 
			: base_type(size) {}
		_GLSL_STRUCT_CONSTEXPR20 value_type(const value_type& other) noexcept = default;
		_GLSL_STRUCT_CONSTEXPR20 value_type(value_type&& other) noexcept = default;
		virtual _GLSL_STRUCT_CONSTEXPR20 ~value_type() noexcept = default;

		_GLSL_STRUCT_CONSTEXPR17 value_type& operator=(const value_type& other) noexcept = default;
		_GLSL_STRUCT_CONSTEXPR17 value_type& operator=(value_type&& other) noexcept = default;

		virtual void accept(base_type_visitor* const visitor) const override {
			visitor->visit(*static_cast<const _Derived*>(this));
		}

		[[nodiscard]] bool operator==(const base_type& other) const noexcept override {
			if (!base_type::operator==(other)) {
				return false;
			}

			eq_type_visitor<_Derived> visitor(static_cast<const _Derived*>(this));
			other.accept(&visitor);
			return visitor.result();
		}

		template<class _DerivedA, class _DerivedB>
		friend _GLSL_STRUCT_CONSTEXPR17 bool operator==(const value_type<_DerivedA>& lhs, const value_type<_DerivedB>& rhs) noexcept;
		template<class _DerivedA, class _DerivedB>
		friend _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const value_type<_DerivedA>& lhs, const value_type<_DerivedB>& rhs) noexcept;
	};

	template<class _Derived, class _ODerived>
	[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator==(const value_type<_Derived>& lhs, const value_type<_ODerived>& rhs) noexcept {
		if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<_Derived, _ODerived>) {
			return *static_cast<const _Derived*>(&lhs) == *static_cast<const _ODerived*>(&rhs);
		}
		else {
			return false;
		}
	}
	template<class _Derived, class _ODerived>
	[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const value_type<_Derived>& lhs, const value_type<_ODerived>& rhs) noexcept {
		return !(lhs == rhs);
	}
}