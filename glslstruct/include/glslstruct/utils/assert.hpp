/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_ASSERT_HPP_
	#define _GLSL_STRUCT_ASSERT_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <mstd/mstd.hpp>

	/**
	 * @def glsl_struct_assert(expression, ...)
	 * @brief glslstruct assert
	 * @ingroup utils
	 */

		#if _DEBUG
			// NOLINTNEXTLINE
			#define glsl_struct_assert(expression, ...)                                                             \
				MSTD_STOP_ASSERT_BASE(expression, [](const std::string_view) -> void {} __VA_OPT__(, ) __VA_ARGS__)
		#elif !defined(GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE)
			// NOLINTNEXTLINE
			#define glsl_struct_assert(expression, ...)                                                            \
				MSTD_LOG_ASSERT_BASE(expression, [](const std::string_view) -> void {} __VA_OPT__(, ) __VA_ARGS__)
		#else
			// NOLINTNEXTLINE
			#define glsl_struct_assert(expression, ...)                                                              \
				MSTD_EMPTY_ASSERT_BASE(expression, [](const std::string_view) -> void {} __VA_OPT__(, ) __VA_ARGS__)
		#endif

	#endif
#endif