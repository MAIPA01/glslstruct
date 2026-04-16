/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_LAYOUT_TRAITS_CONCEPT_HPP_
	#define _GLSL_STRUCT_LAYOUT_TRAITS_CONCEPT_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/utils/ValueType.hpp>

/**
 * @defgroup layout_traits Layout Traits
 * @brief Traits for different layouts
 * @ingroup glslstruct
 * @details Layout Trait needs to have defined static functions:
 * @code{.cpp}
 * size_t get_scalar_alignment(const ValueType type, [[optional(define only if you have using context_type)]] context_type& ctx);                    // returns base alignment of scalar with given scalar type
 * size_t get_vec_alignment(const ValueType type, const size_t length, [[optional(define only if you have using context_type)]] context_type& ctx);  // returns base alignment of vec with given scalar type and length
 * size_t get_array_alignment(const size_t elemBaseAlignment, [[optional(define only if you have using context_type)]] context_type& ctx);           // returns base alignment of array with given array elem base alignment
 * size_t get_struct_alignment([[optional(define only if you have using context_type)]] context_type& ctx);                                          // returns base alignment of struct
 * @endcode
 *
 * There are also optional functions:
 * - To make action before variable is added:
 * @code{.cpp}
 * // Default for all types
 * void before_add(size_t& currentOffset, [[optional(define only if you have using context_type)]] context_type& ctx);
 *
 * // Specialized for given type (if defined they are invoked instead of before_add)
 * void before_add_scalar(size_t& currentOffset, [[optional(define only if you have using context_type)]] context_type& ctx);
 * void before_add_vec(size_t& currentOffset, [[optional(define only if you have using context_type)]] context_type& ctx);
 * void before_add_mat(size_t& currentOffset, [[optional(define only if you have using context_type)]] context_type& ctx);
 * void before_add_array(size_t& currentOffset, [[optional(define only if you have using context_type)]] context_type& ctx);
 * void before_add_struct(size_t& currentOffset, [[optional(define only if you have using context_type)]] context_type& ctx);
 * @endcode
 *
 * - To make some action after variable was added:
 * @code{.cpp}
 * // Default for all types
 * void after_add(size_t& currentOffset, const size_t size, const size_t alignment, [[optional(define only if you have using context_type)]] context_type& ctx);
 *
 * // Specialized for given type (if defined they are invoked instead of after_add)
 * void after_add_scalar(size_t& currentOffset, const size_t size, const size_t alignment, [[optional(define only if you have using context_type)]] context_type& ctx);
 * void after_add_vec(size_t& currentOffset, const size_t size, const size_t alignment, [[optional(define only if you have using context_type)]] context_type& ctx);
 * void after_add_mat(size_t& currentOffset, const size_t size, const size_t alignment, [[optional(define only if you have using context_type)]] context_type& ctx);
 * void after_add_array(size_t& currentOffset, const size_t size, const size_t alignment, [[optional(define only if you have using context_type)]] context_type& ctx);
 * void after_add_struct(size_t& currentOffset, const size_t size, const size_t alignment, [[optional(define only if you have using context_type)]] context_type& ctx);
 * @endcode
 */

namespace glslstruct {
		#if _GLSL_STRUCT_HAS_CXX20
			#pragma region CXX20_CONCEPTS

	namespace utils {
			#pragma region HAS_CONTEXT
		/**
		 * @ingroup utils
		 * @brief Checks if a type T provides a nested 'context_type'.
		 *
		 * Layout traits can optionally depend on a context object (e.g., to track
		 * state across multiple member additions).
		 */
		template<class T> concept has_layout_traits_context = requires { typename T::context_type; };
			#pragma endregion

			#pragma region GET_ALIGNMENT
		/**
		 * @ingroup utils
		 * @brief Validates that T provides mandatory alignment calculation methods.
		 *
		 * A valid layout trait must implement static methods for calculating
		 * scalar, vector, array, and struct alignments.
		 *
		 * @tparam T The layout trait class.
		 * @tparam Args Optional context arguments.
		 */
		template<class T, class... Args>
		concept layout_traits_get_alignment = requires (Args&... args) {
			{ T::get_scalar_alignment(std::declval<const ValueType>(), args...) } -> std::same_as<size_t>;
			{
				T::get_vec_alignment(std::declval<const ValueType>(), std::declval<const size_t>(), args...)
			} -> std::same_as<size_t>;
			{ T::get_array_alignment(std::declval<const size_t>(), args...) } -> std::same_as<size_t>;
			{ T::get_struct_alignment(args...) } -> std::same_as<size_t>;
		};
			#pragma endregion

