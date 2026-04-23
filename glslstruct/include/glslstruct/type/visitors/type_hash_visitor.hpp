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
	class _GLSL_STRUCT_EXPORT type_hash_visitor {
	private:
		/// @brief hash result
		size_t _result = 0;

	public:
		/// @brief default constructor
		type_hash_visitor() noexcept;
		/// @brief default destructor
		~type_hash_visitor() noexcept;

		/// @brief scalar_type visitor
		void operator()(const scalar_type& value) noexcept;
		/// @brief vec_type visitor
		void operator()(const vec_type& value) noexcept;
		/// @brief mat_type visitor
		void operator()(const mat_type& value) noexcept;
		/// @brief struct_type visitor
		void operator()(const struct_type& value) noexcept;
		/// @brief array_type visitor
		void operator()(const array_type& value) noexcept;

		/// @brief returns hash result
		[[nodiscard]] size_t result() const noexcept;
	};
} // namespace glslstruct
	#endif
#endif