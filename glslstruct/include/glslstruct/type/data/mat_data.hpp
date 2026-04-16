/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_MAT_DATA_HPP_
	#define _GLSL_STRUCT_MAT_DATA_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/type/data/vec_data.hpp>
		#include <glslstruct/type/traits/concepts/mat_traits_concept.hpp>

namespace glslstruct {
	/**
	 * @brief glsl mat data container
	 * @ingroup glslstruct
	 */
	class _GLSL_STRUCT_EXPORT mat_data {
	private:
		/// @brief returns vector of data bytes of glm::mat
		template<class T, glm::length_t C, glm::length_t R, glm::qualifier Q>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::vector<vec_data> _get_data(const glm::mat<C, R, T, Q>& value) {
			static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable to be treated as raw bytes!!");

			std::vector<vec_data> data;
				for (size_t c = 0; c < C; ++c) { data.emplace_back(glm::column(value, c)); }
			return data;
		}

		/// @brief returns vector of data bytes of mstd::mat
		template<class T, size_t C, size_t R>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::vector<vec_data> _get_data(const mstd::mat<C, R, T>& value) {
			static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable to be treated as raw bytes!!");

			using column_type = _GLSL_STRUCT_TYPENAME17 mstd::mat<C, R, T>::column_type;

			std::vector<vec_data> data;
				for (size_t c = 0; c < C; ++c) { data.emplace_back(column_type(value[c])); }

			return data;
		}

		/// @brief vector of data bytes
		std::vector<vec_data> _data;

	public:
		/// @brief constructor which accepts raw data (not recommended to use for end user)
		explicit mat_data(const std::vector<vec_data>& data);

		/// @brief constructor for glm::mat
		#if _GLSL_STRUCT_HAS_CXX20
		template<class T, glm::length_t C, glm::length_t R, glm::qualifier Q>
		#else
		template<class T, glm::length_t C, glm::length_t R, glm::qualifier Q,
		  std::enable_if_t<(mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double> && mstd::is_in_range_v<C, 2, 4> &&
							 mstd::is_in_range_v<R, 2, 4>),
			bool> = true>
		#endif
		explicit mat_data(
		  const glm::mat<C, R, T, Q>& value
		) _GLSL_STRUCT_REQUIRES((mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double> &&
								 mstd::is_in_range_v<C, 2, 4> && mstd::is_in_range_v<R, 2, 4>))
			: mat_data(_get_data(value)) {
		}

		/// @brief constructor for mstd::mat
		#if _GLSL_STRUCT_HAS_CXX20
		template<class T, size_t C, size_t R>
		#else
		template<class T, size_t C, size_t R,
		  std::enable_if_t<(mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double> && mstd::is_in_range_v<C, 2, 4> &&
							 mstd::is_in_range_v<R, 2, 4>),
			bool> = true>
		#endif
		explicit mat_data(
		  const mstd::mat<C, R, T>& value
		) _GLSL_STRUCT_REQUIRES((mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double> &&
								 mstd::is_in_range_v<C, 2, 4> && mstd::is_in_range_v<R, 2, 4>))
			: mat_data(_get_data(value)) {
		}

		/// @brief default copy constructor
		mat_data(const mat_data& other);
		/// @brief default move constructor
		mat_data(mat_data&& other) noexcept;

		/// @brief default destructor
		~mat_data();

		/// @brief default copy assign operator
		mat_data& operator=(const mat_data& other);
		/// @brief default move assign operator
		mat_data& operator=(mat_data&& other) noexcept;

		/// @brief returns value from this mat_data container using mat_traits conversion
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_mat_v<T>, bool> = true>
		#endif
		T get() const {
			return mat_traits<T>::get_value(*this);
		}

		/// @brief returns value data in bytes
		[[nodiscard]] const std::vector<vec_data>& data() const noexcept;
	};
} // namespace glslstruct

	#endif
#endif