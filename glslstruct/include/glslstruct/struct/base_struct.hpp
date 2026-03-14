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

#include <glslstruct/layout/std140_layout.hpp>
#include <glslstruct/layout/std430_layout.hpp>
#include <glslstruct/value/glsl_value.hpp>

namespace glslstruct {
	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(Layout, utils::glsl_layout, utils::is_glsl_layout_v<Layout>, )
	class base_struct {
	private:
		friend struct std::hash<base_struct<Layout>>;

		template<class T>
		struct _is_simple_or_struct_with_offset :
			std::bool_constant<utils::is_glsl_simple_or_layout_struct_v<T, Layout>> {};

		Layout _layout;
		std::vector<std::byte> _data;

		template<class T>
		_GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_value_data(const T& value) const {
			const std::byte* valueDataPtr = reinterpret_cast<const std::byte*>(&value);
			return std::vector<std::byte>(valueDataPtr, valueDataPtr + sizeof(T));
		}

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_scalar_value_data(const T& value) {
			return scalar_traits<T>::get_data(value).data();
		}

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_vec_value_data(const T& value) {
			return vec_traits<T>::get_data(value).data();
		}

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_mat_value_data(const T& value, MajorType major) {
			return mat_traits<T>::get_data(value, major).data();
		}

		_GLSL_STRUCT_CONSTEXPR17 size_t _get_array_elem_size(const std::vector<size_t>& offsets) const {
			if (offsets.size() > 1) {
				return offsets[1] - offsets[0];
			}
			return _data.size() - offsets[0];
		}

		template<class _Start, class _Conv>
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _convert_array(const std::string_view& name, const _Start* values,
			size_t size, const mstd::func_t<std::vector<size_t>(const std::string_view&, const _Conv*, size_t)>& arrayFunc) {
			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<_Start, _Conv>) {
				return arrayFunc(name, values, size);
			}
			else {
				std::vector<_Conv> convertedValues;
				convertedValues.reserve(size);
				for (size_t i = 0; i != size; ++i) {
					convertedValues.push_back(static_cast<_Conv>(values[i]));
				}
				return arrayFunc(name, convertedValues.data(), size);
			}
		}

#pragma region ADD
		template<class T, class... Ts, size_t num, size_t... nums>
		_GLSL_STRUCT_CONSTEXPR17 void _add_multiple(const glsl_value<T, num>& value, const glsl_value<Ts, nums>&... values) {
			if _GLSL_STRUCT_CONSTEXPR17 (num == 0) {
				add(value.var_name, value.value);
			}
			else {
				if _GLSL_STRUCT_CONSTEXPR17 (value.is_struct) {
					add(value.var_name, value.layout, value.value);
				}
				else {
					add(value.var_name, value.value);
				}
			}


			if _GLSL_STRUCT_CONSTEXPR17 (sizeof...(Ts) > 0 && sizeof...(nums) > 0) {
				_add_multiple(values...);
			}
		}

		_GLSL_STRUCT_CONSTEXPR17 void _add_value_data(const std::vector<std::byte>& valueData, size_t valueOffset) {
			const size_t dataAvailableSpace = _data.size() - valueOffset;

			if (valueOffset < _data.size()) {
				std::copy_n(valueData.begin(), std::min(dataAvailableSpace, valueData.size()),
					_data.begin() + valueOffset);
			}
			if (valueOffset + valueData.size() > _data.size()) {
				_data.insert(_data.end(), valueData.begin() + dataAvailableSpace, valueData.end());
			}
		}

		template<class T>
		_GLSL_STRUCT_CONSTEXPR17 size_t _add(const std::string_view& name, const T& value) {
			// ADD TO OFFSETS
			size_t valueOffset = _layout.template add<T>(name);

			// CHECK ERROR
			if (valueOffset == bad_offset()) {
				return valueOffset;
			}

			// RESERVE SIZE
			_data.reserve(_layout.size());

			// CHECK VALUE PADDING
			if (_data.size() < valueOffset) {
				_data.resize(valueOffset);
			}

			// SET VALUE DATA
			_add_value_data(_get_value_data(value), valueOffset);

			// UPDATE SIZE
			if (_data.size() < _data.capacity()) {
				_data.resize(_data.capacity());
			}

			return valueOffset;
		}