			#pragma region BEFORE_ADD
			#pragma region OPTIONAL
		template<class T> concept has_layout_traits_opt_before_add = requires { T::before_add; };

		template<class T, class... Args>
		concept layout_traits_opt_before_add = !has_layout_traits_opt_before_add<T> || requires {
			{ T::before_add(std::declval<size_t&>(), std::declval<Args&>()...) } -> std::same_as<void>;
		};

		template<class T> concept has_layout_traits_opt_before_add_scalar = requires { T::before_add_scalar; };

		template<class T, class... Args>
		concept layout_traits_opt_before_add_scalar = !has_layout_traits_opt_before_add_scalar<T> || requires {
			{ T::before_add_scalar(std::declval<size_t&>(), std::declval<Args&>()...) } -> std::same_as<void>;
		};

		template<class T> concept has_layout_traits_opt_before_add_vec = requires { T::before_add_vec; };

		template<class T, class... Args>
		concept layout_traits_opt_before_add_vec = !has_layout_traits_opt_before_add_vec<T> || requires {
			{ T::before_add_vec(std::declval<size_t&>(), std::declval<Args&>()...) } -> std::same_as<void>;
		};

		template<class T> concept has_layout_traits_opt_before_add_mat = requires { T::before_add_mat; };

		template<class T, class... Args>
		concept layout_traits_opt_before_add_mat = !has_layout_traits_opt_before_add_mat<T> || requires {
			{ T::before_add_mat(std::declval<size_t&>(), std::declval<Args&>()...) } -> std::same_as<void>;
		};

		template<class T> concept has_layout_traits_opt_before_add_array = requires { T::before_add_array; };

		template<class T, class... Args>
		concept layout_traits_opt_before_add_array = !has_layout_traits_opt_before_add_array<T> || requires {
			{ T::before_add_array(std::declval<size_t&>(), std::declval<Args&>()...) } -> std::same_as<void>;
		};

		template<class T> concept has_layout_traits_opt_before_add_struct = requires { T::before_add_struct; };

		template<class T, class... Args>
		concept layout_traits_opt_before_add_struct = !has_layout_traits_opt_before_add_struct<T> || requires {
			{ T::before_add_struct(std::declval<size_t&>(), std::declval<Args&>()...) } -> std::same_as<void>;
		};
			#pragma endregion

		/**
		 * @ingroup utils
		 * @brief Validates optional "before_add" hooks.
		 *
		 * These hooks allow a layout trait to modify the current offset before a
		 * member is added to the structure.
		 */
		template<class T, class... Args>
		concept layout_traits_before_add =
		  layout_traits_opt_before_add<T, Args...> && layout_traits_opt_before_add_scalar<T, Args...> &&
		  layout_traits_opt_before_add_vec<T, Args...> && layout_traits_opt_before_add_mat<T, Args...> &&
		  layout_traits_opt_before_add_array<T, Args...> && layout_traits_opt_before_add_struct<T, Args...>;
			#pragma endregion

			#pragma region AFTER_ADD
			#pragma region OPTIONAL
		template<class T> concept has_layout_traits_opt_after_add = requires { T::after_add; };

		template<class T, class... Args>
		concept layout_traits_opt_after_add = !has_layout_traits_opt_after_add<T> || requires {
			{
				T::after_add(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
				  std::declval<Args&>()...)
			} -> std::same_as<void>;
		};

		template<class T> concept has_layout_traits_opt_after_add_scalar = requires { T::after_add_scalar; };

		template<class T, class... Args>
		concept layout_traits_opt_after_add_scalar = !has_layout_traits_opt_after_add_scalar<T> || requires {
			{
				T::after_add_scalar(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
				  std::declval<Args&>()...)
			} -> std::same_as<void>;
		};

		template<class T> concept has_layout_traits_opt_after_add_vec = requires { T::after_add_vec; };

