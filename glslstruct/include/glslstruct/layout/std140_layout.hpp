/*
* glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

#include <glslstruct/layout/base_layout.hpp>

namespace glslstruct {
	namespace utils {
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_std140_value_type_v = utils::is_glsl_simple_v<T> || utils::is_glsl_std140_layout_v<T>;
		template<class T>
		struct is_glsl_std140_value_type : std::bool_constant<is_glsl_std140_value_type_v<T>> {};

#if _GLSL_STRUCT_HAS_CXX20
		template<class T> concept glsl_std140_value_type = is_glsl_std140_value_type_v<T>;
#endif
	}

	class std140_layout : public base_layout {
	private:
		friend struct std::hash<std140_layout>;

	public:
		using base_layout::add;

		std140_layout() noexcept;
#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_std140_value_type... Ts, size_t... nums>
#else
		template<class... Ts, size_t... nums,
			std::enable_if_t<mstd::all_check_v<utils::is_glsl_std140_value_type, Ts...>, bool> = true>
#endif
		explicit std140_layout(const glsl_variable<Ts, nums>&... vars) noexcept : base_layout(vars...) {}
		std140_layout(const std140_layout& other) noexcept;
		std140_layout(std140_layout&& other) noexcept;
		~std140_layout() noexcept override;

		std140_layout& operator=(const std140_layout& other) noexcept;
		std140_layout& operator=(std140_layout&& other) noexcept;

#pragma region ADD_STRUCT
		[[nodiscard]] size_t add(const std::string_view& name, const std140_layout& structLayout);

		[[nodiscard]] std::vector<size_t> add(const std::string_view& name, const std140_layout& structLayout, size_t count);
#pragma endregion

		[[nodiscard]] bool operator==(const std140_layout& other) const noexcept;
		[[nodiscard]] bool operator!=(const std140_layout& other) const noexcept;
	};
}

template<>
struct std::hash<glslstruct::std140_layout> {
	[[nodiscard]] size_t operator()(const glslstruct::std140_layout& std140Layout) const noexcept;
};

#endif