		template<class T> 
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_array(const std::string_view& name, const T* values, size_t size) {
			// CHECK SIZE
			if (size == 0) {
				return std::vector<size_t>();
			}

			// GET OFFSETS
			std::vector<size_t> valuesOffsets = _layout.template add<T>(name, size);

			// CHECK ERROR
			if (valuesOffsets.size() == 0) {
				return valuesOffsets;
			}

			// RESERVE SIZE
			_data.reserve(_layout.size());

			// SET VALUES DATA
			for (size_t i = 0; i < valuesOffsets.size() && i < size; ++i) {
				// CHECK VALUE PADDING
				if (_data.size() < valuesOffsets[i]) {
					_data.resize(valuesOffsets[i]);
				}

				// SET VALUE DATA
				_add_value_data(_get_value_data(values[i]), valuesOffsets[i]);
			}

			// UPDATE SIZE
			if (_data.size() < _data.capacity()) {
				_data.resize(_data.capacity());
			}

			return valuesOffsets;
		}

		_GLSL_STRUCT_CONSTEXPR17 size_t _add_struct(const std::string_view& name, const base_struct& value) {
			// ADD TO OFFSETS
			size_t valueOffset = _layout.add(name, value._layout);

			// CHECK ERROR
			if (valueOffset == bad_offset()) {
				return valueOffset;
			}

			// RESERVE SIZE
			_data.reserve(_layout.size());

			// CHECK PADDING
			if (_data.size() < valueOffset) {
				_data.resize(valueOffset);
			}

			// SET VALUE DATA
			_add_value_data(value._data, valueOffset);

			// CHECK DATA SIZE
			if (_data.size() < _data.capacity()) {
				_data.resize(_data.capacity());
			}

			return valueOffset;
		}

		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_struct_array(const std::string_view& name, const Layout& layout,
			const std::vector<std::byte>* values, size_t size) {
			// CHECK SIZE
			if (size == 0) {
				return std::vector<size_t>();
			}

			// ADD TO OFFSETS
			std::vector<size_t> valuesOffsets = _layout.add(name, layout, size);

			// CHECK ERROR
			if (valuesOffsets.size() == 0) {
				return valuesOffsets;
			}

			// UPDATE SIZE
			_data.reserve(_layout.size());

			// SET VALUES DATA
			for (size_t i = 0; i < valuesOffsets.size() && i < size; ++i) {
				// CHECK PADDING
				if (_data.size() < valuesOffsets[i]) {
					_data.resize(valuesOffsets[i]);
				}

				// SET VALUE DATA
				_add_value_data(values[i], valuesOffsets[i]);
			}

			// CHECK DATA SIZE
			if (_data.size() < _data.capacity()) {
				_data.resize(_data.capacity());
			}

			return valuesOffsets;
		}
#pragma endregion

#pragma region SET
		_GLSL_STRUCT_CONSTEXPR17 void _set_value_data(const std::vector<std::byte>& valueData, size_t valueOffset) noexcept {
			std::copy_n(valueData.begin(), std::min(valueData.size(), _data.size() - valueOffset),
				_data.begin() + valueOffset);
		}
		
		_GLSL_STRUCT_CONSTEXPR17 void _set_array_value_data(const std::vector<std::byte>& valueData, 
			size_t valueOffset, size_t arrayElemSize) noexcept {
			std::copy_n(valueData.begin(), std::min(arrayElemSize, _data.size() - valueOffset),
				_data.begin());
		}

		template<class T> 
		_GLSL_STRUCT_CONSTEXPR17 bool _set(const std::string_view& name, const T& value) {
			// CHECK VARIABLE
			if (!contains(name)) {
				return false;
			}

			// SET VALUE DATA
			_set_value_data(_get_value_data(value), _layout.get_offset(name));

			return true;
		}

