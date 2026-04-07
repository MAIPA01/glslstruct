/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_PARSER_HPP_
	#define _GLSL_STRUCT_PARSER_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_PARSER
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater and when parser functionality is not disabled!");
	#else

		#include <glslstruct/struct/base_struct.hpp>

namespace glslstruct {

	// Add variable (layout/struct, var string, structs_map) -> ref to struct/layout
	// Read structs <layout>(structs string) -> structs_map

	static inline _GLSL_STRUCT_CONSTEXPR17 ValueType get_value_type_from_string(const std::string_view type) noexcept {
			// Option for vec and mat
			if (type.empty() || type[0] == 'f') { return ValueType::Float; }

			if (type[0] == 'i') { return ValueType::Int; }

			if (type[0] == 'u') { return ValueType::Uint; }

			if (type[0] == 'd') { return ValueType::Double; }

			if (type[0] == 'b') { return ValueType::Bool; }

		[[unlikely]] return ValueType::Int;
	}

	template<class T, class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_variable(S& structRef, const std::string_view name, const size_t count) {
			if (count != 0) { structRef.template add<std::vector<T> >(name, count); }
			else { structRef.template add<T>(name); }
		return structRef;
	}

	template<class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_scalar_variable(S& structRef, const ValueType type, const std::string_view name,
	  const size_t count) {
			switch (type) {
			case ValueType::Bool:	return add_variable<bool>(structRef, name, count);
			default:
			case ValueType::Int:	return add_variable<int>(structRef, name, count);
			case ValueType::Uint:	return add_variable<unsigned int>(structRef, name, count);
			case ValueType::Float:	return add_variable<float>(structRef, name, count);
			case ValueType::Double: return add_variable<double>(structRef, name, count);
			}
	}

	template<class T, class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_vec_variable(S& structRef, const size_t length, const std::string_view name,
	  const size_t count) {
			switch (length) {
			default:
			case 2:	 return add_variable<glm::vec<2, T> >(structRef, name, count);
			case 3:	 return add_variable<glm::vec<3, T> >(structRef, name, count);
			case 4:	 return add_variable<glm::vec<4, T> >(structRef, name, count);
			}
	}

	template<class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_vec_variable(S& structRef, const ValueType type, const size_t length,
	  const std::string_view name, const size_t count) {
			switch (type) {
			case ValueType::Bool:	return add_vec_variable<bool>(structRef, length, name, count);
			default:
			case ValueType::Int:	return add_vec_variable<int>(structRef, length, name, count);
			case ValueType::Uint:	return add_vec_variable<unsigned int>(structRef, length, name, count);
			case ValueType::Float:	return add_vec_variable<float>(structRef, length, name, count);
			case ValueType::Double: return add_vec_variable<double>(structRef, length, name, count);
			}
	}

	template<class T, size_t cols, class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_mat_variable(S& structRef, const size_t rows, const std::string_view name,
	  const size_t count) {
			switch (rows) {
			default:
			case 2:	 add_variable<glm::mat<cols, 2, T> >(structRef, name, count);
			case 3:	 add_variable<glm::mat<cols, 3, T> >(structRef, name, count);
			case 4:	 add_variable<glm::mat<cols, 4, T> >(structRef, name, count);
			}

		return structRef;
	}

	template<class T, class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_mat_variable(S& structRef, const size_t cols, const size_t rows,
	  const std::string_view name, const size_t count) {
			switch (cols) {
			default:
			case 2:	 return add_mat_variable<T, 2>(structRef, rows, name, count);
			case 3:	 return add_mat_variable<T, 3>(structRef, rows, name, count);
			case 4:	 return add_mat_variable<T, 3>(structRef, rows, name, count);
			}
	}

	template<class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_mat_variable(S& structRef, const ValueType type, const size_t cols,
	  const size_t rows, const std::string_view name, const size_t count) {
			switch (type) {
			case ValueType::Bool:	return add_mat_variable<bool>(structRef, cols, rows, name, count);
			default:
			case ValueType::Int:	return add_mat_variable<int>(structRef, cols, rows, name, count);
			case ValueType::Uint:	return add_mat_variable<unsigned int>(structRef, cols, rows, name, count);
			case ValueType::Float:	return add_mat_variable<float>(structRef, cols, rows, name, count);
			case ValueType::Double: return add_mat_variable<double>(structRef, cols, rows, name, count);
			}
	}

