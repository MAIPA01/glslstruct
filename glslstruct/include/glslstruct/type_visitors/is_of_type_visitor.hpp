/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_IS_OF_TYPE_VISITOR_HPP_
	#define _GLSL_STRUCT_IS_OF_TYPE_VISITOR_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1!"
);
	#else

		#include <glslstruct/types.hpp>
		#include <glslstruct/utils/assert.hpp>

namespace glslstruct {
	// clang-format off
	ENUM_CLASS_BASE(BaseType, uint8_t, (Scalar), (Vec), (Mat), (Struct), (Array))
		// clang-format on

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	class is_of_type_visitor {
	private:
		bool _result = false;

	public:
		_GLSL_STRUCT_CONSTEXPR20 is_of_type_visitor() noexcept	= default;
		_GLSL_STRUCT_CONSTEXPR20 ~is_of_type_visitor() noexcept = default;

		void visit(const scalar_type&) { _result = std::is_same_v<T, scalar_type>; }

		void visit(const vec_type&) { _result = std::is_same_v<T, vec_type>; }

		void visit(const mat_type&) { _result = std::is_same_v<T, mat_type>; }

		void visit(const struct_type&) { _result = std::is_same_v<T, struct_type>; }

		void visit(const array_type&) { _result = std::is_same_v<T, array_type>; }

		[[nodiscard]] bool result() const noexcept { return _result; }
	};

		#pragma region IS_OF_TYPE

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] inline _GLSL_STRUCT_CONSTEXPR20 bool is_of_type(const base_type_handle& type) {
		is_of_type_visitor<T> visitor;
		type->accept(visitor);
		return visitor.result();
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] inline _GLSL_STRUCT_CONSTEXPR20 bool is_of_type(const std::shared_ptr<T>&) {
		return true;
	}

	[[nodiscard]] inline _GLSL_STRUCT_CONSTEXPR20 bool is_of_type(const base_type_handle& type, BaseType baseType) {
			switch (baseType) {
			case BaseType::Scalar: return is_of_type<scalar_type>(type);
			case BaseType::Vec:	   return is_of_type<vec_type>(type);
			case BaseType::Mat:	   return is_of_type<mat_type>(type);
			case BaseType::Struct: return is_of_type<struct_type>(type);
			case BaseType::Array:  return is_of_type<array_type>(type);
			default:			   return false;
			}
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] inline _GLSL_STRUCT_CONSTEXPR20 bool is_of_type(const std::shared_ptr<T>&, BaseType baseType) {
			switch (baseType) {
			case BaseType::Scalar: return std::is_same_v<T, scalar_type>;
			case BaseType::Vec:	   return std::is_same_v<T, vec_type>;
			case BaseType::Mat:	   return std::is_same_v<T, mat_type>;
			case BaseType::Struct: return std::is_same_v<T, struct_type>;
			case BaseType::Array:  return std::is_same_v<T, array_type>;
			default:			   return false;
			}
	}

		#pragma endregion

		#pragma region DYNAMIC_CAST

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] inline std::shared_ptr<T> dynamic_type_cast(const base_type_handle& type) {
			if (is_of_type<T>(type)) { return std::dynamic_pointer_cast<T>(type); }
		return nullptr;
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] inline base_type_handle dynamic_type_cast(const std::shared_ptr<T>& type) {
		return std::dynamic_pointer_cast<base_type>(type);
	}

		#pragma endregion

		#pragma region STATIC_CAST

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] inline _GLSL_STRUCT_CONSTEXPR20 std::shared_ptr<T> static_type_cast(const base_type_handle& type) {
		std::shared_ptr<T> result = dynamic_type_cast<T>(type);
		glsl_struct_assert(result != nullptr, "Cannot convert type to desired type!");
		return result;
	}

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] inline base_type_handle static_type_cast(const std::shared_ptr<T>& type) {
		return dynamic_type_cast(type);
	}

		#pragma endregion
} // namespace glslstruct
	#endif
#endif