		template<class T> 
		_GLSL_STRUCT_CONSTEXPR20 bool _set_array(const std::string_view& name, const T* values, size_t size) {
			// CHECK SIZE
			if (size == 0) {
				return false;
			}

			// CHECK VARIABLE
			if (!contains(name)) {
				return false;
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = _layout.get_array_offsets(name);

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				return false;
			}

			// GET ARRAY ELEM DATA MAX SIZE
			const size_t arrayElemDataSize = std::min(sizeof(T), _get_array_elem_size(valuesOffsets));

			// SET VALUES DATA
			std::vector<std::byte> valueData;
			for (size_t i = 0; i < valuesOffsets.size() && i < size; ++i) {
				// SET VALUE DATA
				_set_array_value_data(_get_value_data(values[i]), valuesOffsets[i], arrayElemDataSize);
			}

			// CLEAR VALUES OFFSETS
			valuesOffsets.clear();

			return true;
		}

		_GLSL_STRUCT_CONSTEXPR17 bool _set_struct(const std::string_view& name, const std::vector<std::byte>& value) {
			// CHECK VARIABLE
			if (!contains(name)) {
				return false;
			}

			// SET VALUE DATA
			_set_value_data(value, _layout.get_offset(name));

			return true;
		}

		_GLSL_STRUCT_CONSTEXPR20 bool _set_struct_array(const std::string_view& name, const std::vector<std::byte>* values, size_t size) {
			// CHECK SIZE
			if (size == 0) {
				return false;
			}

			// CHECK VARIABLE
			if (!contains(name)) {
				return false;
			}

			// GET OFFSETS
			std::vector<size_t> valuesOffsets = _layout.get_array_offsets(name);

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				return false;
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = std::min(values[0].size(), _get_array_elem_size(valuesOffsets));

			// SET VALUES DATA
			for (size_t i = 0; i < valuesOffsets.size() && i < size; ++i) {
				// SET VALUE DATA
				_set_array_value_data(values[i], valuesOffsets[i], arrayElemDataSize);
			}

			// CLEAR VALUES OFFSETS
			valuesOffsets.clear();

			return true;
		}
#pragma endregion

#pragma region GET
		_GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_value_data(size_t valueOffset, size_t valueSize) const {
			std::vector<std::byte> valueData(valueSize, static_cast<std::byte>(0));
			std::copy_n(_data.begin() + valueOffset, std::min(valueSize, _data.size() - valueOffset),
				valueData.begin());
			return valueData;
		}

		template<class T>
		_GLSL_STRUCT_CONSTEXPR17 T _get_casted_value(const std::vector<std::byte>& valueData) const {
			return *reinterpret_cast<const T*>(valueData.data());
		}

		template<class T> 
		_GLSL_STRUCT_CONSTEXPR17 T _get(const std::string_view& name) const {
			// CHECK VARIABLE
			if (!contains(name)) {
				return T();
			}

			// RETURN VALUE
			return _get_casted_value<T>(_get_value_data(_layout.get_offset(name), sizeof(T)));
		}

