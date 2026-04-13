/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

	#include <glslstruct/type/traits/scalar_traits.hpp>
	#include <pch.hpp>

using namespace glslstruct;

ValueType scalar_traits<bool>::get_value_type() noexcept { return ValueType::Bool; }

scalar_data scalar_traits<bool>::get_data(const bool value) { return scalar_data(value); }

bool scalar_traits<bool>::get_value(const scalar_data& data) {
	const std::vector<std::byte>& bytes = data.data();
	const size_t maxSize				= std::min(sizeof(int), bytes.size());

	int value;
	std::copy_n(bytes.begin(), maxSize, reinterpret_cast<std::byte*>(std::addressof(value)));
	return value > 0;
}

ValueType scalar_traits<int>::get_value_type() noexcept { return ValueType::Int; }

scalar_data scalar_traits<int>::get_data(const int value) { return scalar_data(value); }

int scalar_traits<int>::get_value(const scalar_data& data) {
	const std::vector<std::byte>& bytes = data.data();
	const size_t maxSize				= std::min(sizeof(int), bytes.size());

	int value;
	std::copy_n(bytes.begin(), maxSize, reinterpret_cast<std::byte*>(std::addressof(value)));
	return value;
}

ValueType scalar_traits<unsigned int>::get_value_type() noexcept { return ValueType::Uint; }

scalar_data scalar_traits<unsigned int>::get_data(const unsigned int value) { return scalar_data(value); }

unsigned int scalar_traits<unsigned int>::get_value(const scalar_data& data) {
	const std::vector<std::byte>& bytes = data.data();
	const size_t maxSize				= std::min(sizeof(unsigned int), bytes.size());

	unsigned int value;
	std::copy_n(bytes.begin(), maxSize, reinterpret_cast<std::byte*>(std::addressof(value)));
	return value;
}

ValueType scalar_traits<float>::get_value_type() noexcept { return ValueType::Float; }

scalar_data scalar_traits<float>::get_data(const float value) { return scalar_data(value); }

float scalar_traits<float>::get_value(const scalar_data& data) {
	const std::vector<std::byte>& bytes = data.data();
	const size_t maxSize				= std::min(sizeof(float), bytes.size());

	float value;
	std::copy_n(bytes.begin(), maxSize, reinterpret_cast<std::byte*>(std::addressof(value)));
	return value;
}

ValueType scalar_traits<double>::get_value_type() noexcept { return ValueType::Double; }

scalar_data scalar_traits<double>::get_data(const double value) { return scalar_data(value); }

double scalar_traits<double>::get_value(const scalar_data& data) {
	const std::vector<std::byte>& bytes = data.data();
	const size_t maxSize				= std::min(sizeof(double), bytes.size());

	double value;
	std::copy_n(bytes.begin(), maxSize, reinterpret_cast<std::byte*>(std::addressof(value)));
	return value;
}

std::string glslstruct::scalar_to_string(const ValueType valueType) {
		switch (valueType) {
		[[unlikely]] default:
			return "UNKNOWN";
		case ValueType::Bool:	return "bool";
		case ValueType::Int:	return "int";
		case ValueType::Uint:	return "uint";
		case ValueType::Float:	return "float";
		case ValueType::Double: return "double";
		}
}

#endif