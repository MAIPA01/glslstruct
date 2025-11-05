#pragma once
#include <glslstruct/templates.hpp>

namespace glslstruct {
	struct standard_variable {};

	template<class T>
	struct offset_variable {
		const T struct_offsets;
		offset_variable(const T& offsets) : struct_offsets(offsets) {}
	};

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<utils::any_standard_or_offset_value T, size_t num>
#else
	template<class T, size_t num,
		std::enable_if_t<(utils::is_any_offset_v<T> || utils::is_any_standard_value_v<T>), bool>>
#endif
	struct std_variable : public std::conditional_t< 
		utils::is_any_offset_v<T>,
		offset_variable<T>,
		standard_variable> {
	public:
		using var_type = T;
		static constexpr const size_t array_size = num;
		static constexpr const bool is_offsets = utils::is_any_offset_v<T>;

#pragma region VARIABLES
		const std::string var_name;
#pragma endregion

#pragma region NORMAL_CONSTRUCTOR
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::any_standard_value T = var_type>
		requires (std::is_same_v<T, var_type>)
#else
		template<class T = var_type, 
			std::enable_if_t<(std::is_same_v<T, var_type> && utils::is_any_standard_value_v<T>), bool> = true>
#endif
		std_variable(const std::string& name) : var_name(name) {}
#pragma endregion

#pragma region OFFSETS_CONSTRUCTOR
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::any_offset T = var_type>
		requires (std::is_same_v<T, var_type>)
#else
		template<class T = var_type, 
			std::enable_if_t<(std::is_same_v<T, var_type> && utils::is_any_offset_v<T>), bool> = true>
#endif
		std_variable(const std::string& name, const T& offsets) : offset_variable<T>(offsets), var_name(name) {}
	};
}