		template<class T>
		_GLSL_STRUCT_CONSTEXPR20 std::vector<T> _get_array(const std::string_view& name) const {
			// CHECK VARIABLE
			if (!contains(name)) {
				return std::vector<T>();
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = _layout.get_array_offsets(name);

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				return std::vector<T>();
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = std::min(_get_array_elem_size(valuesOffsets), sizeof(T));

			// GET VALUES DATA
			std::vector<T> values;
			values.reserve(valuesOffsets.size());

			std::vector<std::byte> valueData;
			valueData.resize(sizeof(T));

			size_t maxSize = 0;
			for (size_t i = 0; i < valuesOffsets.size(); ++i) {
				// GET MAX VALUE SIZE
				maxSize = std::min(arrayElemDataSize, _data.size() - valuesOffsets[i]);

				// GET VALUE DATA
				std::copy_n(_data.begin() + valuesOffsets[i], maxSize, valueData.begin());

				// CHECK VALUE DATA SIZE
				if (maxSize < sizeof(T)) {
					std::fill_n(valueData.begin() + maxSize, sizeof(T), T());
				}

				// GET VALUE
				values.push_back(_get_casted_value<T>(valueData));
			}

			// CLEAR TEMP VALUE DATA
			valueData.clear();

			// CLEAR VALUES OFFSETS
			valuesOffsets.clear();

			// RETURN VALUES
			return values;
		}

		_GLSL_STRUCT_CONSTEXPR17 base_struct _get_struct(const std::string_view& name, const Layout& layout) const {
			// CHECK VARIABLE
			if (!contains(name)) {
				return base_struct(layout);
			}

			// GET VALUE OFFSET
			size_t valueOffset = _layout.get_offset(name);

			// MAKE EMPTY STRUCT
			base_struct value(layout);

			// SET VALUE DATA
			std::copy_n(_data.begin() + valueOffset, std::min(layout.size(), _data.size() - valueOffset),
				value._data.begin());

			// RETURN VALUE
			return value;
		}

		_GLSL_STRUCT_CONSTEXPR20 std::vector<base_struct> _get_struct_array(const std::string_view& name, const Layout& layout) const {
			// CHECK VARIABLE
			if (!contains(name)) {
				return std::vector<base_struct>();
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = _layout.get_array_offsets(name);

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				return std::vector<base_struct>();
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = std::min(layout.size(), _get_array_elem_size(valuesOffsets));

			// GET VALUES DATA
			std::vector<base_struct> values;
			for (size_t i = 0; i < valuesOffsets.size(); ++i) {
				// MAKE EMPTY STRUCT
				base_struct value(layout);

				// SET VALUE DATA
				std::copy_n(_data.begin() + valuesOffsets[i], std::min(arrayElemDataSize, _data.size() - valuesOffsets[i]),
					value._data.begin());

				// ADD VALUE TO VALUES
				values.push_back(value);
			}

			// RETURN VALUE
			return values;
		}

		template<class _Start, class _Conv = _Start>
		_GLSL_STRUCT_CONSTEXPR17 std::vector<_Conv> _get_array(const std::string_view& name,
			const mstd::func_t<std::vector<_Start>(const std::string&)>& getArrayFunc) {
			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<_Start, _Conv>) {
				return getArrayFunc(name);
			}
			else {
				std::vector<_Start> values = getArrayFunc(name);
				std::vector<_Conv> convertedValues;
				convertedValues.reserve(values.size());
				for (auto& val : values) {
					convertedValues.push_back(static_cast<_Conv>(val));
				}
				return convertedValues;
			}
		}
#pragma endregion

	public:
		using layout_type = Layout;

		_GLSL_STRUCT_CONSTEXPR20 base_struct() noexcept = default;
		explicit _GLSL_STRUCT_CONSTEXPR20 base_struct(const Layout& layout,
			const std::vector<std::byte>& data = std::vector<std::byte>()) noexcept 
			: _layout(layout), _data(_layout.size(), static_cast<std::byte>(0)) {
			std::copy_n(data.begin(), std::min(data.size(), _data.size()), _data.begin());
		}
#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_simple_or_struct_with_offset_value<_Offset>... Args, size_t... nums>
#else
		template<class... Args, size_t... nums, std::enable_if_t<mstd::all_check_v<_is_simple_or_struct_with_offset, Args...>, bool> = true>
#endif
		explicit _GLSL_STRUCT_CONSTEXPR20 base_struct(const glsl_value<Args, nums>&... values) noexcept {
			_add_multiple(values...);
		}
		_GLSL_STRUCT_CONSTEXPR20 base_struct(const base_struct& other) noexcept = default;
		_GLSL_STRUCT_CONSTEXPR20 base_struct(base_struct&& other) noexcept
			: _layout(std::move(other._layout)), _data(std::exchange(other._data, {})) {}
		_GLSL_STRUCT_CONSTEXPR20 ~base_struct() noexcept = default;

		_GLSL_STRUCT_CONSTEXPR20 base_struct& operator=(const base_struct& other) noexcept = default;
		_GLSL_STRUCT_CONSTEXPR20 base_struct& operator=(base_struct&& other) noexcept {
			_layout = std::move(other._layout);
			_data = std::exchange(other._data, {});
			return *this;
		}

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t bad_offset() noexcept {
			return Layout::bad_offset();
		}

#pragma region ADD
#pragma region ADD_SCALAR
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view& name, const T& value) {
			if _GLSL_STRUCT_CONSTEXPR17(std::is_same_v<T, bool>) {
				return _add(name, static_cast<unsigned int>(value));
			}
			else {
				return _add(name, value);
			}
		}
#pragma endregion

#pragma region ADD_SCALARS_ARRAYS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const T* values, size_t size) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values, size,
				[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const T(&values)[N]) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values, N,
				[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const std::array<T, N>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values.data(), N,
				[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const std::vector<T>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values.data(), values.size(),
				[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}
#pragma endregion

#pragma region ADD_VEC
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view& name, const V& value) {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;

			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				return _add(name, static_cast<glm::vec<L, unsigned int>>(value));
			}
			else {
				return _add(name, value);
			}
		}
#pragma endregion

#pragma region ADD_VEC_ARRAYS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const V* values, size_t size) {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values, size,
				[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const V(&values)[N]) {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values, N,
				[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const std::array<V, N>& values) {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values.data(), N,
				[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const std::vector<V>& values) {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values.data(), values.size(),
				[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}
#pragma endregion

#pragma region ADD_MAT
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, MajorType major = MajorType::Column)
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view& name, const M& value) {
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			if _GLSL_STRUCT_CONSTEXPR17 (major == MajorType::Column) {
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					return _add(name, static_cast<glm::mat<C, R, unsigned int>>(value));
				}
				else {
					return _add(name, value);
				}
			}
			else {
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					return _add(name, glm::transpose(static_cast<glm::mat<C, R, unsigned int>>(value)));
				}
				else {
					return _add(name, glm::transpose(value));
				}
			}
		}
