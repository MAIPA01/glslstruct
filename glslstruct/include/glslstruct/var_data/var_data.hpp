/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_STRUCT_ELEM_DATA_HPP_
	#define _GLSL_STRUCT_STRUCT_ELEM_DATA_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/types.hpp>

namespace glslstruct {
	/**
	 * @brief container for all variable data
	 * @ingroup glslstruct
	 */
	class _GLSL_STRUCT_EXPORT var_data {
	private:
		friend struct std::hash<var_data>;

		#if _GLSL_STRUCT_HAS_TYPES
		/// @brief type of variable
		base_type_handle _type;
		#else
		/// @brief size of variable type
		size_t _size;
		#endif
		/// @brief variable offset
		size_t _offset;
		/// @brief additional variable padding
		size_t _padding;
		/// @brief indicates if variable is in top level
		bool _isTopLevel;

	public:
		#if _GLSL_STRUCT_HAS_TYPES
		/// @brief constructor with offset, var type and padding
		var_data(size_t offset, const base_type_handle& type, bool isTopLevel, size_t padding) noexcept;
		#else
		/// @brief constructor with offset, var type size and padding
		var_data(size_t offset, size_t size, bool isTopLevel, size_t padding) noexcept;
		#endif
		/// @brief default copy constructor
		var_data(const var_data& other) noexcept;
		/// @brief default move constructor
		var_data(var_data&& other) noexcept;
		/// @brief default destructor
		~var_data() noexcept;

		/// @brief default copy assign operator
		var_data& operator=(const var_data& other) noexcept;
		/// @brief default move assign operator
		var_data& operator=(var_data&& other) noexcept;

		/// @brief padding setter
		void set_padding(size_t padding) noexcept;
		/// @brief sets top level
		void set_is_top_level(bool topLevel) noexcept;

		#if _GLSL_STRUCT_HAS_TYPES
		/// @brief returns type of variable
		[[nodiscard]] const base_type_handle& get_type() const noexcept;
		#endif
		/// @brief returns var offset
		[[nodiscard]] size_t get_offset() const noexcept;
		/// @brief returns var total size (size + padding)
		[[nodiscard]] size_t get_total_size() const noexcept;
		/// @brief returns var size
		[[nodiscard]] size_t get_size() const noexcept;
		/// @brief returns var padding
		[[nodiscard]] size_t get_padding() const noexcept;
		/// @brief returns isTopLevel
		[[nodiscard]] bool is_top_level() const noexcept;

		/// @brief checks if this var_data is equal to other var_data
		[[nodiscard]] bool operator==(const var_data& other) const noexcept;
		/// @brief checks if this var_data is not equal to other var_data
		[[nodiscard]] bool operator!=(const var_data& other) const noexcept;
	};
} // namespace glslstruct

/**
 * @brief std::hash overload for var_data
 * @ingroup glsl_types
 */
template<>
struct _GLSL_STRUCT_EXPORT std::hash<glslstruct::var_data> {
	[[nodiscard]] size_t operator()(const glslstruct::var_data& value) const noexcept;
};
	#endif
#endif