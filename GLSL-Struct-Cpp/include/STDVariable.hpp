#pragma once
#include <templates.hpp>

namespace glsl {
	struct StandardVariable {};

	template<class T>
	struct OffsetVariable {
		const T struct_offsets;
		OffsetVariable(const T& offsets) : struct_offsets(offsets) {}
	};

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<utils::any_standard_or_offset_value T, size_t num>
#else
	template<class T, size_t num,
		std::enable_if_t<(utils::is_any_offset_v<T> || utils::is_any_standard_value_v<T>), bool>>
#endif
	struct STDVariable : public std::conditional_t< 
		utils::is_any_offset_v<T>,
		OffsetVariable<T>,
		StandardVariable> {
	public:
		using var_type = T;
		static constexpr size_t array_size = num;
		static constexpr bool is_offsets = utils::is_any_offset_v<T>;

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
		STDVariable(const std::string& name) : var_name(name) {}
#pragma endregion

#pragma region OFFSETS_CONSTRUCTOR
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::any_offset T = var_type>
		requires (std::is_same_v<T, var_type>)
#else
		template<class T = var_type, 
			std::enable_if_t<(std::is_same_v<T, var_type> && utils::is_any_offset_v<T>), bool> = true>
#endif
		STDVariable(const std::string& name, const T& offsets) : OffsetVariable<T>(offsets), var_name(name) {}
	};
}