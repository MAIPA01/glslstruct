/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

#include <glslstruct/value_types/MajorType.hpp>

namespace glslstruct {
	class mat_data {
	private:
		template<class T, size_t N>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_data(const std::array<T, N>& values) {
			static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable to be treated as raw bytes!!");

			std::vector<std::byte> data;
			data.reserve(sizeof(T) * N);

			for (const auto& value : values) {
				const auto* valuePtr = reinterpret_cast<const std::byte*>(std::addressof(value));
				data.insert(data.end(), valuePtr, valuePtr + sizeof(T));
			}
			return data;
		}

		template<class T, size_t C, size_t R, size_t N = C * R>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 std::array<T, N> _to_array(const glm::mat<C, R, T>& value, MajorType major) {
			std::array<T, N> array;
			switch (major) {
			case MajorType::Column:
				std::copy_n(glm::value_ptr(value), N, array.begin());
				break;
			case MajorType::Row:
				for (size_t r = 0; r < R; ++r) {
					for (size_t c = 0; c < C; ++c) {
						array[r * C + c] = value[c][r];
					}
				}
				break;
			default:
				break;
			}
			return array;
		}

		template<class T, size_t C, size_t R, size_t N = C * R>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 std::array<T, N> _to_array(const mstd::mat<C, R, T>& value, MajorType major) {
			std::array<T, N> array;
			switch (major) {
			case MajorType::Column:
				std::copy_n(static_cast<const T*>(value), N, array.begin());
				break;
			case MajorType::Row:
				for (size_t r = 0; r < R; ++r) {
					for (size_t c = 0; c < C; ++c) {
						array[r * C + c] = value[c][r];
					}
				}
				break;
			default:
				break;
			}
			return array;
		}

		std::vector<std::byte> _data;

	public:
		template<class T, size_t C, size_t R>
		explicit mat_data(const glm::mat<C, R, T>& value, MajorType major = MajorType::Column) : _data(_get_data(_to_array(value, major))) {}
		template<class T, size_t C, size_t R>
		explicit mat_data(const mstd::mat<C, R, T>& value, MajorType major = MajorType::Column) : _data(_get_data(_to_array(value, major))) {}

		mat_data(const mat_data& other);
		mat_data(mat_data&& other) noexcept;

		~mat_data();

		mat_data& operator=(const mat_data& other);
		mat_data& operator=(mat_data&& other) noexcept;

		[[nodiscard]] const std::vector<std::byte>& data() const noexcept;
	};
}

#endif