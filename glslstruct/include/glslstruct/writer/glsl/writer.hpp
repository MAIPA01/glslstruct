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
		mstd::ordered_map<struct_type, std::string> _structsNames = {};

	public:
		/// @brief constructor with defined structs names
		explicit glsl_var_type_getter(const mstd::ordered_map<struct_type, std::string>& structsNames);

		/// @brief type visitor
		void visit(auto&& varType) {
			using T = std::decay_t<decltype(varType)>;
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
		/// @brief result
		std::string _result = "";

	public:
		/// @brief visitor function
		void visit(auto&& varType) {
			using T = std::decay_t<decltype(varType)>;
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, array_type>) {
					_result = fmt::format("{}[{}]", _result, varType.get_count());
				}
		}

		/// @brief returns result
		[[nodiscard]] const std::string& get_result() const noexcept;
	};

	/**
	 * @ingroup utils
	 * @brief returns array counts string
	 * @param varType variable for which we get array counts
	 */
	[[nodiscard]] std::string get_glsl_array_count_string(const base_type_handle& varType);

	/**
	 * @ingroup utils
	 * @brief returns variable line `type name[0][1]...`
	 * @param name name of variable
	 * @param varType type of variable
	 * @param structsNames defined structs names
	 */
	[[nodiscard]] std::string get_glsl_variable_string(std::string_view name, const base_type_handle& varType,
	  const mstd::ordered_map<struct_type, std::string>& structsNames);
} // namespace glslstruct::utils

		#include <glslstruct/writer/glsl/opengl/writer.hpp>
		#include <glslstruct/writer/glsl/vulkan/writer.hpp>

	#endif
#endif