/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_MAJOR_TYPE_HPP_
	#define _GLSL_STRUCT_MAJOR_TYPE_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

namespace glslstruct {
	// clang-format off
	/**
	 * @brief Major Type Enum for matrices (for now not used)
	 * @ingroup glslstruct
	 */
	ENUM_CLASS_BASE(MajorType, uint8_t, (Column), (Row))
	// clang-format on
} // namespace glslstruct

	#endif
#endif