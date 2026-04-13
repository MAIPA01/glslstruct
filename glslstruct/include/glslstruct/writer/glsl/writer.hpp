/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_GLSL_WRITER_HPP_
	#define _GLSL_STRUCT_GLSL_WRITER_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with defined GLSL_STRUCT_DISABLE_TYPES!"
);
	#else

		#include <glslstruct/type/containers/struct_type.hpp>
		#include <glslstruct/utils/assert.hpp>

namespace glslstruct::utils {
	/**
	 * @brief type visitor which returns variable type
	 * @ingroup utils
	 */
	class glsl_var_type_getter {
	private:
		/// @brief result
		std::string _result;
		/// @brief defined structs names
		mstd::ordered_map<struct_type, std::string> _structsNames;

	public:
		/// @brief constructor with defined structs names
		explicit glsl_var_type_getter(const mstd::ordered_map<struct_type, std::string>& structsNames);

		/// @brief type visitor
		template<class Type>
		void visit(Type&& varType) {
			using T = std::decay_t<Type>;
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, array_type>) { varType.get_type()->accept(*this); }
				else if _GLSL_STRUCT_CONSTEXPR17 (!std::is_same_v<T, struct_type>) { _result = varType.to_string(); }
				else {
					glsl_struct_assert(_structsNames.contains(varType), "Struct not defined!!");
					_result = _structsNames.at(varType);
				}
		}

		/// @brief returns result
		[[nodiscard]] const std::string& get_result() const noexcept;
	};

	/**
	 * @ingroup utils
	 * @brief returns variable type
	 * @param varType variable of which type is returned
	 * @param structsNames defined structs names
	 */
	[[nodiscard]] std::string get_glsl_var_type_string(const base_type_handle& varType,
	  const mstd::ordered_map<struct_type, std::string>& structsNames);

	/**
	 * @ingroup utils
	 * @brief type visitor which gets array and sub arrays counts in format [0][1]...
	 */
	class glsl_array_count_getter {
	private:
		/// @brief check for variable size arrays
		bool _canBeVariableSize;
		/// @brief result
		std::string _result;

	public:
		explicit glsl_array_count_getter(bool canBeVariableSize) noexcept;

		/// @brief visitor function
		template<class Type>
		void visit(Type&& varType) {
			using T = std::decay_t<Type>;
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, array_type>) {
						if (_canBeVariableSize) {
							_result			   = fmt::format("{}[]", _result);
							_canBeVariableSize = false;
						}
						else { _result = fmt::format("{}[{}]", _result, varType.get_count()); }
				}
		}

		/// @brief returns result
		[[nodiscard]] const std::string& get_result() const noexcept;
	};

	/**
	 * @ingroup utils
	 * @brief returns array counts string
	 * @param varType variable for which we get array counts
	 * @param canBeVariableSize check if array size can be variable size
	 */
	[[nodiscard]] std::string get_glsl_array_count_string(const base_type_handle& varType, bool canBeVariableSize);

	/**
	 * @ingroup utils
	 * @brief returns variable line `type name[0][1]...`
	 * @param name name of variable
	 * @param varType type of variable
	 * @param canBeVariableSize check if array size can be variable size
	 * @param structsNames defined structs names
	 */
	[[nodiscard]] std::string get_glsl_variable_string(std::string_view name, const base_type_handle& varType,
	  bool canBeVariableSize, const mstd::ordered_map<struct_type, std::string>& structsNames);

	/**
	 * @ingroup utils
	 * @brief general glsl writer
	 */
	class glsl_writer {
	private:
		/// @brief result
		std::string _result;

		/// @brief struct name generator idx
		size_t _structIdx = 0;
		/// @brief struct name generator idx
		mstd::ordered_map<struct_type, std::string> _uniqueStructs;

		/// @brief unique names of all strucs, UBOs and SSBOs
		std::unordered_set<std::string> _uniqueNames;

		/// @breif returns layout string
		[[nodiscard]] static std::string _get_layout(std::string_view layoutData);

		/// @brief appends struct body to result
		void _append_struct_body(std::string_view structBody);

		/// @brief returns struct body string
		[[nodiscard]] std::string _get_struct_body(std::string_view type, std::string_view name, bool canHaveVariableSizeArray,
		  const mstd::ordered_map<std::string, var_data>& variables);

		/// @brief appends buffer body to result
		void _append_buffer_body(std::string_view layoutData, std::string_view name, std::string_view bufferType,
		  bool canHaveVariableSizeArray, const mstd::ordered_map<std::string, var_data>& variables,
		  std::string_view varName = "");

		/// @brief appends shader storage buffer body (SSBO)
		void _append_shader_storage_buffer_body(std::string_view layoutData, std::string_view name,
		  const mstd::ordered_map<std::string, var_data>& variables, std::string_view varName = "",
		  std::string_view qualifiers = "");

		/// @brief returns true if struct, UBO or SSBO with given name already was appended
		[[nodiscard]] bool _contains_name(std::string_view name);

	public:
		/// @brief default constructor
		glsl_writer();

		/// @brief type visitor
		template<class Type>
		void visit(Type&& varType) {
			using T = std::decay_t<Type>;
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, struct_type>) {
						if (_uniqueStructs.contains(varType)) { return; }

					append_struct(varType);
				}
		}

		/// @brief appends struct with given name
		void append_struct(std::string_view name, const struct_type& structType);

		/// @brief appends struct with generated name
		void append_struct(const struct_type& structType);

		/// @brief appends uniform buffer
		void append_uniform_buffer(std::string_view layoutData, std::string_view name,
		  const mstd::ordered_map<std::string, var_data>& variables, std::string_view varName = "");

		/// @brief appends shader storage buffer
		void append_shader_storage_buffer(std::string_view layoutData, std::string_view name,
		  const mstd::ordered_map<std::string, var_data>& variables, std::string_view varName = "",
		  std::string_view qualifiers = "");

		/// @brief returns result string
		[[nodiscard]] const std::string& to_string() const noexcept;
	};
} // namespace glslstruct::utils

		#include <glslstruct/writer/glsl/opengl/writer.hpp>
		#include <glslstruct/writer/glsl/vulkan/writer.hpp>

	#endif
#endif