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

#include <glslstruct/types.hpp>
#include <glslstruct/value_types.hpp>
#include <glslstruct/variable/glsl_variable.hpp>

namespace glslstruct {
	class base_layout {
	protected:
		friend struct std::hash<base_layout>;

		std::unordered_map<std::string, value_data> _values;
		size_t _currentOffset = 0;
		size_t _maxAlignment = 0;

		[[nodiscard]] static std::string _get_array_elem_name(const std::string_view& arrayName, size_t elemIdx);
		[[nodiscard]] static std::string _get_struct_elem_name(const std::string_view& structName, const std::string_view& elemName);

		[[nodiscard]] static size_t _ceil_to_nearest_multiple(size_t valueToRoundUp, size_t multipleValue) noexcept;

		void _set_variable(const std::string_view& name, size_t offset, const base_type_handle& type, size_t size, size_t padding);
		void _remove_variable(const std::string_view& name);

		void _update_max_alignment(size_t baseAlignment) noexcept;

		[[nodiscard]] virtual size_t _get_array_elem_base_alignment(size_t baseAlignment) const noexcept;

		[[nodiscard]] virtual size_t _add(const std::string_view& name, size_t baseAlignment, size_t baseOffset, const base_type_handle& type);
		[[nodiscard]] virtual std::vector<size_t> _add_array(const std::string_view& name, size_t arraySize, size_t baseAlignment,
			size_t baseOffset, const base_type_handle& type);

		[[nodiscard]] virtual size_t _add_scalar(const std::string_view& name, size_t baseAlignment, size_t baseOffset, ValueType type);
		[[nodiscard]] virtual std::vector<size_t> _add_scalar_array(const std::string_view& name, size_t baseAlignment, size_t baseOffset,
			ValueType type, size_t arraySize);

		[[nodiscard]] virtual size_t _add_vec(const std::string_view& name, size_t length, size_t baseAlignment, size_t baseOffset, ValueType type);
		[[nodiscard]] virtual std::vector<size_t> _add_vec_array(const std::string_view& name, size_t length, size_t baseAlignment, size_t baseOffset,
			ValueType type, size_t arraySize);

		[[nodiscard]] virtual size_t _add_mat(const std::string_view& name, size_t columns, size_t rows, MajorType major, size_t baseAlignment,
			size_t baseOffset, ValueType type);
		[[nodiscard]] virtual std::vector<size_t> _add_mat_array(const std::string_view& name, size_t columns, size_t rows, MajorType major,
			size_t baseAlignment, size_t baseOffset, ValueType type, size_t arraySize);

		[[nodiscard]] virtual size_t _add_struct(const std::string_view& name, size_t baseAlignment, size_t baseOffset,
			const std::unordered_map<std::string, value_data>& values);
		[[nodiscard]] virtual std::vector<size_t> _add_struct_array(const std::string_view& name, size_t baseAlignment, size_t baseOffset,
			const std::unordered_map<std::string, value_data>& values, size_t arraySize);

		template<class T, class... Ts, size_t num, size_t... nums>
		_GLSL_STRUCT_CONSTEXPR17 void _add_multiple(const glsl_variable<T, num>& var, const glsl_variable<Ts, nums>&... vars) noexcept {
			if _GLSL_STRUCT_CONSTEXPR17 (var.is_layout) {
				if _GLSL_STRUCT_CONSTEXPR17 (num == 0) {
					add(var.var_name, var.layout);
				}
				else {
					add(var.var_name, var.layout, num);
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

		base_layout() noexcept = default;
		template<class... Ts, size_t... nums>
		explicit base_layout(const glsl_variable<Ts, nums>&... vars) noexcept {
			_add_multiple(vars...);
		}
		base_layout(const base_layout& other) noexcept;
		base_layout(base_layout&& other) noexcept;
		virtual ~base_layout() noexcept = default;

		base_layout& operator=(const base_layout& other) noexcept;
		base_layout& operator=(base_layout&& other) noexcept;

	public:
		[[nodiscard]] static size_t bad_offset() noexcept;

		[[nodiscard]] bool contains(const std::string_view& name) const noexcept;

		[[nodiscard]] value_data get(const std::string_view& name) const noexcept;

		[[nodiscard]] size_t get_offset(const std::string_view& name) const noexcept;
		[[nodiscard]] std::vector<size_t> get_array_offsets(const std::string_view& name) const noexcept;

		[[nodiscard]] const base_type_handle& get_type(const std::string_view& name) const noexcept;
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
		[[nodiscard]] std::shared_ptr<T> get_type(const std::string_view& name) const noexcept {
			return dynamic_type_cast<T>(get_type(name));
		}
		[[nodiscard]] size_t get_total_size(const std::string_view& name) const noexcept;
		[[nodiscard]] size_t get_size(const std::string_view& name) const noexcept;
		[[nodiscard]] size_t get_padding(const std::string_view& name) const noexcept;

		[[nodiscard]] std::vector<std::string> get_names() const noexcept;

		[[nodiscard]] virtual size_t base_alignment() const noexcept;
		[[nodiscard]] virtual size_t size() const noexcept;
		[[nodiscard]] size_t padding() const noexcept;

		virtual void clear() noexcept;

#pragma region ADD_SCALAR
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		[[nodiscard]] size_t add(const std::string_view& name) {
			static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

			return _add_scalar(name, typeSize, typeSize, valueType);
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		[[nodiscard]] std::vector<size_t> add(const std::string_view& name, size_t count) {
			static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

			return _add_scalar_array(name, typeSize, typeSize, valueType, count);
		}
#pragma endregion

#pragma region ADD_VEC
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		[[nodiscard]] size_t add(const std::string_view& name) {
			using T = glsl_vec_value_type<V>;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;

			static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

			return _add_vec(name, L, typeSize, typeSize, valueType);
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		[[nodiscard]] std::vector<size_t> add(const std::string_view& name, size_t count) {
			using T = glsl_vec_value_type<V>;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;

			static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

			return _add_vec_array(name, L, typeSize, typeSize, valueType, count);
		}
#pragma endregion

#pragma region ADD_MAT
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, MajorType major = MajorType::Column)
		[[nodiscard]] size_t add(const std::string_view& name) {
			using T = glsl_mat_value_type<M>;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

			return _add_mat(name, C, R, major, typeSize, typeSize, valueType);
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, MajorType major = MajorType::Column)
		[[nodiscard]] std::vector<size_t> add(const std::string_view& name, size_t count) {
			using T = glsl_mat_value_type<M>;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

			return _add_mat_array(name, C, R, major, typeSize, typeSize, valueType, count);
		}
#pragma endregion

#pragma region ADD_STRUCT
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(Layout, utils::glsl_layout, utils::is_glsl_layout_v<Layout>, = true)
		[[nodiscard]] size_t add(const std::string_view& name, const Layout& layout) {
			return _add_struct(name, layout.base_alignment(), layout._currentOffset,
				layout._values);
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(Layout, utils::glsl_layout, utils::is_glsl_layout_v<Layout>, = true)
		[[nodiscard]] std::vector<size_t> add(const std::string_view& name, const Layout& layout, size_t count) {
			return _add_struct_array(name, layout.base_alignment(), layout._currentOffset,
				layout._values, count);
		}
#pragma endregion

		[[nodiscard]] bool operator==(const base_layout& other) const noexcept;
		[[nodiscard]] bool operator!=(const base_layout& other) const noexcept;
	};
}

template<>
struct std::hash<glslstruct::base_layout> {
	[[nodiscard]] size_t operator()(const glslstruct::base_layout& stdOff) const noexcept;
};
#endif