#pragma endregion

#pragma region ADD_MAT_ARRAYS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, MajorType major = MajorType::Column)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const M* values, size_t size) {
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			if _GLSL_STRUCT_CONSTEXPR17 (major == MajorType::Column) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values, size,
					[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(size);
				for (size_t i = 0; i < size; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues, size,
					[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, size_t N, MajorType major = MajorType::Column)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const M(&values)[N]) {
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			if _GLSL_STRUCT_CONSTEXPR17 (major == MajorType::Column) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values, N,
					[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(N);
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues, N,
					[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, size_t N, MajorType major = MajorType::Column)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const std::array<M, N>& values) {
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			if _GLSL_STRUCT_CONSTEXPR17(major == MajorType::Column) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values.data(), N,
					[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(N);
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues, N,
					[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, MajorType major = MajorType::Column)
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const std::vector<M>& values) {
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			if _GLSL_STRUCT_CONSTEXPR17 (major == MajorType::Column) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values.data(), values.size(),
					[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(values.size());
				for (size_t i = 0; i < values.size(); ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues, values.size(),
					[&](const std::string_view& name, const type* values, size_t size) -> std::vector<size_t> { return _addArray(name, values, size); });
			}
		}
#pragma endregion

#pragma region ADD_STRUCT
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view& name, const base_struct& value) {
			return _add_struct(name, value);
		}

		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view& name, const Layout& value,
			const std::vector<std::byte>& data = std::vector<std::byte>()) {
			return _add_struct(name, std_struct(value, data));
		}
#pragma endregion

#pragma region ADD_STRUCT_ARRAYS
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const Layout& layout,
			const std::vector<std::byte>* values, size_t size) {
			return _add_struct_array(name, layout, values, size);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const Layout& layout,
			const std::vector<std::byte>(&values)[N]) {
			return _add_struct_array(name, layout, values, N);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const Layout& layout,
			const std::array<std::vector<std::byte>, N>& values) {
			return _add_struct_array(name, layout, values.data(), N);
		}

		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view& name, const Layout& layout,
			const std::vector<std::vector<std::byte>>& values) {
			return _add_struct_array(name, layout, values.data(), values.size());
		}
#pragma endregion
#pragma endregion

#pragma region SET
#pragma region SET_SCALAR
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const T& value) {
			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				return _set(name, static_cast<unsigned int>(value));
			}
			else {
				return _set(name, value);
			}
		}
#pragma endregion

