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

	namespace utils {
		#pragma region BASE_PARSER_PATTERNS

		/**
		 * @ingroup utils
		 * @brief all parser patterns in one struct
		 */
		struct base_parser_patterns {
			/// @brief pattern to get scalar data
			static inline const auto scalarsPattern = pcre2cpp::regex(R"(^\s*+(?<scalar>(?>bool|double|float|u?int))\s*+$)");

			/// @brief pattern to get vec data
			static inline const auto vecPattern		= pcre2cpp::regex(R"(^\s*+(?>(?<scalar>[idbuf])?vec(?<length>[2-4]))\s*+$)");

			/// @brief pattern to get mat data
			static inline const auto matPattern =
			  pcre2cpp::regex(R"(^\s*+(?>(?<scalar>[idbuf])?mat(?<cols>[2-4])(?>(?:x(?<rows>[2-4]))?))\s*+$)");

			/// @brief pattern to get variable data
			static inline const auto variablePattern = pcre2cpp::regex(
			  R"(^\s*+(?>(?<type>[a-zA-Z_]\w*+))\s++(?>(?<name>[a-zA-Z_]\w*+))\s*+(?>(?<array>\[\s*+(?<count>\d+)?\s*+\]))?\s*+(?>;?)\s*+$)"
			);

			/// @brief pattern to get multiple variables
			static inline const auto multiVariablesPattern = pcre2cpp::regex(
			  R"((?>(?<=^|;))\s*+(?<var>[a-zA-Z_]\w*+\s++[a-zA-Z_]\w*+(?>(?:\s*+\[\s*+\d*+\s*+\])*+))\s*+(?:;|$))",
			  pcre2cpp::compile_options_bits::Multiline
			);

			/// @brief pattern to get struct data
			static inline const auto structPattern =
			  pcre2cpp::regex(R"(^\s*+(?>(?>struct)\s++(?>(?<name>[a-zA-Z_]\w*+))\s*+(?>\{(?<body>[^}]*+)\})\s*+;)\s*+$)");

			/// @brief pattern to get multiple structs
			static inline const auto multiStructsPattern = pcre2cpp::regex(
			  R"(\s*+(?<struct>(?>struct|layout\s*+\([^)]*+\)\s*+(?>uniform|(?:\w++\s++)?buffer))\s++(?>(?:[a-zA-Z_]\w*+))\s*+\{(?>[^}]*+)\}\s*+(?>(?:\w*+\s*+));\s*+))",
			  pcre2cpp::compile_options_bits::Multiline
			);
		};

		#pragma endregion

		#pragma region UBO_PARSER_PATTERN
		/**
		 * @ingroup utils
		 * @brief container for ubo parser pattern
		 */
		#if _GLSL_STRUCT_HAS_CXX20
		template<glsl_layout Layout>
		#else
		template<class Layout, std::enable_if_t<is_glsl_layout_v<Layout>, bool> = true>
		#endif
		struct ubo_parser_pattern {
		};

		/**
		 * @ingroup utils
		 * @brief template overload of container for ubo parser pattern for std140 layout
		 */
		template<>
		struct ubo_parser_pattern<std140_layout> {
			/// @brief pattern for std140 ubo
			static inline const auto pattern = pcre2cpp::regex(
			  R"(^\s*+layout\s*+\((?>(?:(?:std140|set\s*+=\s*+\d++)\s*+,\s*+)?)\s*+binding\s*+=\s*+\d++\s*+\)\s*+uniform\s*+(?<name>[a-zA-Z_]\w*+)\s*+\{(?<body>[^}]*+)\}\s*+(?>(?:[a-zA-Z_]\w*+)?)\s*+;\s*+$)"
			);
		};

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @ingroup utils
		 * @brief concept for check if layout can be used as ubo
		 */
		template<class Layout>
		concept has_ubo_pattern = requires {
			{ ubo_parser_pattern<Layout>::pattern } -> std::convertible_to<pcre2cpp::regex>;
		};

		/**
		 * @ingroup utils
		 * @brief returns true if layout can be used as ubo
		 */
		template<class Layout>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_ubo_pattern_v = has_ubo_pattern<Layout>;

		#else
		/**
		 * @ingroup utils
		 * @brief struct checking if layout can be used as ubo
		 */
		template<class Layout, class = void>
		struct has_ubo_pattern : std::false_type {};

		template<class Layout>
		struct has_ubo_pattern<Layout,
		  std::void_t<std::enable_if_t<std::is_convertible_v<decltype(ubo_parser_pattern<Layout>::pattern), pcre2cpp::regex> > > >
			: std::true_type {};

		/**
		 * @ingroup utils
		 * @brief returns true if layout can be used as ubo
		 */
		template<class Layout>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_ubo_pattern_v = has_ubo_pattern<Layout>::value;
		#endif
		#pragma endregion

		#pragma region SSBO_PARSER_PATTERN
		/**
		 * @ingroup utils
		 * @brief container for ssbo parser pattern
		 */
		#if _GLSL_STRUCT_HAS_CXX20
		template<glsl_layout Layout>
		#else
		template<class Layout, std::enable_if_t<is_glsl_layout_v<Layout>, bool> = true>
		#endif
		struct ssbo_parser_pattern {
		};

		/**
		 * @ingroup utils
		 * @brief template overload of container for ssbo parser pattern for std140 layout
		 */
		template<>
		struct ssbo_parser_pattern<std140_layout> {
			static inline const auto pattern = pcre2cpp::regex(
			  R"(^\s*+layout\s*+\((?>std140(?>\s*+,\s*+set\s*+=\s*+\d++)?)\s*+,\s*+binding\s*+=\s*+\d++\s*+\)\s*+(?>(?:[a-zA-Z]++\s++)?buffer)\s++(?<name>[a-zA-Z_]\w*+)\s*+\{(?<body>[^}]*+)\}\s*+(?>(?:[a-zA-Z_]\w*+)?)\s*+;\s*+$)"
			);
		};

		/**
		 * @ingroup utils
		 * @brief template overload of container for ssbo parser pattern for std430 layout
		 */
		template<>
		struct ssbo_parser_pattern<std430_layout> {
			static inline const auto pattern = pcre2cpp::regex(
			  R"(^\s*+layout\s*+\((?>std430(?>\s*+,\s*+set\s*+=\s*+\d++)?)\s*+,\s*+binding\s*+=\s*+\d++\s*+\)\s*+(?>(?:[a-zA-Z]++\s++)?buffer)\s++(?<name>[a-zA-Z_]\w*+)\s*+\{(?<body>[^}]*+)\}\s*+(?>(?:[a-zA-Z_]\w*+)?)\s*+;\s*+$)"
			);
		};

		/**
		 * @ingroup utils
		 * @brief template overload of container for ssbo parser pattern for scalar layout
		 */
		template<>
		struct ssbo_parser_pattern<scalar_layout> {
			static inline const auto pattern = pcre2cpp::regex(
			  R"(^\s*+layout\s*+\((?>scalar(?>\s*+,\s*+set\s*+=\s*+\d++)?)\s*+,\s*+binding\s*+=\s*+\d++\s*+\)\s*+(?>(?:[a-zA-Z]++\s++)?buffer)\s++(?<name>[a-zA-Z_]\w*+)\s*+\{(?<body>[^}]*+)\}\s*+(?>(?:[a-zA-Z_]\w*+)?)\s*+;\s*+$)"
			);
		};

		#if _GLSL_STRUCT_HAS_CXX20
		/**
		 * @ingroup utils
		 * @brief concept for check if layout can be used as ssbo
		 */
		template<class Layout>
		concept has_ssbo_pattern = requires {
			{ ssbo_parser_pattern<Layout>::pattern } -> std::convertible_to<pcre2cpp::regex>;
		};

		/**
		 * @ingroup utils
		 * @brief returns true if layout can be used as ssbo
		 */
		template<class Layout>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_ssbo_pattern_v = has_ssbo_pattern<Layout>;

		#else
		/**
		 * @ingroup utils
		 * @brief struct checking if layout can be used as ssbo
		 */
		template<class Layout, class = void>
		struct has_ssbo_pattern : std::false_type {};

		template<class Layout>
		struct has_ssbo_pattern<
		  Layout,
		  std::void_t<std::enable_if_t<std::is_convertible_v<decltype(ssbo_parser_pattern<Layout>::pattern), pcre2cpp::regex> > >
		> : std::true_type {};

		/**
		 * @ingroup utils
		 * @brief returns true if layout can be used as ssbo
		 */
		template<class Layout>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_ssbo_pattern_v = has_ssbo_pattern<Layout>::value;
		#endif
		#pragma endregion
	} // namespace utils

		/**
		 * @ingroup glslstruct
		 * @brief base template class of structs parser
		 * @tparam Layout layout type for parser
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_layout Layout>
		#else
	template<class Layout, std::enable_if_t<utils::is_glsl_layout_v<Layout>, bool> = true>
		#endif
	class _GLSL_STRUCT_EXPORT base_parser {
	private:
		/// @brief layout type
		using _layout_type									  = Layout;
		/// @brief structure type
		using _struct_type									  = base_struct<Layout>;

		using _base_patterns								  = utils::base_parser_patterns;
		using _ubo_pattern									  = utils::ubo_parser_pattern<_layout_type>;
		using _ssbo_pattern									  = utils::ssbo_parser_pattern<_layout_type>;

		/// @brief returns true if given layout can be used by ubo
		static _GLSL_STRUCT_CONSTEXPR17 bool has_ubo_pattern  = utils::has_ubo_pattern_v<_layout_type>;
		/// @brief returns true if given layout can be used by ssbo
		static _GLSL_STRUCT_CONSTEXPR17 bool has_ssbo_pattern = utils::has_ssbo_pattern_v<_layout_type>;

		/// @brief loaded and not converted to layouts structs bodies
		std::unordered_map<std::string, std::string> _structsBodies;
		/// @brief loaded and converted to layouts structs
		std::unordered_map<std::string, _layout_type> _structsLayouts = {};

		#pragma region GENERAL_VARIABLE_FUNCTIONS

		/// @brief returns ValueType based on type in string
		static _GLSL_STRUCT_CONSTEXPR17 ValueType _get_value_type_from_string(const std::string_view type) noexcept {
				// Option for vec and mat
				if (type.empty() || type[0] == 'f') { return ValueType::Float; }

				if (type[0] == 'i') { return ValueType::Int; }

				if (type[0] == 'u') { return ValueType::Uint; }

				if (type[0] == 'd') { return ValueType::Double; }

				if (type[0] == 'b') { return ValueType::Bool; }

			[[unlikely]] return ValueType::Int;
		}

		/// @brief adds variable of type T to struct or layout (if count != 0 then it adds array of variables of type T)
		template<class T, class S>
		static _GLSL_STRUCT_CONSTEXPR17 S& _add_variable(S& ref, const std::string_view name, const size_t count) {
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<S, _layout_type> || std::is_same_v<S, _struct_type>) {
						if (count != 0) { ref.template add<std::vector<T> >(name, count); }
						else { ref.template add<T>(name); }
				}
			return ref;
		}

		#pragma endregion

		#pragma region SCALAR_VARIABLE

		/// @brief adds scalar with name and of type based on ValueType to struct or layout
		template<class S>
		static _GLSL_STRUCT_CONSTEXPR17 S& _add_scalar_variable(S& ref, const ValueType type, const std::string_view name,
		  const size_t count) {
				switch (type) {
				case ValueType::Bool:	return _add_variable<bool>(ref, name, count);
				default:
				case ValueType::Int:	return _add_variable<int>(ref, name, count);
				case ValueType::Uint:	return _add_variable<unsigned int>(ref, name, count);
				case ValueType::Float:	return _add_variable<float>(ref, name, count);
				case ValueType::Double: return _add_variable<double>(ref, name, count);
				}
		}

		#pragma endregion

		#pragma region VEC_VARIABLE

		/// @brief adds vec with name and with provided length to struct or layout
		template<class T, class S>
		static _GLSL_STRUCT_CONSTEXPR17 S& _add_vec_variable(S& ref, const size_t length, const std::string_view name,
		  const size_t count) {
				switch (length) {
				default:
				case 2:	 return _add_variable<glm::vec<2, T> >(ref, name, count);
				case 3:	 return _add_variable<glm::vec<3, T> >(ref, name, count);
				case 4:	 return _add_variable<glm::vec<4, T> >(ref, name, count);
				}
		}

		/// @brief adds vec with name and of type based on ValueType and with provided length to struct or layout
		template<class S>
		static _GLSL_STRUCT_CONSTEXPR17 S& _add_vec_variable(S& ref, const ValueType type, const size_t length,
		  const std::string_view name, const size_t count) {
				switch (type) {
				case ValueType::Bool:	return _add_vec_variable<bool>(ref, length, name, count);
				default:
				case ValueType::Int:	return _add_vec_variable<int>(ref, length, name, count);
				case ValueType::Uint:	return _add_vec_variable<unsigned int>(ref, length, name, count);
				case ValueType::Float:	return _add_vec_variable<float>(ref, length, name, count);
				case ValueType::Double: return _add_vec_variable<double>(ref, length, name, count);
				}
		}

		#pragma endregion

		#pragma region MAT_VARIABLE

		/// @brief adds mat with name and with provided rows to struct or layout
		template<class T, size_t Cols, class S>
		static _GLSL_STRUCT_CONSTEXPR17 S& _add_mat_variable(S& ref, const size_t rows, const std::string_view name,
		  const size_t count) {
				switch (rows) {
				default:
				case 2:	 return _add_variable<glm::mat<Cols, 2, T> >(ref, name, count);
				case 3:	 return _add_variable<glm::mat<Cols, 3, T> >(ref, name, count);
				case 4:	 return _add_variable<glm::mat<Cols, 4, T> >(ref, name, count);
				}
		}

		/// @brief adds mat with name and with provided columns and rows to struct or layout
		template<class T, class S>
		static _GLSL_STRUCT_CONSTEXPR17 S& _add_mat_variable(S& ref, const size_t cols, const size_t rows,
		  const std::string_view name, const size_t count) {
				switch (cols) {
				default:
				case 2:	 return _add_mat_variable<T, 2>(ref, rows, name, count);
				case 3:	 return _add_mat_variable<T, 3>(ref, rows, name, count);
				case 4:	 return _add_mat_variable<T, 3>(ref, rows, name, count);
				}
		}

		/// @brief adds mat with name and of type based on ValueType and with provided columns and rows to struct or layout
		template<class S>
		static _GLSL_STRUCT_CONSTEXPR17 S& _add_mat_variable(S& ref, const ValueType type, const size_t cols, const size_t rows,
		  const std::string_view name, const size_t count) {
				switch (type) {
				case ValueType::Bool:	return _add_mat_variable<bool>(ref, cols, rows, name, count);
				default:
				case ValueType::Int:	return _add_mat_variable<int>(ref, cols, rows, name, count);
				case ValueType::Uint:	return _add_mat_variable<unsigned int>(ref, cols, rows, name, count);
				case ValueType::Float:	return _add_mat_variable<float>(ref, cols, rows, name, count);
				case ValueType::Double: return _add_mat_variable<double>(ref, cols, rows, name, count);
				}
		}

		#pragma endregion

		#pragma region EXTRACT_STRUCTS

		/// @brief extracts name and body of structure from string
		_GLSL_STRUCT_CONSTEXPR20 std::pair<std::string, std::string> _extract_struct_data(const std::string_view structStr) {
			pcre2cpp::match_result result;
				if (_base_patterns::structPattern.match(structStr, result)) {
					return std::make_pair(result.get_sub_result_value("name"), result.get_sub_result_value("body"));
				}

				if _GLSL_STRUCT_CONSTEXPR17 (has_ubo_pattern) {
						if (_ubo_pattern::pattern.match(structStr, result)) {
							return std::make_pair(result.get_sub_result_value("name"), result.get_sub_result_value("body"));
						}
				}

				if _GLSL_STRUCT_CONSTEXPR17 (has_ssbo_pattern) {
						if (_ssbo_pattern::pattern.match(structStr, result)) {
							return std::make_pair(result.get_sub_result_value("name"), result.get_sub_result_value("body"));
						}
				}

			glsl_struct_assert(false, "Provided string '{}' was not a struct definition", structStr);
			return std::make_pair(std::string(), std::string());
		}

		/// @brief extracts struct strings from structs list
		_GLSL_STRUCT_CONSTEXPR20 std::vector<std::string> _extract_structs(const std::string_view structsStr) {
			std::vector<pcre2cpp::match_result> results;
				if (!_base_patterns::multiStructsPattern.match_all(structsStr, results)) {
					glsl_struct_assert(false, "Provided structs string '{}' didn't have any struct definition", structsStr);
					return {};
				}

			std::vector<std::string> structs;
			structs.reserve(results.size());
				for (const auto& result : results) { structs.push_back(result.get_sub_result_value("struct")); }
			return structs;
		}

		#pragma endregion

		#pragma region LAYOUT

		/// @brief creates layout with variables in vars list
		_GLSL_STRUCT_CONSTEXPR17 _layout_type _create_layout(const std::string_view varsStr) {
			_layout_type layout;
			return _add_variables(layout, varsStr);
		}

		/// @brief loads layout from structsBodies to structsLayouts and returns if there is a layout with given name in structsLayouts
		_GLSL_STRUCT_CONSTEXPR17 bool _load_layout(const std::string_view structName) {
				if (!_contains_struct_layout(structName) && _contains_struct_body(structName)) {
					_structsLayouts.emplace(structName.data(), _create_layout(_structsBodies.at(structName.data())));
					_structsBodies.erase(structName.data());
				}

			return _contains_struct_layout(structName);
		}

		#pragma endregion

		#pragma region CONTAINS

		/// @brief returns true if struct with given name is in _structsBodies
		_GLSL_STRUCT_CONSTEXPR17 bool _contains_struct_body(const std::string_view name) {
		#if _GLSL_STRUCT_HAS_CXX20
			return _structsBodies.contains(name.data());
		#else
			return _structsBodies.find(name.data()) != _structsBodies.end();
		#endif
		}

		/// @brief returns true if struct with given name is in _structsLayouts
		_GLSL_STRUCT_CONSTEXPR17 bool _contains_struct_layout(const std::string_view name) {
		#if _GLSL_STRUCT_HAS_CXX20
			return _structsLayouts.contains(name.data());
		#else
			return _structsLayouts.find(name.data()) != _structsLayouts.end();
		#endif
		}

		#pragma endregion

		#pragma region PARSE_AND_ADD_VARIABLES

		/// @brief adds variable of type given in string and with name and count
		template<class S>
		_GLSL_STRUCT_CONSTEXPR20 S& _add_variable(S& ref, const std::string_view type, const std::string_view name,
		  const size_t count = 0) {
			pcre2cpp::match_result result;

				// Check if scalar type
				if (_base_patterns::scalarsPattern.match_at(type, result)) {
					return _add_scalar_variable(ref, _get_value_type_from_string(result.get_sub_result_value("scalar")), name,
					  count);
				}

				// Check if vec type
				if (_base_patterns::vecPattern.match_at(type, result)) {
					size_t length;
					mstd::strtounum(result.get_sub_result_value("length"), length);

					return _add_vec_variable(ref, _get_value_type_from_string(result.get_sub_result_value("scalar")), length,
					  name, count);
				}

				// Check if mat type
				if (_base_patterns::matPattern.match_at(type, result)) {
					const ValueType valueType = _get_value_type_from_string(result.get_sub_result_value("scalar"));

					size_t cols;
					mstd::strtounum(result.get_sub_result_value("cols"), cols);

						if (!result.has_sub_value("rows")) { return _add_mat_variable(ref, valueType, cols, cols, name, count); }

					size_t rows;
					mstd::strtounum(result.get_sub_result_value("rows"), rows);

					return _add_mat_variable(ref, valueType, cols, rows, name, count);
				}

				// Check if struct type
				if (_load_layout(type.data())) {
					ref.add(name, _structsLayouts.at(type.data()));
					return ref;
				}

			glsl_struct_assert(false, "Provided type str '{}' was not a correct variable type", type);
			return ref;
		}

		/// @brief adds variable given in string
		template<class S>
		_GLSL_STRUCT_CONSTEXPR20 S& _add_variable(S& ref, const std::string_view varStr) {
			pcre2cpp::match_result result;
				if (!_base_patterns::variablePattern.match_at(varStr, result)) {
					glsl_struct_assert(false, "Provided var str '{}' was not a correct variable", varStr);
					return ref;
				}

			size_t count = 0;
				if (result.has_sub_value("count")) { mstd::strtounum(result.get_sub_result_value("count"), count); }
				else if (result.has_sub_value("array")) { count = 1; }

			return _add_variable(ref, result.get_sub_result_value("type"), result.get_sub_result_value("name"), count);
		}

		/// @brief adds variables given in var list string
		template<class S>
		_GLSL_STRUCT_CONSTEXPR20 S& _add_variables(S& ref, const std::string_view varsStr) {
			std::vector<pcre2cpp::match_result> results;
				if (!_base_patterns::multiVariablesPattern.match_all(varsStr, results)) {
					glsl_struct_assert(false, "Provided vars str '{}' was not a correct variables list", varsStr);
					return ref;
				}

				for (const auto& result : results) { _add_variable(ref, result.get_sub_result_value("var")); }

			return ref;
		}

		#pragma endregion

	public:
		/// @brief default constructor
		_GLSL_STRUCT_CONSTEXPR17 base_parser() = default;

		#pragma region VARIABLE_PARSER

		/// @brief adds variable array to layout
		_GLSL_STRUCT_CONSTEXPR17 _layout_type& add_variable(_layout_type& layoutRef, const std::string_view type,
		  const std::string_view name, const size_t count) {
			return _add_variable(layoutRef, type, name, count);
		}

		/// @brief adds variable array to struct
		_GLSL_STRUCT_CONSTEXPR17 _struct_type& add_variable(_struct_type& structRef, const std::string_view type,
		  const std::string_view name, const size_t count) {
			return _add_variable(structRef, type, name, count);
		}

		/// @brief adds variable to layout
		_GLSL_STRUCT_CONSTEXPR17 _layout_type& add_variable(_layout_type& layoutRef, const std::string_view type,
		  const std::string_view name) {
			return _add_variable(layoutRef, type, name);
		}

		/// @brief adds variable to struct
		_GLSL_STRUCT_CONSTEXPR17 _struct_type& add_variable(_struct_type& structRef, const std::string_view type,
		  const std::string_view name) {
			return _add_variable(structRef, type, name);
		}

		/// @brief adds variable to layout
		_GLSL_STRUCT_CONSTEXPR17 _layout_type& add_variable(_layout_type& layoutRef, const std::string_view varStr) {
			return _add_variable(layoutRef, varStr);
		}

		/// @brief adds variable to struct
		_GLSL_STRUCT_CONSTEXPR17 _struct_type& add_variable(_struct_type& structRef, const std::string_view varStr) {
			return _add_variable(structRef, varStr);
		}

		/// @brief adds variables to layout
		_GLSL_STRUCT_CONSTEXPR17 _layout_type& add_variables(_layout_type& layoutRef, const std::string_view varsStr) {
			return _add_variables(layoutRef, varsStr);
		}

		/// @brief adds variables to struct
		_GLSL_STRUCT_CONSTEXPR17 _struct_type& add_variables(_struct_type& structRef, const std::string_view varsStr) {
			return _add_variables(structRef, varsStr);
		}

		#pragma endregion

		#pragma region STRUCTS_PARSER

		/// @brief adds struct definition
		_GLSL_STRUCT_CONSTEXPR17 void add_struct_definition(const std::string_view name, const std::string_view varsStr) {
			_structsBodies.emplace(name.data(), varsStr.data());
		}

		/// @brief adds struct definition
		_GLSL_STRUCT_CONSTEXPR17 void add_struct_definition(const std::string_view structStr) {
			const auto [name, body] = _extract_struct_data(structStr);
			add_struct_definition(name, body);
		}

		/// @brief adds structs definitions
		_GLSL_STRUCT_CONSTEXPR17 void add_structs_definitions(const std::string_view structsStr) {
				for (const auto& structStr : _extract_structs(structsStr)) { add_struct_definition(structStr); }
		}

		/// @brief creates layout with given variables
		_GLSL_STRUCT_CONSTEXPR17 _layout_type create_struct_layout(const std::string_view varsStr) {
			_layout_type layoutValue;
			return add_variables(layoutValue, varsStr);
		}

		/// @brief creates struct with given variables
		_GLSL_STRUCT_CONSTEXPR17 _struct_type create_struct(const std::string_view varsStr) {
			return _struct_type(create_struct_layout(varsStr));
		}

		/// @brief get layout of struct earlier defined
		_GLSL_STRUCT_CONSTEXPR17 _layout_type get_struct_layout(const std::string_view structName) {
				if (!_load_layout(structName)) {
					glsl_struct_assert(false, "Couldn't find definition for struct with name '{}'", structName);
					return {};
				}

			return _structsLayouts.at(structName.data());
		}

		/// @brief get struct earlier defined
		_GLSL_STRUCT_CONSTEXPR17 _struct_type get_struct(const std::string_view structName) {
			return _struct_type(get_struct_layout(structName));
		}

		/// @brief creates layouts from strings array
		_GLSL_STRUCT_CONSTEXPR20 std::vector<_layout_type> create_structs_layouts(const std::vector<std::string>& structs) {
			std::vector<_layout_type> structLayouts;
			structLayouts.reserve(structs.size());
				for (const auto& structStr : structs) {
					const auto [name, body] = _extract_struct_data(structStr);
					structLayouts.push_back(create_struct_layout(body));
				}
			return structLayouts;
		}

		/// @brief creates layouts from string
		_GLSL_STRUCT_CONSTEXPR17 std::vector<_layout_type> create_structs_layouts(const std::string_view structsStr) {
			return create_structs_layouts(_extract_structs(structsStr));
		}

		/// @brief creates structs from strings array
		_GLSL_STRUCT_CONSTEXPR17 std::vector<_struct_type> create_structs(const std::vector<std::string>& structsStrs) {
			const auto layouts = create_structs_layouts(structsStrs);
			std::vector<_struct_type> structs;
			structs.reserve(layouts.size());

				for (const auto& layout : layouts) { structs.push_back(_struct_type(layout)); }

			return structs;
		}

		/// @brief creates structs from string
		_GLSL_STRUCT_CONSTEXPR17 std::vector<_struct_type> create_structs(const std::string_view structsStr) {
			return create_structs(_extract_structs(structsStr));
		}

		/// @brief gets layouts with names in array
		_GLSL_STRUCT_CONSTEXPR20 std::vector<_layout_type> get_structs_layouts(const std::vector<std::string>& structsNames) {
			std::vector<_layout_type> structLayouts;
			structLayouts.reserve(structsNames.size());

				for (const auto& structName : structsNames) { structLayouts.push_back(get_struct_layout(structName)); }

			return structLayouts;
		}

		/// @brief gets structs with names in array
		_GLSL_STRUCT_CONSTEXPR20 std::vector<_struct_type> get_structs(const std::vector<std::string_view>& structsNames) {
			std::vector<_struct_type> structs;
			structs.reserve(structsNames.size());

				for (const auto& structName : structsNames) { structs.push_back(get_struct(structName)); }

			return structs;
		}

		#pragma endregion
	};

	/**
	 * @ingroup glslstruct
	 * @brief parser for std140 layout
	 */
	using std140_parser = base_parser<std140_layout>;
	/**
	 * @ingroup glslstruct
	 * @brief parser for std430 layout
	 */
	using std430_parser = base_parser<std430_layout>;
	/**
	 * @ingroup glslstruct
	 * @brief parser for scalar layout
	 */
	using scalar_parser = base_parser<scalar_layout>;

} // namespace glslstruct

	#endif
#endif