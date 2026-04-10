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
	/**
	 * @brief base_type types in enum values
	 * @ingroup type_visitors
	 */
	ENUM_CLASS_BASE(BaseType, uint8_t, (Scalar), (Vec), (Mat), (Struct), (Array))
		// clang-format on

		/**
		 * @brief visitor which checks type of glsl type
		 * @ingroup type_visitors
		 * @tparam T requested type
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	class is_of_type_visitor {
	private:
		/// @brief result
		bool _result = false;

	public:
		/// @brief default constructor
		_GLSL_STRUCT_CONSTEXPR20 is_of_type_visitor() noexcept	= default;
		/// @brief default destructor
		_GLSL_STRUCT_CONSTEXPR20 ~is_of_type_visitor() noexcept = default;

		/// @brief scalar_type visitor
		void visit(const scalar_type&) { _result = std::is_same_v<T, scalar_type>; }

		/// @brief vec_type visitor
		void visit(const vec_type&) { _result = std::is_same_v<T, vec_type>; }

		/// @brief mat_type visitor
		void visit(const mat_type&) { _result = std::is_same_v<T, mat_type>; }

		/// @brief struct_type visitor
		void visit(const struct_type&) { _result = std::is_same_v<T, struct_type>; }

		/// @brief array_type visitor
		void visit(const array_type&) { _result = std::is_same_v<T, array_type>; }

		/// @brief returns result
		[[nodiscard]] bool result() const noexcept { return _result; }
	};

		#pragma region IS_OF_TYPE

		/**
		 * @brief requests is_of_type visitor to visit base_type
		 * @ingroup type_visitors
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] _GLSL_STRUCT_INLINE17 _GLSL_STRUCT_CONSTEXPR20 bool is_of_type(const base_type_handle& type) {
		is_of_type_visitor<T> visitor;
		type->accept(visitor);
		return visitor.result();
	}

		/**
		 * @brief requests is_of_type visitor to visit any glsl type
		 * @ingroup type_visitors
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] _GLSL_STRUCT_INLINE17 _GLSL_STRUCT_CONSTEXPR20 bool is_of_type(const std::shared_ptr<T>&) {
		return true;
	}

	/**
	 * @brief requests is_of_type visitor to visit base_type
	 * @ingroup type_visitors
	 * @param type base_type handle
	 * @param baseType type which is required
	 */
	[[nodiscard]] bool is_of_type(const base_type_handle& type, BaseType baseType);

		/**
		 * @brief requests is_of_type visitor to visit any glsl type
		 * @ingroup type_visitors
		 * @param baseType type which is required
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] _GLSL_STRUCT_INLINE17 _GLSL_STRUCT_CONSTEXPR20 bool is_of_type(const std::shared_ptr<T>&, const BaseType baseType) {
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

		/**
		 * @brief checks if base_type is of given type and returns handle to this type or nullptr if it is not
		 * @ingroup type_visitors
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] inline std::shared_ptr<T> dynamic_type_cast(const base_type_handle& type) {
			if (is_of_type<T>(type)) { return std::dynamic_pointer_cast<T>(type); }
		return nullptr;
	}

		/**
		 * @brief converts any glsl type handle to base_type handle
		 * @ingroup type_visitors
		 */
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

		/**
		 * @brief checks if base_type is of given type and returns handle to this type or error if it is not
		 * @ingroup type_visitors
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] _GLSL_STRUCT_INLINE17 _GLSL_STRUCT_CONSTEXPR20 std::shared_ptr<T> static_type_cast(const base_type_handle& type) {
		std::shared_ptr<T> result = dynamic_type_cast<T>(type);
		glsl_struct_assert(result != nullptr, "Cannot convert type to desired type!");
		return result;
	}

		/**
		 * @brief converts any glsl type handle to base_type handle
		 * @ingroup type_visitors
		 */
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