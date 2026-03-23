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

	#include <glslstruct/layout/base_layout.hpp>
	#include <pch.hpp>

using namespace glslstruct;

size_t std::hash<base_layout>::operator()(const base_layout& stdOff) const noexcept {
	size_t seed = mstd::hash_combine(stdOff._currentOffset, stdOff._maxAlignment);
		for (const auto& [name, data] : stdOff._values) { mstd::hash_append(seed, name, data); }
	return seed;
}

#endif