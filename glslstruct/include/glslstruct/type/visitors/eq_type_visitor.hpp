/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_EQ_TYPE_VISITOR_HPP_
	#define _GLSL_STRUCT_EQ_TYPE_VISITOR_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1!"
);
	#else

		#include <glslstruct/types.hpp>

namespace glslstruct {
		/**
		 * @brief visitor which checks if value glsl type A is equal to glsl type B
		 * @ingroup type_visitors
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	class _GLSL_STRUCT_EXPORT eq_type_visitor {
	private:
		/// @brief pointer to value A
		const T* _value;
		/// @brief result value
		bool _result = false;

	public:
		/// @brief constructor with pointer to value A
		explicit _GLSL_STRUCT_CONSTEXPR20 eq_type_visitor(const T* value) noexcept : _value(value) {}
		/// @brief default destructor
		_GLSL_STRUCT_CONSTEXPR20 ~eq_type_visitor() noexcept = default;

		/// @brief type visitor
		template<class Type>
		void operator()(Type&& typeContainer) noexcept {
			using type = std::decay_t<Type>;

			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, type>) {
				_result = *_value == typeContainer;
			}
			else {
				_result = false;
			}
		}

		/// @brief returns result value
		[[nodiscard]] bool result() const noexcept { return _result; }
	};
} // namespace glslstruct
	#endif
#endif