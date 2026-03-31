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
	namespace utils {
		/**
		 * @brief container for single values
		 * @ingroup utils
		 * @tparam T value type
		 */
		template<class T>
		struct single_value {
			/// @brief value
			const T value;

		/// @brief default constructor
		#if !_GLSL_STRUCT_HAS_CXX20
			template<class type = T, std::enable_if_t<std::is_default_constructible_v<T> && std::is_same_v<type, T>, bool> = true>
		#endif
			single_value() _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<T>) : value() {
			}

			/// @brief constructor with value
			explicit single_value(const T& value) : value(value) {}
		};

		/**
		 * @brief container for array values
		 * @ingroup utils
		 * @tparam T value type
		 * @tparam num array size
		 */
		template<class T, size_t num>
		struct array_value {
			/// @brief array value
			const std::array<T, num> value;

			/// @brief static conversion from std::vector to std::array
			static std::array<T, num> init_value(const std::vector<T>& values) {
				std::array<T, num> temp;
				std::copy_n(values.begin(), std::min(num, values.size()), temp.begin());
				return temp;
			}

			/// @brief static conversion from pointer and size to std::array
			static std::array<T, num> init_value(const T* values, const size_t size) {
					if (values == nullptr) { return std::array<T, num>(); }

				std::array<T, num> temp;
				std::copy_n(values, std::min(num, size), temp.begin());
				return temp;
			}

			/// @brief static conversion from c-style array to std::array
			static std::array<T, num> init_value(const T (&values)[num]) {
				std::array<T, num> temp;
				std::copy_n(values, num, temp.begin());
				return temp;
			}

		#if _GLSL_STRUCT_HAS_CXX20
			/// @brief static conversion from std::span to std::array
			static std::array<T, num> init_value(const std::span<const T> values) {
				std::array<T, num> temp;
				std::copy_n(values.begin(), std::min(num, values.size()), temp.begin());
				return temp;
			}
		#endif

			/// @brief default constructor
			array_value() = default;

			/// @brief constructor with std::vector values
			explicit array_value(const std::vector<T>& values) : value(init_value(values)) {}

			/// @brief constructor with std::array values
			explicit array_value(const std::array<T, num>& values) : value(values) {}

			/// @brief constructor with pointer to values and size
			explicit array_value(const T* values, const size_t size) : value(init_value(values, size)) {}

			/// @brief constructor with c-style array values
			explicit array_value(const T (&values)[num]) : value(init_value(values)) {}

		#if _GLSL_STRUCT_HAS_CXX20
			/// @brief constructor with std::span values
			explicit array_value(const std::span<const T> values) : value(init_value(values)) {}
		#endif
		};

		/**
		 * @brief container for array of structs
		 * @ingroup utils
		 * @tparam T struct type
		 * @tparam num number of arrays
		 */
		template<class T, size_t num>
		struct struct_array_value : public array_value<std::vector<std::byte>, num> {
		private:
			/// @brief value type
			using value_type  = std::vector<std::byte>;
			/// @brief type of array
			using array_type  = array_value<value_type, num>;
			/// @brief layout type
			using layout_type = typename T::layout_type;

		public:
			/// @brief layout value
			const layout_type layout;

			/// @brief constructor with layout
			explicit struct_array_value(const layout_type& layout) : array_type(), layout(layout) {}

			/// @brief constructor with layout and std::vector values
			struct_array_value(const layout_type& layout, const std::vector<value_type>& values)
				: array_type(values), layout(layout) {}

			/// @brief constructor with layout and std::array values
			struct_array_value(const layout_type& layout, const std::array<value_type, num>& values)
				: array_type(values), layout(layout) {}

			/// @brief constructor with layout and pointer to values and size
			struct_array_value(const layout_type& layout, const value_type* values, size_t size)
				: array_type(values, size), layout(layout) {}

			/// @brief constructor with layout and c-style array values
			struct_array_value(const layout_type& layout, const value_type (&values)[num]) : array_type(values), layout(layout) {}

		#if _GLSL_STRUCT_HAS_CXX20
			/// @brief constructor with layout and std::span values
			struct_array_value(const layout_type& layout, const std::span<const value_type> values)
				: array_type(values), layout(layout) {}
		#endif
		};
	} // namespace utils

		/**
		 * @brief container for values for easier initialization of structs
		 * @ingroup glslstruct
		 * @tparam T value type
		 * @tparam num number of elements in array (default is 0. if it is 0 then it is not array but a single value)
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_simple_or_struct T, size_t num>
		#else
	template<class T, size_t num, std::enable_if_t<utils::is_glsl_simple_or_struct_v<T>, bool> >
		#endif
	struct glsl_value
		: public std::conditional_t<mstd::is_eq_v<num, 0>, utils::single_value<T>,
			std::conditional_t<utils::is_glsl_simple_v<T>, utils::array_value<T, num>, utils::struct_array_value<T, num> > > {
	private:
		/// @brief base struct type
		using base_struct = std::conditional_t<mstd::is_eq_v<num, 0>, utils::single_value<T>,
		  std::conditional_t<utils::is_glsl_simple_v<T>, utils::array_value<T, num>, utils::struct_array_value<T, num> > >;

	public:
		/// @brief value type
		using value_type								  = T;
		/// @brief array size
		static _GLSL_STRUCT_CONSTEXPR17 size_t array_size = num;
		/// @brief value indicating if value is array or single
		static _GLSL_STRUCT_CONSTEXPR17 bool is_array	  = array_size > 0;
		/// @brief value indicating if value_type is struct
		static _GLSL_STRUCT_CONSTEXPR17 bool is_struct	  = utils::is_glsl_struct_v<value_type>;

		#pragma region VARIABLES
		/// @brief variable name
		const std::string var_name;
		#pragma endregion

		#pragma region DEFAULT_CONSTRUCTOR
		/// @brief default constructor with variable name
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type,
		  std::enable_if_t<(utils::is_glsl_simple_v<Type> && std::is_default_constructible_v<base_struct> &&
							 std::is_same_v<Type, value_type>),
			bool>			= true>
		#endif
		explicit glsl_value(const std::string_view name) _GLSL_STRUCT_REQUIRES(utils::is_glsl_simple_v<value_type>)
			: base_struct(), var_name(name) {
		}

		#pragma endregion

		#pragma region SINGLE_VALUE_CONSTRUCTOR
		/// @brief constructor with value of value_type type
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type = value_type,
		  std::enable_if_t<(utils::is_glsl_simple_or_struct_v<Type> && std::is_same_v<Type, value_type> && !is_array), bool> =
			true>
		#endif
		glsl_value(const std::string_view name,
		  const value_type& value) _GLSL_STRUCT_REQUIRES(utils::is_glsl_simple_or_struct_v<value_type> && !is_array)
			: base_struct(value), var_name(name) {
		}

		#pragma endregion

		#pragma region ARRAY_CONSTRUCTORS
		/// @brief constructor for array values with std::vector
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type																						  = value_type,
		  std::enable_if_t<(utils::is_glsl_simple_v<Type> && std::is_same_v<Type, value_type> && is_array), bool> = true>
		#endif
		glsl_value(const std::string_view name,
		  const std::vector<value_type>& values) _GLSL_STRUCT_REQUIRES(utils::is_glsl_simple_v<value_type>&& is_array)
			: base_struct(values), var_name(name) {
		}

		/// @brief constructor for array values with std::array
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type																						  = value_type,
		  std::enable_if_t<(utils::is_glsl_simple_v<Type> && std::is_same_v<Type, value_type> && is_array), bool> = true>
		#endif
		glsl_value(const std::string_view name,
		  const std::array<value_type, array_size>& values) _GLSL_STRUCT_REQUIRES(utils::is_glsl_simple_v<value_type>&& is_array)
			: base_struct(values), var_name(name) {
		}

		/// @brief constructor for array values with pointer to values and size
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type																						  = value_type,
		  std::enable_if_t<(utils::is_glsl_simple_v<Type> && std::is_same_v<Type, value_type> && is_array), bool> = true>
		#endif
		glsl_value(const std::string_view name, const value_type* values,
		  const size_t size) _GLSL_STRUCT_REQUIRES(utils::is_glsl_simple_v<value_type>&& is_array)
			: base_struct(values, size), var_name(name) {
		}

		/// @brief constructor for array values with c-style array
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class Type																						  = value_type,
		  std::enable_if_t<(utils::is_glsl_simple_v<Type> && std::is_same_v<Type, value_type> && is_array), bool> = true>
		#endif
		glsl_value(const std::string_view name,
		  const value_type (&values)[array_size]) _GLSL_STRUCT_REQUIRES(utils::is_glsl_simple_v<value_type>&& is_array)
			: base_struct(values), var_name(name) {
		}

		#if _GLSL_STRUCT_HAS_CXX20
		/// @brief constructor for array values with std::span
		glsl_value(const std::string_view name,
		  const std::span<const value_type> values) _GLSL_STRUCT_REQUIRES(utils::is_glsl_simple_v<value_type>&& is_array)
			: base_struct(values), var_name(name) {}
		#endif

		#pragma endregion

		#pragma region STRUCT_ARRAY_CONSTRUCTORS
		/// @brief constructor for struct array values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_struct Type = value_type>
		#else
		template<class Type																						  = value_type,
		  std::enable_if_t<(utils::is_glsl_struct_v<Type> && std::is_same_v<Type, value_type> && is_array), bool> = true>
		#endif
		glsl_value(const std::string_view name, const typename Type::layout_type& layout) _GLSL_STRUCT_REQUIRES(
		  (utils::is_glsl_struct_v<value_type> && std::is_same_v<Type, value_type> && is_array)
		)
			: base_struct(layout), var_name(name) {
		}

		/// @brief constructor for array values with std::vector
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_struct Type = value_type>
		#else
		template<class Type																						  = value_type,
		  std::enable_if_t<(utils::is_glsl_struct_v<Type> && std::is_same_v<Type, value_type> && is_array), bool> = true>
		#endif
		glsl_value(
		  const std::string_view name, const typename Type::layout_type& layout,
		  const std::vector<std::vector<std::byte> >& values
		) _GLSL_STRUCT_REQUIRES((utils::is_glsl_struct_v<value_type> && std::is_same_v<Type, value_type> && is_array))
			: base_struct(layout, values), var_name(name) {
		}

		/// @brief constructor for array values with std::array
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_struct Type = value_type>
		#else
		template<class Type																						  = value_type,
		  std::enable_if_t<(utils::is_glsl_struct_v<Type> && std::is_same_v<Type, value_type> && is_array), bool> = true>
		#endif
		glsl_value(
		  const std::string_view name, const typename Type::layout_type& layout,
		  const std::array<std::vector<std::byte>, array_size>& values
		) _GLSL_STRUCT_REQUIRES((utils::is_glsl_struct_v<value_type> && std::is_same_v<Type, value_type> && is_array))
			: base_struct(layout, values), var_name(name) {
		}

		/// @brief constructor for array values with pointer to values and size
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_struct Type = value_type>
		#else
		template<class Type																						  = value_type,
		  std::enable_if_t<(utils::is_glsl_struct_v<Type> && std::is_same_v<Type, value_type> && is_array), bool> = true>
		#endif
		glsl_value(const std::string_view name, const typename Type::layout_type& layout, const std::vector<std::byte>* values,
		  const size_t size) _GLSL_STRUCT_REQUIRES((utils::is_glsl_struct_v<value_type> && std::is_same_v<Type, value_type> &&
													is_array))
			: base_struct(layout, values, size), var_name(name) {
		}

		/// @brief constructor for array values with c-style array values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_struct Type = value_type>
		#else
		template<class Type																						  = value_type,
		  std::enable_if_t<(utils::is_glsl_struct_v<Type> && std::is_same_v<Type, value_type> && is_array), bool> = true>
		#endif
		glsl_value(
		  const std::string_view name, const typename Type::layout_type& layout, const std::vector<std::byte> (&values)[num]
		) _GLSL_STRUCT_REQUIRES((utils::is_glsl_struct_v<value_type> && std::is_same_v<Type, value_type> && is_array))
			: base_struct(layout, values), var_name(name) {
		}

		#if _GLSL_STRUCT_HAS_CXX20
		/// @brief constructor for array values with std::span
		template<utils::glsl_struct Type = value_type>
		glsl_value(
		  const std::string_view name, const typename Type::layout_type& layout,
		  const std::span<const std::vector<std::byte> > values
		) _GLSL_STRUCT_REQUIRES((utils::is_glsl_struct_v<value_type> && std::is_same_v<Type, value_type> && is_array))
			: base_struct(layout, values), var_name(name) {}
		#endif

		#pragma endregion
	};
} // namespace glslstruct

	#endif
#endif