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
	#include <ranges>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/layout/base_layout.hpp>
		#include <glslstruct/value/glsl_value.hpp>

namespace glslstruct {
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_layout Layout>
		#else
	template<class Layout, std::enable_if_t<utils::is_glsl_layout_v<Layout>, bool> >
		#endif
	class base_struct {
	private:
		friend struct std::hash<base_struct>;

		template<class T>
		struct _is_simple_or_struct_with_offset : std::bool_constant<utils::is_glsl_simple_or_layout_struct_v<T, Layout> > {};

		Layout _layout;
		std::vector<std::byte> _data;

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_scalar_value_data(const T& value) {
			return scalar_traits<T>::get_data(value).data();
		}

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get_vec_value_data(const T& value) {
			return vec_traits<T>::get_data(value).data();
		}

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR20 std::vector<std::vector<std::byte> > _get_mat_value_data(const T& value) {
			const std::vector<vec_data>& vecsData = mat_traits<T>::get_data(value).data();

			std::vector<std::vector<std::byte> > data;
			data.reserve(vecsData.size());

				for (size_t i = 0; i != vecsData.size(); ++i) { data.push_back(vecsData[i].data()); }

			return data;
		}

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR20 T _get_scalar_value(const std::vector<std::byte>& data) {
			return scalar_traits<T>::get_value(scalar_data(data));
		}

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR20 T _get_vec_value(const std::vector<std::byte>& data) {
			return vec_traits<T>::get_value(vec_data(data));
		}

		template<class T>
		static _GLSL_STRUCT_CONSTEXPR20 T _get_mat_value(const std::vector<std::vector<std::byte> >& data) {
			std::vector<vec_data> vecsData;
			vecsData.reserve(data.size());
				for (size_t i = 0; i < data.size(); ++i) { vecsData.push_back(vec_data(data[i])); }

			return mat_traits<T>::get_value(mat_data(vecsData));
		}

		_GLSL_STRUCT_CONSTEXPR17 size_t _get_array_elem_size(const std::vector<size_t>& offsets) const {
				if (offsets.size() > 1) { return offsets[1] - offsets[0]; }
			return _data.size() - offsets[0];
		}

		#pragma region ADD

		template<class T, class... Ts, size_t num, size_t... nums>
		_GLSL_STRUCT_CONSTEXPR17 void _add_values(const glsl_value<T, num>& value, const glsl_value<Ts, nums>&... values) {
				if _GLSL_STRUCT_CONSTEXPR17 (num == 0) { add(value.var_name, value.value); }
				else {
						if _GLSL_STRUCT_CONSTEXPR17 (value.is_struct) { add(value.var_name, value.layout, value.value); }
						else { add(value.var_name, value.value); }
				}


				if _GLSL_STRUCT_CONSTEXPR17 (sizeof...(Ts) > 0 && sizeof...(nums) > 0) { _add_values(values...); }
		}

		_GLSL_STRUCT_CONSTEXPR17 size_t _add(const size_t valueOffset, const std::byte* valueData, const size_t dataSize) {
				// CHECK ERROR
				if (valueOffset == bad_offset()) { return valueOffset; }

				// RESIZE DATA
				if (_layout.size() > _data.size()) { _data.resize(_layout.size()); }

			// SET VALUE DATA
			std::copy_n(valueData, dataSize, std::next(_data.begin(), valueOffset));

			return valueOffset;
		}

		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_array(const std::vector<size_t>& valuesOffsets,
		  const std::vector<std::byte>* valuesData) {
				// CHECK ERROR
				if (valuesOffsets.empty()) { return valuesOffsets; }

			// RESIZE DATA
			_data.resize(_layout.size());

				// SET VALUES DATA
				for (size_t i = 0; i < valuesOffsets.size(); ++i) {
					// SET VALUE DATA
					std::copy(valuesData[i].begin(), valuesData[i].end(), std::next(_data.begin(), valuesOffsets[i]));
				}

			return valuesOffsets;
		}

		#pragma endregion

		#pragma region SET

