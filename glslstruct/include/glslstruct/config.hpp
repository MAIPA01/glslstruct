/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once

#pragma region VERSION
#define GLSL_STRUCT_VERSION_MAJOR 1
#define GLSL_STRUCT_VERSION_MINOR 1
#define GLSL_STRUCT_VERSION_PATCH 1

#define _GLSL_STRUCT_STRINGIFY_HELPER(x) #x

#define _GLSL_STRUCT_VERSION_TO_STRING(major, minor, patch) _GLSL_STRUCT_STRINGIFY_HELPER(major)"."\
													 _GLSL_STRUCT_STRINGIFY_HELPER(minor)"."\
													 _GLSL_STRUCT_STRINGIFY_HELPER(patch)
#define _GLSL_STRUCT_VERSION_TO_INT(major, minor, patch) (major * 100 + minor * 10 + patch)

#define GLSL_STRUCT_VERSION_STRING _GLSL_STRUCT_VERSION_TO_STRING(GLSL_STRUCT_VERSION_MAJOR, \
												GLSL_STRUCT_VERSION_MINOR, \
												GLSL_STRUCT_VERSION_PATCH)
#define GLSL_STRUCT_VERSION_INT _GLSL_STRUCT_VERSION_TO_INT(GLSL_STRUCT_VERSION_MAJOR, \
											GLSL_STRUCT_VERSION_MINOR, \
											GLSL_STRUCT_VERSION_PATCH)
#define GLSL_STRUCT_VERSION GLSL_STRUCT_VERSION_STRING
#pragma endregion

#pragma region LAST_UPDATE
#define GLSL_STRUCT_LAST_UPDATE_DAY 24
#define GLSL_STRUCT_LAST_UPDATE_MONTH 02
#define GLSL_STRUCT_LAST_UPDATE_YEAR 2026

#define _GLSL_STRUCT_LAST_UPDATE_DATE_HELPER(day, month, year) _GLSL_STRUCT_STRINGIFY_HELPER(day)"."\
															_GLSL_STRUCT_STRINGIFY_HELPER(month)"."\
															_GLSL_STRUCT_STRINGIFY_HELPER(year)

#define GLSL_STRUCT_LAST_UPDATE_DATE _GLSL_STRUCT_LAST_UPDATE_DATE_HELPER(GLSL_STRUCT_LAST_UPDATE_DAY, \
															GLSL_STRUCT_LAST_UPDATE_MONTH, \
														    GLSL_STRUCT_LAST_UPDATE_YEAR)
#pragma endregion

#ifndef _HAS_CXX20
#define _GLSL_STRUCT_HAS_CXX20 __cplusplus >= 202002L && _GLSL_STRUCT_ENABLE_CXX20
#else
#define _GLSL_STRUCT_HAS_CXX20 _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
#endif

#define _GLSL_STRUCT_CONSTEXPR17 constexpr

#if _GLSL_STRUCT_HAS_CXX20
#define _GLSL_STRUCT_CONSTEXPR20 constexpr
#define _GLSL_STRUCT_REQUIRES(condition) requires (condition)
#define _GLSL_STRUCT_ENABLE_IF_TEMPLATE(class_name, condition)
#define _GLSL_STRUCT_ONE_CLASS_TEMPLATE(value_name, concept_type, condition, init_value, ...) template<concept_type value_name __VA_OPT__(, __VA_ARGS__)>
#define _GLSL_STRUCT_DEFAULT20 = default
#else
#define _GLSL_STRUCT_CONSTEXPR20
#define _GLSL_STRUCT_REQUIRES(condition)
#define _GLSL_STRUCT_ENABLE_IF_TEMPLATE(class_name, condition) template<class class_name, std::enable_if_t<(condition), bool> = true>
#define _GLSL_STRUCT_ONE_CLASS_TEMPLATE(value_name, concept_type, condition, init_value, ...) template<class value_name __VA_OPT__(, __VA_ARGS__), std::enable_if_t<(condition), bool> init_value>
#define _GLSL_STRUCT_DEFAULT20
#endif