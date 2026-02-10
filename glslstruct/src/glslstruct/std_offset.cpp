#include <pch.hpp>
#include <glslstruct/std_offset.hpp>

using namespace glslstruct;

std::string std_offset::_get_array_elem_name(const std::string& arrayName, size_t elemIdx) {
	static _GLSL_STRUCT_CONSTEXPR17 const char* const strFormat = "{}[{}]";
	return fmt::vformat(strFormat, fmt::make_format_args(arrayName, elemIdx));
}
std::string std_offset::_get_struct_elem_name(const std::string& structName, const std::string& elemName) {
	static _GLSL_STRUCT_CONSTEXPR17 const char* const strFormat = "{}.{}";
	return fmt::vformat(strFormat, fmt::make_format_args(structName, elemName));
}

size_t std_offset::_ceil_to_nearest_multiple(size_t valueToRoundUp, size_t multipleValue) noexcept {
	size_t modulo = valueToRoundUp % multipleValue;
	if (modulo != 0) {
		valueToRoundUp += multipleValue - (modulo);
	}
	return valueToRoundUp;
}

void std_offset::_set_variable(const std::string& name, size_t offset, const base_type_handle& type, size_t size, size_t padding) {
	_values.emplace(name, value_data(offset, type, size, padding));
}
void std_offset::_remove_variable(const std::string& name) {
	if (!contains(name)) {
		return;
	}

	_values.erase(name);
}

void std_offset::_update_max_aligement(size_t baseAligement) noexcept {
	if (baseAligement > _maxAligement) {
		_maxAligement = baseAligement;
	}
}

size_t std_offset::_get_array_elem_base_aligement(size_t baseAligement) const noexcept {
	return _ceil_to_nearest_multiple(baseAligement, 16);
}

size_t std_offset::_add(const std::string& name, size_t baseAligement, size_t baseOffset, const base_type_handle& type) {
	// CHECK VARIABLE
	if (contains(name)) {
		return bad_offset();
	}

	// CALCULATE ALIGEMENT
	const size_t aligementOffset = _ceil_to_nearest_multiple(_currentOffset, baseAligement);

	// CHECK IF NOT OVERFLOWED
	if (aligementOffset < _currentOffset) {
		return bad_offset();
	}

	// UPDATE SIZE
	if (mstd::add_overflow(aligementOffset, baseOffset, _currentOffset)) {
		return bad_offset();
	}

	// SET VARIABLE
	_set_variable(name, aligementOffset, type, baseOffset, 0);

	// UPDATE MAX ALIGEMENT
	_update_max_aligement(baseAligement);

	return aligementOffset;
}

std::vector<size_t> std_offset::_add_array(const std::string& name, size_t arraySize, size_t baseAligement, 
	size_t baseOffset, const base_type_handle& type) {
	// CHECK SIZE
	if (arraySize == 0) {
		return std::vector<size_t>();
	}

	// CHECK VARIABLE
	if (contains(name)) {
		return std::vector<size_t>();
	}

	// SET BASE ALIGEMENT
	size_t valuePadding = baseAligement;
	baseAligement = _get_array_elem_base_aligement(baseAligement);
	valuePadding = baseAligement - valuePadding;

	// CALCULATE ALIGEMENT OFFSET
	size_t aligementOffset = _ceil_to_nearest_multiple(_currentOffset, baseAligement);

	// CHECK OVERFLOW
	if (aligementOffset < _currentOffset) {
		return std::vector<size_t>();
	}

	// UPDATE SIZE
	size_t arrayTotalAligement = 0;
	if (mstd::mul_overflow(baseAligement, arraySize, arrayTotalAligement) || 
		mstd::add_overflow(aligementOffset, arrayTotalAligement, _currentOffset)) {
		return std::vector<size_t>();
	}

	// SET ARRAY BEGIN POINTER
	_set_variable(name, aligementOffset, std::make_shared<array_type>(type, arraySize), 
		arrayTotalAligement, 0);

	// ADD ARRAY VALUES
	std::vector<size_t> arrayElemOffsets(arraySize);
	for (size_t i = 0; i < arraySize; ++i) {
		// SET ELEMENT VALUE OFFSET
		arrayElemOffsets[i] = aligementOffset;

		// SET ELEMENT VARIABLE
		_set_variable(_get_array_elem_name(name, i), aligementOffset, type, baseOffset, valuePadding);

		// ADD BASE ALIGEMENT
		aligementOffset += baseAligement;
	}

	// UPDATE MAX ALIGEMENT
	_update_max_aligement(baseAligement);

	return arrayElemOffsets;
}

