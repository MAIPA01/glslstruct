/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_BASE_STRUCT_HPP_
	#define _GLSL_STRUCT_BASE_STRUCT_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/layout/base_layout.hpp>
		#include <glslstruct/value/glsl_value.hpp>

namespace glslstruct {
		/**
		 * @ingroup glslstruct
		 * @brief base class for glsl struct representation
		 * @tparam Layout struct layout
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_layout Layout>
		#else
	template<class Layout, std::enable_if_t<utils::is_glsl_layout_v<Layout>, bool> >
		#endif
	class base_struct {
	public:
		/// @brief struct layout type
		using layout_type = Layout;

	private:
		friend struct std::hash<base_struct>;

		/// @brief check if type is simple glsl type or struct with given layout
		template<class T>
		struct is_glsl_simple_or_layout_struct : std::bool_constant<utils::is_glsl_simple_or_layout_struct_v<T, layout_type> > {};

		/// @brief struct layout
		layout_type _layout;
		/// @brief struct data
		std::vector<std::byte> _data;

		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			#pragma region TYPE_CHECKS

		[[nodiscard]] static bool _is_scalar(const base_type_handle& varType, const ValueType type) {
				if (!is_of_type<scalar_type>(varType)) { return false; }
			return type == static_type_cast<scalar_type>(varType)->get_type();
		}

		[[nodiscard]] static bool _is_vec(const base_type_handle& varType, const ValueType type, const size_t length) {
				if (!is_of_type<vec_type>(varType)) { return false; }

			const vec_type_handle vecVarType = static_type_cast<vec_type>(varType);
			return type == vecVarType->get_type() && length == vecVarType->get_length();
		}

		[[nodiscard]] static bool _is_mat(const base_type_handle& varType, const ValueType type, const size_t columns,
		  const size_t rows) {
				if (!is_of_type<mat_type>(varType)) { return false; }

			const mat_type_handle matVarType = static_type_cast<mat_type>(varType);
			return type == matVarType->get_type() && columns == matVarType->get_cols() && rows == matVarType->get_rows();
		}

		[[nodiscard]] static bool _is_struct(const base_type_handle& varType,
		  const mstd::ordered_map<std::string, var_data>& variables) {
				if (!is_of_type<struct_type>(varType)) { return false; }
			return variables == static_type_cast<struct_type>(varType)->get_variables();
		}

		[[nodiscard]] static bool _is_array(const base_type_handle& varType) { return is_of_type<array_type>(varType); }

		[[nodiscard]] bool _scalar_check(const std::string_view name, const ValueType type) const {
			const base_type_handle& varType = _layout.get_type(name);
			return _is_scalar(varType, type);
		}

		[[nodiscard]] bool _scalar_array_check(const std::string_view name, const ValueType type) const {
			const base_type_handle& varType = _layout.get_type(name);
				if (!_is_array(varType)) { return false; }
			return _is_scalar(static_type_cast<array_type>(varType)->get_type(), type);
		}

		[[nodiscard]] bool _vec_check(const std::string_view name, const ValueType type, const size_t length) const {
			const base_type_handle& varType = _layout.get_type(name);
			return _is_vec(varType, type, length);
		}

		[[nodiscard]] bool _vec_array_check(const std::string_view name, const ValueType type, const size_t length) const {
			const base_type_handle& varType = _layout.get_type(name);
				if (!_is_array(varType)) { return false; }
			return _is_vec(static_type_cast<array_type>(varType)->get_type(), type, length);
		}

		[[nodiscard]] bool _mat_check(const std::string_view name, const ValueType type, const size_t columns,
		  const size_t rows) const {
			const base_type_handle& varType = _layout.get_type(name);
			return _is_mat(varType, type, columns, rows);
		}

		[[nodiscard]] bool _mat_array_check(const std::string_view name, const ValueType type, const size_t columns,
		  const size_t rows) const {
			const base_type_handle& varType = _layout.get_type(name);
				if (!_is_array(varType)) { return false; }
			return _is_mat(static_type_cast<array_type>(varType)->get_type(), type, columns, rows);
		}

		[[nodiscard]] bool _struct_check(const std::string_view name,
		  const mstd::ordered_map<std::string, var_data>& variables) const {
			const base_type_handle& varType = _layout.get_type(name);
			return _is_struct(varType, variables);
		}

		[[nodiscard]] bool _struct_array_check(const std::string_view name,
		  const mstd::ordered_map<std::string, var_data>& variables) const {
			const base_type_handle& varType = _layout.get_type(name);
				if (!_is_array(varType)) { return false; }
			return _is_struct(static_type_cast<array_type>(varType)->get_type(), variables);
		}

			#pragma endregion
		#endif

		#pragma region GET_VALUE_DATA

		/// @brief returns scalar data bytes
		template<class T>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_scalar_value_data(const T& value) {
			return scalar_traits<T>::get_data(value).data();
		}

		/// @brief returns vec data bytes
		template<class T>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_vec_value_data(const T& value) {
			return vec_traits<T>::get_data(value).data();
		}

		/// @brief returns mat data bytes
		template<class T>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::vector<std::byte> > _get_mat_value_data(const T& value) {
			const std::vector<vec_data> vecsData = mat_traits<T>::get_data(value).data();

			std::vector<std::vector<std::byte> > data;
			data.reserve(vecsData.size());

				for (size_t i = 0; i != vecsData.size(); ++i) { data.push_back(vecsData[i].data()); }

			return data;
		}

		#pragma endregion

		#pragma region GET_VALUE_FROM_DATA

		/// @brief returns scalar value from data bytes
		template<class T>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 T _get_scalar_value(const std::vector<std::byte>& data) {
			return scalar_traits<T>::get_value(scalar_data(data));
		}

		/// @brief returns vec value from data bytes
		template<class T>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 T _get_vec_value(const std::vector<std::byte>& data) {
			return vec_traits<T>::get_value(vec_data(data));
		}

		/// @brief returns mat value from data bytes
		template<class T>
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 T _get_mat_value(const std::vector<std::vector<std::byte> >& data) {
			std::vector<vec_data> vecsData;
			vecsData.reserve(data.size());
				for (size_t i = 0; i < data.size(); ++i) { vecsData.push_back(vec_data(data[i])); }

			return mat_traits<T>::get_value(mat_data(vecsData));
		}

		#pragma endregion

		#pragma region ADD

		/// @brief adds value data to given offset
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t _add(const size_t valueOffset, const std::byte* valueData,
		  const size_t dataSize) {
				// CHECK ERROR
				if (valueOffset == bad_offset()) { return valueOffset; }

				// RESIZE DATA
				if (_layout.size() > _data.size()) { _data.resize(_layout.size()); }

			// SET VALUE DATA
			std::copy_n(valueData, dataSize, std::next(_data.begin(), valueOffset));

			return valueOffset;
		}

		/// @brief adds array values to given offsets
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_array(const std::vector<size_t>& valuesOffsets,
		  const std::vector<std::byte>* valuesData) {
				// CHECK ERROR
				if (valuesOffsets.empty()) { return valuesOffsets; }

			// RESIZE DATA
			_data.resize(_layout.size());

				// SET VALUES DATA
				for (size_t i = 0; i < valuesOffsets.size(); ++i) {
		// SET VALUE DATA
		#if _GLSL_STRUCT_HAS_CXX20
					std::ranges::copy(valuesData[i], std::next(_data.begin(), valuesOffsets[i]));
		#else
					std::copy(valuesData[i].begin(), valuesData[i].end(), std::next(_data.begin(), valuesOffsets[i]));
		#endif
				}

			return valuesOffsets;
		}

		/// @brief adds scalar
		template<class T>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 size_t _add_scalar(const std::string_view name, const T& value) {
			const size_t valueOffset		  = _layout.template add<T>(name);

			const std::vector<std::byte> data = _get_scalar_value_data(value);

			return _add(valueOffset, data.data(), data.size());
		}

		/// @brief adds scalars array
		template<class T>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_scalar_array(const std::string_view name, const T* values,
		  const size_t valuesCount) {
			const std::vector<size_t> valuesOffsets = _layout.template add<T>(name, valuesCount);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_scalar_value_data(values[i])); }

			return _add_array(valuesOffsets, valuesData.data());
		}

		/// @brief adds vec
		template<class V>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 size_t _add_vec(const std::string_view name, const V& value) {
			const size_t valueOffset		  = _layout.template add<V>(name);

			const std::vector<std::byte> data = _get_vec_value_data(value);

			return _add(valueOffset, data.data(), data.size());
		}

		/// @brief adds vecs array
		template<class V>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_vec_array(const std::string_view name, const V* values,
		  const size_t valuesCount) {
			const std::vector<size_t> valuesOffsets = _layout.template add<V>(name, valuesCount);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_vec_value_data(values[i])); }

			return _add_array(valuesOffsets, valuesData.data());
		}

		/// @brief adds mat
		template<class M>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 size_t _add_mat(const std::string_view name, const M& value) {
			const std::vector<size_t> valueOffsets				  = _layout.template add<M>(name);

			const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(value);

			return _add_array(valueOffsets, valuesData.data()).front();
		}

		/// @brief adds mats array
		template<class M>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_mat_array(const std::string_view name, const M* values,
		  const size_t valuesCount) {
			const std::vector<std::vector<size_t> > valuesOffsets = _layout.template add<M>(name, valuesCount);

			std::vector<size_t> matsOffsets;
			matsOffsets.reserve(valuesOffsets.size());
				for (size_t m = 0; m < valuesCount; ++m) {
					const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(values[m]);

					matsOffsets.push_back(_add_array(valuesOffsets[m], valuesData.data()).front());
				}

			return matsOffsets;
		}

		/// @brief adds struct
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 size_t _add_struct(const std::string_view name, const layout_type& layout,
		  const std::byte* data, const size_t bytesCount) {
			const size_t valueOffset = _layout.add(name, layout);

			std::vector<std::byte> resizedData;
			resizedData.resize(layout.size(), std::byte {});
			std::copy_n(data, bytesCount, resizedData.begin());

			return _add(valueOffset, resizedData.data(), resizedData.size());
		}

		/// @brief adds structs array
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_struct_array(const std::string_view name,
		  const layout_type& layout, const std::vector<std::byte>* datas, const size_t datasCount) {
			return _add_array(_layout.add(name, layout, datasCount), datas);
		}

		/// @brief adds multiple values
		template<class T, class... Ts, size_t Num, size_t... Nums>
		_GLSL_STRUCT_CONSTEXPR17 void _add_values(const glsl_value<T, Num>& value, const glsl_value<Ts, Nums>&... values) {
			using ValueType = glsl_value<T, Num>;

				if _GLSL_STRUCT_CONSTEXPR17 (ValueType::is_array) {
						if _GLSL_STRUCT_CONSTEXPR17 (ValueType::is_struct) { add(value.varName, value.layout, value.value); }
						else { add(value.varName, value.value); }
				}
				else { add(value.varName, value.value); }


				if _GLSL_STRUCT_CONSTEXPR17 (sizeof...(Ts) > 0 && sizeof...(Nums) > 0) { _add_values(values...); }
		}

		#pragma endregion

		#pragma region SET

		/// @brief sets value data at given offset
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool _set(const size_t valueOffset, const std::byte* valueData,
		  const size_t dataSize) {
				if (valueOffset == bad_offset()) { return false; }

			// SET VALUE DATA
			std::copy_n(valueData, std::min(dataSize, _data.size() - valueOffset), std::next(_data.begin(), valueOffset));

			return true;
		}

		/// @brief sets array values data at given offsets
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 bool _set_array(const std::vector<size_t>& valuesOffsets,
		  const std::vector<std::byte>* valuesData, const size_t valuesCount) {
				if (valuesOffsets.empty()) { return false; }

			// SET VALUES DATA
				for (size_t i = 0; i < std::min(valuesOffsets.size(), valuesCount); ++i) {
					const size_t maxSize = std::min(valuesData[i].size(), _data.size() - valuesOffsets[i]);

					// SET VALUE DATA
					std::copy_n(valuesData[i].begin(), maxSize, std::next(_data.begin(), valuesOffsets[i]));
				}

			return true;
		}

		/// @brief sets scalar
		template<class T>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 bool _set_scalar(const std::string_view name, const T& value) {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_scalar_check(name, get_scalar_value_type<T>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif

			const size_t valueOffset			   = _layout.get_offset(name);

			const std::vector<std::byte> valueData = _get_scalar_value_data(value);

			return _set(valueOffset, valueData.data(), valueData.size());
		}

		/// @brief sets scalar array
		template<class T>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 bool _set_scalar_array(const std::string_view name, const T* values,
		  const size_t valuesCount) {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_scalar_array_check(name, get_scalar_value_type<T>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif

			const std::vector<size_t> valuesOffsets = _layout.get_array_offsets(name);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesCount);

				for (size_t i = 0; i < std::min(valuesCount, valuesOffsets.size()); ++i) {
					valuesData.push_back(_get_scalar_value_data(values[i]));
				}

			return _set_array(valuesOffsets, valuesData.data(), valuesCount);
		}

		/// @brief sets vec
		template<class V>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 bool _set_vec(const std::string_view name, const V& value) {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_vec_check(name, get_vec_value_type<V>(), get_vec_length<V>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif

			const size_t valueOffset		  = _layout.get_offset(name);

			const std::vector<std::byte> data = _get_vec_value_data(value);

			return _set(valueOffset, data.data(), data.size());
		}

		/// @brief sets vec array
		template<class V>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 bool _set_vec_array(const std::string_view name, const V* values,
		  const size_t valuesCount) {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_vec_array_check(name, get_vec_value_type<V>(), get_vec_length<V>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif

			const std::vector<size_t> valuesOffsets = _layout.get_array_offsets(name);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesCount);

				for (size_t i = 0; i < std::min(valuesCount, valuesOffsets.size()); ++i) {
					valuesData.push_back(_get_vec_value_data(values[i]));
				}

			return _set_array(valuesOffsets, valuesData.data(), valuesCount);
		}

		/// @brief sets mat
		template<class M>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 bool _set_mat(const std::string_view name, const M& value) {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_mat_check(name, get_mat_value_type<M>(), get_mat_columns<M>(), get_mat_rows<M>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif

			const std::vector<size_t> valuesOffsets				  = _layout.get_array_offsets(name);

			const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(value);

			return _set_array(valuesOffsets, valuesData.data(), valuesData.size());
		}

		/// @brief sets mat array
		template<class M>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 bool _set_mat_array(const std::string_view name, const M* values,
		  const size_t valuesCount) {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_mat_array_check(name, get_mat_value_type<M>(), get_mat_columns<M>(), get_mat_rows<M>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif

			const size_t count = std::min(valuesCount, _layout.get_array_count(name));

				for (size_t m = 0; m < count; ++m) {
					const std::string matName							  = get_array_elem_name(name, m);

					const std::vector<size_t> valuesOffsets				  = _layout.get_array_offsets(matName);

					const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(values[m]);

						if (!_set_array(valuesOffsets, valuesData.data(), valuesData.size())) { return false; }
				}

			return true;
		}

		/// @brief sets struct
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool _set_struct(const std::string_view name,
		  [[maybe_unused]] const layout_type& layout, const std::byte* data, const size_t bytesCount) {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_struct_check(name, layout.get_variables()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif
			return _set(_layout.get_offset(name), data, bytesCount);
		}

		/// @brief sets struct array
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 bool _set_struct_array(const std::string_view name,
		  [[maybe_unused]] const layout_type& layout, const std::vector<std::byte>* values, const size_t valuesCount) {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_struct_array_check(name, layout.get_variables()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif
			return _set_array(_layout.get_array_offsets(name), values, valuesCount);
		}

		#pragma endregion

		#pragma region GET

		/// @brief returns value data at given offset with given size
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get(const size_t valueOffset,
		  const size_t valueSize) const {
			std::vector<std::byte> valueData;
			valueData.resize(valueSize, static_cast<std::byte>(0));

			std::copy_n(std::next(_data.begin(), valueOffset), std::min(valueSize, _data.size() - valueOffset),
			  valueData.begin());

			return valueData;
		}

		/// @brief returns array values data at given offsets with given value size
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<std::vector<std::byte> > _get_array(
		  const std::vector<size_t>& valuesOffsets, const size_t arrayElemSize, const size_t valueSize
		) const {
			// GET ARRAY ELEM DATA MAX SIZE
			const size_t arrayElemDataSize = std::min(arrayElemSize, valueSize);

			// GET VALUES DATA
			std::vector<std::vector<std::byte> > values;
			values.reserve(valuesOffsets.size());

			std::vector<std::byte> valueData;
			valueData.resize(valueSize);

			size_t maxSize = 0;
				for (size_t i = 0; i < valuesOffsets.size(); ++i) {
					// GET MAX VALUE SIZE
					maxSize = std::min(arrayElemDataSize, _data.size() - valuesOffsets[i]);

					// GET VALUE DATA
					std::copy_n(std::next(_data.begin(), valuesOffsets[i]), maxSize, valueData.begin());

						// CHECK VALUE DATA SIZE
						if (maxSize < valueSize) {
							std::fill_n(std::next(valueData.begin(), maxSize), valueSize - maxSize, static_cast<std::byte>(0));
						}

					// GET VALUE
					values.push_back(valueData);
				}

			// CLEAR TEMP VALUE DATA
			valueData.clear();

			// RETURN VALUES
			return values;
		}

		/// @brief gets scalar value
		template<class T>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 T _get_scalar(const std::string_view name) const {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_scalar_check(name, get_scalar_value_type<T>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif
			return _get_scalar_value<T>(_get(_layout.get_offset(name), _layout.get_size(name)));
		}

		/// @brief gets scalars array value
		template<class T>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<T> _get_scalar_array(const std::string_view name) const {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_scalar_array_check(name, get_scalar_value_type<T>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif

			const std::vector<size_t> valuesOffsets = _layout.get_array_offsets(name);

			const std::vector<std::vector<std::byte> > valuesData =
			  _get_array(valuesOffsets, _layout.get_array_elem_size(name), _layout.get_size(name));

			std::vector<T> values;
			values.reserve(valuesData.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { values.push_back(_get_scalar_value<T>(valuesData[i])); }

			return values;
		}

		/// @brief gets vec value
		template<class V>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 V _get_vec(const std::string_view name) const {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_vec_check(name, get_vec_value_type<V>(), get_vec_length<V>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif
			return _get_vec_value<V>(_get(_layout.get_offset(name), _layout.get_size(name)));
		}

		/// @brief gets vec array value
		template<class V>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<V> _get_vec_array(const std::string_view name) const {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_vec_array_check(name, get_vec_value_type<V>(), get_vec_length<V>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif

			const std::vector<size_t> valuesOffsets = _layout.get_array_offsets(name);

			const std::vector<std::vector<std::byte> > valuesData =
			  _get_array(valuesOffsets, _layout.get_array_elem_size(name), _layout.get_size(name));

			std::vector<V> values;
			values.reserve(valuesOffsets.size());
				for (size_t i = 0; i < valuesOffsets.size(); ++i) { values.push_back(_get_vec_value<V>(valuesData[i])); }

			return values;
		}

		/// @brief gets mat value
		template<class M>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 M _get_mat(const std::string_view name) const {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_mat_check(name, get_mat_value_type<M>(), get_mat_columns<M>(), get_mat_rows<M>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif
			return _get_mat_value<M>(_get_array(_layout.get_array_offsets(name), _layout.get_array_elem_size(name),
			  _layout.get_size(name)));
		}

		/// @brief gets mat array value
		template<class M>
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<M> _get_mat_array(const std::string_view name) const {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_mat_array_check(name, get_mat_value_type<M>(), get_mat_columns<M>(), get_mat_rows<M>()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif

			const size_t matsCount = _layout.get_array_count(name);

			std::vector<M> values;
			values.reserve(matsCount);

				for (size_t m = 0; m < matsCount; ++m) {
					const std::string matName				= get_array_elem_name(name, m);

					const std::vector<size_t> valuesOffsets = _layout.get_array_offsets(matName);

					const std::vector<std::vector<std::byte> > valuesData =
					  _get_array(valuesOffsets, _layout.get_size(matName), _layout.get_size(name));

					values.push_back(_get_mat_value<M>(valuesData));
				}

			return values;
		}

		/// @brief gets struct value
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 base_struct _get_struct(const std::string_view name,
		  const layout_type& layout) const {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_struct_check(name, layout.get_variables()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif
			return base_struct(layout, _get(_layout.get_offset(name), layout.size()));
		}

		/// @brief gets struct array value
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<base_struct> _get_struct_array(const std::string_view name,
		  const layout_type& layout) const {
		#if _GLSL_STRUCT_HAS_TYPE_CHECKS
			glsl_struct_assert(_struct_array_check(name, layout.get_variables()),
			  "Type mismatch! (If you don't want to see this error disable type checks)");
		#endif

			const std::vector<size_t> valuesOffsets = _layout.get_array_offsets(name);

			const std::vector<std::vector<std::byte> > valuesData =
			  _get_array(valuesOffsets, _layout.get_array_elem_size(name), layout.size());

			std::vector<base_struct> values;
			values.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { values.emplace_back(layout, valuesData[i]); }

			return values;
		}

		#pragma endregion

	public:
		#pragma region CONSTRUCTORS
		/// @brief default constructor
		#if _GLSL_STRUCT_HAS_CXX20
		template<class T																						 = layout_type,
		  std::enable_if_t<std::is_same_v<T, layout_type> && std::is_default_constructible_v<layout_type>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 base_struct() noexcept _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<layout_type>)
			: _layout() {
		}

		/// @brief constructor with context
		#if _GLSL_STRUCT_HAS_CXX20
		template<class T = layout_type, std::enable_if_t<std::is_same_v<T, layout_type> && layout_type::has_context, bool> = true>
		#endif
		explicit _GLSL_STRUCT_CONSTEXPR20 base_struct(
		  const _GLSL_STRUCT_TYPENAME17 layout_type::context_type& ctx
		) noexcept _GLSL_STRUCT_REQUIRES(layout_type::has_context)
			: _layout(ctx) {
		}

		/// @brief constructor with layout
		explicit _GLSL_STRUCT_CONSTEXPR20 base_struct(const layout_type& layout) noexcept
			: _layout(layout), _data(_layout.size(), static_cast<std::byte>(0)) {}

		/// @brief constructor with layout and data
		explicit _GLSL_STRUCT_CONSTEXPR20 base_struct(const layout_type& layout, const std::vector<std::byte>& data) noexcept
			: _layout(layout), _data(data) {
			_data.resize(_layout.size(), static_cast<std::byte>(0));
		}

		/// @brief constructor with multiple values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_simple_or_layout_struct<layout_type>... Args, size_t... Nums,
		  std::enable_if_t<std::is_default_constructible_v<layout_type>, bool> = true>
		#else
		template<class... Args, size_t... Nums,
		  std::enable_if_t<mstd::all_check_v<is_glsl_simple_or_layout_struct, Args...>, bool> = true>
		#endif
		explicit _GLSL_STRUCT_CONSTEXPR20 base_struct(
		  const glsl_value<Args, Nums>&... values
		) noexcept _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<layout_type>) {
			_add_values(values...);
		}

		/// @brief constructor with multiple values and context
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_simple_or_layout_struct<layout_type>... Args, size_t... Nums,
		  std::enable_if_t<layout_type::has_context, bool> = true>
		#else
		template<class... Args, size_t... Nums,
		  std::enable_if_t<mstd::all_check_v<is_glsl_simple_or_layout_struct, Args...>, bool> = true>
		#endif
		explicit _GLSL_STRUCT_CONSTEXPR20 base_struct(const glsl_value<Args, Nums>&... values,
		  const _GLSL_STRUCT_TYPENAME17 layout_type::context_type& ctx) noexcept _GLSL_STRUCT_REQUIRES(layout_type::has_context)
			: _layout(ctx) {
			_add_values(values...);
		}

		/// @brief default copy constructor
		_GLSL_STRUCT_CONSTEXPR20 base_struct(const base_struct& other) noexcept = default;

		/// @brief move constructor
		_GLSL_STRUCT_CONSTEXPR20 base_struct(base_struct&& other) noexcept
			: _layout(std::move(other._layout)), _data(std::exchange(other._data, {})) {}

		#pragma endregion

		/// @brief default destructor
		_GLSL_STRUCT_CONSTEXPR20 ~base_struct() noexcept								   = default;

		/// @brief default copy assign operator
		_GLSL_STRUCT_CONSTEXPR20 base_struct& operator=(const base_struct& other) noexcept = default;

		/// @brief move assign operator
		_GLSL_STRUCT_CONSTEXPR20 base_struct& operator=(base_struct&& other) noexcept {
			_layout = std::move(other._layout);
			_data	= std::exchange(other._data, {});
			return *this;
		}

		/// @brief invalid offset value returned when there is error
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t bad_offset() noexcept { return layout_type::bad_offset(); }

		#pragma region ADD
		#pragma region ADD_SCALAR
		/// @brief adds scalar with default value and returns offset
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar S>
		#else
		template<class S, std::enable_if_t<utils::is_glsl_scalar_v<S> && std::is_default_constructible_v<S>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 size_t add(
		  const std::string_view name
		) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<S>) {
			return _add_scalar(name, S());
		}

		/// @brief adds scalar and returns offset
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar S>
		#else
		template<class S, std::enable_if_t<utils::is_glsl_scalar_v<S>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 size_t add(const std::string_view name, const S& value) {
			return _add_scalar(name, value);
		}

		#pragma endregion

		#pragma region ADD_SCALARS_ARRAYS
		/// @brief adds array of scalars with pointer to values and size
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar S>
		#else
		template<class S, std::enable_if_t<utils::is_glsl_scalar_v<S>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const S* values, const size_t valuesCount) {
			return _add_scalar_array(name, values, valuesCount);
		}

		/// @brief adds array of scalars
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_array SA>
		#else
		template<class SA,
		  std::enable_if_t<utils::is_glsl_scalars_array_v<SA> && std::is_default_constructible_v<utils::array_value_type_t<SA> >,
			bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name,
		  const size_t count) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<SA> >) {
			using S = utils::array_value_type_t<SA>;
				if _GLSL_STRUCT_CONSTEXPR17 (std::is_same_v<bool, S>) {
					const std::vector<uint8_t> values(count, S());
					return _add_scalar_array<bool>(name, reinterpret_cast<const bool*>(values.data()), values.size());
				}
				else {
					const std::vector<S> values(count, S());
					return _add_scalar_array(name, values.data(), values.size());
				}
		}

		/// @brief adds array of scalars
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_static_size_array SA>
		#else
		template<class SA, std::enable_if_t<utils::is_glsl_scalars_static_size_array_v<SA> &&
											  std::is_default_constructible_v<utils::array_value_type_t<SA> >,
							 bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(
		  const std::string_view name
		) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<SA> >) {
			return add<SA>(name, utils::array_static_size_v<SA>);
		}

		/// @brief adds array of scalars
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_array SA>
		#else
		template<class SA, std::enable_if_t<utils::is_glsl_scalars_array_v<SA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const SA& values) {
			return _add_scalar_array(name, utils::get_array_data(values), utils::get_array_size(values));
		}

		#pragma endregion

		#pragma region ADD_VEC
		/// @brief adds default vec and returns offset
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V> && std::is_default_constructible_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 size_t add(
		  const std::string_view name
		) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<V>) {
			return _add_vec(name, V());
		}

		/// @brief adds vec and returns offset
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view name, const V& value) {
			return _add_vec(name, value);
		}

		#pragma endregion

		#pragma region ADD_VEC_ARRAYS
		/// @brief adds array of vecs using pointer to values and size
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const V* values, const size_t valuesCount) {
			return _add_vec_array(name, values, valuesCount);
		}

		/// @brief adds array of default vecs
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_array VA>
		#else
		template<class VA,
		  std::enable_if_t<utils::is_glsl_vecs_array_v<VA> && std::is_default_constructible_v<utils::array_value_type_t<VA> >,
			bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name,
		  const size_t count) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<VA> >) {
			using V = utils::array_value_type_t<VA>;
			const std::vector<V> values(count, V());
			return _add_vec_array(name, values.data(), values.size());
		}

		/// @brief adds array of default vecs
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_static_size_array VA>
		#else
		template<class VA, std::enable_if_t<utils::is_glsl_vecs_static_size_array_v<VA> &&
											  std::is_default_constructible_v<utils::array_value_type_t<VA> >,
							 bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(
		  const std::string_view name
		) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<VA> >) {
			return add<VA>(name, utils::array_static_size_v<VA>);
		}

		/// @brief adds array of vecs
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_array VA>
		#else
		template<class VA, std::enable_if_t<utils::is_glsl_vecs_array_v<VA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const VA& values) {
			return _add_vec_array(name, utils::get_array_data(values), utils::get_array_size(values));
		}

		#pragma endregion

		#pragma region ADD_MAT
		/// @brief adds default mat and returns offset
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M> && std::is_default_constructible_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 size_t add(
		  const std::string_view name
		) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<M>) {
			return _add_mat(name, M());
		}

		/// @brief adds mat and returns offset
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 size_t add(const std::string_view name, const M& value) {
			return _add_mat(name, value);
		}

		#pragma endregion

		#pragma region ADD_MAT_ARRAYS
		/// @brief adds array of mats with pointer to values and size
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const M* values, const size_t valuesCount) {
			return _add_mat_array(name, values, valuesCount);
		}

		/// @brief adds array of default mats
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_array MA>
		#else
		template<class MA,
		  std::enable_if_t<utils::is_glsl_mats_array_v<MA> && std::is_default_constructible_v<utils::array_value_type_t<MA> >,
			bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name,
		  const size_t count) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<MA> >) {
			using M = utils::array_value_type_t<MA>;
			const std::vector<M> values(count, M());
			return _add_mat_array(name, values.data(), values.size());
		}

		/// @brief adds array of default mats
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_static_size_array MA>
		#else
		template<class MA, std::enable_if_t<utils::is_glsl_mats_static_size_array_v<MA> &&
											  std::is_default_constructible_v<utils::array_value_type_t<MA> >,
							 bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(
		  const std::string_view name
		) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<MA> >) {
			return add<MA>(name, utils::array_static_size_v<MA>);
		}

		/// @brief adds array of mats
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_array MA>
		#else
		template<class MA, std::enable_if_t<utils::is_glsl_mats_array_v<MA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const MA& values) {
			return _add_mat_array(name, utils::get_array_data(values), utils::get_array_size(values));
		}

		#pragma endregion

		#pragma region ADD_STRUCT

		/// @brief adds struct
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view name, const base_struct& value) {
			return _add_struct(name, value._layout, value._data.data(), value._data.size());
		}

		/// @brief adds struct based on layout
		_GLSL_STRUCT_CONSTEXPR20 size_t add(const std::string_view name, const layout_type& layout) {
			const std::vector<std::byte> data(layout.size(), std::byte {});
			return _add_struct(name, layout, data.data(), data.size());
		}

		/// @brief adds struct based on layout and pointer to data in bytes
		_GLSL_STRUCT_CONSTEXPR20 size_t add(const std::string_view name, const layout_type& layout, const std::byte* data,
		  const size_t bytesCount) {
			return _add_struct(name, layout, data, bytesCount);
		}

		/// @brief adds struct based on layout and array of data in bytes
		template<class BA, std::enable_if_t<utils::is_array_of_v<std::is_same, BA, std::byte>, bool> = true>
		_GLSL_STRUCT_CONSTEXPR20 size_t add(const std::string_view name, const layout_type& layout, const BA& data) {
			return _add_struct(name, layout, utils::get_array_data(data), utils::get_array_size(data));
		}

		#pragma endregion

		#pragma region ADD_STRUCT_ARRAYS

		/// @brief adds array of structs based on layout and pointer to data in bytes
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const layout_type& layout,
		  const std::vector<std::byte>* values, const size_t valuesCount) {
			return _add_struct_array(name, layout, values, valuesCount);
		}

		/// @brief adds array of structs based on layout and array of data in bytes
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const layout_type& layout,
		  const size_t count) {
			const std::vector<std::vector<std::byte> > values(count, std::vector<std::byte> {});
			return _add_struct_array(name, layout, values.data(), values.size());
		}

		/// @brief adds array of structs based on layout and array of data in bytes
		template<class SBA, std::enable_if_t<utils::is_array_of_v<std::is_same, SBA, std::vector<std::byte> >, bool> = true>
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const layout_type& layout,
		  const SBA& values) {
			return _add_struct_array(name, layout, utils::get_array_data(values), utils::get_array_size(values));
		}

		#pragma endregion
		#pragma endregion

		#pragma region SET
		#pragma region SET_SCALAR
		/// @brief sets default scalar value
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar S>
		#else
		template<class S, std::enable_if_t<utils::is_glsl_scalar_v<S> && std::is_default_constructible_v<S>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<S>) {
			return _set_scalar(name, S());
		}

		/// @brief sets scalar value
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar S>
		#else
		template<class S, std::enable_if_t<utils::is_glsl_scalar_v<S>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const S& value) {
			return _set_scalar(name, value);
		}

		#pragma endregion

		#pragma region SET_SCALARS_ARRAYS
		/// @brief sets scalar array value using pointer to values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar S>
		#else
		template<class S, std::enable_if_t<utils::is_glsl_scalar_v<S>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const S* values, const size_t valuesCount) {
			return _set_scalar_array(name, values, valuesCount);
		}

		/// @brief sets scalar array value with array of default values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_array SA>
		#else
		template<class SA,
		  std::enable_if_t<utils::is_glsl_scalars_array_v<SA> && std::is_default_constructible_v<utils::array_value_type_t<SA> >,
			bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name,
		  const size_t count) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<SA> >) {
			using S = utils::array_value_type_t<SA>;
			const std::vector<S> values(count, S());
			return _set_scalar_array(name, values.data(), values.size());
		}

		/// @brief sets scalar array value with array of default values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_static_size_array SA>
		#else
		template<class SA, std::enable_if_t<utils::is_glsl_scalars_static_size_array_v<SA> &&
											  std::is_default_constructible_v<utils::array_value_type_t<SA> >,
							 bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(
		  const std::string_view name
		) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<SA> >) {
			return set<SA>(name, utils::array_static_size_v<SA>);
		}

		/// @brief sets scalar array value with array of values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_array SA>
		#else
		template<class SA, std::enable_if_t<utils::is_glsl_scalars_array_v<SA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const SA& values) {
			return _set_scalar_array(name, utils::get_array_data(values), utils::get_array_size(values));
		}

		#pragma endregion

		#pragma region SET_VEC
		/// @brief sets vec default value
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V> && std::is_default_constructible_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view name) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<V>) {
			return _set_vec(name, V());
		}

		/// @brief sets vec value
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view name, const V& value) {
			return _set_vec(name, value);
		}

		#pragma endregion

		#pragma region SET_VEC_ARRAYS
		/// @brief sets vec array value using pointer to values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const V* values, const size_t size) {
			return _set_vec_array(name, values, size);
		}

		/// @brief sets vec array value with array of default values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_array VA>
		#else
		template<class VA,
		  std::enable_if_t<utils::is_glsl_vecs_array_v<VA> && std::is_default_constructible_v<utils::array_value_type_t<VA> >,
			bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name,
		  const size_t count) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<VA> >) {
			using V = utils::array_value_type_t<VA>;
			const std::vector<V> values(count, V());
			return _set_vec_array(name, values.data(), values.size());
		}

		/// @brief sets vec array value with array of default values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_static_size_array VA>
		#else
		template<class VA, std::enable_if_t<utils::is_glsl_vecs_static_size_array_v<VA> &&
											  std::is_default_constructible_v<utils::array_value_type_t<VA> >,
							 bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(
		  const std::string_view name
		) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<VA> >) {
			return set<VA>(name, utils::array_static_size_v<VA>);
		}

		/// @brief sets vec array value with array of values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_array VA>
		#else
		template<class VA, std::enable_if_t<utils::is_glsl_vecs_array_v<VA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const VA& values) {
			return _set_vec_array(name, utils::get_array_data(values), utils::get_array_size(values));
		}

		#pragma endregion

		#pragma region SET_MAT
		/// @brief sets default mat value
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M> && std::is_default_constructible_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<M>) {
			return _set_mat(name, M());
		}

		/// @brief sets mat value
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const M& value) {
			return _set_mat(name, value);
		}

		#pragma endregion

		#pragma region SET_MAT_ARRAYS
		/// @brief sets mat array value using pointer to values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const M* values, const size_t valuesCount) {
			return _set_mat_array(name, values, valuesCount);
		}

		/// @brief sets mat array value with array of default values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_array MA>
		#else
		template<class MA,
		  std::enable_if_t<utils::is_glsl_mats_array_v<MA> && std::is_default_constructible_v<utils::array_value_type_t<MA> >,
			bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name,
		  const size_t count) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<MA> >) {
			using M = utils::array_value_type_t<MA>;
			const std::vector<M> values(count, M());
			return _set_mat_array(name, values.data(), values.size());
		}

		/// @brief sets mat array value with array of default values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_static_size_array MA>
		#else
		template<class MA, std::enable_if_t<utils::is_glsl_mats_static_size_array_v<MA> &&
											  std::is_default_constructible_v<utils::array_value_type_t<MA> >,
							 bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(
		  const std::string_view name
		) _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<utils::array_value_type_t<MA> >) {
			return set<MA>(name, utils::array_static_size_v<MA>);
		}

		/// @brief sets mat array value with array of values
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_array MA>
		#else
		template<class MA, std::enable_if_t<utils::is_glsl_mats_array_v<MA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const MA& values) {
			return _set_mat_array(name, utils::get_array_data(values), utils::get_array_size(values));
		}

		#pragma endregion

		#pragma region SET_STRUCT

		/// @brief sets struct value
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view name, const base_struct& value) {
			return _set_struct(name, value.get_layout(), value._data.data(), value._data.size());
		}

		/// @brief sets empty struct
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const layout_type& layout) {
			const std::vector<std::byte> data(layout.size(), std::byte {});
			return _set_struct(name, layout, data.data(), data.size());
		}

		/// @brief sets struct value using pointer to data
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view name, const layout_type& layout, const std::byte* data,
		  const size_t bytesCount) {
			return _set_struct(name, layout, data, bytesCount);
		}

		/// @brief sets struct value using array of data bytes
		template<class BA, std::enable_if_t<utils::is_array_of_v<std::is_same, BA, std::byte>, bool> = true>
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view name, const layout_type& layout, const BA& data) {
			return _set_struct(name, layout, utils::get_array_data(data), utils::get_array_size(data));
		}

		#pragma endregion

		#pragma region SET_STRUCT_ARRAYS

		/// @brief sets mat array value with pointer to values
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const layout_type& layout,
		  const std::vector<std::byte>* datas, const size_t datasCount) {
			return _set_struct_array(name, layout, datas, datasCount);
		}

		/// @brief sets mat array value using array of datas
		template<class SBA, std::enable_if_t<utils::is_array_of_v<std::is_same, SBA, std::vector<std::byte> >, bool> = true>
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const layout_type& layout, const size_t count) {
			const std::vector<std::vector<std::byte> > data(count, std::vector<std::byte> {});
			return _set_struct_array(name, layout, data.data(), data.size());
		}

		/// @brief sets mat array value using array of datas
		template<class SBA, std::enable_if_t<utils::is_array_of_v<std::is_same, SBA, std::vector<std::byte> >, bool> = true>
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const layout_type& layout, const SBA& datas) {
			return _set_struct_array(name, layout, utils::get_array_data(datas), utils::get_array_size(datas));
		}

		#pragma endregion
		#pragma endregion

		#pragma region GET
		#pragma region GET_SCALARS
		/// @brief gets scalar value
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 T get(const std::string_view name) const {
			return _get_scalar<T>(name);
		}

		#pragma endregion

		#pragma region GET_SCALARS_ARRAYS
		/// @brief copies scalars array value to provided pointer
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 void get(const std::string_view name, const T* valuesDest, const size_t valuesDestCount) const {
			const std::vector<T> values = _get_scalar_array<T>(name);
			std::copy_n(values.data(), std::min(values.size(), valuesDestCount), valuesDest);
		}

		/// @brief gets scalars array value as std::vector
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_array SA>
		#else
		template<class SA, std::enable_if_t<utils::is_glsl_scalars_array_v<SA>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<utils::array_value_type_t<SA> > get(
		  const std::string_view name
		) const {
			return _get_scalar_array<utils::array_value_type_t<SA> >(name);
		}

		#pragma endregion

		#pragma region GET_VEC
		/// @brief gets vec value
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 V get(const std::string_view name) const {
			return _get_vec<V>(name);
		}

		#pragma endregion

		#pragma region GET_VEC_ARRAYS
		/// @brief copies vecs array value to provided pointer
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 void get(const std::string_view name, const V* valuesDest, const size_t valuesDestCount) const {
			const std::vector<V> values = _get_vec_array<V>(name);
			std::copy_n(values.data(), std::min(values.size(), valuesDestCount), valuesDest);
		}

		/// @brief gets vecs array value as std::vector
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_array VA>
		#else
		template<class VA, std::enable_if_t<utils::is_glsl_vecs_array_v<VA>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<utils::array_value_type_t<VA> > get(
		  const std::string_view name
		) const {
			return _get_vec_array<utils::array_value_type_t<VA> >(name);
		}

		#pragma endregion

		#pragma region GET_MAT
		/// @brief gets mat value
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 M get(const std::string_view name) const {
			return _get_mat<M>(name);
		}

		#pragma endregion

		#pragma region GET_MAT_ARRAYS
		/// @brief copies mats array value to provided pointer
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 void get(const std::string_view name, const M* valuesDest, const size_t valuesDestCount) const {
			const std::vector<M> values = _get_mat_array<M>(name);
			std::copy_n(values.data(), std::min(values.size(), valuesDestCount), valuesDest);
		}

		/// @brief gets mats array value as std::vector
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_array MA>
		#else
		template<class MA, std::enable_if_t<utils::is_glsl_mats_array_v<MA>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<utils::array_value_type_t<MA> > get(
		  const std::string_view name
		) const {
			return _get_mat_array<utils::array_value_type_t<MA> >(name);
		}

		#pragma endregion

		#pragma region GET_STRUCT
		/// @brief gets struct value
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_layout_struct<layout_type> S>
		#else
		template<class S, std::enable_if_t<utils::is_glsl_layout_struct_v<S, layout_type>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 base_struct get(const std::string_view name, const layout_type& layout) const {
			return _get_struct(name, layout);
		}

		#pragma endregion

		#pragma region GET_STRUCT_ARRAYS

		/// @brief copies struct array value to provided pointer
		_GLSL_STRUCT_CONSTEXPR20 void get(const std::string_view name, const layout_type& layout, const base_struct* valuesDest,
		  const size_t valuesDestCount) const {
			const std::vector<base_struct> values = _get_struct_array(name, layout);
			std::copy_n(values.data(), std::min(values.size(), valuesDestCount), valuesDest);
		}

		/// @brief gets struct array value as std::vector
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_layout_structs_array<layout_type> LSA>
		#else
		template<class LSA, std::enable_if_t<utils::is_glsl_layout_structs_array_v<LSA, layout_type>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<utils::array_value_type_t<LSA> > get(const std::string_view name,
		  const layout_type& layout) const {
			return _get_struct_array(name, layout);
		}

		#pragma endregion
		#pragma endregion

		/// @brief returns struct layout
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 const layout_type& get_layout() const noexcept { return _layout; }

		/// @brief returns true if struct contains variable with given name
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool contains(const std::string_view name) const { return _layout.contains(name); }

		/// @brief returns offset of variable
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_offset(const std::string_view name) const {
			return _layout.get_offset(name);
		}

		/// @brief returns offsets of elements of array
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> get_array_offsets(const std::string_view name) const {
			return _layout.get_array_offsets(name);
		}

		#if _GLSL_STRUCT_HAS_TYPES
		/// @brief returns type of variable
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 const base_type_handle& get_type(const std::string_view name) const {
			return _layout.get_type(name);
		}

				/// @brief returns type of variable casted to desired type (using dynamic_type_cast)
			#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_type T>
			#else
		template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
			#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 std::shared_ptr<T> get_type(const std::string_view name) const {
			return _layout.template get_type<T>(name);
		}
		#endif

		/// @brief returns total size of variable (size + padding)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_total_size(const std::string_view name) const noexcept {
			return _layout.get_total_size(name);
		}

		/// @brief returns size of variable
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_size(const std::string_view name) const noexcept {
			return _layout.get_size(name);
		}

		/// @brief returns variable padding
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_padding(const std::string_view name) const noexcept {
			return _layout.get_padding(name);
		}

		/// @brief returns names of all variables
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<std::string> get_names() const { return _layout.get_names(); }

		/// @brief returns all variables and their data
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 const std::unordered_map<std::string, var_data>& get_variables() const noexcept {
			return _layout.get_variables();
		}

		/// @brief returns all top level variables and their data
		[[nodiscard]] mstd::ordered_map<std::string, var_data> get_top_level_variables() noexcept {
			return _layout.get_top_level_variables();
		}

		/// @brief returns struct data in bytes
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 const std::vector<std::byte>& data() const noexcept { return _data; }

		/// @brief returns base alignment of struct
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t base_alignment() const noexcept { return _layout.base_alignment(); }

		/// @brief returns size of struct data
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t size() const noexcept { return _data.size(); }

		/// @brief returns struct padding
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t padding() const noexcept { return _layout.padding(); }

		/// @brief resets data to all zeros
		_GLSL_STRUCT_CONSTEXPR17 void clear_data() noexcept {
		#if _GLSL_STRUCT_HAS_CXX20
			std::ranges::fill(_data, static_cast<std::byte>(0));
		#else
			std::fill(_data.begin(), _data.end(), static_cast<std::byte>(0));
		#endif
		}

		/// @brief clears layout and data
		_GLSL_STRUCT_CONSTEXPR17 void clear() noexcept {
			_layout.clear();
			_data.clear();
		}

		/// @brief checks if two structs are equal
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator==(const base_struct& other) const {
			return _layout == other._layout && _data == other._data;
		}

		/// @brief default not equal operator
		#if _GLSL_STRUCT_HAS_CXX20
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const base_struct& other) const = default;
		#else
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const base_struct& other) const { return !(*this == other); }
		#endif
	};
} // namespace glslstruct

/**
 * @brief std::hash overload for base_struct
 * @tparam Layout layout of struct
 * @ingroup glslstruct
 */
template<class Layout>
struct std::hash<glslstruct::base_struct<Layout> > {
	size_t operator()(const glslstruct::base_struct<Layout>& glslStruct) {
		size_t seed = 0;
		mstd::hash_append(seed, glslStruct._layout);
		mstd::hash_range(seed, glslStruct._data.begin(), glslStruct._data.end());
		return seed;
	}
};

	#endif
#endif