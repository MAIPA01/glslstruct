/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_GLSL_VALUE_HPP_
	#define _GLSL_STRUCT_GLSL_VALUE_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/variable/glsl_variable.hpp>

namespace glslstruct {
	template<class T>
	struct single_value {
		const T value;

		single_value() : value() {}

		explicit single_value(const T& value) : value(value) {}
	};

	template<class T, size_t num>
	struct array_value {
		const std::array<T, num> value;

		static std::array<T, num> init_value(const std::vector<T>& values) {
			std::array<T, num> temp;
			std::copy(values.begin(), values.begin() + std::min(num, values.size()), temp.begin());
			return temp;
		}

		static std::array<T, num> init_value(const std::array<T, num>& values) { return values; }

		static std::array<T, num> init_value(const T* values, size_t size) {
				if (values == nullptr) { return std::array<T, num>(); }

			std::array<T, num> temp;
			std::copy(values, values + std::min(num, size), temp.begin());
			return temp;
		}

		static std::array<T, num> init_value(const T (&values)[num]) {
			std::array<T, num> temp;
			std::copy(values, values + num, temp.begin());
			return temp;
		}

		array_value() : value() {}

		explicit array_value(const std::vector<T>& values) : value(init_value(values)) {}

		explicit array_value(const std::array<T, num>& values) : value(init_value(values)) {}

		explicit array_value(const T* values, size_t size) : value(init_value(values, size)) {}

		explicit array_value(const T (&values)[num]) : value(init_value(values)) {}
	};

	template<class T, size_t num>
	struct struct_array_value : public array_value<std::vector<std::byte>, num> {
	private:
		using array_type  = array_value<std::vector<std::byte>, num>;
		using layout_type = typename T::layout_type;

	public:
		const layout_type layout;

		explicit struct_array_value(const layout_type& layout) : array_type(), layout(layout) {}

		struct_array_value(const layout_type& layout, const std::vector<T>& values) : array_type(values), layout(layout) {}

		struct_array_value(const layout_type& layout, const std::array<T, num>& values) : array_type(values), layout(layout) {}

		struct_array_value(const layout_type& layout, const T* values, size_t size) : array_type(values, size), layout(layout) {}