size_t std_offset::_add_scalar(const std::string& name, size_t baseAligement, size_t baseOffset, ValueType type) {
	return _add(name, baseAligement, baseOffset, std::make_shared<scalar_type>(type));
}

std::vector<size_t> std_offset::_add_scalar_array(const std::string& name, size_t baseAligement, size_t baseOffset, 
	ValueType type, size_t arraySize) {
	return _add_array(name, arraySize, baseAligement, baseOffset, std::make_shared<scalar_type>(type));
}

size_t std_offset::_add_vec(const std::string& name, size_t length, size_t baseAligement, size_t baseOffset, 
	ValueType type) {
	if (length < 1 || length > 4) {
		return bad_offset();
	}

	const size_t vecSize = length * baseOffset;

	if (length != 3) {
		return _add(name, length * baseAligement, vecSize, 
			std::make_shared<vec_type>(type, length));
	}
	else {
		return _add(name, (length + 1) * baseAligement, vecSize, 
			std::make_shared<vec_type>(type, length));
	}
}

std::vector<size_t> std_offset::_add_vec_array(const std::string& name, size_t length, size_t baseAligement, 
	size_t baseOffset, ValueType type, size_t arraySize) {
	if (length < 1 || length > 4) {
		return std::vector<size_t>();
	}

	const size_t vecSize = length * baseOffset;

	if (length != 3) {
		return _add_array(name, arraySize, length * baseAligement, vecSize, 
			std::make_shared<vec_type>(type, length));
	}
	else {
		return _add_array(name, arraySize, (length + 1) * baseAligement, vecSize, 
			std::make_shared<vec_type>(type, length));
	}
}

size_t std_offset::_add_mat(const std::string& name, size_t columns, size_t rows, bool column_major, 
	size_t baseAligement, size_t baseOffset, ValueType type) {
	const size_t arraySize = column_major ? columns : rows;
	const size_t vecSize = column_major ? rows : columns;

	// PRE CHECK IF MAT WILL FIT
	size_t aligement = _get_array_elem_base_aligement(
		(vecSize == 3) ? ((vecSize + 1) * baseAligement) : (vecSize * baseAligement));

	size_t matAligementOffset = _ceil_to_nearest_multiple(_currentOffset, aligement);

	if (mstd::mul_overflow(aligement, arraySize, aligement) ||
		mstd::add_overflow(matAligementOffset, aligement, matAligementOffset)) {
		return bad_offset();
	}

	// ADD VEC ARRAY
	std::vector<size_t> vecsOffsets = _add_vec_array(name, vecSize, baseAligement, 
		baseOffset, type, arraySize);

	// CHAGE ARRAY TYPE TO MAT TYPE
	value_data& varData = _values.at(name);

	varData.change_type(std::make_shared<mat_type>(type, columns, 
			rows, column_major));

	return varData.get_offset();
}

