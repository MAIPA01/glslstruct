#pragma once
#include <STDVariable.hpp>
#include <STDStruct.hpp>

namespace glsl {
	template<class T>
	struct SingleValue {
		const T value;

		SingleValue(const T& value = T()) : value(value) {}
	};

	template<class T, size_t num>
	struct ArrayValue {
		const std::vector<T> value;

		static std::vector<T> init_value(const std::vector<T> values) {
			if (values.size() != num) {
				std::vector<T> temp(num);
				std::copy(values.begin(), values.begin() + std::min(num, values.size()), temp.begin());
				return temp;
			}
			else {
				return values;
			}
		}

		static std::vector<T> init_value(const T*& values, size_t size) {
			std::vector<T> temp(num);
			std::copy(values, values + std::min(num, size), temp.begin());
			return temp;
		}

		static std::vector<T> init_value(const T(&values)[num]) {
			std::vector<T> temp(num);
			std::copy(values, values + num, temp.begin());
			return temp;
		}

		ArrayValue(const std::vector<T>& values = std::vector<T>(num)) : value(init_value(values)) {}
		ArrayValue(const T*& values, size_t size) : value(init_value(values, size)) {}
		ArrayValue(const T(&values)[num]) : value(init_value(values)) {}
	};

	template<class T, size_t num>
	struct StructArrayValue : ArrayValue<std::vector<unsigned char>, num> {
	private:
		using array_type = ArrayValue<std::vector<unsigned char>, num>;
		using offset_type = typename T::offset_type;

	public:
		const offset_type struct_offsets;

		StructArrayValue(const offset_type& offsets, const std::vector<std::vector<unsigned char>>& values) 
			: array_type(values), struct_offsets(offsets) {}
		StructArrayValue(const offset_type& offsets, const std::vector<unsigned char>*& values, size_t size) 
			: array_type(values, size), struct_offsets(offsets) {}
		StructArrayValue(const offset_type& offsets, const std::vector<unsigned char>(&values)[num]) 
			: array_type(values), struct_offsets(offsets) {}
	};

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<utils::any_standard_or_std_struct_value T, size_t num>
#else
	template<class T, size_t num,
		std::enable_if_t<(utils::is_any_standard_value_v<T> || utils::is_any_std_struct_v<T>), bool>>
#endif
	struct STDValue : public std::conditional_t<
		mstd::is_eq_v<num, 0>,
		SingleValue<T>,
		std::conditional_t<
			utils::is_any_standard_value_v<T>,
			ArrayValue<T, num>,
			StructArrayValue<T, num>
		>
	> {
	public:
		using value_type = T;
		static constexpr size_t array_size = num;
		static constexpr bool is_struct = utils::is_any_std_struct_v<T>;

#pragma region VARIABLES
		const std::string var_name;
#pragma endregion

#pragma region DEFAULT_CONSTRUCTOR
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::any_standard_value T = value_type>
		requires (std::is_same_v<T, value_type>)
#else
		template<class T = value_type, std::enable_if_t<(
				std::is_same_v<T, value_type> &&
				utils::is_any_standard_value_v<T>
				), bool> = true>
#endif
		STDValue(const std::string& name) : 
			std::conditional_t<
				mstd::is_gt_v<array_size, 0>,
				ArrayValue<T, array_size>,
				SingleValue<T>
			>(),
			var_name(name) {}
#pragma endregion

#pragma region SINGLE_VALUE_CONSTRUCTOR
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::any_standard_or_std_struct_value T = value_type>
		requires (std::is_same_v<T, value_type> && mstd::is_eq_v<array_size, 0>)
#else
		template<class T = value_type, std::enable_if_t<(
				std::is_same_v<T, value_type>&&
				mstd::is_eq_v<array_size, 0> &&
				(utils::is_any_standard_value_v<T> ||
					utils::is_any_std_struct_v<T>)
				), bool> = true>
#endif
		STDValue(const std::string& name, const T& value) : SingleValue<T>(value), var_name(name) {}
#pragma endregion

#pragma region ARRAY_CONSTRUCTORS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::any_standard_value T = value_type>
		requires (std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>)
#else
		template<class T = value_type, std::enable_if_t<(
				std::is_same_v<T, value_type> &&
				mstd::is_gt_v<array_size, 0> &&
				utils::is_any_standard_value_v<T>
				), bool> = true>
#endif
		STDValue(const std::string& name, const std::vector<T>& values) : 
			ArrayValue<T, array_size>(values),
			var_name(name) {}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::any_standard_value T = value_type>
		requires (std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>)
#else
		template<class T = value_type, std::enable_if_t<(
				std::is_same_v<T, value_type> &&
				mstd::is_gt_v<array_size, 0> &&
				utils::is_any_standard_value_v<T>
				), bool> = true>
#endif
		STDValue(const std::string& name, const T*& values, size_t size) :
			ArrayValue<T, array_size>(values, size),
			var_name(name) {}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::any_standard_value T = value_type>
		requires (std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>)
#else
		template<class T = value_type, std::enable_if_t<(
				std::is_same_v<T, value_type> &&
				mstd::is_gt_v<array_size, 0> &&
				utils::is_any_standard_value_v<T>
				), bool> = true>
#endif
		STDValue(const std::string& name, const T(&values)[array_size]) :
			ArrayValue<T, array_size>(values),
			var_name(name) {}
#pragma endregion

#pragma region STRUCT_ARRAY_CONSTRUCTORS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::any_std_struct T = value_type>
		requires (std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>)
#else
		template<class T = value_type, class = std::enable_if_t<(
			std::is_same_v<T, value_type> &&
			mstd::is_gt_v<array_size, 0> &&
			utils::is_any_std_struct_v<T>
			)>>
#endif
		STDValue(const std::string& name, const typename T::offset_type& offsets,
			const std::vector<std::vector<unsigned char>>& values = std::vector<std::vector<unsigned char>>(array_size)) :
			StructArrayValue<T, array_size>(offsets, values),
			var_name(name) {}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::any_std_struct T = value_type>
		requires (std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>)
#else
		template<class T = value_type, std::enable_if_t<(
			std::is_same_v<T, value_type> &&
			mstd::is_gt_v<array_size, 0> &&
			utils::is_any_std_struct_v<T>
			), bool> = true>
#endif
		STDValue(const std::string& name, const typename T::offset_type& offsets, const std::vector<unsigned char>*& values, size_t size) :
			StructArrayValue<T, array_size>(offsets, values, size),
			var_name(name) {}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::any_std_struct T = value_type>
		requires (std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>)
#else
		template<class T = value_type, std::enable_if_t<(
			std::is_same_v<T, value_type> &&
			mstd::is_gt_v<array_size, 0> &&
			utils::is_any_std_struct_v<T>
			), bool> = true>
#endif
		STDValue(const std::string& name, const typename T::offset_type& offsets, const std::vector<unsigned char>(&values)[num]) :
			StructArrayValue<T, array_size>(offsets, values),
			var_name(name) {}
#pragma endregion
	};
}

