#pragma once

#include <mstd/event_handler.hpp>
#include <STD140Offsets.hpp>
#include <STD430Offsets.hpp>
#include <STDValue.hpp>

namespace glsl {
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<utils::any_offset _Offsets>
#else
	template<class _Offsets, utils::any_offset_enable_if_t<_Offsets, bool>>
#endif
	class STDStruct {
	private:
		_Offsets _dataOffsets;
		std::vector<unsigned char> _data;

		template<class T>
		std::vector<unsigned char> _GetValueData(const T& value) const {
			const unsigned char* valueDataPtr = reinterpret_cast<const unsigned char*>(&value);
			return std::vector<unsigned char>(valueDataPtr, valueDataPtr + sizeof(T));
		}

		size_t _GetArrayElemSize(const std::vector<size_t>& offsets) const {
			if (offsets.size() > 1) {
				return offsets[1] - offsets[0];
			}
			else {
				return _data.size() - offsets[0];
			}
		}

		template<class _Start, class _Conv, class _Type>
		std::vector<size_t> _ConvertArray(const std::string& name, const _Type& values, size_t size, const mstd::func<std::vector<size_t>, const std::string&, const std::vector<_Conv>&>& arrayFunc) {
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
		void _AddMultiple(const STDValue<T, num>& value, const STDValue<Ts, nums>&... values) {
			if constexpr (num == 0) {
				Add(value.var_name, value.value);
			}
			else {
				if constexpr (value.is_struct) {
					Add(value.var_name, value.struct_offsets, value.value);
				}
				else {
					Add(value.var_name, value.value);
				}
			}


			if constexpr (sizeof...(Ts) > 0 && sizeof...(nums) > 0) {
				_AddMultiple(values...);
			}
		}

		template<class T>
		size_t _Add(const std::string& name, const T& value) {
			// ADD TO OFFSETS
			size_t valueOffset = std::move(_dataOffsets.Add<T>(name));

			// CHECK ERROR
			if (valueOffset == 0 && _data.size() != 0) {
				//SPDLOG_ERROR("Variable '{0}' already added to structure", name);
				return valueOffset;
			}

			// RESERVE SIZE
			_data.reserve(_dataOffsets.GetSize());

			// CHECK VALUE PADDING
			if (_data.size() < valueOffset) {
				_data.resize(valueOffset);
			}

			// GET VALUE DATA
			std::vector<unsigned char> valueData = std::move(_GetValueData(value));

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
		std::vector<size_t> _AddArray(const std::string& name, const std::vector<T>& values) {
			// CHECK SIZE
			if (values.size() == 0) {
				return std::vector<size_t>();
			}

			// GET OFFSETS
			std::vector<size_t> valuesOffsets = std::move(_dataOffsets.Add<T>(name, values.size()));

			// CHECK ERROR
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Variable '{0}' already added to structure", name);
				return valuesOffsets;
			}

			// RESERVE SIZE
			_data.reserve(_dataOffsets.GetSize());

			// SET VALUES DATA
			std::vector<unsigned char> valueData;
			for (size_t i = 0; i < valuesOffsets.size() && i < values.size(); ++i) {
				// CHECK VALUE PADDING
				if (_data.size() < valuesOffsets[i]) {
					_data.resize(valuesOffsets[i]);
				}

				// GET VALUE DATA
				valueData = std::move(_GetValueData(values[i]));

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

		size_t _AddStruct(const std::string& name, const STDStruct<_Offsets>& value) {
			// ADD TO OFFSETS
			size_t valueOffset = _dataOffsets.Add(name, value._dataOffsets);

			// CHECK ERROR
			if (valueOffset == 0 && _data.size() != 0) {
				//SPDLOG_ERROR("Variable '{0}' already added to structure", name);
				return valueOffset;
			}

			// RESERVE SIZE
			_data.reserve(_dataOffsets.GetSize());

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

		std::vector<size_t> _AddStructArray(const std::string& name, const _Offsets& structOffsets, const std::vector<std::vector<unsigned char>>& values)
		{
			// CHECK SIZE
			if (values.size() == 0) return std::vector<size_t>();

			// ADD TO OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.Add(name, structOffsets, values.size());

			// CHECK ERROR
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Variable '{0}' already added to structure", name);
				return valuesOffsets;
			}

			// UPDATE SIZE
			_data.reserve(_dataOffsets.GetSize());

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
		bool _Set(const std::string& name, const T& value) {
			// CHECK VARIABLE
			if (!_dataOffsets.Contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return false;
			}

			// GET VALUE OFFSET
			size_t valueOffset = std::move(_dataOffsets.Get(name));

			// GET VALUE DATA
			std::vector<unsigned char> valueData = std::move(_GetValueData(value));

			// SET VALUE DATA
			memcpy(_data.data() + valueOffset, valueData.data(), std::min(valueData.size(), _data.size() - valueOffset));

			// CLEAR TEMP VALUE DATA
			valueData.clear();

			return true;
		}

		template<class T> 
		bool _SetArray(const std::string& name, const std::vector<T>& values) {
			// CHECK SIZE
			if (values.size() == 0) {
				return false;
			}

			// CHECK VARIABLE
			if (!_dataOffsets.Contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return false;
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = std::move(_dataOffsets.GetArray(name));

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Value '{0}' was not declared as any array", name);
				return false;
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = _GetArrayElemSize(valuesOffsets);

			// SET VALUES DATA
			std::vector<unsigned char> valueData;
			for (size_t i = 0; i < valuesOffsets.size() && i < values.size(); ++i) {
				// GET VALUE DATA
				valueData = std::move(_GetValueData(values[i]));

				// SET VALUE DATA
				memcpy(_data.data(), valueData.data(), std::min(std::min(valueData.size(), arrayElemDataSize), _data.size() - valuesOffsets[i]));

				// CLEAR TEMP VALUE DATA
				valueData.clear();
			}

			// CLEAR VALUES OFFSETS
			valuesOffsets.clear();

			return true;
		}

		bool _SetStruct(const std::string& name, const std::vector<unsigned char>& value) {
			// CHECK VARIABLE
			if (!_dataOffsets.Contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return false;
			}

			// GET OFFSET
			size_t valueOffset = _dataOffsets.Get(name);

			// SET VALUE DATA
			memcpy(_data.data() + valueOffset, value.data(), std::min(value.size(), _data.size() - valueOffset));

			return true;
		}

		bool _SetStructArray(const std::string& name, const std::vector<std::vector<unsigned char>>& values) {
			// CHECK SIZE
			if (values.size() == 0) return false;

			// CHECK VARIABLE
			if (!_dataOffsets.Contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return false;
			}

			// GET OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.GetArray(name);

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Value '{0}' was not declared as any array", name);
				return false;
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = _GetArrayElemSize(valuesOffsets);

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
		T _Get(const std::string& name) const {
			// CHECK VARIABLE
			if (!_dataOffsets.Contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return T();
			}

			// GET VALUE OFFSET
			size_t valueOffset = _dataOffsets.Get(name);

			// MAKE EMPTY VALUE DATA
			std::vector<unsigned char> valueData;

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
		std::vector<T> _GetArray(const std::string& name) const {
			// CHECK VARIABLE
			if (!_dataOffsets.Contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return std::vector<T>();
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = std::move(_dataOffsets.GetArray(name));

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Value '{0}' was not declared as any array", name);
				return std::vector<T>();
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = std::min(_GetArrayElemSize(valuesOffsets), sizeof(T));

			// GET VALUES DATA
			std::vector<T> values;
			std::vector<unsigned char> valueData;
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

		STDStruct<_Offsets> _GetStruct(const std::string& name, const _Offsets& structOffsets) const {
			// CHECK VARIABLE
			if (!_dataOffsets.Contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return STDStruct<_Offsets>(structOffsets);
			}

			// GET VALUE OFFSET
			size_t valueOffset = _dataOffsets.Get(name);

			// MAKE EMPTY STRUCT
			STDStruct<_Offsets> value(structOffsets);

			// GET MAX VALUE DATA
			size_t valueDataSize = std::min(structOffsets.GetSize(), _data.size() - valueOffset);

			// SET VALUE DATA
			memcpy(value._data.data(), _data.data() + valueOffset, valueDataSize);

			// RETURN VALUE
			return value;
		}

		std::vector<STDStruct<_Offsets>> _GetStructArray(const std::string& name, const _Offsets& structOffsets) const {
			// CHECK VARIABLE
			if (!_dataOffsets.Contains(name)) {
				//SPDLOG_ERROR("No value called '{0}' was added to this structure", name);
				return std::vector<STDStruct<_Offsets>>();
			}

			// GET VALUES OFFSETS
			std::vector<size_t> valuesOffsets = _dataOffsets.GetArray(name);

			// CHECK ARRAY ELEMENTS OFFSETS
			if (valuesOffsets.size() == 0) {
				//SPDLOG_ERROR("Value '{0}' was not declared as any array", name);
				return std::vector<STDStruct<_Offsets>>();
			}

			// GET ARRAY ELEM DATA MAX SIZE
			size_t arrayElemDataSize = _GetArrayElemSize(valuesOffsets);

			// GET VALUES DATA
			std::vector<STDStruct<_Offsets>> values;
			for (size_t i = 0; i < valuesOffsets.size(); ++i) {
				// MAKE EMPTY STRUCT
				STDStruct<_Offsets> value(structOffsets);

				// GET MAX VALUE DATA
				size_t valueDataSize = std::min(std::min(structOffsets.GetSize(), arrayElemDataSize), _data.size() - valuesOffsets[i]);

				// SET VALUE DATA
				memcpy(value._data.data(), _data.data() + valuesOffsets[i], valueDataSize);

				// ADD VALUE TO VALUES
				values.push_back(value);
			}

			// RETURN VALUE
			return values;
		}

		template<class _Start, class _Conv = _Start>
		std::vector<_Conv> _GetArray(const std::string& name, const mstd::func<std::vector<_Start>, const std::string&>& getArrayFunc) {
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

	public:
		using offset_type = _Offsets;

		STDStruct() = default;
		STDStruct(STDStruct<_Offsets>& stdStruct) {
			stdStruct.CloneTo(this);
		}
		STDStruct(const STDStruct<_Offsets>& stdStruct) {
			stdStruct.CloneTo(this);
		}
		STDStruct(STDStruct<_Offsets>&& stdStruct) {
			stdStruct.CloneTo(this);
		}
		STDStruct(const _Offsets& structOffsets, const std::vector<unsigned char>& data = std::vector<unsigned char>()) {
			_dataOffsets = structOffsets;
			_data.reserve(_dataOffsets.GetSize());
			_data.insert(_data.begin(), data.begin(), data.begin() + std::min(data.size(), _data.capacity()));
			if (_data.size() < _data.capacity()) {
				_data.resize(_data.capacity());
			}
		}
		template<class... Args, size_t... nums>
		STDStruct(const STDValue<Args, nums>&... values) {
			_AddMultiple(values...);
		}
		virtual ~STDStruct() {
			Clear();
		}

		STDStruct<_Offsets>& operator=(STDStruct<_Offsets>& stdStruct) {
			stdStruct.CloneTo(this);
			return *this;
		}
		STDStruct<_Offsets>& operator=(const STDStruct<_Offsets>& stdStruct) {
			stdStruct.CloneTo(this);
			return *this;
		}
		STDStruct<_Offsets>& operator=(STDStruct<_Offsets>&& stdStruct) {
			stdStruct.CloneTo(this);
			return *this;
		}

		DECLARE_CLONE_FUNC_WITH_DEFINITION(STDStruct<_Offsets>, _dataOffsets, _data)

#pragma region ADD_SCALARS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		size_t Add(const std::string& name, const T& value) {
			if constexpr (std::is_same_v<T, bool>) {
				return _Add(name, (unsigned int)value);
			}
			else {
				return _Add(name, value);
			}
		}

#pragma region ADD_SCALARS_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, const T*& values, size_t size) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _ConvertArray<T, type>(name, values, size, 
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T, size_t N>
#else
		template<class T, size_t N, utils::scalar_enable_if_t<T, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, const T(&values)[N]) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _ConvertArray<T, type>(name, values, N, 
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, const std::vector<T>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _ConvertArray<T, type>(name, values, values.size(),
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
		}

#pragma endregion
#pragma endregion

#pragma region ADD_VEC
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		size_t Add(const std::string& name, const V& value) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if constexpr (std::is_same_v<T, bool>) {
				return _Add(name, (glm::vec<L, unsigned int>)value);
			}
			else {
				return _Add(name, value);
			}
		}

#pragma region ADD_VEC_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, const V*& values, size_t size) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _ConvertArray<V, type>(name, values, size, 
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V, size_t N>
#else
		template<class V, size_t N, utils::vec_enable_if_t<V, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, const V(&values)[N]) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _ConvertArray<V, type>(name, values, N, 
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, const std::vector<V>& values) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _ConvertArray<V, type>(name, values, values.size(),
				[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
		}

#pragma endregion
#pragma endregion

#pragma region ADD_MAT
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		size_t Add(const std::string& name, const M& value) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				if constexpr (std::is_same_v<T, bool>) {
					return _Add(name, (glm::mat<C, R, unsigned int>)value);
				}
				else {
					return _Add(name, value);
				}
			}
			else {
				if constexpr (std::is_same_v<T, bool>) {
					return _Add(name, glm::transpose((glm::mat<C, R, unsigned int>)value));
				}
				else {
					return _Add(name, glm::transpose(value));
				}
			}
		}

#pragma region ADD_MAT_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, const M*& values, size_t size) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, values, size,
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < size; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, transposedValues, size,
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, size_t N, bool column_major = true>
#else
		template<class M, size_t N, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, const M(&values)[N]) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, values, N,
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, transposedValues, N,
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, const std::vector<M>& values) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, values, values.size(),
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < values.size(); ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, transposedValues, values.size(),
					[&](const std::string& name, const std::vector<type>& values) -> std::vector<size_t> { return _AddArray(name, values); });
			}
		}

#pragma endregion
#pragma endregion

#pragma region ADD_STRUCT
		size_t Add(const std::string& name, const STDStruct<_Offsets>& value) {
			return _AddStruct(name, value);
		}

		size_t Add(const std::string& name, const _Offsets& value, const std::vector<unsigned char>& data = std::vector<unsigned char>()) {
			return _AddStruct(name, STDStruct<_Offsets>(value, data));
		}

#pragma region ADD_STRUCT_ARRAYS
		std::vector<size_t> Add(const std::string& name, const _Offsets& structOffsets, const std::vector<unsigned char>*& values, size_t size) {
			return _ConvertArray<std::vector<unsigned char>, std::vector<unsigned char>>(name, values, size,
				[&](const std::string& name, const std::vector<std::vector<unsigned char>>& convs) -> std::vector<size_t> {
					return _AddStructArray(name, structOffsets, convs);
				});
		}

		template<size_t N> 
		std::vector<size_t> Add(const std::string& name, const _Offsets& structOffsets, const std::vector<unsigned char>(&values)[N]) {
			return _ConvertArray<std::vector<unsigned char>, std::vector<unsigned char>>(name, values, N,
				[&](const std::string& name, const std::vector<std::vector<unsigned char>>& convs) -> std::vector<size_t> {
					return _AddStructArray(name, structOffsets, convs);
				}
			);
		}

		std::vector<size_t> Add(const std::string& name, const _Offsets& structOffsets, const std::vector<std::vector<unsigned char>>& values) {
			return _AddStructArray(name, structOffsets, values);
		}

#pragma endregion
#pragma endregion


#pragma region SET_SCALARS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		bool Set(const std::string& name, const T& value) {
			if constexpr (std::is_same_v<T, bool>) {
				return _Set(name, (unsigned int)value);
			}
			else {
				return _Set(name, value);
			}
		}

#pragma region SET_SCALARS_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		bool Set(const std::string& name, const T*& values, size_t size) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _ConvertArray<T, type>(name, values, size, 
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T, size_t N>
#else
		template<class T, size_t N, utils::scalar_enable_if_t<T, bool> = true>
#endif
		bool Set(const std::string& name, const T(&values)[N]) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _ConvertArray<T, type>(name, values, N, 
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		bool Set(const std::string& name, const std::vector<T>& values) {
			using type = std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>;
			return _ConvertArray<T, type>(name, values, values.size(),
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
		}

#pragma endregion
#pragma endregion

#pragma region SET_VEC
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		bool Set(const std::string& name, const V& value) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if constexpr (std::is_same_v<T, bool>) {
				return _Set(name, (glm::vec<L, unsigned int>)value);
			}
			else {
				return _Set(name, value);
			}
		}

#pragma region SET_VEC_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		bool Set(const std::string& name, const V*& values, size_t size) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _ConvertArray<V, type>(name, values, size, 
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V, size_t N>
#else
		template<class V, size_t N, utils::vec_enable_if_t<V, bool> = true>
#endif
		bool Set(const std::string& name, const V(&values)[N]) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _ConvertArray<V, type>(name, values, N, 
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		bool Set(const std::string& name, const std::vector<V>& values) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			using type = glm::vec<L, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
			return _ConvertArray<V, type>(name, values, values.size(),
				[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
		}

#pragma endregion
#pragma endregion

#pragma region SET_MAT
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		bool Set(const std::string& name, const M& value) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				if constexpr (std::is_same_v<T, bool>) {
					return _Set(name, (glm::mat<C, R, unsigned int>)value);
				}
				else {
					return _Set(name, value);
				}
			}
			else {
				if constexpr (std::is_same_v<T, bool>) {
					return _Set(name, glm::transpose((glm::mat<C, R, unsigned int>)value));
				}
				else {
					return _Set(name, glm::transpose(value));
				}
			}
		}

#pragma region SET_MAT_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		bool Set(const std::string& name, const M*& values, size_t size) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, values, size,
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < size; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, transposedValues, size,
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, size_t N, bool column_major = true>
#else
		template<class M, size_t N, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		bool Set(const std::string& name, const M(&values)[N]) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, values, N,
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < N; ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, transposedValues, N,
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		bool Set(const std::string& name, const std::vector<M>& values) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				using type = glm::mat<C, R, std::conditional_t<std::is_same<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, values, values.size(),
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
			}
			else {
				std::vector<glm::mat<R, C, T>> transposedValues;
				for (size_t i = 0; i < values.size(); ++i) {
					transposedValues.push_back(glm::transpose(values[i]));
				}

				using type = glm::mat<R, C, std::conditional_t<std::is_same_v<T, bool>, unsigned int, T>>;
				return _ConvertArray<M, type>(name, transposedValues, values.size(),
					[&](const std::string& name, const std::vector<type>& values) -> bool { return _SetArray(name, values); });
			}
		}

#pragma endregion
#pragma endregion

#pragma region SET_STRUCT
		bool Set(const std::string& name, const STDStruct<_Offsets>& value) {
			return _SetStruct(name, value._data);
		}
		
