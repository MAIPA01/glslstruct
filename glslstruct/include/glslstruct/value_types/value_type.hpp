#pragma once
#include <type_traits>
#include <glslstruct/value_types/base_type.hpp>

namespace glslstruct {
	template<class _Derived>
	class value_type : base_type {
	protected:
		value_type() = default;

	public:
		value_type(const value_type<_Derived>& other) = default;
		virtual ~value_type() = default;

		[[nodiscard]] virtual base_type* clone() const noexcept = 0;

		[[nodiscard]] constexpr bool visit_equal(const base_type*& other) const noexcept override {
			_Derived* this_derived = static_cast<_Derived*>(this);
			return other->visit_equal(this_derived);
		}
		[[nodiscard]] constexpr bool visit_equal(const scalar_type*& other) const noexcept override {
			if constexpr (!std::is_same_v<_Derived, scalar_type>) {
				return false;
			}
			else {
				_Derived* this_derived = static_cast<_Derived*>(this);
				return *this_derived == *other;
			}
		}
		[[nodiscard]] constexpr bool visit_equal(const vec_type*& other) const noexcept override {
			if constexpr (!std::is_same_v<_Derived, vec_type>) {
				return false;
			}
			else {
				_Derived* this_derived = static_cast<_Derived*>(this);
				return *this_derived == *other;
			}
		}
		[[nodiscard]] constexpr bool visit_equal(const mat_type*& other) const noexcept override {
			if constexpr (!std::is_same_v<_Derived, mat_type>) {
				return false;
			}
			else {
				_Derived* this_derived = static_cast<_Derived*>(this);
				return *this_derived == *other;
			}
		}
		[[nodiscard]] constexpr bool visit_equal(const struct_type*& other) const noexcept override {
			if constexpr (!std::is_same_v<_Derived, struct_type>) {
				return false;
			}
			else {
				_Derived* this_derived = static_cast<_Derived*>(this);
				return *this_derived == *other;
			}
		}
		[[nodiscard]] constexpr bool visit_equal(const array_type*& other) const noexcept override {
			if constexpr (!std::is_same_v<_Derived, array_type>) {
				return false;
			}
			else {
				_Derived* this_derived = static_cast<_Derived*>(this);
				return *this_derived == *other;
			}
		}

		[[nodiscard]] bool operator==(const base_type* other) const noexcept {
			return visit_equal(other);
		}
		[[nodiscard]] bool operator!=(const base_type* other) const noexcept {
			return *this != other;
		}
	};
}