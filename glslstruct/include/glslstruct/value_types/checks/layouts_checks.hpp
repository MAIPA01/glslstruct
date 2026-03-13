/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

#include <glslstruct/value_types/checks/value_checks.hpp>

namespace glslstruct::utils {
#pragma region IS_LAYOUT
	template<class T, class... Allowed>
	struct is_glsl_layout : std::bool_constant<(std::is_base_of_v<base_layout, T> &&
		!std::is_same_v<base_layout, T> && mstd::is_same_type_in_v<T, Allowed...>)> {};

	template<class T, class... Allowed>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_layout_v = is_glsl_layout<T, Allowed...>::value;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T, class... Allowed> concept glsl_offset = is_glsl_offset_v<T>;
#endif
#pragma endregion

#pragma region IS_SPECIAL_LAYOUT
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_std140_layout_v = is_glsl_layout_v<T, std140_layout>;
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_std430_layout_v = is_glsl_layout_v<T, std430_layout>;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T> concept glsl_std140_layout = is_glsl_std140_layout_v<T>;
	template<class T> concept glsl_std430_layout = is_glsl_std430_layout_v<T>;
#endif
#pragma endregion

#pragma region IS_SIMPLE_OR_LAYOUT
	template<class T, class... Allowed>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_simple_or_layout_v = is_glsl_simple_v<T> || is_glsl_layout_v<T, Allowed...>;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T, class... Allowed> concept glsl_simple_or_layout = is_glsl_simple_or_layout_v<T, Allowed...>;
#endif
#pragma endregion
}
#endif