		bool Set(const std::string& name, const std::vector<unsigned char>& value) {
			return _SetStruct(name, value);
		}

#pragma region SET_STRUCT_ARRAYS
		bool Set(const std::string& name, const std::vector<unsigned char>*& values, size_t size) {
			return _ConvertArray<std::vector<unsigned char>, std::vector<unsigned char>>(name, values, size,
				[&](const std::string& name, const std::vector<std::vector<unsigned char>>& values) -> bool {
					return _SetStructArray(name, values);
				});
		}

		template<size_t N> 
		bool Set(const std::string& name, const std::vector<unsigned char>(&values)[N]) {
			return _ConvertArray<std::vector<unsigned char>, std::vector<unsigned char>>(name, values, N, 
				[&](const std::string& name, const std::vector<std::vector<unsigned char>>& values) -> bool { 
					return _SetStructArray(name, values);
				});
		}

		bool Set(const std::string& name, const std::vector<std::vector<unsigned char>>& values) {
			return _SetStructArray(name, values);
		}
#pragma endregion
#pragma endregion


#pragma region GET_SCALARS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		T Get(const std::string& name) const {
			if constexpr (std::is_same_v<T, bool>) {
				return (T)_Get<unsigned int>(name);
			}
			else {
				return _Get<T>(name);
			}
		}

