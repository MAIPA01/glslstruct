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
			if (type.empty()) { return ValueType::Float; }

			if (type[0] == 'f') { return ValueType::Float; }

			if (type[0] == 'i') { return ValueType::Int; }

			if (type[0] == 'u') { return ValueType::Uint; }

			if (type[0] == 'd') { return ValueType::Double; }

		// We don't need to check because it is our last option anyway
		return ValueType::Bool;
	}

	template<class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_scalar_variable(S& structRef, const ValueType type,
	  const std::string_view name) {
			switch (type) {
			case ValueType::Float:	structRef.template add<float>(name); break;
			case ValueType::Int:	structRef.template add<int>(name); break;
			case ValueType::Uint:	structRef.template add<unsigned int>(name); break;
			case ValueType::Double: structRef.template add<double>(name); break;
			default:
			case ValueType::Bool:	structRef.template add<bool>(name); break;
			}
		return structRef;
	}

	template<class T, class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_vec_variable(S& structRef, const size_t length, const std::string_view name) {
			switch (length) {
			default:
			case 2:	 structRef.template add<glm::vec<2, T> >(name); break;
			case 3:	 structRef.template add<glm::vec<3, T> >(name); break;
			case 4:	 structRef.template add<glm::vec<4, T> >(name); break;
			}
		return structRef;
	}

	template<class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_vec_variable(S& structRef, const ValueType type, const size_t length,
	  const std::string_view name) {
			switch (type) {
			case ValueType::Float:	return add_vec_variable<float>(structRef, length, name);
			case ValueType::Int:	return add_vec_variable<int>(structRef, length, name);
			case ValueType::Uint:	return add_vec_variable<unsigned int>(structRef, length, name);
			case ValueType::Double: return add_vec_variable<double>(structRef, length, name);
			default:
			case ValueType::Bool:	return add_vec_variable<bool>(structRef, length, name);
			}
	}

	template<class T, class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_mat_variable(S& structRef, const size_t size, const std::string_view name) {
			switch (size) {
			default:
			case 2:	 structRef.template add<glm::mat<2, 2, T> >(name); break;
			case 3:	 structRef.template add<glm::mat<3, 3, T> >(name); break;
			case 4:	 structRef.template add<glm::mat<4, 4, T> >(name); break;
			}

		return structRef;
	}

	template<class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_mat_variable(S& structRef, const ValueType type, const size_t size,
	  const std::string_view name) {
			switch (type) {
			case ValueType::Float:	return add_mat_variable<float>(structRef, size, name);
			case ValueType::Int:	return add_mat_variable<int>(structRef, size, name);
			case ValueType::Uint:	return add_mat_variable<unsigned int>(structRef, size, name);
			case ValueType::Double: return add_mat_variable<double>(structRef, size, name);
			default:
			case ValueType::Bool:	return add_mat_variable<bool>(structRef, size, name);
			}
	}

	template<class T, size_t cols, class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_mat_variable(S& structRef, const size_t rows, const std::string_view name) {
			switch (rows) {
			default:
			case 2:	 structRef.template add<glm::mat<cols, 2, T> >(name); break;
			case 3:	 structRef.template add<glm::mat<cols, 3, T> >(name); break;
			case 4:	 structRef.template add<glm::mat<cols, 4, T> >(name); break;
			}

		return structRef;
	}

	template<class T, class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_mat_variable(S& structRef, const size_t cols, const size_t rows,
	  const std::string_view name) {
			switch (cols) {
			default:
			case 2:	 return add_mat_variable<T, 2>(structRef, rows, name);
			case 3:	 return add_mat_variable<T, 3>(structRef, rows, name);
			case 4:	 return add_mat_variable<T, 3>(structRef, rows, name);
			}
	}

	template<class S>
	static inline _GLSL_STRUCT_CONSTEXPR17 S& add_mat_variable(S& structRef, const ValueType type, const size_t cols,
	  const size_t rows, const std::string_view name) {
			switch (type) {
			case ValueType::Float:	return add_mat_variable<float>(structRef, cols, rows, name);
			case ValueType::Int:	return add_mat_variable<int>(structRef, cols, rows, name);
			case ValueType::Uint:	return add_mat_variable<unsigned int>(structRef, cols, rows, name);
			case ValueType::Double: return add_mat_variable<double>(structRef, cols, rows, name);
			default:
			case ValueType::Bool:	return add_mat_variable<bool>(structRef, cols, rows, name);
			}
	}

	template<class S>
	static inline S& add_variable(S& structRef, const std::string_view type, const std::string_view name,
	  const std::unordered_map<std::string, typename S::layout_type>& definedStructs = {}) {
		static pcre2cpp::regex scalarsPattern("^(?:float|int|uint|double|bool)$");
		static pcre2cpp::regex vecPattern("^(?<scalar>[idbuf])?vec(?<length>[2-4])$");
		static pcre2cpp::regex matPattern("^(?<scalar>[idbuf])?mat(?:(?<size>[2-4])|(?<cols>[2-4])x(?<rows>[2-4]))$");

		pcre2cpp::match_result result;

			// Check if scalar type
			if (scalarsPattern.match_at(type)) { return add_scalar_variable(structRef, get_value_type_from_string(type), name); }

			// Check if vec type
			if (vecPattern.match_at(type, result)) {
				size_t length;
				mstd::strtounum(result.get_sub_result_value("length"), length);

				return add_vec_variable(structRef, get_value_type_from_string(result.get_sub_result_value("scalar")), length,
				  name);
			}

			// Check if mat type
			if (matPattern.match_at(type, result)) {
				const ValueType valueType = get_value_type_from_string(result.get_sub_result_value("scalar"));

					if (const std::string sizeStr = result.get_sub_result_value("size"); !sizeStr.empty()) {
						size_t size;
						mstd::strtounum(sizeStr, size);

						return add_mat_variable(structRef, valueType, size, name);
					}

				size_t cols;
				mstd::strtounum(result.get_sub_result_value("cols"), cols);

				size_t rows;
				mstd::strtounum(result.get_sub_result_value("rows"), rows);

				return add_mat_variable(structRef, valueType, cols, rows, name);
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
	static inline S& add_variable(S& structRef, const std::string_view varStr,
	  const std::unordered_map<std::string, typename S::layout_type>& definedStructs = {}) {
		static pcre2cpp::regex variablePattern("^(?<type>[_a-zA-Z][_a-zA-Z0-9]+)\\s+(?<name>[_a-zA-Z][_a-zA-Z0-9]*)$");

		pcre2cpp::match_result result;
			if (!variablePattern.match_at(varStr, result)) {
				glsl_struct_assert(false, "Provided var str '{}' was not a correct variable", varStr);
				return structRef;
			}

		return add_variable(structRef, result.get_sub_result_value("type"), result.get_sub_result_value("name"), definedStructs);
	}

	template<class S>
	static inline S& add_variables(S& structRef, const std::string_view varsStr,
	  const std::unordered_map<std::string, typename S::layout_type>& definedStructs = {}) {
		using std::literals::operator""sv;
		static pcre2cpp::regex multiVariablesPattern(
		  "(?<=^|;)\\s*(?<var>[_a-zA-Z][_a-zA-Z0-9]*\\s+[_a-zA-Z][_a-zA-Z0-9]*)(?=[^;\\n]*(?:;|$))"sv,
		  static_cast<pcre2cpp::regex_compile_options>(pcre2cpp::regex_compile_options_bits::MULTILINE)
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
	static inline void process_structs(const std::string_view structsStr,
	  std::unordered_map<std::string, typename S::layout_type>& definedStructs) {
		using std::literals::operator""sv;
		static pcre2cpp::regex structPattern("struct\\s+(?<name>[_a-zA-Z][_a-zA-Z0-9]*)\\s+{(?<vars>[^}]*)};"sv,
		  static_cast<pcre2cpp::regex_compile_options>(pcre2cpp::regex_compile_options_bits::MULTILINE));

		std::vector<pcre2cpp::match_result> results;
			if (!structPattern.match_all(structsStr, results)) {
				glsl_struct_assert(false, "String of structs was incorect!!");
				return;
			}

			for (const auto& result : results) {
				definedStructs.emplace(result.get_sub_result_value("name"),
				  create_struct<S>(result.get_sub_result_value("vars"), definedStructs).get_layout());
			}
	}

} // namespace glslstruct

	#endif
#endif