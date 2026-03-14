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

namespace glslstruct {
	class vec_data {
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

		template<class T, size_t L>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 std::array<T, L> _to_array(const glm::vec<L, T>& value) {
			std::array<T, L> array;
			std::copy_n(glm::value_ptr(value), L, array.begin());
			return array;
		}

		template<class T, size_t N>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 std::array<T, N> _to_array(const mstd::vec<N, T>& value) {
			std::array<T, N> array;
			std::copy_n(static_cast<const T*>(value), N, array.begin());
			return array;
		}

		std::vector<std::byte> _data;

	public:
		template<class T, size_t L>
		explicit vec_data(const glm::vec<L, T>& value) : _data(_get_data(_to_array(value))) {}
		template<class T, size_t N>
		explicit vec_data(const mstd::vec<N, T>& value) : _data(_get_data(_to_array(value))) {}

		vec_data(const vec_data& other);
		vec_data(vec_data&& other) noexcept;

		~vec_data();

		vec_data& operator=(const vec_data& other);
		vec_data& operator=(vec_data&& other) noexcept;

		[[nodiscard]] const std::vector<std::byte>& data() const noexcept;
	};
}

#endif