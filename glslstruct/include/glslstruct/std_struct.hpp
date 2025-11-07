#pragma once

#include <mstd/event_handler.hpp>
#include <glslstruct/std140_offset.hpp>
#include <glslstruct/std430_offset.hpp>
#include <glslstruct/std_value.hpp>

namespace std {
	template<class _Offset>
	struct hash<glslstruct::std_struct<_Offset>> {
		size_t operator()(const glslstruct::std_struct<_Offset>& stdStruct);
	};
}

namespace glslstruct {
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<utils::any_offset _Offset>
#else
	template<class _Offset, utils::any_offset_enable_if_t<_Offset, bool>>
#endif
	class std_struct {
	private:
		friend struct std::hash<glslstruct::std_struct<_Offset>>;

		_Offset _dataOffsets;
		std::vector<std::byte> _data;

		template<class T>
		std::vector<std::byte> _getValueData(const T& value) const {
			const std::byte* valueDataPtr = reinterpret_cast<const std::byte*>(&value);
			return std::vector<std::byte>(valueDataPtr, valueDataPtr + sizeof(T));
		}

		size_t _getArrayElemSize(const std::vector<size_t>& offsets) const {
			if (offsets.size() > 1) {
				return offsets[1] - offsets[0];
			}
			else {
				return _data.size() - offsets[0];
			}
		}

		template<class _Start, class _Conv, class _Type>
		std::vector<size_t> _convertArray(const std::string& name, const _Type& values, size_t size, const mstd::func<std::vector<size_t>, const std::string&, 
			const std::vector<_Conv>&>& arrayFunc) {
			if constexpr (std::is_same_v<_Type, std::vector<_Start>> && std::is_same_v<_Start, _Conv>) {
				return arrayFunc(name, values);
			}
			else {
				std::vector<_Conv> convertedValues;
				convertedValues.reserve(size);
				for (size_t i = 0; i < size; ++i) {
					if constexpr (std::is_same_v<_Start, _Conv>) {
						convertedValues.insert(convertedValues.end(), values[i]);
					}
					else {
						convertedValues.insert(convertedValues.end(), (_Conv)values[i]);
					}
				}

				return arrayFunc(name, std::move(convertedValues));
			}
		}

#pragma region ADD
		template<class T, class... Ts, size_t num, size_t... nums>
		void _addMultiple(const std_value<T, num>& value, const std_value<Ts, nums>&... values) {
			if constexpr (num == 0) {
				add(value.var_name, value.value);
			}
			else {
				if constexpr (value.is_struct) {
					add(value.var_name, value.struct_offsets, value.value);
				}
				else {
					add(value.var_name, value.value);
				}
			}


			if constexpr (sizeof...(Ts) > 0 && sizeof...(nums) > 0) {
				_addMultiple(values...);
			}
		}

		template<class T>
		size_t _add(const std::string& name, const T& value) {
			// ADD TO OFFSETS
			size_t valueOffset = std::move(_dataOffsets.add<T>(name));

			// CHECK ERROR
			if (valueOffset == 0 && _data.size() != 0) {
				//SPDLOG_ERROR("Variable '{0}' already added to structure", name);
				return valueOffset;
			}

			// RESERVE SIZE
			_data.reserve(_dataOffsets.size());

			// CHECK VALUE PADDING
			if (_data.size() < valueOffset) {
				_data.resize(valueOffset);
			}

			// GET VALUE DATA
			std::vector<std::byte> valueData = std::move(_getValueData(value));

			// SET VALUE DATA
			if (valueOffset < _data.size()) {
				memcpy(_data.data() + valueOffset, valueData.data(), std::min(_data.size() - valueOffset, valueData.size()));
			}
			if (valueOffset + valueData.size() > _data.size()) {
				_data.insert(_data.end(), valueData.begin() + (_data.size() - valueOffset), valueData.end());
			}

			// CLEAR TEMP VALUE DATA
			valueData.clear();

			// UPDATE SIZE
			if (_data.size() < _data.capacity()) {
				_data.resize(_data.capacity());
			}

			return valueOffset;
		}