		_GLSL_STRUCT_CONSTEXPR17 bool _set(const size_t valueOffset, const std::byte* valueData, const size_t dataSize) {
				if (valueOffset == bad_offset()) { return false; }

			// SET VALUE DATA
			std::copy_n(valueData, std::min(dataSize, _data.size() - valueOffset), std::next(_data.begin(), valueOffset));

			return true;
		}

		_GLSL_STRUCT_CONSTEXPR20 bool _set_array(const std::vector<size_t>& valuesOffsets,
		  const std::vector<std::byte>* valuesData) {
				if (valuesOffsets.empty()) { return false; }

			// SET VALUES DATA
				for (size_t i = 0; i < valuesOffsets.size(); ++i) {
					const size_t maxSize = std::min(valuesData[i].size(), _data.size() - valuesOffsets[i]);

					// SET VALUE DATA
					std::copy_n(valuesData[i].begin(), maxSize, std::next(_data.begin(), valuesOffsets[i]));
				}

			return true;
		}

		#pragma endregion

		#pragma region GET

		_GLSL_STRUCT_CONSTEXPR20 std::vector<std::byte> _get(const size_t valueOffset, const size_t valueSize) const {
			const size_t maxDataSize = std::min(valueSize, _data.size() - valueOffset);

			std::vector<std::byte> valueData;
			valueData.resize(valueSize, static_cast<std::byte>(0));

			std::copy_n(std::next(_data.begin(), valueOffset), maxDataSize, valueData.begin());

			return valueData;
		}

		_GLSL_STRUCT_CONSTEXPR20 std::vector<std::vector<std::byte> > _get_array(const std::vector<size_t>& valuesOffsets,
		  const size_t valueSize) const {
			// GET ARRAY ELEM DATA MAX SIZE
			const size_t arrayElemDataSize = std::min(_get_array_elem_size(valuesOffsets), valueSize);

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

		#pragma endregion

	public:
		using layout_type								= Layout;

		_GLSL_STRUCT_CONSTEXPR20 base_struct() noexcept = default;

		explicit _GLSL_STRUCT_CONSTEXPR20 base_struct(const layout_type& layout) noexcept
			: _layout(layout), _data(_layout.size(), static_cast<std::byte>(0)) {}

		explicit _GLSL_STRUCT_CONSTEXPR20 base_struct(const Layout& layout, const std::vector<std::byte>& data) noexcept
			: _layout(layout), _data(data) {
			_data.resize(_layout.size(), static_cast<std::byte>(0));
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_simple_or_struct_with_offset_value<_Offset>... Args, size_t... nums>
		#else
		template<class... Args, size_t... nums,
		  std::enable_if_t<mstd::all_check_v<_is_simple_or_struct_with_offset, Args...>, bool> = true>
		#endif
		explicit _GLSL_STRUCT_CONSTEXPR20 base_struct(const glsl_value<Args, nums>&... values) noexcept {
			_add_values(values...);
		}

		_GLSL_STRUCT_CONSTEXPR20 base_struct(const base_struct& other) noexcept = default;

		_GLSL_STRUCT_CONSTEXPR20 base_struct(base_struct&& other) noexcept
			: _layout(std::move(other._layout)), _data(std::exchange(other._data, {})) {}

		_GLSL_STRUCT_CONSTEXPR20 ~base_struct() noexcept								   = default;

		_GLSL_STRUCT_CONSTEXPR20 base_struct& operator=(const base_struct& other) noexcept = default;

		_GLSL_STRUCT_CONSTEXPR20 base_struct& operator=(base_struct&& other) noexcept {
			_layout = std::move(other._layout);
			_data	= std::exchange(other._data, {});
			return *this;
		}

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t bad_offset() noexcept { return Layout::bad_offset(); }

		#pragma region ADD
		#pragma region ADD_SCALAR
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 size_t add(const std::string_view name, const T& value) {
			size_t valueOffset				   = _layout.template add<T>(name);

			const std::vector<std::byte>& data = _get_scalar_value_data(value);

			return _add(valueOffset, data.data(), data.size());
		}

		#pragma endregion

		#pragma region ADD_SCALARS_ARRAYS
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const T* values, size_t size) {
			std::vector<size_t> valuesOffsets = _layout.template add<T>(name, size);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_scalar_value_data(values[i])); }

