/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/std_variable.hpp>

namespace glslstruct {
	template<class T>
	struct single_value {
		const T value;

		single_value() : value() {}
		single_value(const T& value) : value(value) {}
	};

	template<class T, size_t num>
	struct array_value {
		const std::array<T, num> value;

		static std::array<T, num> init_value(const std::vector<T>& values) {
			std::array<T, num> temp;
			std::copy(values.begin(), values.begin() + std::min(num, values.size()), temp.begin());
			return temp;
		}

		static std::array<T, num> init_value(const std::array<T, num>& values) {
			return values;
		}

		static std::array<T, num> init_value(const T* values, size_t size) {
			if (values == nullptr) {
				return std::array<T, num>();
			}

			std::array<T, num> temp;
			std::copy(values, values + std::min(num, size), temp.begin());
			return temp;
		}

		static std::array<T, num> init_value(const T(&values)[num]) {
			std::array<T, num> temp;
			std::copy(values, values + num, temp.begin());
			return temp;
		}

		array_value() : value() {}
		array_value(const std::vector<T>& values) : value(init_value(values)) {}
		array_value(const std::array<T, num>& values) : value(init_value(values)) {}
		array_value(const T* values, size_t size) : value(init_value(values, size)) {}
		array_value(const T(&values)[num]) : value(init_value(values)) {}
	};

	template<class T, size_t num>
	struct struct_array_value : public array_value<std::vector<std::byte>, num> {
	private:
		using array_type = array_value<std::vector<std::byte>, num>;
		using offset_type = typename T::offset_type;

	public:
		const offset_type struct_offsets;

		struct_array_value(const offset_type& offsets)
			: array_type(), struct_offsets(offsets) {}
		struct_array_value(const offset_type& offsets, const std::vector<T>& values) 
			: array_type(values), struct_offsets(offsets) {}
		struct_array_value(const offset_type& offsets, const std::array<T, num>& values)
			: array_type(values), struct_offsets(offsets) {}
		struct_array_value(const offset_type& offsets, const T* values, size_t size)
			: array_type(values, size), struct_offsets(offsets) {}
		struct_array_value(const offset_type& offsets, const T(&values)[num])
			: array_type(values), struct_offsets(offsets) {}
	};

	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_simple_or_struct_value, utils::is_glsl_simple_or_struct_value_v<T>, , size_t num)
	struct std_value : public std::conditional_t<
		mstd::is_eq_v<num, 0>,
		single_value<T>,
		std::conditional_t<
		utils::is_glsl_simple_value_v<T>,
		array_value<T, num>,
		struct_array_value<T, num>
		>
	> {
	private:
		using base_struct = std::conditional_t<
			mstd::is_eq_v<num, 0>,
			single_value<T>,
			std::conditional_t<
			utils::is_glsl_simple_value_v<T>,
			array_value<T, num>,
			struct_array_value<T, num>
			>
		>;

	public:
		using value_type = T;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t array_size = num;
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_struct = utils::is_glsl_struct_v<T>;

#pragma region VARIABLES
		const std::string var_name;
#pragma endregion

#pragma region DEFAULT_CONSTRUCTOR
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T = value_type, utils::glsl_simple_value, 
			(utils::is_glsl_simple_value_v<T> && std::is_same_v<T, value_type>), = true)
		std_value(const std::string& name) _GLSL_STRUCT_REQUIRES((std::is_same_v<T, value_type>))
			: base_struct(), var_name(name) {}
#pragma endregion

#pragma region SINGLE_VALUE_CONSTRUCTOR
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T = value_type, utils::glsl_simple_or_struct_value,
				(utils::is_glsl_simple_or_struct_value_v<T> && std::is_same_v<T, value_type> && mstd::is_eq_v<array_size, 0>), = true)
		std_value(const std::string& name, const T& value) _GLSL_STRUCT_REQUIRES((std::is_same_v<T, value_type> && mstd::is_eq_v<array_size, 0>))
			: base_struct(value), var_name(name) {}
