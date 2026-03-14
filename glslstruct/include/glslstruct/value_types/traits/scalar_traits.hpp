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

#include <glslstruct/types.hpp>
#include <glslstruct/value_types/data/scalar_data.hpp>

namespace glslstruct {
	template<>
	struct scalar_traits<bool> {
		static scalar_data get_data(bool value) {
			return scalar_data(value);
		}
	};

	template<>
	struct scalar_traits<int> {
		static scalar_data get_data(int value) {
			return scalar_data(value);
		}
	};

	template<>
	struct scalar_traits<unsigned int> {
		static scalar_data get_data(unsigned int value) {
			return scalar_data(value);
		}
	};

	template<>
	struct scalar_traits<float> {
		static scalar_data get_data(float value) {
			return scalar_data(value);
		}
	};

	template<>
	struct scalar_traits<double> {
		static scalar_data get_data(double value) {
			return scalar_data(value);
		}
	};
}

#endif