		template<class T, class... Args>
		concept layout_traits_opt_after_add_vec = !has_layout_traits_opt_after_add_vec<T> || requires {
			{
				T::after_add_vec(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
				  std::declval<Args&>()...)
			} -> std::same_as<void>;
		};

		template<class T> concept has_layout_traits_opt_after_add_mat = requires { T::after_add_mat; };

		template<class T, class... Args>
		concept layout_traits_opt_after_add_mat = !has_layout_traits_opt_after_add_mat<T> || requires {
			{
				T::after_add_mat(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
				  std::declval<Args&>()...)
			} -> std::same_as<void>;
		};

		template<class T> concept has_layout_traits_opt_after_add_array = requires { T::after_add_array; };

		template<class T, class... Args>
		concept layout_traits_opt_after_add_array = !has_layout_traits_opt_after_add_array<T> || requires {
			{
				T::after_add_array(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
				  std::declval<Args&>()...)
			} -> std::same_as<void>;
		};

		template<class T> concept has_layout_traits_opt_after_add_struct = requires { T::after_add_struct; };

		template<class T, class... Args>
		concept layout_traits_opt_after_add_struct = !has_layout_traits_opt_after_add_struct<T> || requires {
			{
				T::after_add_struct(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
				  std::declval<Args&>()...)
			} -> std::same_as<void>;
		};
			#pragma endregion

		/**
		 * @ingroup utils
		 * @brief Validates optional "after_add" hooks.
		 *
		 * These hooks allow a layout trait to update the current offset or context
		 * after a member has been placed, often used to handle padding.
		 */
		template<class T, class... Args>
		concept layout_traits_after_add =
		  layout_traits_opt_after_add<T, Args...> && layout_traits_opt_after_add_scalar<T, Args...> &&
		  layout_traits_opt_after_add_vec<T, Args...> && layout_traits_opt_after_add_mat<T, Args...> &&
		  layout_traits_opt_after_add_array<T, Args...> && layout_traits_opt_after_add_struct<T, Args...>;
			#pragma endregion

		template<class T>
		concept layout_traits_with_context =
		  has_layout_traits_context<T> && layout_traits_get_alignment<T, typename T::context_type> &&
		  layout_traits_before_add<T, typename T::context_type> && layout_traits_after_add<T, typename T::context_type>;

		template<class T>
		concept layout_traits_without_context = !has_layout_traits_context<T> && layout_traits_get_alignment<T> &&
												layout_traits_before_add<T> && layout_traits_after_add<T>;
	} // namespace utils

	/**
	 * @ingroup layout_traits
	 * @brief The primary concept for GLSL layout trait definitions.
	 *
	 * A type satisfies `layout_traits` if it provides the necessary alignment
	 * logic and follows the optional hook patterns, either with or without
	 * a nested context_type.
	 */
	template<class T> concept layout_traits = utils::layout_traits_with_context<T> || utils::layout_traits_without_context<T>;

	/**
	 * @ingroup layout_traits
	 * @brief Compile-time constant to check if T is a valid layout trait.
	 */
	template<class T>
	static _GLSL_STRUCT_EXPORT _GLSL_STRUCT_CONSTEXPR17 bool is_layout_traits_v = layout_traits<T>;

	/**
	 * @ingroup layout_traits
	 * @brief Type trait to check if T is a valid layout trait.
	 */
	template<class T>
	struct _GLSL_STRUCT_EXPORT is_layout_traits : std::bool_constant<is_layout_traits_v<T> > {};

	namespace utils {
		/**
		 * @ingroup layout_traits
		 * @name Feature Detection Constants
		 * @brief Boolean constants to detect specific optional methods within a layout trait.
		 * @{
		 */
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_context_v = layout_traits<T> && has_layout_traits_context<T>;

			#pragma region HAS_BEFORE_ADD
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_v =
		  layout_traits<T> && has_layout_traits_opt_before_add<T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_scalar_v =
		  layout_traits<T> && has_layout_traits_opt_before_add_scalar<T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_vec_v =
		  layout_traits<T> && has_layout_traits_opt_before_add_vec<T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_mat_v =
		  layout_traits<T> && has_layout_traits_opt_before_add_mat<T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_array_v =
		  layout_traits<T> && has_layout_traits_opt_before_add_array<T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_struct_v =
		  layout_traits<T> && has_layout_traits_opt_before_add_struct<T>;
			#pragma endregion

