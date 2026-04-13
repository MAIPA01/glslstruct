/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_LIBS_HPP_
	#define _GLSL_STRUCT_LIBS_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <fmt/core.h>
		#include <array>
		#include <cstddef>
		#include <cstdint>
		#include <functional>
		#include <glm/glm.hpp>
		#include <glm/gtc/matrix_access.hpp>
		#include <glm/gtc/matrix_transform.hpp>
		#include <glm/gtc/type_ptr.hpp>
		#include <memory>
		#include <mstd/mstd.hpp>
		#if _GLSL_STRUCT_HAS_PARSER
			#include <pcre2cpp/pcre2cpp.hpp>
		#endif
		#include <string>
		#include <type_traits>
		#include <unordered_map>
		#include <utility>
		#include <vector>
		#if _GLSL_STRUCT_HAS_CXX20
			#include <ranges>
			#include <span>
		#endif

	#endif
#endif