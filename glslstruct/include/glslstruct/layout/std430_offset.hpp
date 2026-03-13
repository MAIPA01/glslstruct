/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/std_offset.hpp>

namespace glslstruct {
	namespace utils {
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_std430_value_type_v = utils::is_glsl_simple_value_v<T> || utils::is_glsl_offset430_v<T>;
		template<class T>
		struct is_glsl_std430_value_type : std::bool_constant<is_glsl_std430_value_type_v<T>> {};

#if _GLSL_STRUCT_HAS_CXX20
		template<class T> concept glsl_std430_value_type = is_glsl_std430_value_type_v<T>;
#endif
	}

	class std430_offset : public std_offset {
	private:
		friend struct std::hash<std430_offset>;

		[[nodiscard]] size_t _get_array_elem_base_aligement(size_t baseAligement) const noexcept override;

	public:
		using std_offset::add;

		std430_offset() noexcept = default;
#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_std430_value_type... Ts, size_t... nums>
#else
		template<class... Ts, size_t... nums,
			std::enable_if_t<mstd::all_check_v<utils::is_glsl_std430_value_type, Ts...>, bool> = true>
#endif
		std430_offset(const std_variable<Ts, nums>&... vars) noexcept
			: std_offset(vars...) {
		}
		std430_offset(const std430_offset& other) noexcept = default;
		std430_offset(std430_offset&& other) noexcept = default;
		virtual ~std430_offset() noexcept = default;

		std430_offset& operator=(const std430_offset& other) noexcept = default;
		std430_offset& operator=(std430_offset&& other) noexcept = default;

#pragma region ADD_STRUCT
		[[nodiscard]] size_t add(const std::string& name, const std430_offset& structTemplate);

		[[nodiscard]] std::vector<size_t> add(const std::string& name, const std430_offset& structTemplate, size_t count);
#pragma endregion

		[[nodiscard]] size_t base_aligement() const noexcept override;

		[[nodiscard]] bool operator==(const std430_offset& other) const noexcept _GLSL_STRUCT_DEFAULT20;
		[[nodiscard]] bool operator!=(const std430_offset& other) const noexcept _GLSL_STRUCT_DEFAULT20;
	};
}

template<>
struct std::hash<glslstruct::std430_offset> {
	[[nodiscard]] size_t operator()(const glslstruct::std430_offset& stdOff) noexcept;
};