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
#include <glslstruct/value_types/scalar_type.hpp>
#include <glslstruct/value_types/vec_type.hpp>
#include <glslstruct/value_types/mat_type.hpp>
#include <glslstruct/value_types/struct_type.hpp>
#include <glslstruct/value_types/array_type.hpp>

namespace glslstruct {
	ENUM_CLASS_BASE(BaseType, uint8_t,
		(Scalar),
		(Vec),
		(Mat),
		(Struct),
		(Array)
	)

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	class is_of_type_visitor : public base_type_visitor {
	private:
		bool _result = false;

	public:
		_GLSL_STRUCT_CONSTEXPR20 is_of_type_visitor() noexcept = default;
		virtual _GLSL_STRUCT_CONSTEXPR20 ~is_of_type_visitor() noexcept = default;

		void visit(const scalar_type&) override {
			_result = std::is_same_v<T, scalar_type>;
		}
		void visit(const vec_type&) override {
			_result = std::is_same_v<T, vec_type>;
		}
		void visit(const mat_type&) override {
			_result = std::is_same_v<T, mat_type>;
		}
		void visit(const struct_type&) override {
			_result = std::is_same_v<T, struct_type>;
		}
		void visit(const array_type&) override {
			_result = std::is_same_v<T, array_type>;
		}

		[[nodiscard]] bool result() const noexcept {
			return _result;
		}
	};

#pragma region IS_OF_TYPE

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 bool is_of_type(const base_type_handle& type) {
		is_of_type_visitor<T> visitor;
		type->accept(&visitor);
		return visitor.result();
	}

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 bool is_of_type(const std::shared_ptr<T>&) {
		return true;
	}

	[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 bool is_of_type(const base_type_handle& type, BaseType baseType) {
		switch (baseType)
		{
		case BaseType::Scalar:
			return is_of_type<scalar_type>(type);
		case BaseType::Vec:
			return is_of_type<vec_type>(type);
		case BaseType::Mat:
			return is_of_type<mat_type>(type);
		case BaseType::Struct:
			return is_of_type<struct_type>(type);
		case BaseType::Array:
			return is_of_type<array_type>(type);
		default:
			return false;
		}
	}

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 bool is_of_type(const std::shared_ptr<T>&, BaseType baseType) {
		switch (baseType)
		{
		case BaseType::Scalar:
			return std::is_same_v<T, scalar_type>;
		case BaseType::Vec:
			return std::is_same_v<T, vec_type>;
		case BaseType::Mat:
			return std::is_same_v<T, mat_type>;
		case BaseType::Struct:
			return std::is_same_v<T, struct_type>;
		case BaseType::Array:
			return std::is_same_v<T, array_type>;
		default:
			return false;
		}
	}

#pragma endregion

#pragma region DYNAMIC_CAST

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	[[nodiscard]] static std::shared_ptr<T> dynamic_type_cast(const base_type_handle& type) {
		if (is_of_type<T>(type)) {
			return std::dynamic_pointer_cast<T>(type);
		}
		else {
			return nullptr;
		}
	}

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	[[nodiscard]] static base_type_handle dynamic_type_cast(const std::shared_ptr<T>& type) {
		return std::dynamic_pointer_cast<base_type>(type);
	}

#pragma endregion

#pragma region STATIC_CAST

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::shared_ptr<T> static_type_cast(const base_type_handle& type) {
		std::shared_ptr<T> result = dynamic_type_cast<T>(type);
		assert(result != nullptr && "cannot convert type to desired type");
		return result;
	}

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
	[[nodiscard]] static base_type_handle static_type_cast(const std::shared_ptr<T>& type) {
		return dynamic_type_cast(type);
	}

#pragma endregion
}