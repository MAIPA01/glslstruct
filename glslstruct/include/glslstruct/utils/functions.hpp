/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_FUNCTIONS_HPP_
	#define _GLSL_STRUCT_FUNCTIONS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

namespace glslstruct {
	/**
	 * @brief returns value rounded up to nearest multiple of given value
	 * @ingroup glslstruct
	 */
	[[nodiscard]] size_t ceil_to_nearest_multiple(size_t valueToRoundUp, size_t multipleValue) noexcept;

	/**
	 * @brief returns array elem name
	 * @ingroup glslstruct
	 */
	[[nodiscard]] std::string get_array_elem_name(std::string_view arrayName, size_t elemIdx);

	/**
	 * @brief returns struct elem name
	 * @ingroup glslstruct
	 */
	[[nodiscard]] std::string get_struct_elem_name(std::string_view structName, std::string_view elemName);
} // namespace glslstruct

	#endif
#endif