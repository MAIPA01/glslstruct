/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/value_types/base_type.hpp>
#include <glslstruct/value_types/value_type.hpp>
#include <glslstruct/value_types/ValueType.hpp>
#include <glslstruct/value_types/scalar_type.hpp>
#include <glslstruct/value_types/vec_type.hpp>
#include <glslstruct/value_types/mat_type.hpp>
#include <glslstruct/value_types/struct_type.hpp>
#include <glslstruct/value_types/array_type.hpp>
#include <glslstruct/value_types/visitors/base_type_visitor.hpp>
#include <glslstruct/value_types/visitors/eq_type_visitor.hpp>
#include <glslstruct/value_types/visitors/is_of_type_visitor.hpp>
#include <glslstruct/value_types/visitors/type_hash_visitor.hpp>