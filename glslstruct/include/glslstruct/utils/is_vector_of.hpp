/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_IS_VECTOR_OF_HPP_
	#define _GLSL_STRUCT_IS_VECTOR_OF_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/libs.hpp>

namespace glslstruct::utils {
	template<template<class, class...> class Test, class V, class... Args>
	struct is_vector_of : std::false_type {};

	template<template<class, class...> class Test, class T, class... Args>
	struct is_vector_of<Test, std::vector<T>, Args...> : Test<T, Args...> {};

	template<template<class, class...> class Test, class V, class... Args>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_vector_of_v = is_vector_of<Test, V, Args...>::value;
} // namespace glslstruct::utils

	#endif
#endif