		struct_array_value(const layout_type& layout, const T (&values)[num]) : array_type(values), layout(layout) {}
	};

		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_simple_or_struct T, size_t num>
		#else
	template<class T, size_t num, std::enable_if_t<utils::is_glsl_simple_or_struct_v<T>, bool> >
		#endif
	struct glsl_value : public std::conditional_t<mstd::is_eq_v<num, 0>, single_value<T>,
						  std::conditional_t<utils::is_glsl_simple_v<T>, array_value<T, num>, struct_array_value<T, num> > > {
	private:
		using base_struct = std::conditional_t<mstd::is_eq_v<num, 0>, single_value<T>,
		  std::conditional_t<utils::is_glsl_simple_v<T>, array_value<T, num>, struct_array_value<T, num> > >;

	public:
		using value_type										= T;
		static _GLSL_STRUCT_CONSTEXPR17 const size_t array_size = num;
		static _GLSL_STRUCT_CONSTEXPR17 const bool is_struct	= utils::is_glsl_struct_v<T>;

		#pragma region VARIABLES
		const std::string var_name;
		#pragma endregion

		#pragma region DEFAULT_CONSTRUCTOR
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type																			  = value_type,
		  std::enable_if_t<(utils::is_glsl_simple_v<Type> && std::is_same_v<Type, value_type>), bool> = true>
		#endif
		explicit glsl_value(const std::string_view name)
		  // clang-format off
		_GLSL_STRUCT_REQUIRES((utils::is_glsl_simple_v<value_type>))
			// clang-format on
			: base_struct(), var_name(name) {
		}

		#pragma endregion

		#pragma region SINGLE_VALUE_CONSTRUCTOR
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type, std::enable_if_t<(utils::is_glsl_simple_or_struct_v<Type> &&
															 std::is_same_v<Type, value_type> && mstd::is_eq_v<array_size, 0>),
											bool> = true>
		#endif
		glsl_value(
		  const std::string_view name, const value_type& value
		) _GLSL_STRUCT_REQUIRES((utils::is_glsl_simple_or_struct_v<value_type> && mstd::is_eq_v<array_size, 0>))
			: base_struct(value), var_name(name) {
		}

		#pragma endregion

		#pragma region ARRAY_CONSTRUCTORS
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type,
		  std::enable_if_t<(utils::is_glsl_simple_v<Type> && std::is_same_v<Type, value_type> && mstd::is_gt_v<array_size, 0>),
			bool>			= true>
		#endif
		glsl_value(
		  const std::string_view name, const std::vector<value_type>& values
		) _GLSL_STRUCT_REQUIRES((utils::is_glsl_simple_v<value_type> && mstd::is_gt_v<array_size, 0>))
			: base_struct(values), var_name(name) {
		}

		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type,
		  std::enable_if_t<(utils::is_glsl_simple_v<Type> && std::is_same_v<Type, value_type> && mstd::is_gt_v<array_size, 0>),
			bool>			= true>
		#endif
		glsl_value(
		  const std::string_view name, const std::array<value_type, array_size>& values
		) _GLSL_STRUCT_REQUIRES((utils::is_glsl_simple_v<value_type> && mstd::is_gt_v<array_size, 0>))
			: base_struct(values), var_name(name) {
		}

		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type,
		  std::enable_if_t<(utils::is_glsl_simple_v<Type> && std::is_same_v<Type, value_type> && mstd::is_gt_v<array_size, 0>),
			bool>			= true>
		#endif
		glsl_value(const std::string_view name, const value_type* values,
		  size_t size) _GLSL_STRUCT_REQUIRES((utils::is_glsl_simple_v<value_type> && mstd::is_gt_v<array_size, 0>))
			: base_struct(values, size), var_name(name) {
		}

		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type,
		  std::enable_if_t<(utils::is_glsl_simple_v<Type> && std::is_same_v<Type, value_type> && mstd::is_gt_v<array_size, 0>),
			bool>			= true>
		#endif
		glsl_value(
		  const std::string_view name, const value_type (&values)[array_size]
		) _GLSL_STRUCT_REQUIRES((utils::is_glsl_simple_v<value_type> && mstd::is_gt_v<array_size, 0>))
			: base_struct(values), var_name(name) {
		}

		#pragma endregion

		#pragma region STRUCT_ARRAY_CONSTRUCTORS
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type,
		  std::enable_if_t<(utils::is_glsl_struct_v<Type> && std::is_same_v<Type, value_type> && mstd::is_gt_v<array_size, 0>),
			bool>			= true>
		#endif
		glsl_value(
		  const std::string_view name, const typename value_type::layout_type& layout
		) _GLSL_STRUCT_REQUIRES((utils::is_glsl_struct_v<value_type> && mstd::is_gt_v<array_size, 0>))
			: base_struct(layout), var_name(name) {
		}

		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type,
		  std::enable_if_t<(utils::is_glsl_struct_v<Type> && std::is_same_v<Type, value_type> && mstd::is_gt_v<array_size, 0>),
			bool>			= true>
		#endif
		glsl_value(const std::string_view name, const typename value_type::layout_type& layout,
		  const std::vector<std::vector<std::byte> >& values) _GLSL_STRUCT_REQUIRES((utils::is_glsl_struct_v<value_type> &&
																					 mstd::is_gt_v<array_size, 0>))
			: base_struct(layout, values), var_name(name) {
		}

		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type,
		  std::enable_if_t<(utils::is_glsl_struct_v<Type> && std::is_same_v<Type, value_type> && mstd::is_gt_v<array_size, 0>),
			bool>			= true>
		#endif
		glsl_value(
		  const std::string_view name, const typename value_type::layout_type& layout,
		  const std::array<std::vector<std::byte>, array_size>& values
		) _GLSL_STRUCT_REQUIRES((utils::is_glsl_struct_v<value_type> && mstd::is_gt_v<array_size, 0>))
			: base_struct(layout, values), var_name(name) {
		}

		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type,
		  std::enable_if_t<(utils::is_glsl_struct_v<Type> && std::is_same_v<Type, value_type> && mstd::is_gt_v<array_size, 0>),
			bool>			= true>
		#endif
		glsl_value(const std::string_view name, const typename value_type::layout_type& layout,
		  const std::vector<std::byte>* values,
		  size_t size) _GLSL_STRUCT_REQUIRES((utils::is_glsl_struct_v<value_type> && mstd::is_gt_v<array_size, 0>))
			: base_struct(layout, values, size), var_name(name) {
		}

		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type,
		  std::enable_if_t<(utils::is_glsl_struct_v<Type> && std::is_same_v<Type, value_type> && mstd::is_gt_v<array_size, 0>),
			bool>			= true>
		#endif
		glsl_value(
		  const std::string_view name, const typename value_type::layout_type& layout, const std::vector<std::byte> (&values)[num]
		) _GLSL_STRUCT_REQUIRES((utils::is_glsl_struct_v<value_type> && mstd::is_gt_v<array_size, 0>))
			: base_struct(layout, values), var_name(name) {
		}

		#pragma endregion
	};
} // namespace glslstruct

	#endif
#endif