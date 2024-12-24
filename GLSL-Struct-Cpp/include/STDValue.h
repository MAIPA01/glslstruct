#pragma once
#include <STDVariable.h>
#include <STDStruct.h>

namespace glsl {
	template<class T, size_t num = 0>
	struct STDValue {
	private:
#pragma region CHECKS
		template<class _Type, size_t length, class Ret = void> using scalar_array_enable_if_t = std::enable_if_t<(extra::scalar_check_v<_Type>&& length > 0), Ret>;
		template<class _Type, size_t L, size_t length, class Ret = void> using vec_array_enable_if_t = std::enable_if_t<(extra::vec_check_v<_Type, L>&& length > 0), Ret>;
		template<class _Type, size_t C, size_t R, size_t length, class Ret = void> using mat_array_enable_if_t = std::enable_if_t<(extra::mat_check_v<_Type, C, R> && length > 0), Ret>;
		template<class _Type, size_t length, class Ret = void> using scalar_offsets_array_enable_if_t = std::enable_if_t<((extra::scalar_check_v<_Type> || extra::offsets_check_v<_Type>) && length > 0), Ret>;
#pragma endregion

	public:
		using value_type = T;
		static constexpr size_t array_size = num;
		static constexpr bool is_offsets = extra::offsets_check_v<T>;
		static constexpr bool is_struct = false;

#pragma region VARIABLES
		const std::string var_name;
		const std::vector<T> value;
#pragma endregion

#pragma region CONSTRUCTORS
		template<class T = value_type, typename = scalar_array_enable_if_t<T, num>>
		STDValue(const std::string& name) : var_name(name), value(std::vector<T>(num)) {}

		template<class T = value_type, typename = scalar_offsets_array_enable_if_t<T, num>>
		STDValue(const std::string& name, const std::vector<T>& values) : var_name(name), value(values) {}

		template<class T = value_type, typename = scalar_offsets_array_enable_if_t<T, num>>
		STDValue(const std::string& name, const T*& values, size_t size) : var_name(name) {
			std::vector<T> array_values;
			array_values.reserve(num);
			for (size_t i = 0; i < num; ++i) {
				if (i < size) {
					array_values.push_back(values[i]);
				}
				else {
					array_values.push_back(T());
				}
			}
			value = array_values;
		}

		template<class T = value_type, typename = scalar_offsets_array_enable_if_t<T, num>>
		STDValue(const std::string& name, const T(&values)[num]) : var_name(name) {
			std::vector<T> array_values;
			array_values.reserve(num);
			for (size_t i = 0; i < num; ++i) {
				array_values.push_back(values[i]);
			}
			value = array_values;
		}

		template<class T = value_type, class VT = T::value_type, size_t L = T::length(), typename = vec_array_enable_if_t<VT, L, num>>
		STDValue(const std::string& name, const std::vector<T>& values = std::vector<T>(num)) : var_name(name), value(values) {}

		template<class T = value_type, class VT = T::value_type, size_t L = T::length(), typename = vec_array_enable_if_t<VT, L, num>>
		STDValue(const std::string& name, const T*& values, size_t size) : var_name(name) {
			std::vector<T> array_values;
			array_values.reserve(num);
			for (size_t i = 0; i < num; ++i) {
				if (i < size) {
					array_values.push_back(values[i]);
				}
				else {
					array_values.push_back(T());
				}
			}
			value = array_values;
		}

		template<class T = value_type, class VT = T::value_type, size_t L = T::length(), typename = vec_array_enable_if_t<VT, L, num>>
		STDValue(const std::string& name, const T(&values)[num]) : var_name(name) {
			std::vector<T> array_values;
			array_values.reserve(num);
			for (size_t i = 0; i < num; ++i) {
				array_values.push_back(values[i]);
			}
			value = array_values;
		}

		template<class T = value_type, class MT = T::value_type, size_t C = T::row_type::length(), size_t R = T::col_type::length(), typename = mat_array_enable_if_t<MT, C, R, num>>
		STDValue(const std::string& name, const std::vector<T>& values = std::vector<T>(num)) : var_name(name), value(values) {}

		template<class T = value_type, class MT = T::value_type, size_t C = T::row_type::length(), size_t R = T::col_type::length(), typename = mat_array_enable_if_t<MT, C, R, num>>
		STDValue(const std::string& name, const T*& values, size_t size) : var_name(name) {
			std::vector<T> array_values;
			array_values.reserve(num);
			for (size_t i = 0; i < num; ++i) {
				if (i < size) {
					array_values.push_back(values[i]);
				}
				else {
					array_values.push_back(T());
				}
			}
			value = array_values;
		}

		template<class T = value_type, class MT = T::value_type, size_t C = T::row_type::length(), size_t R = T::col_type::length(), typename = mat_array_enable_if_t<MT, C, R, num>>
		STDValue(const std::string& name, const T(&values)[num]) : var_name(name) {
			std::vector<T> array_values;
			array_values.reserve(num);
			for (size_t i = 0; i < num; ++i) {
				array_values.push_back(values[i]);
			}
			value = array_values;
		}
#pragma endregion
	};