		template<class T> 
		std::vector<size_t> _addArray(const std::string& name, const std::vector<T>& values) {
			// CHECK SIZE
			if (values.size() == 0) {
				return std::vector<size_t>();
			}

			// GET OFFSETS
			std::vector<size_t> valuesOffsets = std::move(_dataOffsets.add<T>(name, values.size()));

			// CHECK ERROR
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Variable '{0}' already added to structure", name);
				return valuesOffsets;
			}

			// RESERVE SIZE
			_data.reserve(_dataOffsets.size());

			// SET VALUES DATA
			std::vector<std::byte> valueData;
			for (size_t i = 0; i < valuesOffsets.size() && i < values.size(); ++i) {
				// CHECK VALUE PADDING
				if (_data.size() < valuesOffsets[i]) {
					_data.resize(valuesOffsets[i]);
				}

				// GET VALUE DATA
				valueData = std::move(_getValueData(values[i]));

				// SET VALUE DATA
				if (valuesOffsets[i] < _data.size()) {
					memcpy(_data.data() + valuesOffsets[i], valueData.data(), std::min(_data.size() - valuesOffsets[i], valueData.size()));
				}
				if (valuesOffsets[i] + valueData.size() > _data.size()) {
					_data.insert(_data.end(), valueData.begin() + (_data.size() - valuesOffsets[i]), valueData.end());
				}

				// CLEAR VALUE TEMP DATA
				valueData.clear();
			}

			// UPDATE SIZE
			if (_data.size() < _data.capacity()) {
				_data.resize(_data.capacity());
			}

			return valuesOffsets;
		}

		size_t _addStruct(const std::string& name, const std_struct<_Offset>& value) {
			// ADD TO OFFSETS
			size_t valueOffset = _dataOffsets.add(name, value._dataOffsets);

			// CHECK ERROR
			if (valueOffset == 0 && _data.size() != 0) {
				//SPDLOG_ERROR("Variable '{0}' already added to structure", name);
				return valueOffset;
			}

			// RESERVE SIZE
			_data.reserve(_dataOffsets.size());

			// CHECK PADDING
			if (_data.size() < valueOffset) {
				_data.resize(valueOffset);
			}

			// SET VALUE DATA
			if (valueOffset < _data.size()) {
				memcpy(_data.data() + valueOffset, value._data.data(), std::min(_data.size() - valueOffset, value._data.size()));
			}
			if (valueOffset + value._data.size() > _data.size()) {
				_data.insert(_data.end(), value._data.begin() + (_data.size() - valueOffset), value._data.end());
			}

			// CHECK DATA SIZE
			if (_data.size() < _data.capacity()) {
				_data.resize(_data.capacity());
			}

			return valueOffset;
		}

		std::vector<size_t> _addStructArray(const std::string& name, const _Offset& structOffsets, const std::vector<std::vector<std::byte>>& values)
		{
			// CHECK SIZE
			if (values.size() == 0) return std::vector<size_t>();

			// ADD TO OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.add(name, structOffsets, values.size());

			// CHECK ERROR
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Variable '{0}' already added to structure", name);
				return valuesOffsets;
			}

			// UPDATE SIZE
			_data.reserve(_dataOffsets.size());

			// SET VALUES DATA
			for (size_t i = 0; i < valuesOffsets.size() && i < values.size(); ++i) {
				// CHECK PADDING
				if (_data.size() < valuesOffsets[i]) {
					_data.resize(valuesOffsets[i]);
				}

				// SET VALUE DATA
				if (valuesOffsets[i] < _data.size()) {
					memcpy(_data.data() + valuesOffsets[i], values[i].data(), std::min(_data.size() - valuesOffsets[i], values[i].size()));
				}
				if (valuesOffsets[i] + values[i].size() > _data.size()) {
					_data.insert(_data.end(), values[i].begin() + (_data.size() - valuesOffsets[i]), values[i].end());
				}
			}

			// CHECK DATA SIZE
			if (_data.size() < _data.capacity()) {
				_data.resize(_data.capacity());
			}

			return valuesOffsets;
		}

#pragma endregion

