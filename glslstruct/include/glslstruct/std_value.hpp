#pragma once
#include <glslstruct/std_variable.hpp>
#include <glslstruct/std_struct.hpp>
#include <cstdint>
#include <cstddef>

namespace glslstruct {
	template<class T>
	struct single_value {
		const T value;

		single_value(const T& value = T()) : value(value) {}
	};

	template<class T, size_t num>
	struct array_value {
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

		array_value(const std::vector<T>& values = std::vector<T>(num)) : value(init_value(values)) {}
		array_value(const T*& values, size_t size) : value(init_value(values, size)) {}
		array_value(const T(&values)[num]) : value(init_value(values)) {}
	};

	template<class T, size_t num>
	struct struct_array_value : array_value<std::vector<std::byte>, num> {
	private:
		using array_type = array_value<std::vector<std::byte>, num>;
		using offset_type = typename T::offset_type;

	public:
		const offset_type struct_offsets;

		struct_array_value(const offset_type& offsets, const std::vector<std::vector<std::byte>>& values)
			: array_type(values), struct_offsets(offsets) {}
		struct_array_value(const offset_type& offsets, const std::vector<std::byte>*& values, size_t size)
			: array_type(values, size), struct_offsets(offsets) {}
		struct_array_value(const offset_type& offsets, const std::vector<std::byte>(&values)[num])
			: array_type(values), struct_offsets(offsets) {}
	};

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<utils::any_standard_or_std_struct_value T, size_t num>
#else
	template<class T, size_t num,
		std::enable_if_t<(utils::is_any_standard_value_v<T> || utils::is_any_std_struct_v<T>), bool>>
#endif
	struct std_value : public std::conditional_t<
		mstd::is_eq_v<num, 0>,
		single_value<T>,
		std::conditional_t<
			utils::is_any_standard_value_v<T>,
			array_value<T, num>,
			struct_array_value<T, num>
		>
	> {
	public:
		using value_type = T;
		static constexpr const size_t array_size = num;
		static constexpr const bool is_struct = utils::is_any_std_struct_v<T>;

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
		std_value(const std::string& name) :
			std::conditional_t<
				mstd::is_gt_v<array_size, 0>,
				array_value<T, array_size>,
				single_value<T>
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
		std_value(const std::string& name, const T& value) : single_value<T>(value), var_name(name) {}
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
		std_value(const std::string& name, const std::vector<T>& values) :
			array_value<T, array_size>(values),
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
		std_value(const std::string& name, const T*& values, size_t size) :
			array_value<T, array_size>(values, size),
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
		std_value(const std::string& name, const T(&values)[array_size]) :
			array_value<T, array_size>(values),
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
		std_value(const std::string& name, const typename T::offset_type& offsets,
			const std::vector<std::vector<std::byte>>& values = std::vector<std::vector<std::byte>>(array_size)) :
			struct_array_value<T, array_size>(offsets, values),
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
		std_value(const std::string& name, const typename T::offset_type& offsets, const std::vector<std::byte>*& values, size_t size) :
			struct_array_value<T, array_size>(offsets, values, size),
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
		std_value(const std::string& name, const typename T::offset_type& offsets, const std::vector<std::byte>(&values)[num]) :
			struct_array_value<T, array_size>(offsets, values),
			var_name(name) {}
#pragma endregion
	};
}

