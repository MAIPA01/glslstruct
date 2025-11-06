#pragma once
#include <type_traits>
#include <glslstruct/value_types/base_type.hpp>
#include <glslstruct/value_types/type_eq_visitor.hpp>

namespace glslstruct {
	template<class _Derived>
	class value_type : public base_type {
	protected:
		value_type() = default;

	public:
		value_type(const value_type<_Derived>& other) = default;
		virtual ~value_type() = default;

		[[nodiscard]] virtual base_type* clone() const noexcept = 0;
		[[nodiscard]] virtual void accept(base_type_visitor* const visitor) const override {
			visitor->visit(*static_cast<const _Derived*>(this));
		}

		[[nodiscard]] bool operator==(const base_type& other) const noexcept override {
			return other == *static_cast<const _Derived*>(this);
		}
		[[nodiscard]] bool operator!=(const base_type& other) const noexcept override {
			return other != *static_cast<const _Derived*>(this);
		}

		[[nodiscard]] bool operator==(const scalar_type& other) const noexcept override {
			type_eq_visitor<_Derived> eq_visitor = type_eq_visitor<_Derived>(static_cast<const _Derived*>(this));
			accept(&eq_visitor);
			return eq_visitor.result();
		}
		[[nodiscard]] bool operator!=(const scalar_type& other) const noexcept override {
			return !(*this == other);
		}

		[[nodiscard]] bool operator==(const vec_type& other) const noexcept override {
			type_eq_visitor<_Derived> eq_visitor = type_eq_visitor<_Derived>(static_cast<const _Derived*>(this));
			accept(&eq_visitor);
			return eq_visitor.result();
		}
		[[nodiscard]] bool operator!=(const vec_type& other) const noexcept override {
			return !(*this == other);
		}

		[[nodiscard]] bool operator==(const mat_type& other) const noexcept override {
			type_eq_visitor<_Derived> eq_visitor = type_eq_visitor<_Derived>(static_cast<const _Derived*>(this));
			accept(&eq_visitor);
			return eq_visitor.result();
		}
		[[nodiscard]] bool operator!=(const mat_type& other) const noexcept override {
			return !(*this == other);
		}

		[[nodiscard]] bool operator==(const struct_type& other) const noexcept override {
			type_eq_visitor<_Derived> eq_visitor = type_eq_visitor<_Derived>(static_cast<const _Derived*>(this));
			accept(&eq_visitor);
			return eq_visitor.result();
		}
		[[nodiscard]] bool operator!=(const struct_type& other) const noexcept override {
			return !(*this == other);
		}

		[[nodiscard]] bool operator==(const array_type& other) const noexcept override {
			type_eq_visitor<_Derived> eq_visitor = type_eq_visitor<_Derived>(static_cast<const _Derived*>(this));
			accept(&eq_visitor);
			return eq_visitor.result();
		}
		[[nodiscard]] bool operator!=(const array_type& other) const noexcept override {
			return !(*this == other);
		}
	};
}