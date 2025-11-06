#pragma once
#include <type_traits>
#include <glslstruct/value_types/base_type.hpp>

namespace glslstruct {
	template<class _Derived>
	class value_type : public base_type {
	protected:
		value_type() = default;

	public:
		value_type(const value_type<_Derived>& other) = default;
		virtual ~value_type() = default;

		[[nodiscard]] virtual base_type* clone() const noexcept = 0;

		[[nodiscard]] constexpr bool visit_equal(const base_type* other) const noexcept override {
			const _Derived* this_derived = static_cast<const _Derived*>(this);
			return other->visit_equal(this_derived);
		}
		[[nodiscard]] bool operator==(const base_type& other) const noexcept override {
			return visit_equal(&other);
		}
		[[nodiscard]] bool operator!=(const base_type& other) const noexcept override {
			return *this != other;
		}

		[[nodiscard]] constexpr bool visit_equal(const scalar_type* other) const noexcept override {
			if constexpr (!std::is_same_v<_Derived, scalar_type>) {
				return false;
			}
			else {
				const _Derived* this_derived = static_cast<const _Derived*>(this);
				return *this_derived == *other;
			}
		}
		[[nodiscard]] bool operator==(const scalar_type& other) const noexcept override {
			return visit_equal(&other);
		}
		[[nodiscard]] bool operator!=(const scalar_type& other) const noexcept override {
			return *this != other;
		}

		[[nodiscard]] constexpr bool visit_equal(const vec_type* other) const noexcept override {
			if constexpr (!std::is_same_v<_Derived, vec_type>) {
				return false;
			}
			else {
				const _Derived* this_derived = static_cast<const _Derived*>(this);
				return *this_derived == *other;
			}
		}
		[[nodiscard]] bool operator==(const vec_type& other) const noexcept override {
			return visit_equal(&other);
		}
		[[nodiscard]] bool operator!=(const vec_type& other) const noexcept override {
			return *this != other;
		}

		[[nodiscard]] constexpr bool visit_equal(const mat_type* other) const noexcept override {
			if constexpr (!std::is_same_v<_Derived, mat_type>) {
				return false;
			}
			else {
				const _Derived* this_derived = static_cast<const _Derived*>(this);
				return *this_derived == *other;
			}
		}
		[[nodiscard]] bool operator==(const mat_type& other) const noexcept override {
			return visit_equal(&other);
		}
		[[nodiscard]] bool operator!=(const mat_type& other) const noexcept override {
			return *this != other;
		}

		[[nodiscard]] constexpr bool visit_equal(const struct_type* other) const noexcept override {
			if constexpr (!std::is_same_v<_Derived, struct_type>) {
				return false;
			}
			else {
				const _Derived* this_derived = static_cast<const _Derived*>(this);
				return *this_derived == *other;
			}
		}
		[[nodiscard]] bool operator==(const struct_type& other) const noexcept override {
			return visit_equal(&other);
		}
		[[nodiscard]] bool operator!=(const struct_type& other) const noexcept override {
			return *this != other;
		}

		[[nodiscard]] constexpr bool visit_equal(const array_type* other) const noexcept override {
			if constexpr (!std::is_same_v<_Derived, array_type>) {
				return false;
			}
			else {
				const _Derived* this_derived = static_cast<const _Derived*>(this);
				return *this_derived == *other;
			}
		}
		[[nodiscard]] bool operator==(const array_type& other) const noexcept override {
			return visit_equal(&other);
		}
		[[nodiscard]] bool operator!=(const array_type& other) const noexcept override {
			return *this != other;
		}
	};
}