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

		#include <glslstruct/utils/MajorType.hpp>
		#include <glslstruct/utils/ValueType.hpp>

		#if _GLSL_STRUCT_HAS_TYPES
			#include <glslstruct/type/containers/array_type.hpp>
			#include <glslstruct/type/containers/base_type.hpp>
			#include <glslstruct/type/containers/mat_type.hpp>
			#include <glslstruct/type/containers/scalar_type.hpp>
			#include <glslstruct/type/containers/struct_type.hpp>
			#include <glslstruct/type/containers/type.hpp>
			#include <glslstruct/type/containers/vec_type.hpp>

			#include <glslstruct/type/visitors/eq_type_visitor.hpp>
			#include <glslstruct/type/visitors/is_of_type_visitor.hpp>
			#include <glslstruct/type/visitors/type_hash_visitor.hpp>
			#include <glslstruct/type/visitors/type_visitor_concept.hpp>
		#endif

		#include <glslstruct/type/checks/layouts_checks.hpp>
		#include <glslstruct/type/checks/simple_checks.hpp>
		#include <glslstruct/type/checks/struct_checks.hpp>
		#if _GLSL_STRUCT_HAS_TYPES
			#include <glslstruct/type/checks/type_checks.hpp>
		#endif

		#include <glslstruct/type/data/mat_data.hpp>
		#include <glslstruct/type/data/scalar_data.hpp>
		#include <glslstruct/type/data/vec_data.hpp>

		#include <glslstruct/var_data/var_data.hpp>

		#include <glslstruct/type/traits/mat_traits.hpp>
		#include <glslstruct/type/traits/scalar_traits.hpp>
		#include <glslstruct/type/traits/vec_traits.hpp>

	#endif
#endif