#pragma region SET

		template<class T> 
		bool _set(const std::string& name, const T& value) {
			// CHECK VARIABLE
			if (!_dataOffsets.Contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return false;
			}

			// GET VALUE OFFSET
			size_t valueOffset = std::move(_dataOffsets.get(name));

			// GET VALUE DATA
			std::vector<unsigned char> valueData = std::move(_getValueData(value));

			// SET VALUE DATA
			memcpy(_data.data() + valueOffset, valueData.data(), std::min(valueData.size(), _data.size() - valueOffset));

			// CLEAR TEMP VALUE DATA
			valueData.clear();

			return true;
		}

		template<class T> 
		bool _setArray(const std::string& name, const std::vector<T>& values) {
			// CHECK SIZE
			if (values.size() == 0) {
				return false;
			}

			// CHECK VARIABLE
			if (!_dataOffsets.contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return false;
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = std::move(_dataOffsets.getArray(name));

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Value '{0}' was not declared as any array", name);
				return false;
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = _getArrayElemSize(valuesOffsets);

			// SET VALUES DATA
			std::vector<unsigned char> valueData;
			for (size_t i = 0; i < valuesOffsets.size() && i < values.size(); ++i) {
				// GET VALUE DATA
				valueData = std::move(_getValueData(values[i]));

				// SET VALUE DATA
				memcpy(_data.data(), valueData.data(), std::min(std::min(valueData.size(), arrayElemDataSize), _data.size() - valuesOffsets[i]));

				// CLEAR TEMP VALUE DATA
				valueData.clear();
			}

			// CLEAR VALUES OFFSETS
			valuesOffsets.clear();

			return true;
		}

		bool _setStruct(const std::string& name, const std::vector<std::byte>& value) {
			// CHECK VARIABLE
			if (!_dataOffsets.contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return false;
			}

			// GET OFFSET
			size_t valueOffset = _dataOffsets.get(name);

			// SET VALUE DATA
			memcpy(_data.data() + valueOffset, value.data(), std::min(value.size(), _data.size() - valueOffset));

			return true;
		}

		bool _setStructArray(const std::string& name, const std::vector<std::vector<std::byte>>& values) {
			// CHECK SIZE
			if (values.size() == 0) return false;

			// CHECK VARIABLE
			if (!_dataOffsets.contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return false;
			}

			// GET OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.getArray(name);

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Value '{0}' was not declared as any array", name);
				return false;
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = _getArrayElemSize(valuesOffsets);

			// SET VALUES DATA
			size_t valueDataSize;
			for (size_t i = 0; i < valuesOffsets.size() && i < values.size(); ++i) {
				// GET VALUE DATA MAX SIZE
				size_t valueDataSize = std::min(std::min(values[i].size(), arrayElemDataSize), _data.size() - valuesOffsets[i]);

				// SET VALUE DATA
				memcpy(_data.data() + valuesOffsets[i], values[i].data(), valueDataSize);
			}

			// CLEAR VALUES OFFSETS
			valuesOffsets.clear();

			return true;
		}

#pragma endregion

#pragma region GET

		template<class T> 
		T _get(const std::string& name) const {
			// CHECK VARIABLE
			if (!_dataOffsets.contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return T();
			}

			// GET VALUE OFFSET
			size_t valueOffset = _dataOffsets.get(name);

			// MAKE EMPTY VALUE DATA
			std::vector<std::byte> valueData;

			// RESERVE SPACE
			valueData.reserve(sizeof(T));

			// GET VALUE DATA
			valueData.insert(valueData.begin(), _data.begin() + valueOffset, _data.begin() + valueOffset + glm::min(valueData.capacity(), _data.size() - valueOffset));

			// CHECK VALUE DATA SIZE
			if (valueData.size() < valueData.capacity()) {
				valueData.resize(valueData.capacity());
			}

			// GET VALUE
			T value = *reinterpret_cast<T*>(valueData.data());

			// CLEAR TEMP VALUE DATA
			valueData.clear();

			// RETURN VALUE
			return value;
		}

		template<class T> 
		std::vector<T> _getArray(const std::string& name) const {
			// CHECK VARIABLE
			if (!_dataOffsets.contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return std::vector<T>();
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = std::move(_dataOffsets.getArray(name));

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Value '{0}' was not declared as any array", name);
				return std::vector<T>();
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = std::min(_getArrayElemSize(valuesOffsets), sizeof(T));

			// GET VALUES DATA
			std::vector<T> values;
			std::vector<std::byte> valueData;
			valueData.resize(sizeof(T));
			size_t maxSize;
			for (size_t i = 0; i < valuesOffsets.size(); ++i) {
				// GET MAX VALUE SIZE
				maxSize = std::min(arrayElemDataSize, _data.size() - valuesOffsets[i]);

				// GET VALUE DATA
				memcpy(valueData.data(), _data.data() + valuesOffsets[i], maxSize);

				// CHECK VALUE DATA SIZE
				if (maxSize < sizeof(T)) {
					memset(valueData.data() + maxSize, 0, sizeof(T) - maxSize);
				}

				// GET VALUE
				values.push_back(*reinterpret_cast<T*>(valueData.data()));
			}

			// CLEAR TEMP VALUE DATA
			valueData.clear();

			// CLEAR VALUES OFFSETS
			valuesOffsets.clear();

			// RETURN VALUES
			return values;
		}

		std_struct<_Offset> _getStruct(const std::string& name, const _Offset& structOffsets) const {
			// CHECK VARIABLE
			if (!_dataOffsets.contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return std_struct<_Offset>(structOffsets);
			}

			// GET VALUE OFFSET
			size_t valueOffset = _dataOffsets.get(name);

			// MAKE EMPTY STRUCT
			std_struct<_Offset> value(structOffsets);

			// GET MAX VALUE DATA
			size_t valueDataSize = std::min(structOffsets.size(), _data.size() - valueOffset);

			// SET VALUE DATA
			memcpy(value._data.data(), _data.data() + valueOffset, valueDataSize);

			// RETURN VALUE
			return value;
		}

		std::vector<std_struct<_Offset>> _getStructArray(const std::string& name, const _Offset& structOffsets) const {
			// CHECK VARIABLE
			if (!_dataOffsets.contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return std::vector<std_struct<_Offset>>();
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.getArray(name);

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Value '{0}' was not declared as any array", name);
				return std::vector<std_struct<_Offset>>();
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = _getArrayElemSize(valuesOffsets);

			// GET VALUES DATA
			std::vector<std_struct<_Offset>> values;
			for (size_t i = 0; i < valuesOffsets.size(); ++i) {
				// MAKE EMPTY STRUCT
				std_struct<_Offset> value(structOffsets);

				// GET MAX VALUE DATA
				size_t valueDataSize = std::min(std::min(structOffsets.size(), arrayElemDataSize), _data.size() - valuesOffsets[i]);

				// SET VALUE DATA
				memcpy(value._data.data(), _data.data() + valuesOffsets[i], valueDataSize);

				// ADD VALUE TO VALUES
				values.push_back(value);
			}

			// RETURN VALUE
			return values;
		}

		template<class _Start, class _Conv = _Start>
		std::vector<_Conv> _getArray(const std::string& name, const mstd::func<std::vector<_Start>, const std::string&>& getArrayFunc) {
			if (std::is_same_v<_Start, _Conv>) {
				return getArrayFunc(name);
			}
			else {
				std::vector<_Start> values = getArrayFunc(name);
				std::vector<_Conv> convertedValues;
				for (auto& val : values) {
					convertedValues.push_back((_Conv)val);
				}
				return convertedValues;
			}
		}

#pragma endregion

		void _cloneFrom(const std_struct<_Offset>& stdStruct) noexcept {
			_dataOffsets = stdStruct._dataOffsets;
			_data = stdStruct._data;
		}

	public:
		using offset_type = _Offset;

		std_struct() = default;
		std_struct(std_struct<_Offset>& stdStruct) {
			_cloneFrom(stdStruct);
		}
		std_struct(const std_struct<_Offset>& stdStruct) {
			_cloneFrom(stdStruct);
		}
		std_struct(std_struct<_Offset>&& stdStruct) {
			_cloneFrom(stdStruct);
		}
		std_struct(const _Offset& structOffsets, const std::vector<std::byte>& data = std::vector<std::byte>()) {
			_dataOffsets = structOffsets;
			_data.reserve(_dataOffsets.size());
			_data.insert(_data.begin(), data.begin(), data.begin() + std::min(data.size(), _data.capacity()));
			if (_data.size() < _data.capacity()) {
				_data.resize(_data.capacity());
			}
		}
		template<class... Args, size_t... nums>
		std_struct(const std_value<Args, nums>&... values) {
			_addMultiple(values...);
		}
		virtual ~std_struct() {
			clear();
		}

		std_struct<_Offset>& operator=(std_struct<_Offset>& stdStruct) {
			clear();
			_cloneFrom(stdStruct);
			return *this;
		}
		std_struct<_Offset>& operator=(const std_struct<_Offset>& stdStruct) {
			clear();
			_cloneFrom(stdStruct);
			return *this;
		}
		std_struct<_Offset>& operator=(std_struct<_Offset>&& stdStruct) {
			clear();
			_cloneFrom(stdStruct);
			return *this;
		}

		[[nodiscard]] std_struct<_Offset>* clone() const noexcept {
			return new std_struct<_Offset>(*this);
		}

#pragma region ADD_SCALARS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		size_t add(const std::string& name, const T& value) {
			if constexpr (std::is_same_v<T, bool>) {
				return _add(name, (unsigned int)value);
			}
			else {
				return _add(name, value);
			}
		}

#pragma region ADD_SCALARS_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		std::vector<size_t> add(const std::string& name, const T*& values, size_t size) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _convertArray<T, type>(name, values, size, 
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T, size_t N>
#else
		template<class T, size_t N, utils::scalar_enable_if_t<T, bool> = true>
#endif
		std::vector<size_t> add(const std::string& name, const T(&values)[N]) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _convertArray<T, type>(name, values, N, 
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		std::vector<size_t> add(const std::string& name, const std::vector<T>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _convertArray<T, type>(name, values, values.size(),
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
		}

#pragma endregion
#pragma endregion

#pragma region ADD_VEC
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		size_t add(const std::string& name, const V& value) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if constexpr (std::is_same_v<T, bool>) {
				return _add(name, (glm::vec<L, unsigned int>)value);
			}
			else {
				return _add(name, value);
			}
		}

#pragma region ADD_VEC_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		std::vector<size_t> add(const std::string& name, const V*& values, size_t size) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _convertArray<V, type>(name, values, size, 
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V, size_t N>
#else
		template<class V, size_t N, utils::vec_enable_if_t<V, bool> = true>
#endif
		std::vector<size_t> add(const std::string& name, const V(&values)[N]) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _convertArray<V, type>(name, values, N, 
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		std::vector<size_t> add(const std::string& name, const std::vector<V>& values) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _convertArray<V, type>(name, values, values.size(),
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
		}

#pragma endregion
#pragma endregion

#pragma region ADD_MAT
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		size_t add(const std::string& name, const M& value) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				if constexpr (std::is_same_v<T, bool>) {
					return _add(name, (glm::mat<C, R, unsigned int>)value);
				}
				else {
					return _add(name, value);
				}
			}
			else {
				if constexpr (std::is_same_v<T, bool>) {
					return _add(name, glm::transpose((glm::mat<C, R, unsigned int>)value));
				}
				else {
					return _add(name, glm::transpose(value));
				}
			}
		}

#pragma region ADD_MAT_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		std::vector<size_t> add(const std::string& name, const M*& values, size_t size) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, values, size,
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < size; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, transposedValues, size,
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, size_t N, bool column_major = true>
#else
		template<class M, size_t N, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		std::vector<size_t> add(const std::string& name, const M(&values)[N]) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, values, N,
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, transposedValues, N,
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		std::vector<size_t> add(const std::string& name, const std::vector<M>& values) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, values, values.size(),
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < values.size(); ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, transposedValues, values.size(),
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _addArray(name, values); });
			}
		}

