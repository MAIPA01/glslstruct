#pragma once
#include <templates.h>

namespace glsl {
	class STD140Offsets;
	class STD430Offsets;

	namespace extra {
		template<class T> static constexpr bool offsets140_check_v = std::is_same_v<T, STD140Offsets>;
		template<class T> static constexpr bool offsets430_check_v = std::is_same_v<T, STD430Offsets>;
		template<class T> static constexpr bool offsets_check_v = offsets140_check_v<T> || offsets430_check_v<T>;

		template<class T, class Ret = void> using offsets140_enable_if_t = std::enable_if_t<offsets140_check_v<T>, Ret>;
		template<class T, class Ret = void> using offsets430_enable_if_t = std::enable_if_t<offsets430_check_v<T>, Ret>;
		template<class T, class Ret = void> using offsets_enable_if_t = std::enable_if_t<offsets_check_v<T>, Ret>;
	}

	template<class T, size_t num = 0>
	struct STDVariable {
	private:
#pragma region CHECKS
		template<class _Type> using offsets_var_enable_if_t = extra::type_test_t<extra::offsets_check_v<_Type>,
			extra::type_test_t<extra::offsets140_check_v<_Type>, STD140Offsets, STD430Offsets>,
			void(*)>;
#pragma endregion

	public:
		using var_type = T;
		static constexpr size_t array_size = num;

#pragma region VARIABLES
		const std::string var_name;
		const typename offsets_var_enable_if_t<T> struct_offsets;
#pragma endregion

#pragma region NORMAL_CONSTRUCTOR
		template<class T = var_type, typename = extra::scalar_enable_if_t<T>>
		STDVariable(const std::string& name) : var_name(name), struct_offsets(nullptr) {}

		template<class T = var_type, class VT = T::value_type, size_t L = T::length(), typename = extra::vec_enable_if_t<VT, L>>
		STDVariable(const std::string& name) : var_name(name), struct_offsets(nullptr) {}

		template<class T = var_type, class MT = T::value_type, size_t C = T::row_type::length(), size_t R = T::col_type::length(), typename = extra::mat_enable_if_t<MT, C, R>>
		STDVariable(const std::string& name) : var_name(name), struct_offsets(nullptr) {}
#pragma endregion

#pragma region OFFSETS_CONSTRUCTOR
		template<class T = var_type, typename = extra::offsets140_enable_if_t<T>>
		STDVariable(const std::string& name, const STD140Offsets offsets) : var_name(name), struct_offsets(offsets) {}

		template<class T = var_type, typename = extra::offsets430_enable_if_t<T>>
		STDVariable(const std::string& name, const STD430Offsets offsets) : var_name(name), struct_offsets(offsets) {}
#pragma endregion
	};
}