#pragma region SET_SCALARS_ARRAYS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const T* values, size_t size) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values, size,
				[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const T(&values)[N]) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values, N,
				[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const std::array<T, N>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values.data(), N,
				[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const std::vector<T>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values.data(), values.size(),
				[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}
#pragma endregion

#pragma region SET_VEC
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const V& value) {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;

			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				return _set(name, static_cast<glm::vec<L, unsigned int>>(value));
			}
			else {
				return _set(name, value);
			}
		}
#pragma endregion

#pragma region SET_VEC_ARRAYS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const V* values, size_t size) {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values, size,
				[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const V(&values)[N]) {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values, N,
				[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const std::array<V, N>& values) {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values, N,
				[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const std::vector<V>& values) {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values.data(), values.size(),
				[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}
#pragma endregion

#pragma region SET_MAT
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, MajorType major = MajorType::Column)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const M& value) {
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			if _GLSL_STRUCT_CONSTEXPR17 (major == MajorType::Column) {
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					return _set(name, static_cast<glm::mat<C, R, unsigned int>>(value));
				}
				else {
					return _set(name, value);
				}
			}
			else {
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					return _set(name, glm::transpose(static_cast<glm::mat<C, R, unsigned int>>(value)));
				}
				else {
					return _set(name, glm::transpose(value));
				}
			}
		}
#pragma endregion

#pragma region SET_MAT_ARRAYS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, MajorType major = MajorType::Column)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const M* values, size_t size) {
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			if _GLSL_STRUCT_CONSTEXPR17 (major == MajorType::Column) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values, size,
					[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(size);
				for (size_t i = 0; i < size; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues.data(), size,
					[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, size_t N, MajorType major = MajorType::Column)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const M(&values)[N]) {
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			if _GLSL_STRUCT_CONSTEXPR17 (major == MajorType::Column) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values, N,
					[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(N);
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues.data(), N,
					[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, size_t N, MajorType major = MajorType::Column)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const std::array<M, N>& values) {
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			if _GLSL_STRUCT_CONSTEXPR17(major == MajorType::Column) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values.data(), N,
					[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(N);
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues.data(), N,
					[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, MajorType major = MajorType::Column)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const std::vector<M>& values) {
			using T = typename M::value_type;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17 (major == MajorType::Column) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values.data(), values.size(),
					[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(values.size());
				for (size_t i = 0; i < values.size(); ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues.data(), values.size(),
					[&](const std::string_view& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
		}
#pragma endregion

#pragma region SET_STRUCT
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const base_struct& value) {
			return _set_struct(name, value._data);
		}
		
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view& name, const std::vector<std::byte>& value) {
			return _set_struct(name, value);
		}
#pragma endregion

#pragma region SET_STRUCT_ARRAYS
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view& name, const std::vector<std::byte>* values, size_t size) {
			return _set_struct_array(name, values, size);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view& name, const std::vector<std::byte>(&values)[N]) {
			return _set_struct_array(name, values, N);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view& name, const std::array<std::vector<std::byte>, N>& values) {
			return _set_struct_array(name, values.data(), N);
		}

		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view& name, const std::vector<std::vector<std::byte>>& values) {
			return _set_struct_array(name, values.data(), values.size());
		}
#pragma endregion
#pragma endregion

#pragma region GET
#pragma region GET_SCALARS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 T get(const std::string_view& name) const {
			if _GLSL_STRUCT_CONSTEXPR17(std::is_same_v<T, bool>) {
				return static_cast<T>(_get<unsigned int>(name));
			}
			else {
				return _get<T>(name);
			}
		}
#pragma endregion

#pragma region GET_SCALARS_ARRAYS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR17 void get(const std::string_view& name, T*& valuesDest, size_t size) const {
			std::vector<T> values;
			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				values = _get_array<unsigned int, T>(name, 
					[&](const std::string_view& name) -> std::vector<T> { return _get_array<unsigned int>(name); });
			}
			else {
				values = _get_array<T>(name);
			}
			std::copy_n(values.begin(), std::min(values.size(), size), valuesDest);
			if (size > values.size()) {
				std::fill_n(valuesDest + values.size(), size - values.size(), T());
			}
			values.clear();
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(SV, utils::glsl_scalars_vector, utils::is_glsl_scalars_vector_v<SV>, = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 SV get(const std::string_view& name) const {
			using T = typename SV::value_type;

			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				return _get_array<unsigned int, T>(name, 
					[&](const std::string_view& name) -> std::vector<T> { return _get_array<unsigned int>(name); });
			}
			else {
				return _get_array<T>(name);
			}
		}
