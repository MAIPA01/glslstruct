/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with GLSL_STRUCT_DISABLE_TYPES set to 1!"
);
#else

	#include <glslstruct/type/containers/array_type.hpp>
	#include <glslstruct/type/containers/base_type.hpp>
	#include <glslstruct/type/containers/mat_type.hpp>
	#include <glslstruct/type/containers/scalar_type.hpp>
	#include <glslstruct/type/containers/struct_type.hpp>
	#include <glslstruct/type/containers/type.hpp>
	#include <glslstruct/type/containers/vec_type.hpp>

	#include <glslstruct/type/visitors/is_of_type_visitor.hpp>
	#include <pch.hpp>

using namespace glslstruct;

bool glslstruct::is_of_type(const base_type_handle& type, const BaseType baseType) {
		switch (baseType) {
		case BaseType::Scalar: return is_of_type<scalar_type>(type);
		case BaseType::Vec:	   return is_of_type<vec_type>(type);
		case BaseType::Mat:	   return is_of_type<mat_type>(type);
		case BaseType::Struct: return is_of_type<struct_type>(type);
		case BaseType::Array:  return is_of_type<array_type>(type);
		default:			   return false;
		}
}

#endif