/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/templates.hpp>
#include <glslstruct/std_offset.hpp>

namespace glslstruct {
	namespace utils {
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_std140_value_type_v = utils::is_glsl_simple_value_v<T> || utils::is_glsl_offset140_v<T>;
		template<class T>
		struct is_glsl_std140_value_type : std::bool_constant<is_glsl_std140_value_type_v<T>> {};

#if _GLSL_STRUCT_HAS_CXX20
		template<class T> concept glsl_std140_value_type = is_glsl_std140_value_type_v<T>;
#endif
	}

	class std140_offset : public std_offset {
	private:
		friend struct std::hash<std140_offset>;

	public:
		using std_offset::add;

		std140_offset() noexcept = default;
#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_std140_value_type... Ts, size_t... nums>
#else
		template<class... Ts, size_t... nums,
			std::enable_if_t<mstd::all_check_v<utils::is_glsl_std140_value_type, Ts...>, bool> = true>
#endif
		std140_offset(const std_variable<Ts, nums>&... vars) noexcept
			: std_offset(vars...) {
		}
		std140_offset(const std140_offset& other) noexcept = default;
		std140_offset(std140_offset&& other) noexcept = default;
		virtual ~std140_offset() noexcept = default;

		std140_offset& operator=(const std140_offset& other) noexcept = default;
		std140_offset& operator=(std140_offset&& other) noexcept = default;

#pragma region ADD_STRUCT
		[[nodiscard]] size_t add(const std::string& name, const std140_offset& structOffsets);

		[[nodiscard]] std::vector<size_t> add(const std::string& name, const std140_offset& structOffsets, size_t count);
#pragma endregion

		[[nodiscard]] bool operator==(const std140_offset& other) const noexcept _GLSL_STRUCT_DEFAULT20;
		[[nodiscard]] bool operator!=(const std140_offset& other) const noexcept _GLSL_STRUCT_DEFAULT20;
	};
}

template<>
struct std::hash<glslstruct::std140_offset> {
	size_t operator()(const glslstruct::std140_offset& std140Off);
};