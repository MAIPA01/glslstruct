/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#include <glslstruct/config.hpp>

#if _GLSL_STRUCT_HAS_TYPES

	#include <glslstruct/pch.hpp>
	#include <glslstruct/type/containers/struct_type.hpp>

using namespace glslstruct;

struct_type::struct_type(const mstd::ordered_map<std::string, var_data>& values, const size_t size) noexcept
	: base_type(size), _variables(values) {}

struct_type::struct_type(const struct_type& other) noexcept			   = default;
struct_type::struct_type(struct_type&& other) noexcept				   = default;

struct_type::~struct_type() noexcept								   = default;

struct_type& struct_type::operator=(const struct_type& other) noexcept = default;

struct_type& struct_type::operator=(struct_type&& other) noexcept	   = default;

bool struct_type::contains(const std::string_view name) const noexcept { return _variables.contains(name.data()); }

const mstd::ordered_map<std::string, var_data>& struct_type::get_variables() const noexcept { return _variables; }

mstd::ordered_map<std::string, var_data> struct_type::get_top_level_variables() const noexcept {
	mstd::ordered_map<std::string, var_data> result;
		for (const auto& [name, data] : _variables) {
				if (!data.is_top_level()) { continue; }
			result.emplace_back(name, data);
		}
	return result;
}

std::string struct_type::to_string() const noexcept { return "struct"; }

bool glslstruct::operator==(const struct_type& lhs, const struct_type& rhs) noexcept { return lhs._variables == rhs._variables; }

	#if _GLSL_STRUCT_HAS_CXX20
bool glslstruct::operator!=(const struct_type& lhs, const struct_type& rhs) noexcept = default;
	#else
bool glslstruct::operator!=(const struct_type& lhs, const struct_type& rhs) noexcept { return !(lhs == rhs); }
	#endif

size_t std::hash<struct_type>::operator()(const struct_type& type) const noexcept {
	size_t seed = 0;
		for (const auto& [name, data] : type._variables) { mstd::hash_append(seed, name, data); }
	return seed;
}

#endif