			return _add_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T, size_t N>
		#else
		template<class T, size_t N, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const T (&values)[N]) {
			std::vector<size_t> valuesOffsets = _layout.template add<T>(name, N);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_scalar_value_data(values[i])); }

			return _add_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T, size_t N>
		#else
		template<class T, size_t N, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const std::array<T, N>& values) {
			std::vector<size_t> valuesOffsets = _layout.template add<T>(name, N);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_scalar_value_data(values[i])); }

			return _add_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const std::vector<T>& values) {
			std::vector<size_t> valuesOffsets = _layout.template add<T>(name, values.size());

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_scalar_value_data(values[i])); }

			return _add_array(valuesOffsets, valuesData.data());
		}

		#pragma endregion

		#pragma region ADD_VEC
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view name, const V& value) {
			const size_t valueOffset		   = _layout.template add<V>(name);

			const std::vector<std::byte>& data = _get_vec_value_data(value);

			return _add(valueOffset, data.data(), data.size());
		}

		#pragma endregion

		#pragma region ADD_VEC_ARRAYS
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const V* values, size_t size) {
			const std::vector<size_t> valuesOffsets = _layout.template add<V>(name, size);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_vec_value_data(values[i])); }

			return _add_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V, size_t N>
		#else
		template<class V, size_t N, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const V (&values)[N]) {
			std::vector<size_t> valuesOffsets = _layout.template add<V>(name, N);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_vec_value_data(values[i])); }

			return _add_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V, size_t N>
		#else
		template<class V, size_t N, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const std::array<V, N>& values) {
			std::vector<size_t> valuesOffsets = _layout.template add<V>(name, N);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_vec_value_data(values[i])); }

			return _add_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const std::vector<V>& values) {
			std::vector<size_t> valuesOffsets = _layout.template add<V>(name, values.size());

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_vec_value_data(values[i])); }

			return _add_array(valuesOffsets, valuesData.data());
		}

		#pragma endregion

		#pragma region ADD_MAT
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 size_t add(const std::string_view name, const M& value) {
			const std::vector<size_t> valuesOffsets			= _layout.template add<M>(name);

			const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(value);

			return _add_array(valuesOffsets, valuesData.data()).front();
		}

		#pragma endregion

		#pragma region ADD_MAT_ARRAYS
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const M* values, size_t size) {
			const std::vector<std::vector<size_t> > valuesOffsets = _layout.template add<M>(name, size);

			std::vector<size_t> matsOffsets;
			matsOffsets.reserve(valuesOffsets.size());
				for (size_t m = 0; m < size; ++m) {
					const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(values[m]);

					matsOffsets.push_back(_add_array(valuesOffsets[m], valuesData.data()).front());
				}


			return matsOffsets;
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M, size_t N>
		#else
		template<class M, size_t N, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const M (&values)[N]) {
			const std::vector<std::vector<size_t> > valuesOffsets = _layout.template add<M>(name, N);

			std::vector<size_t> matsOffsets;
			matsOffsets.reserve(valuesOffsets.size());
				for (size_t m = 0; m < N; ++m) {
					const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(values[m]);

					matsOffsets.push_back(_add_array(valuesOffsets[m], valuesData.data()).front());
				}


			return matsOffsets;
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M, size_t N>
		#else
		template<class M, size_t N, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const std::array<M, N>& values) {
			const std::vector<std::vector<size_t> > valuesOffsets = _layout.template add<M>(name, N);

			std::vector<size_t> matsOffsets;
			matsOffsets.reserve(valuesOffsets.size());
				for (size_t m = 0; m < N; ++m) {
					const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(values[m]);

					matsOffsets.push_back(_add_array(valuesOffsets[m], valuesData.data()).front());
				}


			return matsOffsets;
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const std::vector<M>& values) {
			const std::vector<std::vector<size_t> > valuesOffsets = _layout.template add<M>(name, values.size());

			std::vector<size_t> matsOffsets;
			matsOffsets.reserve(valuesOffsets.size());
				for (size_t m = 0; m < values.size(); ++m) {
					const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(values[m]);

					matsOffsets.push_back(_add_array(valuesOffsets[m], valuesData.data()).front());
				}


			return matsOffsets;
		}

		#pragma endregion

		#pragma region ADD_STRUCT

		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view name, const base_struct& value) {
			return add(name, value._layout, value._data);
		}

		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view name, const Layout& value) {
			return add(name, value, std::vector<std::byte>(value.size(), std::byte {}));
		}

		_GLSL_STRUCT_CONSTEXPR20 size_t add(const std::string_view name, const Layout& value,
		  const std::vector<std::byte>& data) {
			const size_t valueOffset				   = _layout.add(name, value);

			std::vector<std::byte> resizedData = data;
			resizedData.resize(value.size(), std::byte {});

			return _add(valueOffset, resizedData.data(), resizedData.size());
		}

		#pragma endregion

		#pragma region ADD_STRUCT_ARRAYS

		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const Layout& layout,
		  const std::vector<std::byte>* values, size_t size) {
			const std::vector<size_t>& valuesOffsets = _layout.add(name, layout, size);

			return _add_array(valuesOffsets, values);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const Layout& layout,
		  const std::vector<std::byte> (&values)[N]) {
			const std::vector<size_t>& valuesOffsets = _layout.add(name, layout, N);

			return _add_array(valuesOffsets, values);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const Layout& layout,
		  const std::array<std::vector<std::byte>, N>& values) {
			const std::vector<size_t>& valuesOffsets = _layout.add(name, layout, N);

			return _add_array(valuesOffsets, values.data());
		}

		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const Layout& layout,
		  const std::vector<std::vector<std::byte> >& values) {
			const std::vector<size_t>& valuesOffsets = _layout.add(name, layout, values.size());

			return _add_array(valuesOffsets, values.data());
		}

		#pragma endregion
		#pragma endregion

		#pragma region SET
		#pragma region SET_SCALAR
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const T& value) {
			const size_t valueOffset			   = _layout.template get<T>(name);

			const std::vector<std::byte> valueData = _get_scalar_data(value);

			return _set(valueOffset, valueData.data(), valueData.size());
		}

		#pragma endregion

		#pragma region SET_SCALARS_ARRAYS
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const T* values, const size_t size) {
			const std::vector<size_t> valuesOffsets = _layout.template get<T>(name);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(size);

				for (size_t i = 0; i < size; ++i) { valuesData.push_back(_get_scalar_value_data(values[i])); }

			return _set_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T, size_t N>
		#else
		template<class T, size_t N, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const T (&values)[N]) {
			const std::vector<size_t> valuesOffsets = _layout.template get<T>(name);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(N);

				for (size_t i = 0; i < N; ++i) { valuesData.push_back(_get_scalar_value_data(values[i])); }

			return _set_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T, size_t N>
		#else
		template<class T, size_t N, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const std::array<T, N>& values) {
			const std::vector<size_t> valuesOffsets = _layout.template get<T>(name);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(N);

				for (size_t i = 0; i < N; ++i) { valuesData.push_back(_get_scalar_value_data(values[i])); }

			return _set_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const std::vector<T>& values) {
			const std::vector<size_t> valuesOffsets = _layout.template get<T>(name);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(values.size());

				for (size_t i = 0; i < values.size(); ++i) { valuesData.push_back(_get_scalar_value_data(values[i])); }

			return _set_array(valuesOffsets, valuesData.data());
		}

		#pragma endregion

		#pragma region SET_VEC
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view name, const V& value) {
			const size_t valueOffset		   = _layout.template get<V>(name);

			const std::vector<std::byte>& data = _get_vec_value_data(value);

			return _set(valueOffset, data.data(), data.size());
		}

		#pragma endregion

		#pragma region SET_VEC_ARRAYS
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const V* values, size_t size) {
			const std::vector<size_t> valuesOffsets = _layout.template get<V>(name, size);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_vec_value_data(values[i])); }

			return _set_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V, size_t N>
		#else
		template<class V, size_t N, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const V (&values)[N]) {
			const std::vector<size_t> valuesOffsets = _layout.template get<V>(name, N);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_vec_value_data(values[i])); }

			return _set_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V, size_t N>
		#else
		template<class V, size_t N, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const std::array<V, N>& values) {
			const std::vector<size_t> valuesOffsets = _layout.template get<V>(name, N);

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_vec_value_data(values[i])); }

			return _set_array(valuesOffsets, valuesData.data());
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const std::vector<V>& values) {
			const std::vector<size_t> valuesOffsets = _layout.template get<V>(name, values.size());

			std::vector<std::vector<std::byte> > valuesData;
			valuesData.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { valuesData.push_back(_get_vec_value_data(values[i])); }

			return _set_array(valuesOffsets, valuesData.data());
		}

		#pragma endregion

		#pragma region SET_MAT
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const M& value) {
			const std::vector<size_t> valuesOffsets			= _layout.template get<M>(name);

			const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(value);

			return _set_array(valuesOffsets, valuesData.data());
		}

		#pragma endregion

		#pragma region SET_MAT_ARRAYS
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const M* values, size_t size) {
			const std::vector<std::vector<size_t> > valuesOffsets = _layout.template get<M>(name, size);

				for (size_t m = 0; m < size; ++m) {
					const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(values[m]);

						if (!_set_array(valuesOffsets[m], valuesData.data())) { return false; }
				}


			return true;
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M, size_t N>
		#else
		template<class M, size_t N, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const M (&values)[N]) {
			const std::vector<std::vector<size_t> > valuesOffsets = _layout.template get<M>(name, N);

				for (size_t m = 0; m < N; ++m) {
					const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(values[m]);

						if (!_set_array(valuesOffsets[m], valuesData.data())) { return false; }
				}


			return true;
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M, size_t N>
		#else
		template<class M, size_t N, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const std::array<M, N>& values) {
			const std::vector<std::vector<size_t> > valuesOffsets = _layout.template get<M>(name, N);

				for (size_t m = 0; m < N; ++m) {
					const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(values[m]);

						if (!_set_array(valuesOffsets[m], valuesData.data())) { return false; }
				}


			return true;
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const std::vector<M>& values) {
			const std::vector<std::vector<size_t> > valuesOffsets = _layout.template get<M>(name, values.size());

				for (size_t m = 0; m < values.size(); ++m) {
					const std::vector<std::vector<std::byte> > valuesData = _get_mat_value_data(values[m]);

						if (!_set_array(valuesOffsets[m], valuesData.data())) { return false; }
				}


			return true;
		}

		#pragma endregion

		#pragma region SET_STRUCT

		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view name, const base_struct& value) {
			return set(name, value._data);
		}

		_GLSL_STRUCT_CONSTEXPR17 bool set(const std::string_view name, const std::vector<std::byte>& value) {
			const size_t valueOffset = _layout.get(name);

			return _set(valueOffset, value.data(), value.size());
		}

		#pragma endregion

		#pragma region SET_STRUCT_ARRAYS

		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const std::vector<std::byte>* values, size_t size) {
			const std::vector<size_t>& valuesOffsets = _layout.get(name, size);

			return _set_array(valuesOffsets, values);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const std::vector<std::byte> (&values)[N]) {
			const std::vector<size_t>& valuesOffsets = _layout.get(name, N);

			return _set_array(valuesOffsets, values);
		}

		template<size_t N>
		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const std::array<std::vector<std::byte>, N>& values) {
			const std::vector<size_t>& valuesOffsets = _layout.get(name, N);

			return _set_array(valuesOffsets, values.data());
		}

		_GLSL_STRUCT_CONSTEXPR20 bool set(const std::string_view name, const std::vector<std::vector<std::byte> >& values) {
			const std::vector<size_t>& valuesOffsets = _layout.get(name, values.size());

			return _set_array(valuesOffsets, values.data());
		}

		#pragma endregion
		#pragma endregion

		#pragma region GET
		#pragma region GET_SCALARS
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 T get(const std::string_view name) const {
			const size_t valueOffset = _layout.get_offset(name);

			return _get_scalar_value<T>(_get(valueOffset, _layout.get_size(name)));
		}

		#pragma endregion

		#pragma region GET_SCALARS_ARRAYS
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 void get(const std::string_view name, T*& valuesDest, const size_t size) const {
			const std::vector<size_t> valuesOffsets				  = _layout.template get_array<T>(name);

			const std::vector<std::vector<std::byte> > valuesData = _get_array(valuesOffsets, _layout.get_size(name));

				for (size_t i = 0; i < size && i < valuesOffsets.size(); ++i) {
					valuesDest[i] = _get_scalar_value<T>(valuesData[i]);
				}
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_vector SV>
		#else
		template<class SV, std::enable_if_t<utils::is_glsl_scalars_vector_v<SV>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 SV get(const std::string_view name) const {
			using T												  = typename SV::value_type;

			const std::vector<size_t> valuesOffsets				  = _layout.template get_array<T>(name);

			const std::vector<std::vector<std::byte> > valuesData = _get_array(valuesOffsets, _layout.get_size(name));

			SV values;
			values.reserve(valuesData.size());
				for (size_t i = 0; i < values.size(); ++i) { values.push_back(_get_scalar_value<T>(valuesData[i])); }

			return values;
		}

		#pragma endregion

		#pragma region GET_VEC
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 V get(const std::string_view name) const {
			const size_t valueOffset = _layout.get_offset(name);

			return _get_vec_value<V>(_get(valueOffset, _layout.get_size(name)));
		}

		#pragma endregion

		#pragma region GET_VEC_ARRAYS
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 void get(const std::string_view name, V*& valuesDest, const size_t size) const {
			const std::vector<size_t> valuesOffsets				  = _layout.template get_array<V>(name);

			const std::vector<std::vector<std::byte> > valuesData = _get_array(valuesOffsets, _layout.get_size(name));

				for (size_t i = 0; i < size && i < valuesOffsets.size(); ++i) {
					valuesDest[i] = _get_vec_value<V>(valuesData[i]);
				}
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_vector VV>
		#else
		template<class VV, std::enable_if_t<utils::is_glsl_vecs_vector_v<VV>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 VV get(const std::string_view name) const {
			using V												  = typename VV::value_type;

			const std::vector<size_t> valuesOffsets				  = _layout.template get_array<V>(name);

			const std::vector<std::vector<std::byte> > valuesData = _get_array(valuesOffsets, _layout.get_size(name));

			VV values;
			values.reserve(valuesData.size());
				for (size_t i = 0; i < values.size(); ++i) { values.push_back(_get_vec_value<V>(valuesData[i])); }

			return values;
		}

		#pragma endregion

		#pragma region GET_MAT
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 M get(const std::string_view name) const {
			const std::vector<size_t> valuesOffsets = _layout.get_array_offsets(name);

			return _get_mat_value<M>(_get_array(valuesOffsets, _layout.get_size(name)));
		}

		#pragma endregion

		#pragma region GET_MAT_ARRAYS
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 void get(const std::string_view name, M*& valuesDest, const size_t size) const {
			const std::vector<std::vector<size_t> > valuesOffsets = _layout.template get_array<M>(name);

				for (size_t m = 0; m < size && m < valuesOffsets.size(); ++m) {
					const std::vector<std::vector<std::byte> > valuesData = _get_array(valuesOffsets[m], _layout.get_size(name));

					valuesDest[m]										  = _get_mat_value<M>(valuesData);
				}
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_vector MV>
		#else
		template<class MV, std::enable_if_t<utils::is_glsl_mats_vector_v<MV>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 MV get(const std::string_view name) const {
			using M												  = typename MV::value_type;

			const std::vector<std::vector<size_t> > valuesOffsets = _layout.template get_array<M>(name);

			MV values;
			values.reserve(valuesOffsets.size());

				for (size_t m = 0; m < values.size(); ++m) {
					const std::vector<std::vector<std::byte> > valuesData = _get_array(valuesOffsets[m], _layout.get_size(name));

					values.push_back(_get_mat_value<M>(valuesData));
				}

			return values;
		}

		#pragma endregion

		#pragma region GET_STRUCT
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_layout_struct<Layout> S>
		#else
		template<class S, std::enable_if_t<utils::is_glsl_layout_struct_v<S, Layout>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 S get(const std::string_view name, const Layout& layout) const {
			const size_t valueOffset = _layout.get_offset(name);

			return S(layout, _get(valueOffset, layout.size()));
		}

		#pragma endregion

		#pragma region GET_STRUCT_ARRAYS
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_layout_struct<Layout> S>
		#else
		template<class S, std::enable_if_t<utils::is_glsl_layout_struct_v<S, Layout>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 void get(const std::string_view name, const Layout& layout, S*& valuesDest,
		  const size_t size) const {
			const std::vector<size_t> valuesOffsets			= _layout.get_array(name, layout);

			const std::vector<std::vector<std::byte> > valuesData = _get_array(valuesOffsets, layout.size());

				for (size_t i = 0; i < valuesOffsets.size() && i < size; ++i) { valuesDest[i] = S(layout, valuesData[i]); }
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_layout_structs_vector<Layout> VS>
		#else
		template<class VS, std::enable_if_t<utils::is_glsl_layout_structs_vector_v<VS, Layout>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 VS get(const std::string_view name, const Layout& layout) const {
			using S											= typename VS::value_type;

			const std::vector<size_t> valuesOffsets			= _layout.get_array(name, layout);

			const std::vector<std::vector<std::byte> > valuesData = _get_array(valuesOffsets, layout.size());

			VS values;
			values.reserve(valuesOffsets.size());

				for (size_t i = 0; i < valuesOffsets.size(); ++i) { values.emplace_back(layout, valuesData[i]); }

			return values;
		}

		#pragma endregion
		#pragma endregion

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 const Layout& get_layout() const noexcept { return _layout; }

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool contains(const std::string_view name) const { return _layout.contains(name); }

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_offset(const std::string_view name) const {
			return _layout.get_offset(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> get_array_offsets(const std::string_view name) const {
			return _layout.get_array_offsets(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 const base_type_handle& get_type(const std::string_view name) const {
			return _layout.get_type(name);
		}
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_type T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 std::shared_ptr<T> get_type(const std::string_view name) const {
			return _layout.template get_type<T>(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_total_size(const std::string_view name) const noexcept {
			return _layout.get_total_size(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_size(const std::string_view name) const noexcept {
			return _layout.get_size(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_padding(const std::string_view name) const noexcept {
			return _layout.get_padding(name);
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<std::string> get_names() const { return _layout.get_names(); }

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 const std::vector<std::byte>& data() const noexcept { return _data; }

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t base_alignment() const noexcept { return _layout.base_alignment(); }

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t size() const noexcept { return _data.size(); }

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t padding() const noexcept { return _layout.padding(); }

		_GLSL_STRUCT_CONSTEXPR17 void clear_data() noexcept { std::fill(_data.begin(), _data.end(), static_cast<std::byte>(0)); }

		_GLSL_STRUCT_CONSTEXPR17 void clear() noexcept {
			_layout.clear();
			_data.clear();
		}

		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator==(const base_struct& other) const {
			return _layout == other._layout && _data == other._data;
		}
		#if _GLSL_STRUCT_HAS_CXX20
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const base_struct& other) const = default;
		#else
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const base_struct& other) const { return !(*this == other); }
		#endif
	};
} // namespace glslstruct

template<class Layout>
struct std::hash<glslstruct::base_struct<Layout> > {
	size_t operator()(const glslstruct::base_struct<Layout>& stdStruct) {
		size_t seed = 0;
		mstd::hash_append(seed, stdStruct._layout);
		mstd::hash_range(seed, stdStruct._data.begin(), stdStruct._data.end());
		return seed;
	}
};

	#endif
#endif