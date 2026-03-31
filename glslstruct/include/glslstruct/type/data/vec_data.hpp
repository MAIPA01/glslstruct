/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_VEC_DATA_HPP_
	#define _GLSL_STRUCT_VEC_DATA_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/type/traits/concepts/vec_traits_concept.hpp>

namespace glslstruct {
	/**
	 * @brief glsl vec data container
	 * @ingroup glslstruct
	 */
	class vec_data {
	private:
		/// @brief returns vector of data bytes of array of N values of type T
		template<class T, size_t N>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_data(const std::array<T, N>& values) {
			static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable to be treated as raw bytes!!");

				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					std::vector<std::byte> data;
					data.reserve(N * sizeof(int));

						for (const auto& value : values) {
							int iValue			  = static_cast<int>(value);
							const auto* iValuePtr = reinterpret_cast<const std::byte*>(std::addressof(iValue));
							data.insert(data.end(), iValuePtr, iValuePtr + sizeof(int));
						}

					return data;
				}
				else {
					const auto* valuesPtr = reinterpret_cast<const std::byte*>(values.data());
					return { valuesPtr, valuesPtr + sizeof(T) * N };
				}
		}

		/// @brief returns array of N values of type T from glm::vec
		template<class T, glm::length_t L, glm::qualifier Q>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 std::array<T, L> _to_array(const glm::vec<L, T, Q>& value) {
			std::array<T, L> array;
			std::copy_n(glm::value_ptr(value), L, array.begin());
			return array;
		}

		/// @brief returns array of N values of type T from mstd::vec
		template<class T, size_t N>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 std::array<T, N> _to_array(const mstd::vec<N, T>& value) {
			std::array<T, N> array;
			std::copy_n(static_cast<const T*>(value), N, array.begin());
			return array;
		}

		/// @brief vector of data bytes
		std::vector<std::byte> _data;

	public:
		/// @brief constructor which accepts raw data (not recommended to use for end user)
		explicit vec_data(const std::vector<std::byte>& data);

		/// @brief constructor for glm::vec
		#if _GLSL_STRUCT_HAS_CXX20
		template<class T, glm::length_t L, glm::qualifier Q>
		#else
		template<class T, glm::length_t L, glm::qualifier Q,
		  std::enable_if_t<(mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double> && mstd::is_in_range_v<L, 2, 4>),
			bool> = true>
		#endif
		explicit vec_data(const glm::vec<L, T, Q>& value) _GLSL_STRUCT_REQUIRES((
		  mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double> && mstd::is_in_range_v<L, 2, 4>
		))
			: vec_data(_get_data(_to_array(value))) {
		}

		/// @brief constructor for mstd::vec
		#if _GLSL_STRUCT_HAS_CXX20
		template<class T, size_t N>
		#else
		template<class T, size_t N,
		  std::enable_if_t<(mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double> && mstd::is_in_range_v<N, 2, 4>),
			bool> = true>
		#endif
		explicit vec_data(const mstd::vec<N, T>& value) _GLSL_STRUCT_REQUIRES((
		  mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double> && mstd::is_in_range_v<N, 2, 4>
		))
			: vec_data(_get_data(_to_array(value))) {
		}

		/// @brief default copy constructor
		vec_data(const vec_data& other);
		/// @brief default move constructor
		vec_data(vec_data&& other) noexcept;

		/// @brief default destructor
		~vec_data();

		/// @brief default copy assign operator
		vec_data& operator=(const vec_data& other);
		/// @brief default move assign operator
		vec_data& operator=(vec_data&& other) noexcept;

		/// @brief returns value from this vec_data container using vec_traits conversion
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_vec_v<T>, bool> = true>
		#endif
		T get() const {
			return vec_traits<T>::get_value(*this);
		}

		/// @brief returns value data in bytes
		[[nodiscard]] const std::vector<std::byte>& data() const noexcept;
	};
} // namespace glslstruct

	#endif
#endif