#pragma endregion
#pragma endregion

#pragma region ADD_STRUCT
		size_t add(const std::string& name, const std_struct<_Offset>& value) {
			return _addStruct(name, value);
		}

		size_t add(const std::string& name, const _Offset& value, const std::vector<std::byte>& data = std::vector<std::byte>()) {
			return _addStruct(name, std_struct<_Offset>(value, data));
		}

#pragma region ADD_STRUCT_ARRAYS
		std::vector<size_t> add(const std::string& name, const _Offset& structOffsets, const std::vector<std::byte>*& values, size_t size) {
			return _convertArray<std::vector<std::byte>, std::vector<std::byte>>(name, values, size,
				[&](const std::string& name, const std::vector<std::vector<std::byte>>& convs) -> std::vector<size_t> {
					return _addStructArray(name, structOffsets, convs);
				});
		}

		template<size_t N> 
		std::vector<size_t> add(const std::string& name, const _Offset& structOffsets, const std::vector<std::byte>(&values)[N]) {
			return _convertArray<std::vector<std::byte>, std::vector<std::byte>>(name, values, N,
				[&](const std::string& name, const std::vector<std::vector<std::byte>>& convs) -> std::vector<size_t> {
					return _addStructArray(name, structOffsets, convs);
				}
			);
		}

		std::vector<size_t> add(const std::string& name, const _Offset& structOffsets, const std::vector<std::vector<std::byte>>& values) {
			return _addStructArray(name, structOffsets, values);
		}

