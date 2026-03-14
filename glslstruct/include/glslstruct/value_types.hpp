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

#include <glslstruct/value_types/ValueType.hpp>
#include <glslstruct/value_types/MajorType.hpp>

#include <glslstruct/value_types/types/base_type.hpp>
#include <glslstruct/value_types/types/value_type.hpp>
#include <glslstruct/value_types/types/scalar_type.hpp>
#include <glslstruct/value_types/types/vec_type.hpp>
#include <glslstruct/value_types/types/mat_type.hpp>
#include <glslstruct/value_types/types/struct_type.hpp>
#include <glslstruct/value_types/types/array_type.hpp>

#include <glslstruct/value_types/visitors/type_visitor_concept.hpp>
#include <glslstruct/value_types/visitors/eq_type_visitor.hpp>
#include <glslstruct/value_types/visitors/is_of_type_visitor.hpp>
#include <glslstruct/value_types/visitors/type_hash_visitor.hpp>

#include <glslstruct/value_types/checks/type_checks.hpp>
#include <glslstruct/value_types/checks/value_checks.hpp>
#include <glslstruct/value_types/checks/layouts_checks.hpp>
#include <glslstruct/value_types/checks/struct_checks.hpp>

#include <glslstruct/value_types/data/value_data.hpp>
#include <glslstruct/value_types/data/scalar_data.hpp>
#include <glslstruct/value_types/data/vec_data.hpp>
#include <glslstruct/value_types/data/mat_data.hpp>

#include <glslstruct/value_types/traits/scalar_traits.hpp>
#include <glslstruct/value_types/traits/vec_traits.hpp>
#include <glslstruct/value_types/traits/mat_traits.hpp>

#endif