#pragma region GET_SCALARS_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		void Get(const std::string& name, T*& valuesDest, size_t size) const {
			std::vector<T> values;
			if constexpr (std::is_same_v<T, bool>) {
				values = _GetArray<unsigned int, T>(name, [&](const std::string& name) -> std::vector<T> { return _GetArray<unsigned int>(name); });
			}
			else {
				values = _GetArray<T>(name);
			}
			memcpy(valuesDest, values.data(), std::min(values.size(), size));
			values.clear();
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalars_vector SV>
#else
		template<class SV, utils::scalars_vector_enable_if_t<SV, bool> = true> 
#endif
		SV Get(const std::string& name) const {
			using T = typename SV::value_type;
			if constexpr (std::is_same_v<T, bool>) {
				return _GetArray<unsigned int, T>(name, [&](const std::string& name) -> std::vector<T> { return _GetArray<unsigned int>(name); });
			}
			else {
				return _GetArray<T>(name);
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
		V Get(const std::string& name) const {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if (std::is_same_v<T, bool>) {
				return (V)_Get<glm::vec<L, unsigned int>>(name);
			}
			else {
				return _Get<V>(name);
			}
		}

#pragma region GET_VEC_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		void Get(const std::string& name, V*& valuesDest, size_t size) const {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			std::vector<V> values;
			if constexpr (std::is_same_v<T, bool>) {
				using type = glm::vec<L, unsigned int>;
				values = _GetArray<type, V>(name, [&](const std::string& name) -> std::vector<type> {
						return _GetArray<type>(name); 
					});
			}
			else {
				values = _GetArray<V>(name);
			}
			memcpy(valuesDest, values.data(), std::min(values.size(), size));
			values.clear();
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vecs_vector VV>
#else
		template<class VV, utils::vecs_vector_enable_if_t<VV, bool> = true>
#endif
		VV Get(const std::string& name) const {
			using V = typename VV::value_type;
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if constexpr (std::is_same_v<T, bool>) {
				using type = glm::vec<L, unsigned int>;
				return _GetArray<type, V>(name, [&](const std::string& name) -> std::vector<type> {
						return _GetArray<type>(name); 
					});
			}
			else {
				return GetArray<V>(name);
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
		M Get(const std::string& name) const {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				if constexpr (std::is_same_v<T, bool>) {
					return (M)_Get<glm::mat<C, R, unsigned int>>(name);
				}
				else {
					return _Get<M>(name);
				}
			}
			else {
				if constexpr (std::is_same_v<T, bool>) {
					return (M)glm::transpose(_Get<glm::mat<R, C, unsigned int>>(name));
				}
				else {
					return glm::transpose(_Get<glm::mat<R, C, T>>(name));
				}
			}
		}

#pragma region GET_MAT_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		void Get(const std::string& name, M*& valuesDest, size_t size) const {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			std::vector<M> values;
			if constexpr (column_major) {
				if constexpr (std::is_same_v<T, bool>) {
					using type = glm::mat<C, R, unsigned int>;
					values = _GetArray<type, M>(name, [&](const std::string& name) -> std::vector<type> {
						return _GetArray<type>(name);
					});
				}
				else {
					values = _GetArray<M>(name);
				}
			}
			else {
				using transposedType = glm::mat<R, C, T>;
				std::vector<transposedType> transposedValues;
				if constexpr (std::is_same_v<T, bool>) {
					using type = glm::mat<R, C, unsigned int>;
					transposedValues = _GetArray<type, transposedType>(name, [&](const std::string& name) -> std::vector<type> {
						return _GetArray<type>(name);
					});
				}
				else {
					transposedValues = _GetArray<transposedType>(name);
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
		MV Get(const std::string& name) const {
			using M = typename MV::value_type;
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (column_major) {
				if constexpr (std::is_same_v<T, bool>) {
					using type = glm::mat<C, R, unsigned int>;
					return _GetArray<type, M>(name, [&](const std::string& name) -> std::vector<type> {
						return _GetArray<type>(name);
						});
				}
				else {
					return _GetArray<M>(name);
				}
			}
			else {
				using transposedType = glm::mat<R, C, T>;
				std::vector<transposedType> transposedValues;
				std::vector<M> values;
				if constexpr (std::is_same_v<T, bool>) {
					using type = glm::mat<R, C, unsigned int>;
					transposedValues = _GetArray<type, transposedType>(name, [&](const std::string& name) -> std::vector<type> {
						return _GetArray<type>(name);
						});
				}
				else {
					transposedValues = _GetArray<transposedType>(name);
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
		template<class S, utils::std_struct_enable_if_t<S, _Offsets, bool> = true>
#endif
		S Get(const std::string& name, const _Offsets& structOffsets) const {
			return _GetStruct(name, structOffsets);
		}

#pragma region GET_STRUCT_ARRAYS
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::std_struct<_Offsets> S>
#else
		template<class S, utils::std_struct_enable_if_t<S, _Offsets, bool> = true>
#endif
		void Get(const std::string& name, const _Offsets& structOffsets, S*& valueDest, size_t size) const {
			std::vector<STDStruct<_Offsets>> values = _GetStructArray(name, structOffsets);
			memcpy(valueDest, values.data(), std::min(values.size(), size));
			values.clear();
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::std_structs_vector<_Offsets> VS>
#else
		template<class VS, utils::std_structs_vector_enable_if_t<VS, _Offsets, bool> = true>
#endif
		VS Get(const std::string& name, const _Offsets& structTemplate) const {
			return _GetStructArray(name, structTemplate);
		}

#pragma endregion
#pragma endregion

		_Offsets GetOffsets() const {
			return _dataOffsets;
		}
		size_t GetOffset(const std::string& name) const {
			return _dataOffsets.Get(name);
		}
		std::vector<size_t> GetArrayOffsets(const std::string& name) const {
			return _dataOffsets.GetArray(name);
		}

		const ValueType* GetType(const std::string& name) const {
			return _dataOffsets.GetType(name);
		}

		std::vector<std::string> GetNames() const {
			return _dataOffsets.GetNames();
		}

		std::vector<unsigned char> GetData() const {
			return _data;
		}

		size_t GetBaseAligement() const {
			return _dataOffsets.GetBaseAligement();
		}
		size_t GetSize() const {
			return _data.size();
		}

		void ClearData() {
			memset(_data.data(), 0, _data.size());
		}
		void Clear() {
			_dataOffsets.Clear();
			_data.clear();
		}
	};

	using STD140Struct = STDStruct<STD140Offsets>;
	using STD430Struct = STDStruct<STD430Offsets>;
}