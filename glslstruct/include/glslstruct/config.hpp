/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_CONFIG_HPP_
	#define _GLSL_STRUCT_CONFIG_HPP_

	#pragma region VERSION
	#define GLSL_STRUCT_VERSION_MAJOR 1
	#define GLSL_STRUCT_VERSION_MINOR 3
	#define GLSL_STRUCT_VERSION_PATCH 3

	#define _GLSL_STRUCT_STRINGIFY_HELPER(x) #x

	#define _GLSL_STRUCT_VERSION_TO_STRING(major, minor, patch)                                                                \
		_GLSL_STRUCT_STRINGIFY_HELPER(major) "." _GLSL_STRUCT_STRINGIFY_HELPER(minor) "." _GLSL_STRUCT_STRINGIFY_HELPER(patch)
	#define _GLSL_STRUCT_VERSION_TO_INT(major, minor, patch) (major * 100 + minor * 10 + patch)

	#define GLSL_STRUCT_VERSION_STRING                                                                                  \
		_GLSL_STRUCT_VERSION_TO_STRING(GLSL_STRUCT_VERSION_MAJOR, GLSL_STRUCT_VERSION_MINOR, GLSL_STRUCT_VERSION_PATCH)
	#define GLSL_STRUCT_VERSION_INT                                                                                  \
		_GLSL_STRUCT_VERSION_TO_INT(GLSL_STRUCT_VERSION_MAJOR, GLSL_STRUCT_VERSION_MINOR, GLSL_STRUCT_VERSION_PATCH)
	#define GLSL_STRUCT_VERSION GLSL_STRUCT_VERSION_STRING
	#pragma endregion

	#pragma region LAST_UPDATE
	#define GLSL_STRUCT_LAST_UPDATE_DAY	  19
	#define GLSL_STRUCT_LAST_UPDATE_MONTH 03
	#define GLSL_STRUCT_LAST_UPDATE_YEAR  2026

	#define _GLSL_STRUCT_LAST_UPDATE_DATE_HELPER(day, month, year)                                                          \
		_GLSL_STRUCT_STRINGIFY_HELPER(day) "." _GLSL_STRUCT_STRINGIFY_HELPER(month) "." _GLSL_STRUCT_STRINGIFY_HELPER(year)

	#define GLSL_STRUCT_LAST_UPDATE_DATE                                                                 \
		_GLSL_STRUCT_LAST_UPDATE_DATE_HELPER(GLSL_STRUCT_LAST_UPDATE_DAY, GLSL_STRUCT_LAST_UPDATE_MONTH, \
		  GLSL_STRUCT_LAST_UPDATE_YEAR)
	#pragma endregion

	#pragma region CXX_VERSIONS
	#ifndef _HAS_CXX17
	// clang-format off
	#define _GLSL_STRUCT_HAS_CXX17 __cplusplus >= 201703l
	// clang-format on
	#else
		#define _GLSL_STRUCT_HAS_CXX17 _HAS_CXX17
	#endif

	#ifndef GLSL_STRUCT_ENABLE_CXX20
		#define _GLSL_STRUCT_HAS_CXX20 0
	#elif !defined(_HAS_CXX20)
	// clang-format off
	#define _GLSL_STRUCT_HAS_CXX20 __cplusplus >= 202002l && GLSL_STRUCT_ENABLE_CXX20
	// clang-format on
	#else
		#define _GLSL_STRUCT_HAS_CXX20 _HAS_CXX20&& GLSL_STRUCT_ENABLE_CXX20
	#endif
	#pragma endregion

	#ifdef GLSL_STRUCT_DISABLE_TYPES
		#define _GLSL_STRUCT_HAS_TYPES 0
	#else
		#define _GLSL_STRUCT_HAS_TYPES _GLSL_STRUCT_HAS_CXX17
	#endif

	#if defined(GLSL_STRUCT_ENABLE_TYPE_CHECKS) && !defined(GLSL_STRUCT_DISABLE_TYPES)
		#define _GLSL_STRUCT_HAS_TYPE_CHECKS 1
	#else
		#define _GLSL_STRUCT_HAS_TYPE_CHECKS 0
	#endif

	#pragma region VERSION_SPECIFIC_VALUES
	#if _GLSL_STRUCT_HAS_CXX17
		#define _GLSL_STRUCT_CONSTEXPR17 constexpr
	#else
		#define _GLSL_STRUCT_CONSTEXPR17
	#endif

	#if _GLSL_STRUCT_HAS_CXX20
		#define _GLSL_STRUCT_CONSTEXPR20		 constexpr
		#define _GLSL_STRUCT_REQUIRES(condition) requires (condition)
	#else
		#define _GLSL_STRUCT_CONSTEXPR20
		#define _GLSL_STRUCT_REQUIRES(condition)
	#endif
	#pragma endregion

	#include <mstd/mstd.hpp>

	#define _GLSL_STRUCT_MESSAGE(MESSAGE) _MSTD_MESSAGE(MESSAGE)
	#define _GLSL_STRUCT_WARNING(MESSAGE) _MSTD_WARNING(MESSAGE)
	#define _GLSL_STRUCT_ERROR(MESSAGE)	  _MSTD_ERROR(MESSAGE)
#endif