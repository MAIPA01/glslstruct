/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_VALUE_TYPES_HPP_
	#define _GLSL_STRUCT_VALUE_TYPES_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <utils/MajorType.hpp>
		#include <utils/ValueType.hpp>

		#include <glslstruct/value_types/types/array_type.hpp>
		#include <glslstruct/value_types/types/base_type.hpp>
		#include <glslstruct/value_types/types/glsl_type.hpp>
		#include <glslstruct/value_types/types/mat_type.hpp>
		#include <glslstruct/value_types/types/scalar_type.hpp>
		#include <glslstruct/value_types/types/struct_type.hpp>
		#include <glslstruct/value_types/types/vec_type.hpp>

		#include <glslstruct/value_types/visitors/eq_type_visitor.hpp>
		#include <glslstruct/value_types/visitors/is_of_type_visitor.hpp>
		#include <glslstruct/value_types/visitors/type_hash_visitor.hpp>
		#include <glslstruct/value_types/visitors/type_visitor_concept.hpp>

		#include <type_checks/layouts_checks.hpp>
		#include <type_checks/struct_checks.hpp>
		#include <type_checks/type_checks.hpp>
		#include <type_checks/value_checks.hpp>

		#include <struct_elem/struct_elem_data.hpp>
		#include <type_data/mat_data.hpp>
		#include <type_data/scalar_data.hpp>
		#include <type_data/vec_data.hpp>

		#include <type_traits/mat_traits.hpp>
		#include <type_traits/scalar_traits.hpp>
		#include <type_traits/vec_traits.hpp>

	#endif
#endif