#pragma endregion
#pragma endregion


#pragma region SET_SCALARS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		bool set(const std::string& name, const T& value) {
			if constexpr (std::is_same_v<T, bool>) {
				return _set(name, (unsigned int)value);
			}
			else {
				return _set(name, value);
			}
		}

#pragma region SET_SCALARS_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		bool set(const std::string& name, const T*& values, size_t size) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _convertArray<T, type>(name, values, size, 
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T, size_t N>
#else
		template<class T, size_t N, utils::scalar_enable_if_t<T, bool> = true>
#endif
		bool set(const std::string& name, const T(&values)[N]) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _convertArray<T, type>(name, values, N, 
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		bool set(const std::string& name, const std::vector<T>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _convertArray<T, type>(name, values, values.size(),
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
		}

#pragma endregion
#pragma endregion

#pragma region SET_VEC
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		bool set(const std::string& name, const V& value) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if constexpr (std::is_same_v<T, bool>) {
				return _set(name, (glm::vec<L, unsigned int>)value);
			}
			else {
				return _set(name, value);
			}
		}

#pragma region SET_VEC_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		bool set(const std::string& name, const V*& values, size_t size) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _convertArray<V, type>(name, values, size, 
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V, size_t N>
#else
		template<class V, size_t N, utils::vec_enable_if_t<V, bool> = true>
#endif
		bool set(const std::string& name, const V(&values)[N]) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _convertArray<V, type>(name, values, N, 
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		bool set(const std::string& name, const std::vector<V>& values) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _convertArray<V, type>(name, values, values.size(),
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
		}

#pragma endregion
#pragma endregion

#pragma region SET_MAT
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		bool set(const std::string& name, const M& value) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				if constexpr (std::is_same_v<T, bool>) {
					return _set(name, (glm::mat<C, R, unsigned int>)value);
				}
				else {
					return _set(name, value);
				}
			}
			else {
				if constexpr (std::is_same_v<T, bool>) {
					return _set(name, glm::transpose((glm::mat<C, R, unsigned int>)value));
				}
				else {
					return _set(name, glm::transpose(value));
				}
			}
		}

