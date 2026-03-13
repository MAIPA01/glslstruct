/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/templates.hpp>
#include <glslstruct/value_types.hpp>
#include <glslstruct/value_data.hpp>

namespace glslstruct {
	class std_offset {
	protected:
		friend struct std::hash<std_offset>;

		std::unordered_map<std::string, value_data> _values;
		size_t _currentOffset = 0;
		size_t _maxAligement = 0;

		[[nodiscard]] static std::string _get_array_elem_name(const std::string& arrayName, size_t elemIdx);
		[[nodiscard]] static std::string _get_struct_elem_name(const std::string& structName, const std::string& elemName);

		[[nodiscard]] static size_t _ceil_to_nearest_multiple(size_t valueToRoundUp, size_t multipleValue) noexcept;

		void _set_variable(const std::string& name, size_t offset, const base_type_handle& type, size_t size, size_t padding);
		void _remove_variable(const std::string& name);

		void _update_max_aligement(size_t baseAligement) noexcept;

		[[nodiscard]] virtual size_t _get_array_elem_base_aligement(size_t baseAligement) const noexcept;

		[[nodiscard]] virtual size_t _add(const std::string& name, size_t baseAligement, size_t baseOffset, const base_type_handle& type);
		[[nodiscard]] virtual std::vector<size_t> _add_array(const std::string& name, size_t arraySize, size_t baseAligement,
			size_t baseOffset, const base_type_handle& type);

		[[nodiscard]] virtual size_t _add_scalar(const std::string& name, size_t baseAligement, size_t baseOffset, ValueType type);
		[[nodiscard]] virtual std::vector<size_t> _add_scalar_array(const std::string& name, size_t baseAligement, size_t baseOffset,
			ValueType type, size_t arraySize);

		[[nodiscard]] virtual size_t _add_vec(const std::string& name, size_t length, size_t baseAligement, size_t baseOffset, ValueType type);
		[[nodiscard]] virtual std::vector<size_t> _add_vec_array(const std::string& name, size_t length, size_t baseAligement, size_t baseOffset,
			ValueType type, size_t arraySize);

		[[nodiscard]] virtual size_t _add_mat(const std::string& name, size_t columns, size_t rows, bool column_major, size_t baseAligement,
			size_t baseOffset, ValueType type);
		[[nodiscard]] virtual std::vector<size_t> _add_mat_array(const std::string& name, size_t columns, size_t rows, bool column_major,
			size_t baseAligement, size_t baseOffset, ValueType type, size_t arraySize);

		[[nodiscard]] virtual size_t _add_struct(const std::string& name, size_t baseAligement, size_t baseOffset,
			const std::unordered_map<std::string, value_data>& values);
		[[nodiscard]] virtual std::vector<size_t> _add_struct_array(const std::string& name, size_t baseAligement, size_t baseOffset,
			const std::unordered_map<std::string, value_data>& values, size_t arraySize);

		template<class T, class... Ts, size_t num, size_t... nums>
		_GLSL_STRUCT_CONSTEXPR17 void _add_multiple(const std_variable<T, num>& var, const std_variable<Ts, nums>&... vars) noexcept {
			if _GLSL_STRUCT_CONSTEXPR17 (var.is_offsets) {
				if _GLSL_STRUCT_CONSTEXPR17 (num == 0) {
					add(var.var_name, var.struct_offsets);
				}
				else {
					add(var.var_name, var.struct_offsets, num);
				}
			}
			else {
				if _GLSL_STRUCT_CONSTEXPR17 (num == 0) {
					add<T>(var.var_name);
				}
				else {
					add<T>(var.var_name, num);
				}
			}
			if _GLSL_STRUCT_CONSTEXPR17 (sizeof...(Ts) > 0 && sizeof...(nums) > 0) {
				_add_multiple(vars...);
			}
		}

		std_offset() noexcept = default;
		template<class... Ts, size_t... nums>
		std_offset(const std_variable<Ts, nums>&... vars) noexcept {
			_add_multiple(vars...);
		}
		std_offset(const std_offset& other) noexcept;
		std_offset(std_offset&& other) noexcept;
		virtual ~std_offset() noexcept = default;

		std_offset& operator=(const std_offset& other) noexcept;
		std_offset& operator=(std_offset&& other) noexcept;