#pragma endregion

#pragma region GET_VEC
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 V get(const std::string_view& name) const {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;

			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				return static_cast<V>(_get<glm::vec<L, unsigned int>>(name));
			}
			else {
				return _get<V>(name);
			}
		}
#pragma endregion

#pragma region GET_VEC_ARRAYS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		_GLSL_STRUCT_CONSTEXPR17 void get(const std::string_view& name, V*& valuesDest, size_t size) const {
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;

			std::vector<V> values;
			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				using type = glm::vec<L, unsigned int>;

				values = _get_array<type, V>(name, 
					[&](const std::string_view& name) -> std::vector<type> { return _get_array<type>(name); });
			}
			else {
				values = _get_array<V>(name);
			}

			std::copy_n(values.begin(), std::min(values.size(), size), valuesDest);
			if (size > values.size()) {
				std::fill_n(valuesDest + values.size(), size - values.size(), V());
			}
			values.clear();
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(VV, utils::glsl_vecs_vector, utils::is_glsl_vecs_vector_v<VV>, = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 VV get(const std::string_view& name) const {
			using V = typename VV::value_type;
			using T = glsl_vec_value_type<V>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;

			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				using type = glm::vec<L, unsigned int>;

				return _get_array<type, V>(name, 
					[&](const std::string_view& name) -> std::vector<type> { return _get_array<type>(name); });
			}
			else {
				return _get_array<V>(name);
			}
		}
#pragma endregion

#pragma region GET_MAT
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, MajorType major = MajorType::Column)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 M get(const std::string_view& name) const {
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			if _GLSL_STRUCT_CONSTEXPR17 (major == MajorType::Column) {
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					return static_cast<M>(_get<glm::mat<C, R, unsigned int>>(name));
				}
				else {
					return _get<M>(name);
				}
			}
			else {
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					return static_cast<M>(glm::transpose(_get<glm::mat<R, C, unsigned int>>(name)));
				}
				else {
					return glm::transpose(_get<glm::mat<R, C, T>>(name));
				}
			}
		}
#pragma endregion

#pragma region GET_MAT_ARRAYS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, MajorType major = MajorType::Column)
		_GLSL_STRUCT_CONSTEXPR17 void get(const std::string_view& name, M*& valuesDest, size_t size) const {
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			std::vector<M> values;
			if _GLSL_STRUCT_CONSTEXPR17 (major == MajorType::Column) {
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					using type = glm::mat<C, R, unsigned int>;

					values = _get_array<type, M>(name, 
						[&](const std::string_view& name) -> std::vector<type> { return _get_array<type>(name); });
				}
				else {
					values = _get_array<M>(name);
				}
			}
			else {
				using transposedType = glm::mat<R, C, T>;

				std::vector<transposedType> transposedValues;
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					using type = glm::mat<R, C, unsigned int>;

					transposedValues = _get_array<type, transposedType>(name, 
						[&](const std::string_view& name) -> std::vector<type> { return _get_array<type>(name); });
				}
				else {
					transposedValues = _get_array<transposedType>(name);
				}

				values.reserve(transposedValues.size());
				for (auto& value : transposedValues) {
					values.push_back(glm::transpose(value));
				}
				transposedValues.clear();
			}

			std::copy_n(values.begin(), std::min(values.size(), size), valuesDest);
			if (size > values.size()) {
				std::fill_n(valuesDest + values.size(), size - values.size(), M());
			}
			values.clear();
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(MV, utils::glsl_mats_vector, utils::is_glsl_mats_vector_v<MV>, = true, MajorType major = MajorType::Column)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 MV get(const std::string_view& name) const {
			using M = typename MV::value_type;
			using T = glsl_mat_value_type<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
			_GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

			if _GLSL_STRUCT_CONSTEXPR17 (major == MajorType::Column) {
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					using type = glm::mat<C, R, unsigned int>;

					return _get_array<type, M>(name, 
						[&](const std::string_view& name) -> std::vector<type> { return _get_array<type>(name); });
				}
				else {
					return _get_array<M>(name);
				}
			}
			else {
				using transposedType = glm::mat<R, C, T>;

				std::vector<transposedType> transposedValues;
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					using type = glm::mat<R, C, unsigned int>;

					transposedValues = _get_array<type, transposedType>(name, 
						[&](const std::string_view& name) -> std::vector<type> { return _get_array<type>(name); });
				}
				else {
					transposedValues = _get_array<transposedType>(name);
				}

				std::vector<M> values(transposedValues.size());
				for (auto& value : transposedValues) {
					values.push_back(glm::transpose(value));
				}
				transposedValues.clear();
				return values;
			}
		}
