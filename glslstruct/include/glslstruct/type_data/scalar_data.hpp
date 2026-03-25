/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_SCALAR_DATA_HPP_
	#define _GLSL_STRUCT_SCALAR_DATA_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/type_traits_concepts/scalar_traits_concept.hpp>

namespace glslstruct {
	template<class>
	struct scalar_traits;

	class scalar_data {
	private:
		template<class T>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_value_data(const T& value) {
			static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable to be treated as raw bytes!!");

			const auto* valueDataPtr = reinterpret_cast<const std::byte*>(std::addressof(value));
			return { valueDataPtr, valueDataPtr + sizeof(T) };
		}

		std::vector<std::byte> _data;

	public:
		explicit scalar_data(const std::vector<std::byte>& data);
		explicit scalar_data(bool value);
		explicit scalar_data(int value);
		explicit scalar_data(unsigned int value);
		explicit scalar_data(float value);
		explicit scalar_data(double value);

		scalar_data(const scalar_data& other);
		scalar_data(scalar_data&& other) noexcept;

		~scalar_data();

		scalar_data& operator=(const scalar_data& other);
		scalar_data& operator=(scalar_data&& other) noexcept;

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		T get() const {
			return scalar_traits<T>::get_value(*this);
		}

		[[nodiscard]] const std::vector<std::byte>& data() const noexcept;
	};
} // namespace glslstruct

	#endif
#endif