	public:
		[[nodiscard]] static size_t bad_offset() noexcept;

		[[nodiscard]] bool contains(const std::string& name) const noexcept;

		[[nodiscard]] value_data get(const std::string& name) const noexcept;

		[[nodiscard]] size_t get_offset(const std::string& name) const noexcept;
		[[nodiscard]] std::vector<size_t> get_array_offsets(const std::string& name) const noexcept;

		[[nodiscard]] base_type_handle get_type(const std::string& name) const noexcept;
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
		[[nodiscard]] std::shared_ptr<T> get_type(const std::string& name) const noexcept {
			return dynamic_type_cast<T>(get_type(name));
		}
		[[nodiscard]] size_t get_total_size(const std::string& name) const noexcept;
		[[nodiscard]] size_t get_size(const std::string& name) const noexcept;
		[[nodiscard]] size_t get_padding(const std::string& name) const noexcept;

		[[nodiscard]] std::vector<std::string> get_names() const noexcept;

		[[nodiscard]] virtual size_t base_aligement() const noexcept;
		[[nodiscard]] virtual size_t size() const noexcept;
		[[nodiscard]] size_t padding() const noexcept;

		virtual void clear() noexcept;

#pragma region ADD_SCALAR
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		[[nodiscard]] size_t add(const std::string& name) {
			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = 
				std::is_same_v<T, bool> ? sizeof(unsigned int) : sizeof(T);
			static const ValueType valueType = get_value_type<T>();

			return _add_scalar(name, typeSize, typeSize, valueType);
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		[[nodiscard]] std::vector<size_t> add(const std::string& name, size_t count) {
			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize =
				std::is_same_v<T, bool> ? sizeof(unsigned int) : sizeof(T);
			static const ValueType valueType = get_value_type<T>();

			return _add_scalar_array(name, typeSize, typeSize, valueType, count);
		}
#pragma endregion

#pragma region ADD_VEC
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		[[nodiscard]] size_t add(const std::string& name) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();

			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize =
				std::is_same_v<T, bool> ? sizeof(unsigned int) : sizeof(T);
			static const ValueType valueType = get_value_type<T>();

			return _add_vec(name, L, typeSize, typeSize, valueType);
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		[[nodiscard]] std::vector<size_t> add(const std::string& name, size_t count) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();

			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize =
				std::is_same_v<T, bool> ? sizeof(unsigned int) : sizeof(T);
			static const ValueType valueType = get_value_type<T>();

			return _add_vec_array(name, L, typeSize, typeSize, valueType, count);
		}
#pragma endregion

#pragma region ADD_MAT
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, bool column_major = true)
		[[nodiscard]] size_t add(const std::string& name) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize =
				std::is_same_v<T, bool> ? sizeof(unsigned int) : sizeof(T);
			static const ValueType valueType = get_value_type<T>();

			return _add_mat(name, C, R, column_major, typeSize, typeSize, valueType);
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, bool column_major = true)
		[[nodiscard]] std::vector<size_t> add(const std::string& name, size_t count) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 size_t R = M::col_type::length();

			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize =
				std::is_same_v<T, bool> ? sizeof(unsigned int) : sizeof(T);
			static const ValueType valueType = get_value_type<T>();

			return _add_mat_array(name, C, R, column_major, typeSize, typeSize, valueType, count);
		}
#pragma endregion

#pragma region ADD_STRUCT
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(Off, utils::glsl_offset, utils::is_glsl_offset_v<Off>, = true)
		[[nodiscard]] size_t add(const std::string& name, const Off& offsets) {
			return _add_struct(name, offsets.base_aligement(), offsets._currentOffset, 
				offsets._values);
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(Off, utils::glsl_offset, utils::is_glsl_offset_v<Off>, = true)
		[[nodiscard]] std::vector<size_t> add(const std::string& name, const Off& offsets, size_t count) {
			return _add_struct_array(name, offsets.base_aligement(), offsets._currentOffset,
				offsets._values, count);
		}
#pragma endregion

		[[nodiscard]] bool operator==(const std_offset& other) const noexcept;
		[[nodiscard]] bool operator!=(const std_offset& other) const noexcept _GLSL_STRUCT_DEFAULT20;
	};
}

template<>
struct std::hash<glslstruct::std_offset> {
	size_t operator()(const glslstruct::std_offset& stdOff);
};