#pragma region SET_MAT_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		bool set(const std::string& name, const M*& values, size_t size) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, values, size,
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < size; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, transposedValues, size,
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, size_t N, bool column_major = true>
#else
		template<class M, size_t N, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		bool set(const std::string& name, const M(&values)[N]) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, values, N,
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, transposedValues, N,
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		bool set(const std::string& name, const std::vector<M>& values) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, values, values.size(),
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < values.size(); ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _convertArray<M, type>(name, transposedValues, values.size(),
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _setArray(name, values); });
			}
		}

#pragma endregion
#pragma endregion

#pragma region SET_STRUCT
		bool set(const std::string& name, const std_struct<_Offset>& value) {
			return _setStruct(name, value._data);
		}
		
		bool set(const std::string& name, const std::vector<std::byte>& value) {
			return _setStruct(name, value);
		}

#pragma region SET_STRUCT_ARRAYS
		bool set(const std::string& name, const std::vector<std::byte>*& values, size_t size) {
			return _convertArray<std::vector<std::byte>, std::vector<std::byte>>(name, values, size,
				[&](const std::string& name, const std::vector<std::vector<std::byte>>& values) -> bool {
					return _setStructArray(name, values);
				});
		}

		template<size_t N> 
		bool set(const std::string& name, const std::vector<std::byte>(&values)[N]) {
			return _convertArray<std::vector<std::byte>, std::vector<std::byte>>(name, values, N,
				[&](const std::string& name, const std::vector<std::vector<std::byte>>& values) -> bool {
					return _setStructArray(name, values);
				});
		}

		bool set(const std::string& name, const std::vector<std::vector<std::byte>>& values) {
			return _setStructArray(name, values);
		}