	// count == 0 -> scalar, count > 0 -> array
	template<class S>
	static inline S& add_variable(S& structRef, const std::string_view type, const std::string_view name, const size_t count = 0,
	  const std::unordered_map<std::string, typename S::layout_type>& definedStructs = {}) {
		static pcre2cpp::regex scalarsPattern(R"(^(?>bool|double|float|u?int)$)");
		static pcre2cpp::regex vecPattern(R"(^(?>(?<scalar>[idbuf])?vec(?<length>[2-4]))$)");
		static pcre2cpp::regex matPattern(R"(^(?>(?<scalar>[idbuf])?mat(?<cols>[2-4])(?>(?:x(?<rows>[2-4]))?))$)");

		pcre2cpp::match_result result;

			// Check if scalar type
			if (scalarsPattern.match_at(type)) {
				return add_scalar_variable(structRef, get_value_type_from_string(type), name, count);
			}

			// Check if vec type
			if (vecPattern.match_at(type, result)) {
				size_t length;
				mstd::strtounum(result.get_sub_result_value("length"), length);

				return add_vec_variable(structRef, get_value_type_from_string(result.get_sub_result_value("scalar")), length,
				  name, count);
			}

			// Check if mat type
			if (matPattern.match_at(type, result)) {
				const ValueType valueType = get_value_type_from_string(result.get_sub_result_value("scalar"));

				size_t cols;
				mstd::strtounum(result.get_sub_result_value("cols"), cols);

					if (!result.has_sub_value("rows")) { return add_mat_variable(structRef, valueType, cols, cols, name, count); }

				size_t rows;
				mstd::strtounum(result.get_sub_result_value("rows"), rows);

				return add_mat_variable(structRef, valueType, cols, rows, name, count);
			}

		// Check if struct type
		#if _GLSL_STRUCT_HAS_CXX20
			if (definedStructs.contains(type.data())) {
		#else
			if (definedStructs.find(type.data()) != definedStructs.end()) {
		#endif
				structRef.add(name, definedStructs.at(type.data()));
				return structRef;
			}

		glsl_struct_assert(false, "Provided type str '{}' was not a correct variable type", type);
		return structRef;
	}

	template<class S>
	static inline S& add_variable(S& structRef, const std::string_view type, const std::string_view name,
	  const std::unordered_map<std::string, typename S::layout_type>& definedStructs = {}) {
		return add_variable(structRef, type, name, 0, definedStructs);
	}

	template<class S>
	static inline S& add_variable(S& structRef, const std::string_view varStr,
	  const std::unordered_map<std::string, typename S::layout_type>& definedStructs = {}) {
		static pcre2cpp::regex variablePattern(
		  R"(^\h*(?>(?<type>[a-zA-Z_]\w*+))\s++(?>(?<name>[a-zA-Z_]\w*+))\s*+(?>(?<array>\[\s*+(?<count>\d+)?\s*+\]))?\s*+(?>;?)\h*$)"
		);

		pcre2cpp::match_result result;
			if (!variablePattern.match_at(varStr, result)) {
				glsl_struct_assert(false, "Provided var str '{}' was not a correct variable", varStr);
				return structRef;
			}

		size_t count = 0;
			if (result.has_sub_value("count")) { mstd::strtounum(result.get_sub_result_value("count"), count); }
			else if (result.has_sub_value("array")) { count = 1; }

		return add_variable(structRef, result.get_sub_result_value("type"), result.get_sub_result_value("name"), count,
		  definedStructs);
	}

	template<class S>
	static inline S& add_variables(S& structRef, const std::string_view varsStr,
	  const std::unordered_map<std::string, typename S::layout_type>& definedStructs = {}) {
		static pcre2cpp::regex multiVariablesPattern(
		  R"((?>(?:^|;))\h*+(?<var>[a-zA-Z_]\w*+\s++[a-zA-Z_]\w*+(?>(?:\s*+\[\s*+\d*+\s*+\])*+))\h*+(?=;|$))",
		  pcre2cpp::compile_options_bits::Multiline
		);

		std::vector<pcre2cpp::match_result> results;
			if (!multiVariablesPattern.match_all(varsStr, results)) {
				glsl_struct_assert(false, "Provided vars str '{}' was not a correct variables list", varsStr);
				return structRef;
			}

			for (const auto& result : results) { add_variable(structRef, result.get_sub_result_value("var"), definedStructs); }

		return structRef;
	}

	template<class S>
	static inline S create_struct(const std::string_view varsStr,
	  const std::unordered_map<std::string, typename S::layout_type>& definedStructs = {}) {
		S structValue;
		return add_variables(structValue, varsStr, definedStructs);
	}

	template<class S>
	static inline std::unordered_map<std::string, std::string> get_structs_bodies(const std::string_view structsStr) {
		static pcre2cpp::regex structPattern(R"(struct\s++(?<name>[a-zA-Z_]\w*+)\s*+\{(?<body>[^}]*+)\}\s*+;)",
		  pcre2cpp::compile_options_bits::Multiline);

		std::vector<pcre2cpp::match_result> results;
			if (!structPattern.match_all(structsStr, results)) { return {}; }

		std::unordered_map<std::string, std::string> structs;
		structs.reserve(results.size());
			for (const auto& result : results) {
				structs.emplace(result.get_sub_result_value("name"), result.get_sub_result_value("body"));
			}

		return structs;
	}

	// Don't allow any other than std140 layout
	static pcre2cpp::regex uboPattern(
	  R"(layout\s*\((?:(?:std140|set\s*=\s*[0-9]+)\s*,)?\s*binding\s*=\s*[0-9]+\s*\)\s+
				uniform\s+(?<name>[_a-zA-Z][_a-zA-Z0-9]*)\s+
				{(?<body>[^}]*)}\s*(?:[_a-zA-Z][_a-zA-Z0-9]+\s*)?;)",
	  pcre2cpp::compile_options_bits::Multiline
	);

	// Allow std140, std430, scalar layouts
	static pcre2cpp::regex ssboPattern(
	  R"(layout\s*\((?:std(?:140|430)(?:\s*,\s*set\s*=\s*[0-9]+)?|scalar\s*,\s*set\s*=\s*[0-9]+)\s*,\s*
				binding\s*=\s*[0-9]+\s*\)\s+(?:[a-zA-Z]+\s*)?buffer\s+(?<name>[_a-zA-Z][_a-zA-Z0-9]*)\s+
				{(?<body>[^}]*)}\s*(?:[_a-zA-Z][_a-zA-Z0-9]+\\s*)?;)",
	  pcre2cpp::compile_options_bits::Multiline
	);

} // namespace glslstruct

	#endif
#endif