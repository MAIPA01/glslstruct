#include <pch.hpp>
#include <glslstruct/std_offset.hpp>

using namespace glslstruct;

void std_offset::_setVariable(const std::string& name, size_t offset, const base_type* type) {
	_values[name] = { offset, type };
}

[[nodiscard]] size_t std_offset::_add(const std::string& name, size_t baseAligement, size_t baseOffset, const base_type* type) {
	// CHECK VARIABLE
	if (contains(name)) {
		return 0;
	}

	// CALCULATE ALIGEMENT
	size_t aligementOffset = std::move(_currentOffset);
	if (aligementOffset % baseAligement != 0) {
		aligementOffset += baseAligement - (aligementOffset % baseAligement);
	}

	// SET VARIABLE
	_setVariable(name, aligementOffset, type);

	// UPDATE SIZE
	_currentOffset = aligementOffset + baseOffset;

	// CHECK MAX ALIGEMENT
	if (baseAligement > _maxAligement) {
		_maxAligement = std::move(baseAligement);
	}
	return aligementOffset;
}

[[nodiscard]] std::vector<size_t> std_offset::_addArray(const std::string& name, size_t arraySize, size_t baseAligement, 
	size_t baseOffset, const base_type* type) {
	// CHECK SIZE
	if (arraySize == 0) {
		return std::vector<size_t>();
	}

	// CHECK VARIABLE
	if (contains(name)) {
		return std::vector<size_t>();
	}

	std::vector<size_t> arrayElemOffsets(arraySize);

	// SET BASE ALIGEMENT
	if (baseAligement % 16 != 0) {
		baseAligement += 16 - (baseAligement % 16);
	}

	// CHECK MAX ALIGEMENT
	if (baseAligement > _maxAligement) {
		_maxAligement = baseAligement;
	}

	// CALCULATE ALIGEMENT OFFSET
	size_t aligementOffset = _currentOffset;
	if (aligementOffset % baseAligement != 0) {
		aligementOffset += baseAligement - (aligementOffset % baseAligement);
	}

	// ADD ARRAY VALUES
	std::string valueName;
	size_t valueAligementOffset;
	for (size_t i = 0; i < arraySize; ++i) {
		// ELEMENT VALUE NAME
		valueName = std::move(fmt::vformat(_arrayElemFormat, fmt::make_format_args(name, i)));

		// CALCULATE VALUE OFFSET
		valueAligementOffset = aligementOffset + i * baseAligement;

		// SET ELEMENT VALUE OFFSET
		arrayElemOffsets[i] = valueAligementOffset;

		// SET ELEMENT VARIABLE
		_setVariable(valueName, valueAligementOffset, type->clone());
	}

	// UPDATE SIZE
	_currentOffset = aligementOffset + arraySize * baseAligement;

	// SET ARRAY BEGIN POINTER
	_setVariable(name, arrayElemOffsets[0], new array_type(type, arraySize));

	return arrayElemOffsets;
}

[[nodiscard]] size_t std_offset::_addScalar(const std::string& name, size_t baseAligement, size_t baseOffset, const ValueType& type) {
	return _add(name, baseAligement, baseOffset, new scalar_type(type));
}

[[nodiscard]] std::vector<size_t> std_offset::_addScalarArray(const std::string& name, size_t baseAligement, size_t baseOffset, 
	const ValueType& type, size_t arraySize) {
	return _addArray(name, arraySize, baseAligement, baseOffset, new scalar_type(type));
}

[[nodiscard]] size_t std_offset::_addVector(const std::string& name, size_t length, size_t baseAligement, size_t baseOffset, 
	const ValueType& type) {
	if (length < 2 || length > 4) {
		return 0;
	}

	if (length != 3) {
		return _add(name, length * baseAligement, length * baseOffset, new vec_type(type, length));
	}
	else {
		return _add(name, (length + 1) * baseAligement, length * baseOffset, new vec_type(type, length));
	}
}

[[nodiscard]] std::vector<size_t> std_offset::_addVectorArray(const std::string& name, size_t length, size_t baseAligement, 
	size_t baseOffset, const ValueType& type, size_t arraySize) {
	if (length < 2 || length > 4) {
		return std::vector<size_t>();
	}

	if (length != 3) {
		return _addArray(name, arraySize, length * baseAligement, length * baseOffset, new vec_type(type, length));
	}
	else {
		return _addArray(name, arraySize, (length + 1) * baseAligement, length * baseOffset, new vec_type(type, length));
	}
}

[[nodiscard]] size_t std_offset::_addMatrix(const std::string& name, size_t columns, size_t rows, bool column_major, 
	size_t baseAligement, size_t baseOffset, const ValueType& type) {
	const size_t arraySize = column_major ? columns : rows;
	const size_t vecSize = column_major ? rows : columns;

	size_t offset = _addVectorArray(name, vecSize, baseAligement, baseOffset, type, arraySize)[0];
	delete _values[name].type;
	_values[name].type = new mat_type(type, columns, rows);
	return offset;
}