			#pragma region HAS_AFTER_ADD
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_v =
		  layout_traits<T> && has_layout_traits_opt_after_add<T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_scalar_v =
		  layout_traits<T> && has_layout_traits_opt_after_add_scalar<T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_vec_v =
		  layout_traits<T> && has_layout_traits_opt_after_add_vec<T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_mat_v =
		  layout_traits<T> && has_layout_traits_opt_after_add_mat<T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_array_v =
		  layout_traits<T> && has_layout_traits_opt_after_add_array<T>;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_struct_v =
		  layout_traits<T> && has_layout_traits_opt_after_add_struct<T>;
			#pragma endregion
		/** @} */
	} // namespace utils

			#pragma endregion
		#else
			#pragma region CXX17_CONCEPTS

	namespace utils {
			#pragma region HAS_CONTEXT

		/**
		 * @ingroup layout_traits
		 * @brief Checks if a type T provides a nested 'context_type'.
		 *
		 * Layout traits can optionally depend on a context object (e.g., to track
		 * state across multiple member additions).
		 */
		template<class T, class = void>
		struct has_layout_traits_context : std::false_type {};

		template<class T>
		struct has_layout_traits_context<T, std::void_t<typename T::context_type> > : std::true_type {};

			#pragma endregion

			#pragma region GET_ALIGNMENT

		/**
		 * @ingroup layout_traits
		 * @brief Validates that T provides mandatory alignment calculation methods.
		 *
		 * A valid layout trait must implement static methods for calculating
		 * scalar, vector, array, and struct alignments.
		 *
		 * @tparam T The layout trait class.
		 * @tparam Args Optional context arguments.
		 */
		template<class T, class Enable, class... Args>
		struct layout_traits_get_alignment : std::false_type {};

