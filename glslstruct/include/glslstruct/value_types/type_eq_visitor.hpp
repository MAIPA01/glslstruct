#pragma once
#include <glslstruct/value_types/base_type_visitor.hpp>

namespace glslstruct {
	template<class T>
	class type_eq_visitor : public base_type_visitor {
	private:
		const T* const _value;
		bool _result = false;

	public:
		type_eq_visitor(const T* const value) : _value(value) {}
		virtual ~type_eq_visitor() = default;

		void visit(const scalar_type& other) noexcept override {
			if constexpr (!std::is_same_v<T, scalar_type>) {
				_result = false;
			}
			else {
				_result = (*_value == other);
			}
		}
		void visit(const vec_type& other) noexcept override {
			if constexpr (!std::is_same_v<T, vec_type>) {
				_result = false;
			}
			else {
				_result = (*_value == other);
			}
		}
		void visit(const mat_type& other) noexcept override {
			if constexpr (!std::is_same_v<T, mat_type>) {
				_result = false;
			}
			else {
				_result = (*_value == other);
			}
		}
		void visit(const struct_type& other) noexcept override {
			if constexpr (!std::is_same_v<T, struct_type>) {
				_result = false;
			}
			else {
				_result = (*_value == other);
			}
		}
		void visit(const array_type& other) noexcept override {
			if constexpr (!std::is_same_v<T, array_type>) {
				_result = false;
			}
			else {
				_result = (*_value == other);
			}
		}

		constexpr bool result() const noexcept {
			return _result;
		}
	};
}