[[nodiscard]] std::vector<size_t> std_offset::_addMatrixArray(const std::string& name, size_t columns, size_t rows, bool column_major, 
	size_t baseAligement, size_t baseOffset, const ValueType& type, size_t arraySize) {
	// CHECK ARRAY SIZE
	if (arraySize == 0) {
		return std::vector<size_t>();
	}

	// CHECK VARIABLE
	if (contains(name)) {
		return std::vector<size_t>();
	}

	// ADD ARRAY ELEMENTS
	std::vector<size_t> values(arraySize);
	for (size_t i = 0; i < arraySize; ++i) {
		std::string valueName = fmt::vformat(_arrayElemFormat, fmt::make_format_args(name, i));
		values[i] = std::move(_addMatrix(valueName, columns, rows, column_major, baseAligement, baseOffset, type));
	}

	// SET ARRAY BEGIN POINTER
	_setVariable(name, values[0], new array_type(new mat_type(type, columns, rows), arraySize));

	// RETURN
	return values;
}

[[nodiscard]] size_t std_offset::_addStruct(const std::string& name, size_t baseAligement, size_t baseOffset, const values_map& values) {
	size_t lastOffset = _currentOffset;
	size_t aligementOffset = std::move(_add(name, baseAligement, baseOffset, new struct_type(values)));

	if (aligementOffset == 0 && lastOffset != 0) {
		return 0;
	}

	std::string valueName;
	for (const auto& [value_name, data] : values) {
		valueName = std::move(fmt::vformat(_subElemFormat, fmt::make_format_args(name, value_name)));

		_setVariable(valueName, aligementOffset + data.offset, data.type->clone());
	}

	// ADD PADDING
	if (_currentOffset % 16 != 0) {
		_currentOffset += 16 - (_currentOffset % 16);
	}
	return aligementOffset;
}

[[nodiscard]] std::vector<size_t> std_offset::_addStructArray(const std::string& name, size_t baseAligement, size_t baseOffset, 
	const values_map& values, size_t arraySize) {
	if (arraySize == 0) {
		return std::vector<size_t>();
	}

	if (contains(name)) {
		return std::vector<size_t>();
	}

	std::vector<size_t> values_offsets(arraySize, 0);
	std::string arrayElemName;

	for (size_t i = 0; i < arraySize; ++i) {
		arrayElemName = std::move(fmt::vformat(_arrayElemFormat, fmt::make_format_args(name, i)));
		values_offsets[i] = std::move(_addStruct(arrayElemName, baseAligement, baseOffset, values));
	}

	// SET ARRAY BEGIN POINTER
	_setVariable(name, values_offsets[0], new array_type(new struct_type(values), arraySize));

	// RETURN
	return values_offsets;
}

void std_offset::_cloneFrom(const std_offset& stdOff) noexcept {
	_currentOffset = stdOff._currentOffset;
	_maxAligement = stdOff._maxAligement;
	for (const auto& [name, data] : stdOff._values) {
		_values[name] = { data.offset, data.type->clone() };
	}
}

std_offset::std_offset(std_offset& stdOff) {
	_cloneFrom(stdOff);
}

std_offset::std_offset(const std_offset& stdOff) {
	_cloneFrom(stdOff);
}

std_offset::std_offset(std_offset&& stdOff) {
	_cloneFrom(stdOff);
}

std_offset::~std_offset() {
	clear();
}

std_offset& std_offset::operator=(std_offset& stdOff) {
	_cloneFrom(stdOff);
	return *this;
}

std_offset& std_offset::operator=(const std_offset& stdOff) {
	_cloneFrom(stdOff);
	return *this;
}

std_offset& std_offset::operator=(std_offset&& stdOff) {
	_cloneFrom(stdOff);
	return *this;
}

[[nodiscard]] std_offset* std_offset::clone() const noexcept {
	return new std_offset(*this);
}

[[nodiscard]] bool std_offset::contains(const std::string& name) const {
	return _values.find(std::move(name)) != _values.end();
}

[[nodiscard]] size_t std_offset::get(const std::string& name) const {
	size_t value = 0;
	auto map_iterator = _values.find(std::move(name));
	if (map_iterator != _values.end()) {
		value = map_iterator->second.offset;
	}
	return value;
}

[[nodiscard]] std::vector<size_t> std_offset::getArray(const std::string& name) const {
	std::vector<size_t> values;

	size_t i = 0;
	auto map_iterator = _values.find(std::move(fmt::vformat(_arrayElemFormat, fmt::make_format_args(name, i))));
	++i;

	while (map_iterator != _values.end()) {
		values.push_back(map_iterator->second.offset);

		map_iterator = _values.find(std::move(fmt::vformat(_arrayElemFormat, fmt::make_format_args(name, i))));
		++i;
	}

	return values;
}

[[nodiscard]] const base_type* std_offset::getType(const std::string& name) const {
	const base_type* value = nullptr;
	auto map_iterator = _values.find(std::move(name));
	if (map_iterator != _values.end()) {
		value = map_iterator->second.type;
	}
	return value;
}

[[nodiscard]] std::vector<std::string> std_offset::getNames() const {
	std::vector<std::string> names;
	names.reserve(_values.size());

	for (const auto& [name, data] : _values) {
		names.push_back(name);
	}

	return names;
}

[[nodiscard]] size_t std_offset::baseAligement() const
{
	size_t baseAligement = _maxAligement;
	if (_maxAligement % 16 != 0) {
		baseAligement += 16 - (baseAligement % 16);
	}
	return baseAligement;
}

[[nodiscard]] size_t std_offset::size() const
{
	size_t size = _currentOffset;
	if (size % 16 != 0) {
		size += 16 - (size % 16);
	}
	return size;
}

void std_offset::clear()
{
	_currentOffset = 0;
	_maxAligement = 0;

	for (const auto& value : _values) {
		if (value.second.type != nullptr) 
			delete value.second.type;
	}
	_values.clear();
}