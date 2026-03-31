/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

	#include <glslstruct/utils/functions.hpp>
	#include <pch.hpp>

using namespace glslstruct;

size_t glslstruct::ceil_to_nearest_multiple(size_t valueToRoundUp, const size_t multipleValue) noexcept {
		if (const size_t modulo = valueToRoundUp % multipleValue; modulo != 0) { valueToRoundUp += multipleValue - modulo; }
	return valueToRoundUp;
}

#endif