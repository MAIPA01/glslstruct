/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_TYPE_HASH_VISITOR_HPP_
	#define _GLSL_STRUCT_TYPE_HASH_VISITOR_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1!"
);
	#else

		#include <glslstruct/types.hpp>

namespace glslstruct {
	/**
	 * @brief visitor which calculate has of type
	 * @ingroup type_visitors
	 */
	class type_hash_visitor {
	private:
		/// @brief hash result
		size_t _result = 0;

	public:
		/// @brief default constructor
		type_hash_visitor() noexcept;
		/// @brief default destructor
		~type_hash_visitor() noexcept;

		/// @brief scalar_type visitor
		void visit(const scalar_type& value);
		/// @brief vec_type visitor
		void visit(const vec_type& value);
		/// @brief mat_type visitor
		void visit(const mat_type& value);
		/// @brief struct_type visitor
		void visit(const struct_type& value);
		/// @brief array_type visitor
		void visit(const array_type& value);

		/// @brief returns hash result
		[[nodiscard]] size_t result() const noexcept;
	};
} // namespace glslstruct
	#endif
#endif