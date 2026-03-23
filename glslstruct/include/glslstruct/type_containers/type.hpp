/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_GLSL_TYPE_HPP_
	#define _GLSL_STRUCT_GLSL_TYPE_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with defined GLSL_STRUCT_DISABLE_TYPES!"
);
	#else

		#include <glslstruct/type_containers/base_type.hpp>
		#include <glslstruct/type_visitors/eq_type_visitor.hpp>

namespace glslstruct {
	template<class Derived>
	class type : public base_type {
	private:
		_GLSL_STRUCT_CONSTEXPR17 const Derived& _get_derived() const noexcept { return *static_cast<const Derived*>(this); }

	protected:
		void _accept(const mstd::function_view<void(const scalar_type&)> scalar_visit,
		  const mstd::function_view<void(const vec_type&)> vec_visit, const mstd::function_view<void(const mat_type&)> mat_visit,
		  const mstd::function_view<void(const struct_type&)> struct_visit,
		  const mstd::function_view<void(const array_type&)> array_visit) const override {
				if _MSTD_CONSTEXPR17 (std::is_same_v<Derived, scalar_type>) { scalar_visit(_get_derived()); }
				else if _MSTD_CONSTEXPR17 (std::is_same_v<Derived, vec_type>) { vec_visit(_get_derived()); }
				else if _MSTD_CONSTEXPR17 (std::is_same_v<Derived, mat_type>) { mat_visit(_get_derived()); }
				else if _MSTD_CONSTEXPR17 (std::is_same_v<Derived, struct_type>) { struct_visit(_get_derived()); }
				else if _MSTD_CONSTEXPR17 (std::is_same_v<Derived, array_type>) { array_visit(_get_derived()); }
		}

	public:
		using base_type::operator!=;

		explicit _GLSL_STRUCT_CONSTEXPR20 type(size_t size) noexcept : base_type(size) {}

		_GLSL_STRUCT_CONSTEXPR20 type(const type& other) noexcept			 = default;
		_GLSL_STRUCT_CONSTEXPR20 type(type&& other) noexcept				 = default;
		virtual _GLSL_STRUCT_CONSTEXPR20 ~type() noexcept override			 = default;

		_GLSL_STRUCT_CONSTEXPR17 type& operator=(const type& other) noexcept = default;
		_GLSL_STRUCT_CONSTEXPR17 type& operator=(type&& other) noexcept		 = default;

		#if _GLSL_STRUCT_HAS_CXX20
		template<type_visitor T>
		#else
		template<class T, std::enable_if_t<is_type_visitor_v<T>, bool> = true>
		#endif
		void accept(T& visitor) const {
			visitor.visit(_get_derived());
		}

		[[nodiscard]] bool operator==(const base_type& other) const noexcept override {
				if (!base_type::operator==(other)) { return false; }

			eq_type_visitor<Derived> visitor(&_get_derived());
			other.accept(visitor);
			return visitor.result();
		}

		template<class DerivedA, class DerivedB>
		friend _GLSL_STRUCT_CONSTEXPR17 bool operator==(const type<DerivedA>& lhs, const type<DerivedB>& rhs) noexcept;
		template<class DerivedA, class DerivedB>
		friend _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const type<DerivedA>& lhs, const type<DerivedB>& rhs) noexcept;
	};

	template<class DerivedA, class DerivedB>
	[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator==(const type<DerivedA>& lhs, const type<DerivedB>& rhs) noexcept {
			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<DerivedA, DerivedB>) { return lhs._get_derived() == rhs._get_derived(); }
			else { return false; }
	}

	template<class DerivedA, class DerivedB>
	[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const type<DerivedA>& lhs, const type<DerivedB>& rhs) noexcept {
		return !(lhs == rhs);
	}
} // namespace glslstruct
	#endif
#endif