#pragma endregion

#pragma region ARRAY_CONSTRUCTORS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T = value_type, utils::glsl_simple_value,
			(utils::is_glsl_simple_value_v<T>&& std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>), = true)
		std_value(const std::string& name, const std::vector<T>& values) 
			_GLSL_STRUCT_REQUIRES((std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>))
			: base_struct(values), var_name(name) {}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T = value_type, utils::glsl_simple_value,
			(utils::is_glsl_simple_value_v<T>&& std::is_same_v<T, value_type>&& mstd::is_eq_v<array_size, 0>), = true)
		std_value(const std::string& name, const std::array<T, array_size>& values) 
			_GLSL_STRUCT_REQUIRES((std::is_same_v<T, value_type>&& mstd::is_gt_v<array_size, 0>))
			: base_struct(values), var_name(name) {}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T = value_type, utils::glsl_simple_value,
			(utils::is_glsl_simple_value_v<T> && std::is_same_v<T, value_type>&& mstd::is_eq_v<array_size, 0>), = true)
		std_value(const std::string& name, const T* values, size_t size) 
			_GLSL_STRUCT_REQUIRES((std::is_same_v<T, value_type>&& mstd::is_gt_v<array_size, 0>))
			: base_struct(values, size), var_name(name) {}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T = value_type, utils::glsl_simple_value,
			(utils::is_glsl_simple_value_v<T> && std::is_same_v<T, value_type>&& mstd::is_eq_v<array_size, 0>), = true)
		std_value(const std::string& name, const T(&values)[array_size]) 
			_GLSL_STRUCT_REQUIRES((std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>))
			: base_struct(values), var_name(name) {}
#pragma endregion

#pragma region STRUCT_ARRAY_CONSTRUCTORS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T = value_type, utils::glsl_struct,
			(utils::is_glsl_struct_v<T> && std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>), = true)
		std_value(const std::string& name, const typename T::offset_type& offsets) 
			_GLSL_STRUCT_REQUIRES((std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>))
			: base_struct(offsets), var_name(name) {}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T = value_type, utils::glsl_struct,
			(utils::is_glsl_struct_v<T> && std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>), = true)
		std_value(const std::string& name, const typename T::offset_type& offsets,
			const std::vector<std::vector<std::byte>>& values) 
			_GLSL_STRUCT_REQUIRES((std::is_same_v<T, value_type>&& mstd::is_gt_v<array_size, 0>))
			: base_struct(offsets, values), var_name(name) {}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T = value_type, utils::glsl_struct,
			(utils::is_glsl_struct_v<T> && std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>), = true)
		std_value(const std::string& name, const typename T::offset_type& offsets,
			const std::array<std::vector<std::byte>, array_size>& values) 
			_GLSL_STRUCT_REQUIRES((std::is_same_v<T, value_type>&& mstd::is_gt_v<array_size, 0>))
			: base_struct(offsets, values), var_name(name) {}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T = value_type, utils::glsl_struct,
			(utils::is_glsl_struct_v<T> && std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>), = true)
		std_value(const std::string& name, const typename T::offset_type& offsets, 
			const std::vector<std::byte>* values, size_t size) 
			_GLSL_STRUCT_REQUIRES((std::is_same_v<T, value_type>&& mstd::is_gt_v<array_size, 0>))
			: base_struct(offsets, values, size), var_name(name) {}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T = value_type, utils::glsl_struct,
			(utils::is_glsl_struct_v<T> && std::is_same_v<T, value_type> && mstd::is_gt_v<array_size, 0>), = true)
		std_value(const std::string& name, const typename T::offset_type& offsets, 
			const std::vector<std::byte>(&values)[num]) 
			_GLSL_STRUCT_REQUIRES((std::is_same_v<T, value_type>&& mstd::is_gt_v<array_size, 0>))
			: base_struct(offsets, values), var_name(name) {}
#pragma endregion
	};
}