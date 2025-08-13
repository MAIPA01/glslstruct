#include "pch.hpp"
#include <STDOffsets.hpp>

using namespace glsl;
using namespace std;

hash<string> STDOffsets::_hasher;

const char* const STDOffsets::_arrayElemFormat = "{0}[{1}]";
const char* const STDOffsets::_subElemFormat = "{0}.{1}";

bool STDOffsets::_CheckVariable(const string& name) const
{
	bool c = move(Contains(name));
	if (c) {
		//SPDLOG_ERROR("Variable '{0}' already added to structure", name);
	}
	return c;
}

void STDOffsets::_SetVariable(const string& name, size_t offset, const ValueType* type)
{
	size_t nameHash = move(_hasher(name));
	_offsets[nameHash] = offset;
	_names[nameHash] = name;
	_types[nameHash] = type;
}

size_t STDOffsets::_Add(const string& name, size_t baseAligement, size_t baseOffset, const ValueType* type)
{
	// CHECK VARIABLE
	if (_CheckVariable(name)) {
		return 0;
	}

	// CALCULATE ALIGEMENT
	size_t aligementOffset = move(_currentOffset);
	if (aligementOffset % baseAligement != 0) {
		aligementOffset += baseAligement - (aligementOffset % baseAligement);
	}

	// SET VARIABLE
	_SetVariable(name, aligementOffset, type);

	// UPDATE SIZE
	_currentOffset = aligementOffset + baseOffset;

	// CHECK MAX ALIGEMENT
	if (baseAligement > _maxAligement) {
		_maxAligement = move(baseAligement);
	}
	return aligementOffset;
}

vector<size_t> STDOffsets::_AddArray(const string& name, size_t arraySize, size_t baseAligement, size_t baseOffset, const ValueType* type)
{
	// CHECK SIZE
	if (arraySize == 0) {
		return vector<size_t>();
	}

	// CHECK VARIABLE
	if (_CheckVariable(name)) {
		return vector<size_t>();
	}

	vector<size_t> arrayElemOffsets(arraySize);

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
	string valueName;
	size_t valueAligementOffset;
	for (size_t i = 0; i < arraySize; ++i) {
		// ELEMENT VALUE NAME
		valueName = move(fmt::vformat(_arrayElemFormat, fmt::make_format_args(name, i)));

		// CALCULATE VALUE OFFSET
		valueAligementOffset = aligementOffset + i * baseAligement;

		// SET ELEMENT VALUE OFFSET
		arrayElemOffsets[i] = valueAligementOffset;

		// SET ELEMENT VARIABLE
		_SetVariable(valueName, valueAligementOffset, type->Clone());
	}

	// UPDATE SIZE
	_currentOffset = aligementOffset + arraySize * baseAligement;

	// SET ARRAY BEGIN POINTER
	_SetVariable(name, arrayElemOffsets[0], new ArrayType(type, arraySize));

	return arrayElemOffsets;
}

size_t STDOffsets::_AddScalar(const string& name, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type)
{
	return _Add(name, baseAligement, baseOffset, new ScalarType(type));
}

vector<size_t> STDOffsets::_AddScalarArray(const string& name, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type, size_t arraySize)
{
	return _AddArray(name, arraySize, baseAligement, baseOffset, new ScalarType(type));
}

size_t STDOffsets::_AddVector(const string& name, size_t length, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type)
{
	if (length < 2 || length > 4) {
		return 0;
	}

	if (length != 3) {
		return _Add(name, length * baseAligement, length * baseOffset, new VecType(type, length));
	}
	else {
		return _Add(name, (length + 1) * baseAligement, length * baseOffset, new VecType(type, length));
	}
}

vector<size_t> STDOffsets::_AddVectorArray(const string& name, size_t length, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type, size_t arraySize)
{
	if (length < 2 || length > 4) {
		return vector<size_t>();
	}

	if (length != 3) {
		return _AddArray(name, arraySize, length * baseAligement, length * baseOffset, new VecType(type, length));
	}
	else {
		return _AddArray(name, arraySize, (length + 1) * baseAligement, length * baseOffset, new VecType(type, length));
	}
}

size_t STDOffsets::_AddMatrix(const string& name, size_t columns, size_t rows, bool column_major, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type)
{
	const size_t arraySize = column_major ? columns : rows;
	const size_t vecSize = column_major ? rows : columns;

	size_t offset = _AddVectorArray(name, vecSize, baseAligement, baseOffset, type, arraySize)[0];
	size_t nameHash = move(_hasher(name));
	delete _types[nameHash];
	_types[nameHash] = new MatType(type, columns, rows);
	return offset;
}

vector<size_t> STDOffsets::_AddMatrixArray(const string& name, size_t columns, size_t rows, bool column_major, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type, size_t arraySize)
{
	// CHECK ARRAY SIZE
	if (arraySize == 0) {
		return vector<size_t>();
	}

	// CHECK VARIABLE
	if (_CheckVariable(name)) {
		return vector<size_t>();
	}

	// ADD ARRAY ELEMENTS
	vector<size_t> values(arraySize);
	for (size_t i = 0; i < arraySize; ++i) {
		string valueName = fmt::vformat(_arrayElemFormat, fmt::make_format_args(name, i));
		values[i] = move(_AddMatrix(valueName, columns, rows, column_major, baseAligement, baseOffset, type));
	}

	// SET ARRAY BEGIN POINTER
	_SetVariable(name, values[0], new ArrayType(new MatType(type, columns, rows), arraySize));

	// RETURN
	return values;
}