#pragma endregion

#pragma region GET_STRUCT
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(S, utils::glsl_layout_struct<Layout>, (utils::is_glsl_layout_struct_v<S, Layout>), = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 S get(const std::string_view& name, const Layout& layout) const {
			return _get_struct(name, layout);
		}
#pragma endregion

#pragma region GET_STRUCT_ARRAYS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(S, utils::glsl_layout_struct<Layout>, (utils::is_glsl_layout_struct_v<S, Layout>), = true)
		_GLSL_STRUCT_CONSTEXPR17 void get(const std::string_view& name, const Layout& layout, S*& valuesDest, size_t size) const {
			std::vector<base_struct> values = _get_struct_array(name, layout);
			std::copy_n(values.begin(), std::min(values.size(), size). valuesDest);
			if (size > values.size()) {
				std::fill_n(valuesDest + values.size(), size - values.size(), base_struct(layout));
			}
			values.clear();
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(VS, utils::glsl_layout_structs_vector<Layout>, (utils::is_glsl_layout_structs_vector_v<VS, Layout>), = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 VS get(const std::string_view& name, const Layout& layout) const {
			return _get_struct_array(name, layout);
		}
#pragma endregion
#pragma endregion

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 const Layout& get_layout() const noexcept {
			return _layout;
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool contains(const std::string_view& name) const {
			return _layout.contains(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_offset(const std::string_view& name) const {
			return _layout.get_offset(name);
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> get_array_offsets(const std::string_view& name) const {
			return _layout.get_array_offsets(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 const base_type_handle& get_type(const std::string_view& name) const {
			return _layout.get_type(name);
		}
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 std::shared_ptr<T> get_type(const std::string_view& name) const {
			return _layout.template get_type<T>(name);
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_total_size(const std::string_view& name) const noexcept {
			return _layout.get_total_size(name);
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_size(const std::string_view& name) const noexcept {
			return _layout.get_size(name);
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_padding(const std::string_view& name) const noexcept {
			return _layout.get_padding(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<std::string> get_names() const {
			return _layout.get_names();
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 const std::vector<std::byte>& data() const noexcept {
			return _data;
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t base_alignment() const noexcept {
			return _layout.base_alignment();
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t size() const noexcept {
			return _data.size();
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t padding() const noexcept {
			return _layout.padding();
		}

		_GLSL_STRUCT_CONSTEXPR17 void clear_data() noexcept {
			std::fill(_data.begin(), _data.end(), static_cast<std::byte>(0));
		}
		_GLSL_STRUCT_CONSTEXPR17 void clear() noexcept {
			_layout.clear();
			_data.clear();
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator==(const base_struct& other) const {
			return _layout == other._layout &&
				_data == other._data;
		}
		#if _GLSL_STRUCT_HAS_CXX20
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const base_struct& other) const = default;
		#else
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const base_struct& other) const {
			return !(*this == other);
		}
		#endif
	};

	using std140_struct = base_struct<std140_layout>;
	using std430_struct = base_struct<std430_layout>;
}

template<class Layout>
struct std::hash<glslstruct::base_struct<Layout>> {
	size_t operator()(const glslstruct::base_struct<Layout>& stdStruct) {
		size_t seed = 0;
		mstd::hash_append(seed, stdStruct._layout);
		mstd::hash_range(seed, stdStruct._data.begin(), stdStruct._data.end());
		return seed;
	}
};

#endif