std::vector<size_t> std_offset::_add_mat_array(const std::string& name, size_t columns, size_t rows, bool column_major, 
	size_t baseAligement, size_t baseOffset, ValueType type, size_t arraySize) {
	// CHECK ARRAY SIZE
	if (arraySize == 0) {
		return std::vector<size_t>();
	}

	// CHECK VARIABLE
	if (contains(name)) {
		return std::vector<size_t>();
	}

	// PRE CHECK IF MATS ARRAY WILL FIT
	const size_t vecArraySize = column_major ? columns : rows;
	const size_t vecSize = column_major ? rows : columns;

	size_t matAligement = _get_array_elem_base_aligement(
		(vecSize == 3) ? (vecSize + 1) * baseAligement : vecSize * baseAligement);

	size_t matAligementOffset = _ceil_to_nearest_multiple(_currentOffset, matAligement);

	size_t aligement = 0;
	if (mstd::mul_overflow(matAligement, vecArraySize, matAligement) ||
		mstd::mul_overflow(matAligement, arraySize, aligement) ||
		mstd::add_overflow(matAligementOffset, aligement, matAligementOffset)) {
		return std::vector<size_t>();
	}

	// ADD ARRAY ELEMENTS
	std::vector<size_t> values(arraySize);
	for (size_t i = 0; i != arraySize; ++i) {
		values[i] = _add_mat(_get_array_elem_name(name, i),
			columns, rows, column_major, baseAligement, baseOffset, type);
	}

	// SET ARRAY BEGIN POINTER
	_set_variable(name, values[0], std::make_shared<array_type>(type, columns, 
		rows, column_major, arraySize), aligement, 0);

	// RETURN
	return values;
}

size_t std_offset::_add_struct(const std::string& name, size_t baseAligement, size_t baseOffset, 
	const std::unordered_map<std::string, value_data>& values) {
	size_t aligementOffset = _add(name, baseAligement, baseOffset, 
		std::make_shared<struct_type>(values, baseOffset));

	// CHECK RESULT
	if (aligementOffset == bad_offset()) {
		return bad_offset();
	}

	for (const auto& [value_name, data] : values) {
		_set_variable(_get_struct_elem_name(name, value_name), 
			aligementOffset + data.get_offset(), data.get_type(), data.get_size(), data.get_padding());
	}

	// ADD PADDING
	size_t oldCurrentOffset = _currentOffset;
	_currentOffset = _ceil_to_nearest_multiple(_currentOffset, 16);

	_values.at(name).set_padding(_currentOffset - oldCurrentOffset);

	return aligementOffset;
}

std::vector<size_t> std_offset::_add_struct_array(const std::string& name, size_t baseAligement, size_t baseOffset, 
	const std::unordered_map<std::string, value_data>& values, size_t arraySize) {
	// CHECK ARRAY SIZE
	if (arraySize == 0) {
		return std::vector<size_t>();
	}

	// CHECK NAME
	if (contains(name)) {
		return std::vector<size_t>();
	}

	// CHECK IF STRUCTS ARRAY WILL FIT
	size_t aligementOffset = _ceil_to_nearest_multiple(_currentOffset, baseAligement);

	size_t structEndOffset = 0;
	if (mstd::add_overflow(aligementOffset, baseOffset, structEndOffset)) {
		return std::vector<size_t>();
	}

	structEndOffset = _ceil_to_nearest_multiple(structEndOffset, 16);

	size_t structTotalSize = structEndOffset - aligementOffset;
	size_t arrayTotalSize = 0;
	if (mstd::mul_overflow(structTotalSize, arraySize, arrayTotalSize) ||
		mstd::add_overflow(aligementOffset, arrayTotalSize, aligementOffset)) {
		return std::vector<size_t>();
	}

	std::vector<size_t> valuesOffsets(arraySize);
	for (size_t i = 0; i < arraySize; ++i) {
		valuesOffsets[i] = _add_struct(_get_array_elem_name(name, i), 
			baseAligement, baseOffset, values);
	}

	// SET ARRAY BEGIN POINTER
	_set_variable(name, valuesOffsets[0], 
		std::make_shared<array_type>(values, baseOffset, arraySize), 
		arrayTotalSize, 0);

	// RETURN
	return valuesOffsets;
}

