/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/std140_offset.hpp>
#include <glslstruct/std430_offset.hpp>
#include <glslstruct/std_value.hpp>

namespace glslstruct {
	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(_Offset, utils::glsl_offset, utils::is_glsl_offset_v<_Offset>, )
	class std_struct {
	private:
		friend struct std::hash<glslstruct::std_struct<_Offset>>;

		template<class T>
		struct _is_simple_or_struct_with_offset :
			std::bool_constant<utils::is_glsl_simple_or_struct_with_offset_value_v<T, _Offset>> {};

		_Offset _dataOffsets;
		std::vector<std::byte> _data;

		template<class T>
		_GLSL_STRUCT_CONSTEXPR17 std::vector<std::byte> _get_value_data(const T& value) const {
			const std::byte* valueDataPtr = reinterpret_cast<const std::byte*>(&value);
			return std::vector<std::byte>(valueDataPtr, valueDataPtr + sizeof(T));
		}

		_GLSL_STRUCT_CONSTEXPR17 size_t _get_array_elem_size(const std::vector<size_t>& offsets) const {
			if (offsets.size() > 1) {
				return offsets[1] - offsets[0];
			}
			else {
				return _data.size() - offsets[0];
			}
		}

		template<class _Start, class _Conv>
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> _convert_array(const std::string& name, const _Start* values, 
			size_t size, const mstd::func<std::vector<size_t>, const std::string&, const _Conv*, size_t>& arrayFunc) {
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
		_GLSL_STRUCT_CONSTEXPR17 void _add_multiple(const std_value<T, num>& value, const std_value<Ts, nums>&... values) {
			if _GLSL_STRUCT_CONSTEXPR17 (num == 0) {
				add(value.var_name, value.value);
			}
			else {
				if _GLSL_STRUCT_CONSTEXPR17 (value.is_struct) {
					add(value.var_name, value.struct_offsets, value.value);
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
			const size_t dataAvilableSpace = _data.size() - valueOffset;

			if (valueOffset < _data.size()) {
				std::copy(valueData.begin(), 
					valueData.begin() + std::min(dataAvilableSpace, valueData.size()), 
					_data.begin() + valueOffset);
			}
			if (valueOffset + valueData.size() > _data.size()) {
				_data.insert(_data.end(), valueData.begin() + dataAvilableSpace, valueData.end());
			}
		}

		template<class T>
		_GLSL_STRUCT_CONSTEXPR17 size_t _add(const std::string& name, const T& value) {
			// ADD TO OFFSETS
			size_t valueOffset = _dataOffsets.add<T>(name);

			// CHECK ERROR
			if (valueOffset == bad_offset()) {
				return valueOffset;
			}

			// RESERVE SIZE
			_data.reserve(_dataOffsets.size());

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
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> _add_array(const std::string& name, const T* values, size_t size) {
			// CHECK SIZE
			if (size == 0) {
				return std::vector<size_t>();
			}

			// GET OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.add<T>(name, size);

			// CHECK ERROR
			if (valuesOffsets.size() == 0) {
				return valuesOffsets;
			}

			// RESERVE SIZE
			_data.reserve(_dataOffsets.size());

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

		_GLSL_STRUCT_CONSTEXPR17 size_t _add_struct(const std::string& name, const std_struct<_Offset>& value) {
			// ADD TO OFFSETS
			size_t valueOffset = _dataOffsets.add(name, value._dataOffsets);

			// CHECK ERROR
			if (valueOffset == bad_offset()) {
				return valueOffset;
			}

			// RESERVE SIZE
			_data.reserve(_dataOffsets.size());

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

		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> _add_struct_array(const std::string& name, const _Offset& structOffsets, 
			const std::vector<std::byte>* values, size_t size) {
			// CHECK SIZE
			if (size == 0) {
				return std::vector<size_t>();
			}

			// ADD TO OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.add(name, structOffsets, size);

			// CHECK ERROR
			if (valuesOffsets.size() == 0) {
				return valuesOffsets;
			}

			// UPDATE SIZE
			_data.reserve(_dataOffsets.size());

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
			std::copy(valueData.begin(), valueData.begin() + std::min(valueData.size(), _data.size() - valueOffset),
				_data.begin() + valueOffset);
		}
		
		_GLSL_STRUCT_CONSTEXPR17 void _set_array_value_data(const std::vector<std::byte>& valueData, 
			size_t valueOffset, size_t arrayElemSize) noexcept {
			std::copy(valueData.begin(), 
				valueData.begin() + std::min(arrayElemSize, _data.size() - valueOffset),
				_data.begin());
		}

		template<class T> 
		_GLSL_STRUCT_CONSTEXPR17 bool _set(const std::string& name, const T& value) {
			// CHECK VARIABLE
			if (!contains(name)) {
				return false;
			}

			// SET VALUE DATA
			_set_value_data(_get_value_data(value), _dataOffsets.get_offset(name));

			return true;
		}

		template<class T> 
		_GLSL_STRUCT_CONSTEXPR17 bool _set_array(const std::string& name, const T* values, size_t size) {
			// CHECK SIZE
			if (size == 0) {
				return false;
			}

			// CHECK VARIABLE
			if (!contains(name)) {
				return false;
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.get_array_offsets(name);

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

		_GLSL_STRUCT_CONSTEXPR17 bool _set_struct(const std::string& name, const std::vector<std::byte>& value) {
			// CHECK VARIABLE
			if (!contains(name)) {
				return false;
			}

			// SET VALUE DATA
			_set_value_data(value, _dataOffsets.get_offset(name));

			return true;
		}

		_GLSL_STRUCT_CONSTEXPR17 bool _set_struct_array(const std::string& name, const std::vector<std::byte>* values, size_t size) {
			// CHECK SIZE
			if (size == 0) {
				return false;
			}

			// CHECK VARIABLE
			if (!contains(name)) {
				return false;
			}

			// GET OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.get_array_offsets(name);

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
		_GLSL_STRUCT_CONSTEXPR17 std::vector<std::byte> _get_value_data(size_t valueOffset, size_t valueSize) const {
			std::vector<std::byte> valueData(valueSize, static_cast<std::byte>(0));
			std::copy(_data.begin() + valueOffset, 
				_data.begin() + valueOffset + std::min(valueSize, _data.size() - valueOffset), 
				valueData.begin());
			return valueData;
		}

		template<class T>
		_GLSL_STRUCT_CONSTEXPR17 T _get_casted_value(const std::vector<std::byte>& valueData) const {
			return *reinterpret_cast<const T*>(valueData.data());
		}

		template<class T> 
		_GLSL_STRUCT_CONSTEXPR17 T _get(const std::string& name) const {
			// CHECK VARIABLE
			if (!contains(name)) {
				return T();
			}

			// RETURN VALUE
			return _get_casted_value<T>(_get_value_data(_dataOffsets.get_offset(name), sizeof(T)));
		}

		template<class T>
		_GLSL_STRUCT_CONSTEXPR17 std::vector<T> _get_array(const std::string& name) const {
			// CHECK VARIABLE
			if (!contains(name)) {
				return std::vector<T>();
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.get_array_offsets(name);

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

			size_t maxSize;
			for (size_t i = 0; i < valuesOffsets.size(); ++i) {
				// GET MAX VALUE SIZE
				maxSize = std::min(arrayElemDataSize, _data.size() - valuesOffsets[i]);

				// GET VALUE DATA
				std::copy(_data.begin() + valuesOffsets[i], _data.begin() + valuesOffsets[i] + maxSize, valueData.begin());

				// CHECK VALUE DATA SIZE
				if (maxSize < sizeof(T)) {
					std::fill(valueData.begin() + maxSize, valueData.begin() + sizeof(T), T());
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

		_GLSL_STRUCT_CONSTEXPR17 std_struct _get_struct(const std::string& name, const _Offset& structOffsets) const {
			// CHECK VARIABLE
			if (!contains(name)) {
				return std_struct(structOffsets);
			}

			// GET VALUE OFFSET
			size_t valueOffset = _dataOffsets.get_offset(name);

			// MAKE EMPTY STRUCT
			std_struct value(structOffsets);

			// SET VALUE DATA
			std::copy(_data.begin() + valueOffset, 
				_data.begin() + valueOffset + std::min(structOffsets.size(), _data.size() - valueOffset),
				value._data.begin());

			// RETURN VALUE
			return value;
		}

		_GLSL_STRUCT_CONSTEXPR17 std::vector<std_struct> _get_struct_array(const std::string& name, const _Offset& structOffsets) const {
			// CHECK VARIABLE
			if (!contains(name)) {
				return std::vector<std_struct>();
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.get_array_offsets(name);

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				return std::vector<std_struct>();
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = std::min(structOffsets.size(), _get_array_elem_size(valuesOffsets));

			// GET VALUES DATA
			std::vector<std_struct> values;
			for (size_t i = 0; i < valuesOffsets.size(); ++i) {
				// MAKE EMPTY STRUCT
				std_struct value(structOffsets);

				// SET VALUE DATA
				std::copy(_data.begin() + valuesOffsets[i],
					_data.begin() + valuesOffsets[i] + std::min(arrayElemDataSize, _data.size() - valuesOffsets[i]),
					value._data.begin());

				// ADD VALUE TO VALUES
				values.push_back(value);
			}

			// RETURN VALUE
			return values;
		}

		template<class _Start, class _Conv = _Start>
		_GLSL_STRUCT_CONSTEXPR17 std::vector<_Conv> _get_array(const std::string& name, 
			const mstd::func<std::vector<_Start>, const std::string&>& getArrayFunc) {
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
		using offset_type = _Offset;

		_GLSL_STRUCT_CONSTEXPR20 std_struct() noexcept = default;
		explicit _GLSL_STRUCT_CONSTEXPR20 std_struct(const _Offset& structOffsets,
			const std::vector<std::byte>& data = std::vector<std::byte>()) noexcept 
			: _dataOffsets(structOffsets), _data(_dataOffsets.size(), 0) {
			std::copy(data.begin(), data.begin() + std::min(data.size(), _data.size()), _data.begin());
		}
#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_simple_or_struct_with_offset_value<_Offset>... Args, size_t... nums>
#else
		template<class... Args, size_t... nums, std::enable_if_t<mstd::all_check_v<_is_simple_or_struct_with_offset, Args...>, bool> = true>
#endif
		explicit _GLSL_STRUCT_CONSTEXPR20 std_struct(const std_value<Args, nums>&... values) noexcept {
			_add_multiple(values...);
		}
		_GLSL_STRUCT_CONSTEXPR20 std_struct(const std_struct& other) noexcept = default;
		_GLSL_STRUCT_CONSTEXPR20 std_struct(std_struct&& other) noexcept
			: _dataOffsets(std::move(other._dataOffsets)), _data(std::exchange(other._data, {})) {}
		_GLSL_STRUCT_CONSTEXPR20 ~std_struct() noexcept = default;

		_GLSL_STRUCT_CONSTEXPR20 std_struct& operator=(const std_struct& other) noexcept = default;
		_GLSL_STRUCT_CONSTEXPR20 std_struct& operator=(std_struct&& other) noexcept {
			_dataOffsets = std::move(other._dataOffsets);
			_data = std::exchange(other._data, {});
			return *this;
		}

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t bad_offset() noexcept {
			return _Offset::bad_offset();
		}

#pragma region ADD
#pragma region ADD_SCALAR
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string& name, const T& value) {
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
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const T* values, size_t size) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values, size,
				[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const T(&values)[N]) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values, N,
				[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const std::array<T, N>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values.data(), N,
				[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const std::vector<T>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values.data(), values.size(),
				[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}
#pragma endregion

#pragma region ADD_VEC
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string& name, const V& value) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();

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
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const V* values, size_t size) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values, size,
				[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const V(&values)[N]) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values, N,
				[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const std::array<V, N>& values) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values.data(), N,
				[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const std::vector<V>& values) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values.data(), values.size(),
				[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
		}
#pragma endregion

#pragma region ADD_MAT
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, bool column_major = true)
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string& name, const M& value) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17 (column_major) {
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
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, bool column_major = true)
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const M* values, size_t size) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17 (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values, size,
					[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(size);
				for (size_t i = 0; i < size; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues, size,
					[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, size_t N, bool column_major = true)
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const M(&values)[N]) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17 (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values, N,
					[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(N);
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues, N,
					[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, size_t N, bool column_major = true)
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const std::array<M, N>& values) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17(column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values.data(), N,
					[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(N);
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues, N,
					[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, bool column_major = true)
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const std::vector<M>& values) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17 (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values.data(), values.size(),
					[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _add_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(values.size());
				for (size_t i = 0; i < values.size(); ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues, values.size(),
					[&](const std::string& name, const type* values, size_t size) -> std::vector<size_t> { return _addArray(name, values, size); });
			}
		}
#pragma endregion

#pragma region ADD_STRUCT
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string& name, const std_struct<_Offset>& value) {
			return _add_struct(name, value);
		}

		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string& name, const _Offset& value, 
			const std::vector<std::byte>& data = std::vector<std::byte>()) {
			return _add_struct(name, std_struct(value, data));
		}
#pragma endregion

#pragma region ADD_STRUCT_ARRAYS
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const _Offset& structOffsets, 
			const std::vector<std::byte>* values, size_t size) {
			return _add_struct_array(name, structOffsets, values, size);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const _Offset& structOffsets, 
			const std::vector<std::byte>(&values)[N]) {
			return _add_struct_array(name, structOffsets, values, N);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const _Offset& structOffsets,
			const std::array<std::vector<std::byte>, N>& values) {
			return _add_struct_array(name, structOffsets, values.data(), N);
		}

		_GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> add(const std::string& name, const _Offset& structOffsets, 
			const std::vector<std::vector<std::byte>>& values) {
			return _add_struct_array(name, structOffsets, values.data(), values.size());
		}
#pragma endregion
#pragma endregion

#pragma region SET
#pragma region SET_SCALAR
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const T& value) {
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
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const T* values, size_t size) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values, size,
				[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const T(&values)[N]) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values, N,
				[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std::array<T, N>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values.data(), N,
				[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std::vector<T>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;

			return _convert_array<T, type>(name, values.data(), values.size(),
				[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}
#pragma endregion

#pragma region SET_VEC
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const V& value) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();

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
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const V* values, size_t size) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values, size,
				[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const V(&values)[N]) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values, N,
				[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true, size_t N)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std::array<V, N>& values) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values, N,
				[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std::vector<V>& values) {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

			return _convert_array<V, type>(name, values.data(), values.size(),
				[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
		}
#pragma endregion

#pragma region SET_MAT
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, bool column_major = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const M& value) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17 (column_major) {
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
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, bool column_major = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const M* values, size_t size) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17 (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values, size,
					[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(size);
				for (size_t i = 0; i < size; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues.data(), size,
					[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, size_t N, bool column_major = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const M(&values)[N]) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17 (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values, N,
					[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(N);
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues.data(), N,
					[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, size_t N, bool column_major = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std::array<M, N>& values) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17(column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values.data(), N,
					[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(N);
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues.data(), N,
					[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, bool column_major = true)
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std::vector<M>& values) {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17 (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, values.data(), values.size(),
					[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues(values.size());
				for (size_t i = 0; i < values.size(); ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;

				return _convert_array<M, type>(name, transposedValues.data(), values.size(),
					[&](const std::string& name, const type* values, size_t size) -> bool { return _set_array(name, values, size); });
			}
		}
#pragma endregion

#pragma region SET_STRUCT
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std_struct<_Offset>& value) {
			return _set_struct(name, value._data);
		}
		
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std::vector<std::byte>& value) {
			return _set_struct(name, value);
		}
#pragma endregion

#pragma region SET_STRUCT_ARRAYS
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std::vector<std::byte>* values, size_t size) {
			return _set_struct_array(name, values, size);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std::vector<std::byte>(&values)[N]) {
			return _set_struct_array(name, values, N);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std::array<std::vector<std::byte>, N>& values) {
			return _set_struct_array(name, values.data(), N);
		}

		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string& name, const std::vector<std::vector<std::byte>>& values) {
			return _set_struct_array(name, values.data(), values.size());
		}
#pragma endregion
#pragma endregion

#pragma region GET
#pragma region GET_SCALARS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_scalar, utils::is_glsl_scalar_v<T>, = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 T get(const std::string& name) const {
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
		_GLSL_STRUCT_CONSTEXPR17 void get(const std::string& name, T*& valuesDest, size_t size) const {
			std::vector<T> values;
			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				values = _get_array<unsigned int, T>(name, 
					[&](const std::string& name) -> std::vector<T> { return _get_array<unsigned int>(name); });
			}
			else {
				values = _get_array<T>(name);
			}
			std::copy(values.begin(), values.begin() + std::min(values.size(), size), valuesDest);
			if (size > values.size()) {
				std::fill(valuesDest + values.size(), valuesDest + size, T());
			}
			values.clear();
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(SV, utils::glsl_scalars_vector, utils::is_glsl_scalars_vector_v<SV>, = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 SV get(const std::string& name) const {
			using T = typename SV::value_type;

			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				return _get_array<unsigned int, T>(name, 
					[&](const std::string& name) -> std::vector<T> { return _get_array<unsigned int>(name); });
			}
			else {
				return _get_array<T>(name);
			}
		}
#pragma endregion

#pragma region GET_VEC
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(V, utils::glsl_vec, utils::is_glsl_vec_v<V>, = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 V get(const std::string& name) const {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();

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
		_GLSL_STRUCT_CONSTEXPR17 void get(const std::string& name, V*& valuesDest, size_t size) const {
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();

			std::vector<V> values;
			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				using type = glm::vec<L, unsigned int>;

				values = _get_array<type, V>(name, 
					[&](const std::string& name) -> std::vector<type> { return _get_array<type>(name); });
			}
			else {
				values = _get_array<V>(name);
			}

			std::copy(values.begin(), values.begin() + std::min(values.size(), size), valuesDest);
			if (size > values.size()) {
				std::fill(valuesDest + values.size(), valuesDest + size, V());
			}
			values.clear();
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(VV, utils::glsl_vecs_vector, utils::is_glsl_vecs_vector_v<VV>, = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 VV get(const std::string& name) const {
			using V = typename VV::value_type;
			using T = typename V::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t L = V::length();

			if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
				using type = glm::vec<L, unsigned int>;

				return _get_array<type, V>(name, 
					[&](const std::string& name) -> std::vector<type> { return _get_array<type>(name); });
			}
			else {
				return _get_array<V>(name);
			}
		}
#pragma endregion

#pragma region GET_MAT
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, bool column_major = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 M get(const std::string& name) const {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17 (column_major) {
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
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(M, utils::glsl_mat, utils::is_glsl_mat_v<M>, = true, bool column_major = true)
		_GLSL_STRUCT_CONSTEXPR17 void get(const std::string& name, M*& valuesDest, size_t size) const {
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			std::vector<M> values;
			if _GLSL_STRUCT_CONSTEXPR17 (column_major) {
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					using type = glm::mat<C, R, unsigned int>;

					values = _get_array<type, M>(name, 
						[&](const std::string& name) -> std::vector<type> { return _get_array<type>(name); });
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
						[&](const std::string& name) -> std::vector<type> { return _get_array<type>(name); });
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

			std::copy(values.begin(), values.begin() + std::min(values.size(), size), valuesDest);
			if (size > values.size()) {
				std::fill(valuesDest + values.size(), valuesDest + size, M());
			}
			values.clear();
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(MV, utils::glsl_mats_vector, utils::is_glsl_mats_vector_v<MV>, = true, bool column_major = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 MV get(const std::string& name) const {
			using M = typename MV::value_type;
			using T = typename M::value_type;
			static _GLSL_STRUCT_CONSTEXPR17 const size_t C = M::row_type::length();
			static _GLSL_STRUCT_CONSTEXPR17 const size_t R = M::col_type::length();

			if _GLSL_STRUCT_CONSTEXPR17 (column_major) {
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<T, bool>) {
					using type = glm::mat<C, R, unsigned int>;

					return _get_array<type, M>(name, 
						[&](const std::string& name) -> std::vector<type> { return _get_array<type>(name); });
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
						[&](const std::string& name) -> std::vector<type> { return _get_array<type>(name); });
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
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(S, utils::glsl_struct_with_offset<_Offset>, (utils::is_glsl_struct_with_offset_v<S, _Offset>), = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 S get(const std::string& name, const _Offset& structOffsets) const {
			return _get_struct(name, structOffsets);
		}
#pragma endregion

#pragma region GET_STRUCT_ARRAYS
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(S, utils::glsl_struct_with_offset<_Offset>, (utils::is_glsl_struct_with_offset_v<S, _Offset>), = true)
		_GLSL_STRUCT_CONSTEXPR17 void get(const std::string& name, const _Offset& structOffsets, S*& valuesDest, size_t size) const {
			std::vector<std_struct> values = _get_struct_array(name, structOffsets);
			std::copy(values.begin(), values.begin() + std::min(values.size(), size). valuesDest);
			if (size > values.size()) {
				std::fill(valuesDest + values.size(), valuesDest + size, std_struct(structOffsets));
			}
			values.clear();
		}

		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(VS, utils::glsl_structs_vector_with_offset<_Offset>, (utils::is_glsl_structs_vector_with_offset_v<VS, _Offset>), = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 VS get(const std::string& name, const _Offset& structTemplate) const {
			return _get_struct_array(name, structTemplate);
		}
#pragma endregion
#pragma endregion

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 const _Offset& get_offsets() const noexcept {
			return _dataOffsets;
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool contains(const std::string& name) const {
			return _dataOffsets.contains(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_offset(const std::string& name) const {
			return _dataOffsets.get_offset(name);
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 std::vector<size_t> get_array_offsets(const std::string& name) const {
			return _dataOffsets.get_array_offsets(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 base_type_handle get_type(const std::string& name) const {
			return _dataOffsets.get_type(name);
		}
		_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_type, utils::is_glsl_type_v<T>, = true)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 std::shared_ptr<T> get_type(const std::string& name) const {
			return dynamic_type_cast<T>(get_type(name));
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_total_size(const std::string& name) const noexcept {
			return _dataOffsets.get_total_size(name);
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_size(const std::string& name) const noexcept {
			return _dataOffsets.get_size(name);
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_padding(const std::string& name) const noexcept {
			return _dataOffsets.get_padding(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 std::vector<std::string> get_names() const {
			return _dataOffsets.get_names();
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 std::vector<std::byte> data() const noexcept {
			return _data;
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t base_aligement() const noexcept {
			return _dataOffsets.base_aligement();
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t size() const noexcept {
			return _data.size();
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t padding() const noexcept {
			return _dataOffsets.padding();
		}

		_GLSL_STRUCT_CONSTEXPR17 void clear_data() noexcept {
			std::fill(_data.begin(), _data.end(), static_cast<std::byte>(0));
		}
		_GLSL_STRUCT_CONSTEXPR17 void clear() noexcept {
			_dataOffsets.clear();
			_data.clear();
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator==(const std_struct& other) const {
			return _dataOffsets == other._dataOffsets &&
				_data == other._data;
		}
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const std_struct& other) const = default;
	};

	using std140_struct = std_struct<std140_offset>;
	using std430_struct = std_struct<std430_offset>;
}

template<class _Offset>
struct std::hash<glslstruct::std_struct<_Offset>> {
	size_t operator()(const glslstruct::std_struct<_Offset>& stdStruct) {
		size_t seed = 0;
		mstd::hash_append(seed, stdStruct._dataOffsets);
		mstd::hash_range(seed, stdStruct._data.begin(), stdStruct._data.end());
		return seed;
	}
};