	template<class T>
	struct STDValue<T, 0> {
	private:
#pragma region CHECKS
		template<class _Type, class Ret = void> using scalar_offsets_enable_if_t = std::enable_if_t<(extra::scalar_check_v<_Type> || extra::offsets_check_v<_Type>), Ret>;
#pragma endregion

	public:
		using value_type = T;
		static constexpr size_t array_size = 0;
		static constexpr bool is_offsets = extra::offsets_check_v<T>;
		static constexpr bool is_struct = false;

#pragma region VARIABLES
		const std::string var_name;
		const T value;
#pragma endregion

#pragma region CONSTRUCTORS
		template<class T = value_type, typename = extra::scalar_enable_if_t<T>>
		STDValue(const std::string& name) : var_name(name), value(T()) {}

		template<class T = value_type, typename = scalar_offsets_enable_if_t<T>>
		STDValue(const std::string& name, const T& value) : var_name(name), value(value) {}

		template<class T = value_type, class VT = T::value_type, size_t L = T::length(), typename = extra::vec_enable_if_t<T, VT, L>>
		STDValue(const std::string& name, const T& value = T()) : var_name(name), value(value) {}

		template<class T = value_type, class MT = T::value_type, size_t C = T::row_type::length(), size_t R = T::col_type::length(), typename = extra::mat_enable_if_t<T, MT, C, R>>
		STDValue(const std::string& name, const T& value = T()) : var_name(name), value(value) {}
#pragma endregion
	};
	
	template<size_t num, class _Offset>
	struct STDValue<STDStruct<_Offset>, num> {
	private:
#pragma region CHECKS
		template<class _Type, class _Offsets> static constexpr bool struct_check_v = std::is_same_v<_Type, STDStruct<_Offsets>>&& extra::offsets_check_v<_Offsets>;

		template<class _Type, class _Offsets, size_t length, class Ret = void> using struct_array_enable_if_t = std::enable_if_t<(struct_check_v<_Type, _Offsets>&& length > 0), Ret>;
#pragma endregion
	public:
		using value_type = STDStruct<_Offset>;
		static constexpr size_t array_size = num;
		static constexpr bool is_offsets = false;
		static constexpr bool is_struct = true;

#pragma region VARIABLES
		const std::string var_name;
		const std::vector<std::vector<unsigned char>> value;
		const _Offset struct_offsets;
#pragma endregion

#pragma region CONSTRUCTORS
		template<class T = value_type, typename = struct_array_enable_if_t<T, _Offset, num>>
		STDValue(const std::string& name, const _Offset& offsets, const std::vector<std::vector<unsigned char>>& values = std::vector<std::vector<unsigned char>>(num)) : var_name(name), value(values), struct_offsets(offsets) {}

		template<class T = value_type, typename = struct_array_enable_if_t<T, _Offset, num>>
		STDValue(const std::string& name, const _Offset& offsets, const std::vector<unsigned char>*& values, size_t size) : var_name(name), struct_offsets(offsets) {
			std::vector<std::vector<unsigned char>> array_values;
			array_values.reserve(num);
			for (size_t i = 0; i < num; ++i) {
				if (i < size) {
					array_values.push_back(values[i]);
				}
				else {
					array_values.push_back(std::vector<unsigned char>());
				}
			}
			value = array_values;
		}

		template<class T = value_type, typename = struct_array_enable_if_t<T, _Offset, num>>
		STDValue(const std::string& name, const _Offset& offsets, const std::vector<unsigned char>(&values)[num]) : var_name(name), struct_offsets(offsets) {
			std::vector<std::vector<unsigned char>> array_values;
			array_values.reserve(num);
			for (size_t i = 0; i < num; ++i) {
				array_values.push_back(values[i]);
			}
			value = array_values;
		}
#pragma endregion
	};

	template<class _Offset>
	struct STDValue<STDStruct<_Offset>, 0> {
	private:
#pragma region CHECKS
		template<class _Type, class _Offsets> static constexpr bool struct_check_v = std::is_same_v<_Type, STDStruct<_Offsets>>&& extra::offsets_check_v<_Offsets>;

		template<class _Type, class _Offsets, class Ret = void> using struct_enable_if_t = std::enable_if_t<struct_check_v<_Type, _Offsets>, Ret>;
#pragma endregion
	public:
		using value_type = STDStruct<_Offset>;
		static constexpr size_t array_size = 0;
		static constexpr bool is_offsets = false;
		static constexpr bool is_struct = true;

#pragma region VARIABLES
		const std::string var_name;
		const value_type value;
#pragma endregion

#pragma region CONSTRUCTORS
		template<class T = value_type, typename = struct_enable_if_t<T, _Offset>>
		STDValue(const std::string& name, const T& value) : var_name(name), value(value) {}
#pragma endregion
	};
}