		template<class T, class... Args>
		struct layout_traits_get_alignment<T,
		  std::void_t<std::enable_if_t<std::is_same_v<size_t,
						decltype(T::get_scalar_alignment(std::declval<const ValueType>(), std::declval<Args&>()...))> >,
			std::enable_if_t<std::is_same_v<size_t, decltype(T::get_vec_alignment(std::declval<const ValueType>(),
													  std::declval<const size_t>(), std::declval<Args&>()...))> >,
			std::enable_if_t<std::is_same_v<size_t,
			  decltype(T::get_array_alignment(std::declval<const size_t>(), std::declval<Args&>()...))> >,
			std::enable_if_t<std::is_same_v<size_t, decltype(T::get_struct_alignment(std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

			#pragma endregion

			#pragma region BEFORE_ADD
			#pragma region OPTIONAL

		template<class T, class = void>
		struct has_layout_traits_opt_before_add : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_before_add<T, std::void_t<decltype(T::before_add)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_before_add_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_before_add_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::before_add(std::declval<size_t&>(), std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_before_add : std::bool_constant<!has_layout_traits_opt_before_add<T>::value ||
																 is_layout_traits_opt_before_add_valid<T, void, Args...>::value> {
		};

		template<class T, class = void>
		struct has_layout_traits_opt_before_add_scalar : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_before_add_scalar<T, std::void_t<decltype(T::before_add_scalar)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_before_add_scalar_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_before_add_scalar_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::before_add_scalar(std::declval<size_t&>(), std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_before_add_scalar
			: std::bool_constant<!has_layout_traits_opt_before_add_scalar<T>::value ||
								 is_layout_traits_opt_before_add_scalar_valid<T, void, Args...>::value> {};

		template<class T, class = void>
		struct has_layout_traits_opt_before_add_vec : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_before_add_vec<T, std::void_t<decltype(T::before_add_vec)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_before_add_vec_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_before_add_vec_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::before_add_vec(std::declval<size_t&>(), std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_before_add_vec
			: std::bool_constant<!has_layout_traits_opt_before_add_vec<T>::value ||
								 is_layout_traits_opt_before_add_vec_valid<T, void, Args...>::value> {};

		template<class T, class = void>
		struct has_layout_traits_opt_before_add_mat : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_before_add_mat<T, std::void_t<decltype(T::before_add_mat)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_before_add_mat_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_before_add_mat_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::before_add_mat(std::declval<size_t&>(), std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_before_add_mat
			: std::bool_constant<!has_layout_traits_opt_before_add_mat<T>::value ||
								 is_layout_traits_opt_before_add_mat_valid<T, void, Args...>::value> {};

		template<class T, class = void>
		struct has_layout_traits_opt_before_add_array : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_before_add_array<T, std::void_t<decltype(T::before_add_array)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_before_add_array_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_before_add_array_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::before_add_array(std::declval<size_t&>(), std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_before_add_array
			: std::bool_constant<!has_layout_traits_opt_before_add_array<T>::value ||
								 is_layout_traits_opt_before_add_array_valid<T, void, Args...>::value> {};

		template<class T, class = void>
		struct has_layout_traits_opt_before_add_struct : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_before_add_struct<T, std::void_t<decltype(T::before_add_struct)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_before_add_struct_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_before_add_struct_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::before_add_struct(std::declval<size_t&>(), std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_before_add_struct
			: std::bool_constant<!has_layout_traits_opt_before_add_struct<T>::value ||
								 is_layout_traits_opt_before_add_struct_valid<T, void, Args...>::value> {};

			#pragma endregion

		/**
		 * @ingroup layout_traits
		 * @brief Validates optional "before_add" hooks.
		 *
		 * These hooks allow a layout trait to modify the current offset before a
		 * member is added to the structure.
		 */
		template<class T, class... Args>
		struct layout_traits_before_add
			: std::bool_constant<
				layout_traits_opt_before_add<T, Args...>::value && layout_traits_opt_before_add_scalar<T, Args...>::value &&
				layout_traits_opt_before_add_vec<T, Args...>::value && layout_traits_opt_before_add_mat<T, Args...>::value &&
				layout_traits_opt_before_add_array<T, Args...>::value && layout_traits_opt_before_add_struct<T, Args...>::value
			  > {};

			#pragma endregion

			#pragma region AFTER_ADD
			#pragma region OPTIONAL

		template<class T, class = void>
		struct has_layout_traits_opt_after_add : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_after_add<T, std::void_t<decltype(T::after_add)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_after_add_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_after_add_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::after_add(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
			  std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_after_add : std::bool_constant<!has_layout_traits_opt_after_add<T>::value ||
																is_layout_traits_opt_after_add_valid<T, void, Args...>::value> {};

		template<class T, class = void>
		struct has_layout_traits_opt_after_add_scalar : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_after_add_scalar<T, std::void_t<decltype(T::after_add_scalar)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_after_add_scalar_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_after_add_scalar_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::after_add_scalar(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
			  std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_after_add_scalar
			: std::bool_constant<!has_layout_traits_opt_after_add_scalar<T>::value ||
								 is_layout_traits_opt_after_add_scalar_valid<T, void, Args...>::value> {};

		template<class T, class = void>
		struct has_layout_traits_opt_after_add_vec : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_after_add_vec<T, std::void_t<decltype(T::after_add_vec)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_after_add_vec_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_after_add_vec_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::after_add_vec(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
			  std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_after_add_vec
			: std::bool_constant<!has_layout_traits_opt_after_add_vec<T>::value ||
								 is_layout_traits_opt_after_add_vec_valid<T, void, Args...>::value> {};

		template<class T, class = void>
		struct has_layout_traits_opt_after_add_mat : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_after_add_mat<T, std::void_t<decltype(T::after_add_mat)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_after_add_mat_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_after_add_mat_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::after_add_mat(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
			  std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_after_add_mat
			: std::bool_constant<!has_layout_traits_opt_after_add_mat<T>::value ||
								 is_layout_traits_opt_after_add_mat_valid<T, void, Args...>::value> {};

		template<class T, class = void>
		struct has_layout_traits_opt_after_add_array : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_after_add_array<T, std::void_t<decltype(T::after_add_array)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_after_add_array_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_after_add_array_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::after_add_array(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
			  std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_after_add_array
			: std::bool_constant<!has_layout_traits_opt_after_add_array<T>::value ||
								 is_layout_traits_opt_after_add_array_valid<T, void, Args...>::value> {};

		template<class T, class = void>
		struct has_layout_traits_opt_after_add_struct : std::false_type {};

		template<class T>
		struct has_layout_traits_opt_after_add_struct<T, std::void_t<decltype(T::after_add_struct)> > : std::true_type {};

		template<class T, class Enable, class... Args>
		struct is_layout_traits_opt_after_add_struct_valid : std::false_type {};

		template<class T, class... Args>
		struct is_layout_traits_opt_after_add_struct_valid<T,
		  std::void_t<std::enable_if_t<std::is_same_v<void,
			decltype(T::after_add_struct(std::declval<size_t&>(), std::declval<const size_t>(), std::declval<const size_t>(),
			  std::declval<Args&>()...))> > >,
		  Args...> : std::true_type {};

		template<class T, class... Args>
		struct layout_traits_opt_after_add_struct
			: std::bool_constant<!has_layout_traits_opt_after_add_struct<T>::value ||
								 is_layout_traits_opt_after_add_struct_valid<T, void, Args...>::value> {};

			#pragma endregion

		/**
		 * @ingroup layout_traits
		 * @brief Validates optional "after_add" hooks.
		 *
		 * These hooks allow a layout trait to update the current offset or context
		 * after a member has been placed, often used to handle padding.
		 */
		template<class T, class... Args>
		struct layout_traits_after_add
			: std::bool_constant<
				layout_traits_opt_after_add<T, Args...>::value && layout_traits_opt_after_add_scalar<T, Args...>::value &&
				layout_traits_opt_after_add_vec<T, Args...>::value && layout_traits_opt_after_add_mat<T, Args...>::value &&
				layout_traits_opt_after_add_array<T, Args...>::value && layout_traits_opt_after_add_struct<T, Args...>::value
			  > {};

			#pragma endregion

		template<class T>
		struct layout_traits_with_context
			: std::bool_constant<has_layout_traits_context<T>::value &&
								 layout_traits_get_alignment<T, void, typename T::context_type>::value &&
								 layout_traits_before_add<T, typename T::context_type>::value &&
								 layout_traits_after_add<T, typename T::context_type>::value> {};

		template<class T>
		struct layout_traits_without_context
			: std::bool_constant<!has_layout_traits_context<T>::value && layout_traits_get_alignment<T, void>::value &&
								 layout_traits_before_add<T>::value && layout_traits_after_add<T>::value> {};
	} // namespace utils

	/**
	 * @ingroup layout_traits
	 * @brief The primary concept for GLSL layout trait definitions.
	 *
	 * A type satisfies `layout_traits` if it provides the necessary alignment
	 * logic and follows the optional hook patterns, either with or without
	 * a nested context_type.
	 */
	template<class T>
	struct is_layout_traits
		: std::bool_constant<utils::layout_traits_with_context<T>::value || utils::layout_traits_without_context<T>::value> {};

	/**
	 * @ingroup layout_traits
	 * @brief Compile-time constant to check if T is a valid layout trait.
	 */
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_layout_traits_v = is_layout_traits<T>::value;

	namespace utils {
		/**
		 * @ingroup layout_traits
		 * @name Feature Detection Constants
		 * @brief Boolean constants to detect specific optional methods within a layout trait.
		 * @{
		 */
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_context_v =
		  is_layout_traits<T>::value && has_layout_traits_context<T>::value;

			#pragma region HAS_BEFORE_ADD
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_before_add<T>::value;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_scalar_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_before_add_scalar<T>::value;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_vec_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_before_add_vec<T>::value;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_mat_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_before_add_mat<T>::value;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_array_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_before_add_array<T>::value;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_before_add_struct_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_before_add_struct<T>::value;
			#pragma endregion

			#pragma region HAS_AFTER_ADD
		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_after_add<T>::value;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_scalar_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_after_add_scalar<T>::value;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_vec_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_after_add_vec<T>::value;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_mat_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_after_add_mat<T>::value;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_array_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_after_add_array<T>::value;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR17 bool has_layout_traits_after_add_struct_v =
		  is_layout_traits_v<T> && has_layout_traits_opt_after_add_struct<T>::value;
			#pragma endregion
		/** @} */
	} // namespace utils

			#pragma endregion
		#endif
} // namespace glslstruct

	#endif
#endif