#pragma endregion
#pragma endregion


#pragma region GET_SCALARS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		T get(const std::string& name) const {
			if constexpr (std::is_same_v<T, bool>) {
				return (T)_get<unsigned int>(name);
			}
			else {
				return _get<T>(name);
			}
		}

#pragma region GET_SCALARS_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		void get(const std::string& name, T*& valuesDest, size_t size) const {
			std::vector<T> values;
			if constexpr (std::is_same_v<T, bool>) {
				values = _getArray<unsigned int, T>(name, [&](const std::string& name) -> std::vector<T> { return _getArray<unsigned int>(name); });
			}
			else {
				values = _getArray<T>(name);
			}
			memcpy(valuesDest, values.data(), std::min(values.size(), size));
			values.clear();
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalars_vector SV>
#else
		template<class SV, utils::scalars_vector_enable_if_t<SV, bool> = true> 
#endif
		SV get(const std::string& name) const {
			using T = typename SV::value_type;
			if constexpr (std::is_same_v<T, bool>) {
				return _getArray<unsigned int, T>(name, [&](const std::string& name) -> std::vector<T> { return _getArray<unsigned int>(name); });
			}
			else {
				return _getArray<T>(name);
			}
		}

#pragma endregion
#pragma endregion

#pragma region GET_VEC
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		V get(const std::string& name) const {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if (std::is_same_v<T, bool>) {
				return (V)_get<glm::vec<L, unsigned int>>(name);
			}
			else {
				return _get<V>(name);
			}
		}

#pragma region GET_VEC_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		void get(const std::string& name, V*& valuesDest, size_t size) const {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			std::vector<V> values;
			if constexpr (std::is_same_v<T, bool>) {
				using type = glm::vec<L, unsigned int>;
				values = _getArray<type, V>(name, [&](const std::string& name) -> std::vector<type> {
						return _getArray<type>(name); 
					});
			}
			else {
				values = _getArray<V>(name);
			}
			memcpy(valuesDest, values.data(), std::min(values.size(), size));
			values.clear();
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vecs_vector VV>
#else
		template<class VV, utils::vecs_vector_enable_if_t<VV, bool> = true>
#endif
		VV get(const std::string& name) const {
			using V = typename VV::value_type;
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if constexpr (std::is_same_v<T, bool>) {
				using type = glm::vec<L, unsigned int>;
				return _getArray<type, V>(name, [&](const std::string& name) -> std::vector<type> {
						return _getArray<type>(name); 
					});
			}
			else {
				return getArray<V>(name);
			}
		}

#pragma endregion
#pragma endregion

#pragma region GET_MAT
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		M get(const std::string& name) const {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				if constexpr (std::is_same_v<T, bool>) {
					return (M)_get<glm::mat<C, R, unsigned int>>(name);
				}
				else {
					return _get<M>(name);
				}
			}
			else {
				if constexpr (std::is_same_v<T, bool>) {
					return (M)glm::transpose(_get<glm::mat<R, C, unsigned int>>(name));
				}
				else {
					return glm::transpose(_get<glm::mat<R, C, T>>(name));
				}
			}
		}

#pragma region GET_MAT_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		void get(const std::string& name, M*& valuesDest, size_t size) const {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			std::vector<M> values;
			if constexpr (column_major) {
				if constexpr (std::is_same_v<T, bool>) {
					using type = glm::mat<C, R, unsigned int>;
					values = _getArray<type, M>(name, [&](const std::string& name) -> std::vector<type> {
						return _getArray<type>(name);
					});
				}
				else {
					values = _getArray<M>(name);
				}
			}
			else {
				using transposedType = glm::mat<R, C, T>;
				std::vector<transposedType> transposedValues;
				if constexpr (std::is_same_v<T, bool>) {
					using type = glm::mat<R, C, unsigned int>;
					transposedValues = _getArray<type, transposedType>(name, [&](const std::string& name) -> std::vector<type> {
						return _getArray<type>(name);
					});
				}
				else {
					transposedValues = _getArray<transposedType>(name);
				}

				for (auto& value : transposedValues) {
					values.push_back(glm::transpose(value));
				}
				transposedValues.clear();
			}
			memcpy(valuesDest, values.data(), std::min(values.size(), size));
			values.clear();
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mats_vector MV, bool column_major = true>
#else
		template<class MV, bool column_major = true, utils::mats_vector_enable_if_t<MV, bool> = true>