size_t STDOffsets::_AddStruct(const string& name, size_t baseAligement, size_t baseOffset, const offsets_map& offsets, const names_map& names, const types_map& types)
{
	size_t lastOffset = _currentOffset;
	size_t aligementOffset = move(_Add(name, baseAligement, baseOffset, new StructType(offsets, names, types)));

	if (aligementOffset == 0 && lastOffset != 0) {
		return 0;
	}

	string valueName;
	for (const auto& off : offsets) {
		valueName = move(fmt::vformat(_subElemFormat, fmt::make_format_args(name, (*names.find(off.first)).second)));

		_SetVariable(valueName, aligementOffset + off.second, (*types.find(off.first)).second->Clone());
	}

	// ADD PADDING
	if (_currentOffset % 16 != 0) {
		_currentOffset += 16 - (_currentOffset % 16);
	}
	return aligementOffset;
}

vector<size_t> STDOffsets::_AddStructArray(const string& name, size_t baseAligement, size_t baseOffset, const offsets_map& offsets, const names_map& names, const types_map& types, size_t arraySize)
{
	if (arraySize == 0) {
		return vector<size_t>();
	}

	if (_CheckVariable(name)) {
		return vector<size_t>();
	}

	vector<size_t> values(arraySize);
	string arrayElemName;

	for (size_t i = 0; i < arraySize; ++i) {
		arrayElemName = move(fmt::vformat(_arrayElemFormat, fmt::make_format_args(name, i)));
		values[i] = move(_AddStruct(arrayElemName, baseAligement, baseOffset, offsets, names, types));
	}

	// SET ARRAY BEGIN POINTER
	_SetVariable(name, values[0], new ArrayType(new StructType(offsets, names, types), arraySize));

	// RETURN
	return values;
}

STDOffsets::STDOffsets(STDOffsets& stdOff)
{
	stdOff.CloneTo(this);
}

STDOffsets::STDOffsets(const STDOffsets& stdOff)
{
	stdOff.CloneTo(this);
}

STDOffsets::STDOffsets(STDOffsets&& stdOff)
{
	stdOff.CloneTo(this);
}

STDOffsets::~STDOffsets()
{
	Clear();
}

STDOffsets& STDOffsets::operator=(STDOffsets& stdOff)
{
	stdOff.CloneTo(this);
	return *this;
}

STDOffsets& STDOffsets::operator=(const STDOffsets& stdOff)
{
	stdOff.CloneTo(this);
	return *this;
}

STDOffsets& STDOffsets::operator=(STDOffsets&& stdOff)
{
	stdOff.CloneTo(this);
	return *this;
}

CLONE_FUNC_DEFINITION_ADVANCED(STDOffsets,
	STANDARD_CLONE(_currentOffset),
	STANDARD_CLONE(_maxAligement),
	STANDARD_CLONE(_offsets),
	STANDARD_CLONE(_names); for (const auto& type : _types) { cloned->_types[type.first] = type.second->Clone(); }
)

bool STDOffsets::Contains(const string& name) const
{
	return _offsets.find(move(_hasher(name))) != _offsets.end();
}

size_t STDOffsets::Get(const string& name) const
{
	size_t value = 0;
	offsets_map::const_iterator map_iterator = _offsets.find(move(_hasher(name)));
	if (map_iterator != _offsets.end()) {
		value = (*map_iterator).second;
	}
	return value;
}

vector<size_t> STDOffsets::GetArray(const string& name) const
{
	vector<size_t> values;

	size_t i = 0;
	offsets_map::const_iterator map_iterator = _offsets.find(move(_hasher(move(fmt::vformat(_arrayElemFormat, fmt::make_format_args(name, i))))));
	++i;

	while (map_iterator != _offsets.end()) {
		values.push_back((*map_iterator).second);

		map_iterator = _offsets.find(move(_hasher(move(fmt::vformat(_arrayElemFormat, fmt::make_format_args(name, i))))));
		++i;
	}

	return values;
}

const ValueType* STDOffsets::GetType(const string& name) const
{
	const ValueType* value = nullptr;
	types_map::const_iterator map_iterator = _types.find(move(_hasher(name)));
	if (map_iterator != _types.end()) {
		value = (*map_iterator).second;
	}
	return value;
}

vector<string> STDOffsets::GetNames() const
{
	vector<string> names;
	names.reserve(_names.size());

	for (const auto& name : _names) {
		names.push_back(name.second);
	}

	return names;
}

size_t STDOffsets::GetBaseAligement() const
{
	size_t baseAligement = _maxAligement;
	if (_maxAligement % 16 != 0) {
		baseAligement += 16 - (baseAligement % 16);
	}
	return baseAligement;
}

size_t STDOffsets::GetSize() const
{
	size_t size = _currentOffset;
	if (size % 16 != 0) {
		size += 16 - (size % 16);
	}
	return size;
}

void STDOffsets::Clear()
{
	_currentOffset = 0;
	_maxAligement = 0;

	_offsets.clear();
	_names.clear();
	for (auto& type : _types) {
		if (type.second == nullptr) continue;
		delete type.second;
		type.second = nullptr;
	}
	_types.clear();
}