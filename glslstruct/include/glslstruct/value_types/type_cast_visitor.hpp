#pragma once
#include <cassert>
#include <glslstruct/value_types/base_type_visitor.hpp>
#include <glslstruct/value_types/base_type.hpp>

namespace glslstruct {
	template<class T>
	class type_cast_visitor : public base_type_visitor {
	private:
		const T* _result = nullptr;

	public:
		type_cast_visitor() = default;
		virtual ~type_cast_visitor() = default;

		void visit(const scalar_type& value) {
			if constexpr (!std::is_same_v<T, scalar_type>) {
				_result = nullptr;
			}
			else {
				_result = &value;
			}
		}
		void visit(const vec_type& value) {
			if constexpr (!std::is_same_v<T, vec_type>) {
				_result = nullptr;
			}
			else {
				_result = &value;
			}
		}
		void visit(const mat_type& value) {
			if constexpr (!std::is_same_v<T, mat_type>) {
				_result = nullptr;
			}
			else {
				_result = &value;
			}
		}
		void visit(const struct_type& value) {
			if constexpr (!std::is_same_v<T, struct_type>) {
				_result = nullptr;
			}
			else {
				_result = &value;
			}
		}
		void visit(const array_type& value) {
			if constexpr (!std::is_same_v<T, array_type>) {
				_result = nullptr;
			}
			else {
				_result = &value;
			}
		}

		const T* result() const noexcept {
			return _result;
		}
	};

	template<class T>
	static constexpr const T* dynamic_type_cast(const base_type* type) {
		type_cast_visitor<T> visitor;
		type->accept(&visitor);
		return visitor.result();
	}

	template<class T>
	static constexpr const T* static_type_cast(const base_type* type) {
		const T* result = dynamic_type_cast<T>(type);
		assert(result != nullptr && "cannot convert type to desired type");
		return result;
	}
}