#endif
		MV get(const std::string& name) const {
			using M = typename MV::value_type;
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				if constexpr (std::is_same_v<T, bool>) {
					using type = glm::mat<C, R, unsigned int>;
					return _getArray<type, M>(name, [&](const std::string& name) -> std::vector<type> {
						return _getArray<type>(name);
						});
				}
				else {
					return _getArray<M>(name);
				}
			}
			else {
				using transposedType = glm::mat<R, C, T>;
				std::vector<transposedType> transposedValues;
				std::vector<M> values;
				if constexpr (std::is_same_v<T, bool>) {
					using type = glm::mat<R, C, unsigned int>;
					transposedValues = _getArray<type, transposedType>(name, [&](const std::string& name) -> std::vector<type> {
						return _getArray<type>(name);
						});
				}
				else {
					transposedValues = _getArray<transposedType>(name);
				}

				for (auto& value : transposedValues) {
					values.push_back(glm::transpose(value));
				}
				return values;
			}
		}
#pragma endregion
#pragma endregion

#pragma region GET_STRUCT
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::std_struct<_Offset> S>
#else
		template<class S, utils::std_struct_enable_if_t<S, _Offset, bool> = true>
#endif
		S get(const std::string& name, const _Offset& structOffsets) const {
			return _getStruct(name, structOffsets);
		}

#pragma region GET_STRUCT_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::std_struct<_Offset> S>
#else
		template<class S, utils::std_struct_enable_if_t<S, _Offset, bool> = true>
#endif
		void get(const std::string& name, const _Offset& structOffsets, S*& valueDest, size_t size) const {
			std::vector<std_struct<_Offset>> values = _getStructArray(name, structOffsets);
			memcpy(valueDest, values.data(), std::min(values.size(), size));
			values.clear();
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::std_structs_vector<_Offset> VS>
#else
		template<class VS, utils::std_structs_vector_enable_if_t<VS, _Offset, bool> = true>
#endif
		VS get(const std::string& name, const _Offset& structTemplate) const {
			return _getStructArray(name, structTemplate);
		}

#pragma endregion
#pragma endregion

		_Offset getOffsets() const {
			return _dataOffsets;
		}
		size_t getOffset(const std::string& name) const {
			return _dataOffsets.get(name);
		}
		std::vector<size_t> getArrayOffsets(const std::string& name) const {
			return _dataOffsets.getArray(name);
		}

		const base_type* getType(const std::string& name) const {
			return _dataOffsets.getType(name);
		}

		std::vector<std::string> getNames() const {
			return _dataOffsets.getNames();
		}

		std::vector<std::byte> data() const {
			return _data;
		}

		size_t baseAligement() const {
			return _dataOffsets.baseAligement();
		}
		size_t size() const {
			return _data.size();
		}

		void clearData() {
			memset(_data.data(), 0, _data.size());
		}
		void clear() {
			_dataOffsets.clear();
			_data.clear();
		}

		bool operator==(const std_struct<_Offset>& stdStruct) const {
			return _dataOffsets == stdStruct._dataOffsets &&
				_data == stdStruct._data;
		}
		bool operator!=(const std_struct<_Offset>& stdStruct) const {
			return !(*this == stdStruct);
		}
	};

	using std140_struct = std_struct<std140_offset>;
	using std430_struct = std_struct<std430_offset>;
}

template<class _Offset>
size_t std::hash<glslstruct::std_struct<_Offset>>::operator()(const glslstruct::std_struct<_Offset>& stdStruct) {
	size_t seed = 0;
	mstd::hash_append(seed, stdStruct._dataOffsets);
	mstd::hash_range(seed, stdStruct._data.begin(), stdStruct._data.end());
	return seed;
}