std_offset::std_offset(const std_offset& other) noexcept 
	: _values(other._values), _currentOffset(other._currentOffset), 
	_maxAligement(other._maxAligement) {}
std_offset::std_offset(std_offset&& other) noexcept 
	: _values(std::exchange(other._values, {})), _currentOffset(other._currentOffset), 
	_maxAligement(other._maxAligement) {}

std_offset& std_offset::operator=(const std_offset& other) noexcept {
	_values = other._values;
	_currentOffset = other._currentOffset;
	_maxAligement = other._maxAligement;
	return *this;
}
std_offset& std_offset::operator=(std_offset&& other) noexcept {
	_values = std::exchange(other._values, {});
	_currentOffset = other._currentOffset;
	_maxAligement = other._maxAligement;
	return *this;
}

size_t std_offset::bad_offset() noexcept {
	return ~static_cast<size_t>(0);
}

bool std_offset::contains(const std::string& name) const noexcept {
	return _values.find(name) != _values.end();
}

value_data std_offset::get(const std::string& name) const noexcept {
	if (!contains(name)) {
		return value_data(bad_offset(), nullptr, 0, 0);
	}

	return _values.at(name);
}

size_t std_offset::get_offset(const std::string& name) const noexcept {
	return get(name).get_offset();
}

std::vector<size_t> std_offset::get_array_offsets(const std::string& name) const noexcept {
	if (!contains(name)) {
		return std::vector<size_t>();
	}

	const base_type_handle& varType = _values.at(name).get_type();

	size_t arraySize;
	if (is_of_type<mat_type>(varType)) {
		arraySize = static_type_cast<mat_type>(varType)->get_array_count();
	}
	else if (is_of_type<array_type>(varType)) {
		arraySize = static_type_cast<array_type>(varType)->get_count();
	}
	else {
		return std::vector<size_t>({ _values.at(name).get_offset() });
	}

	std::vector<size_t> values(arraySize);
	for (size_t i = 0; i != arraySize; ++i) {
		values[i] = _values.at(
_get_array_elem_name(name, i))
			.get_offset();
	}

	return values;
}

base_type_handle std_offset::get_type(const std::string& name) const noexcept {
	return get(name).get_type();
}
size_t std_offset::get_total_size(const std::string& name) const noexcept {
	return get(name).get_total_size();
}
size_t std_offset::get_size(const std::string& name) const noexcept {
	return get(name).get_size();
}
size_t std_offset::get_padding(const std::string& name) const noexcept {
	return get(name).get_padding();
}

std::vector<std::string> std_offset::get_names() const noexcept {
	std::vector<std::string> names;
	names.reserve(_values.size());

	for (const auto& [name, data] : _values) {
		names.push_back(name);
	}

	return names;
}

size_t std_offset::base_aligement() const noexcept {
	return _ceil_to_nearest_multiple(_maxAligement, 16);
}

size_t std_offset::size() const noexcept {
	return _ceil_to_nearest_multiple(_currentOffset, 16);
}

size_t std_offset::padding() const noexcept {
	return size() - _currentOffset;
}

void std_offset::clear() noexcept {
	_currentOffset = 0;
	_maxAligement = 0;
	_values.clear();
}

bool std_offset::operator==(const std_offset& other) const noexcept {
	return _currentOffset == other._currentOffset &&
		_maxAligement == other._maxAligement &&
		_values == other._values;
}
#if !_GLSL_STRUCT_HAS_CXX20
bool std_offset::operator!=(const std_offset& other) const noexcept {
	return !(*this == other);
}
#endif

size_t std::hash<std_offset>::operator()(const std_offset& stdOff) {
	size_t seed = mstd::hash_combine(stdOff._currentOffset, stdOff._maxAligement);
	for (const auto& [name, data] : stdOff._values) {
		mstd::hash_append(seed, name, data);
	}
	return seed;
}