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

	#include <glslstruct/type_containers/scalar_type.hpp>
	#include <pch.hpp>

using namespace glslstruct;

scalar_type::scalar_type(const ValueType type, const size_t size) noexcept : base_type(size), _type(type) {}

scalar_type::scalar_type(const scalar_type& other) noexcept			   = default;
scalar_type::scalar_type(scalar_type&& other) noexcept				   = default;
scalar_type::~scalar_type() noexcept								   = default;

scalar_type& scalar_type::operator=(const scalar_type& other) noexcept = default;
scalar_type& scalar_type::operator=(scalar_type&& other) noexcept	   = default;

ValueType scalar_type::get_type() const noexcept { return _type; }

std::string scalar_type::to_string() const noexcept { return scalar_to_string(_type); }

bool glslstruct::operator==(const scalar_type& lhs, const scalar_type& rhs) noexcept { return lhs._type == rhs._type; }

	#if _GLSL_STRUCT_HAS_CXX20
bool glslstruct::operator!=(const scalar_type& lhs, const scalar_type& rhs) noexcept = default;
	#else
bool glslstruct::operator!=(const scalar_type& lhs, const scalar_type& rhs) noexcept { return !(lhs == rhs); }
	#endif

size_t std::hash<scalar_type>::operator()(const scalar_type& value) const noexcept { return static_cast<size_t>(value._type); }

#endif