/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_GLSLSTRUCT_HPP_
	#define _GLSL_STRUCT_GLSLSTRUCT_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/types.hpp>
		#include <glslstruct/value_types.hpp>

		#include <glslstruct/value/glsl_value.hpp>

		#include <glslstruct/variable/glsl_variable.hpp>

		#include <glslstruct/layout/base_layout.hpp>

		#include <glslstruct/struct/base_struct.hpp>

		#if _GLSL_STRUCT_HAS_TYPES
			#include <glslstruct/writer/glsl/writer.hpp>
		#endif

		#if _GLSL_STRUCT_HAS_PARSER
			#include <glslstruct/parser/parser.hpp>
		#endif

	#endif
#endif