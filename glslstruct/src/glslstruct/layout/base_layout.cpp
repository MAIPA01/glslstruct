/*
* glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#include <glslstruct/config.hpp>

#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
#else

#include <pch.hpp>
#include <glslstruct/layout/base_layout.hpp>

using namespace glslstruct;

std::string base_layout::_get_array_elem_name(const std::string_view& arrayName, size_t elemIdx) {
	return fmt::format("{}[{}]", arrayName, elemIdx);
}
std::string base_layout::_get_struct_elem_name(const std::string_view& structName, const std::string_view& elemName) {
	return fmt::format("{}.{}", structName, elemName);
}

size_t base_layout::_ceil_to_nearest_multiple(size_t valueToRoundUp, size_t multipleValue) noexcept {
	size_t modulo = valueToRoundUp % multipleValue;
	if (modulo != 0) {
		valueToRoundUp += multipleValue - modulo;
	}
	return valueToRoundUp;
}

void base_layout::_set_variable(const std::string_view& name, size_t alignmentOffset, const base_type_handle& type, size_t size, size_t padding) {
	_values.emplace(name, value_data(alignmentOffset, type, size, padding));
}
void base_layout::_remove_variable(const std::string_view& name) {
	if (!contains(name)) {
		return;
	}

	_values.erase(name.data());
}

void base_layout::_update_max_alignment(size_t baseAlignment) noexcept {
	_maxAlignment = std::max(baseAlignment, _maxAlignment);
}

size_t base_layout::_get_array_elem_base_alignment(size_t baseAlignment) const noexcept {
	static _GLSL_STRUCT_CONSTEXPR17 const size_t alignment_multiple = 16;
	return _ceil_to_nearest_multiple(baseAlignment, alignment_multiple);
}

size_t base_layout::_add(const std::string_view& name, size_t baseAlignment, size_t baseOffset, const base_type_handle& type) {
	// CHECK VARIABLE
	if (contains(name)) {
		return bad_offset();
	}

	// CALCULATE ALIGNMENT
	const size_t alignmentOffset = _ceil_to_nearest_multiple(_currentOffset, baseAlignment);

	// CHECK IF NOT OVERFLOWED
	if (alignmentOffset < _currentOffset) {
		return bad_offset();
	}

	// UPDATE SIZE
	if (mstd::add_overflow(alignmentOffset, baseOffset, _currentOffset)) {
		return bad_offset();
	}

	// SET VARIABLE
	_set_variable(name, alignmentOffset, type, baseOffset, 0);

	// UPDATE MAX ALIGEMENT
	_update_max_alignment(baseAlignment);

	return alignmentOffset;
}

std::vector<size_t> base_layout::_add_array(const std::string_view& name, size_t arraySize, size_t baseAlignment,
	size_t baseOffset, const base_type_handle& type) {
	// CHECK SIZE
	if (arraySize == 0) {
		return std::vector<size_t>();
	}

	// CHECK VARIABLE
	if (contains(name)) {
		return std::vector<size_t>();
	}

	// SET BASE ALIGNMENT
	size_t valuePadding = baseAlignment;
	baseAlignment = _get_array_elem_base_alignment(baseAlignment);
	valuePadding = baseAlignment - valuePadding;

	// CALCULATE ALIGNMENT OFFSET
	size_t alignmentOffset = _ceil_to_nearest_multiple(_currentOffset, baseAlignment);

	// CHECK OVERFLOW
	if (alignmentOffset < _currentOffset) {
		return std::vector<size_t>();
	}

	// UPDATE SIZE
	size_t arrayTotalAlignment = 0;
	if (mstd::mul_overflow(baseAlignment, arraySize, arrayTotalAlignment) ||
		mstd::add_overflow(alignmentOffset, arrayTotalAlignment, _currentOffset)) {
		return std::vector<size_t>();
	}

	// SET ARRAY BEGIN POINTER
	_set_variable(name, alignmentOffset, std::make_shared<array_type>(type, arraySize),
		arrayTotalAlignment, 0);

	// ADD ARRAY VALUES
	std::vector<size_t> arrayElemOffsets(arraySize);
	for (size_t i = 0; i < arraySize; ++i) {
		// SET ELEMENT VALUE OFFSET
		arrayElemOffsets[i] = alignmentOffset;

		// SET ELEMENT VARIABLE
		_set_variable(_get_array_elem_name(name, i), alignmentOffset, type, baseOffset, valuePadding);

		// ADD BASE ALIGNMENT
		alignmentOffset += baseAlignment;
	}

	// UPDATE MAX ALIGEMENT
	_update_max_alignment(baseAlignment);

	return arrayElemOffsets;
}

size_t base_layout::_add_scalar(const std::string_view& name, size_t baseAlignment, size_t baseOffset, ValueType type) {
	return _add(name, baseAlignment, baseOffset, std::make_shared<scalar_type>(type));
}

std::vector<size_t> base_layout::_add_scalar_array(const std::string_view& name, size_t baseAlignment, size_t baseOffset,
	ValueType type, size_t arraySize) {
	return _add_array(name, arraySize, baseAlignment, baseOffset, std::make_shared<scalar_type>(type));
}

size_t base_layout::_add_vec(const std::string_view& name, size_t length, size_t baseAlignment, size_t baseOffset,
	ValueType type) {
	if (length < 2 || length > 4) {
		return bad_offset();
	}

	const size_t vecSize = length * baseOffset;

	if (length != 3) {
		return _add(name, length * baseAlignment, vecSize,
			std::make_shared<vec_type>(type, length));
	}
	return _add(name, (length + 1) * baseAlignment, vecSize,
			std::make_shared<vec_type>(type, length));
}

std::vector<size_t> base_layout::_add_vec_array(const std::string_view& name, size_t length, size_t baseAlignment,
	size_t baseOffset, ValueType type, size_t arraySize) {
	if (length < 2 || length > 4) {
		return std::vector<size_t>();
	}

	const size_t vecSize = length * baseOffset;

	if (length != 3) {
		return _add_array(name, arraySize, length * baseAlignment, vecSize,
			std::make_shared<vec_type>(type, length));
	}
	return _add_array(name, arraySize, (length + 1) * baseAlignment, vecSize,
			std::make_shared<vec_type>(type, length));
}

size_t base_layout::_add_mat(const std::string_view& name, size_t columns, size_t rows, MajorType major,
	size_t baseAlignment, size_t baseOffset, ValueType type) {
	const size_t arraySize = major == MajorType::Column ? columns : rows;
	const size_t vecSize = major == MajorType::Column ? rows : columns;

	// PRE CHECK IF MAT WILL FIT
	size_t alignment = _get_array_elem_base_alignment(
		(vecSize == 3) ? ((vecSize + 1) * baseAlignment) : (vecSize * baseAlignment));

	size_t matAlignmentOffset = _ceil_to_nearest_multiple(_currentOffset, alignment);

	if (mstd::mul_overflow(alignment, arraySize, alignment) ||
		mstd::add_overflow(matAlignmentOffset, alignment, matAlignmentOffset)) {
		return bad_offset();
	}

	// ADD VEC ARRAY
	std::vector<size_t> vecsOffsets = _add_vec_array(name, vecSize, baseAlignment,
		baseOffset, type, arraySize);

	// CHANGE ARRAY TYPE TO MAT TYPE
	value_data& varData = _values.at(name.data());

	varData.change_type(std::make_shared<mat_type>(type, columns, 
			rows, major));

	return varData.get_offset();
}

std::vector<size_t> base_layout::_add_mat_array(const std::string_view& name, size_t columns, size_t rows, MajorType major,
	size_t baseAlignment, size_t baseOffset, ValueType type, size_t arraySize) {
	// CHECK ARRAY SIZE
	if (arraySize == 0) {
		return std::vector<size_t>();
	}

	// CHECK VARIABLE
	if (contains(name)) {
		return std::vector<size_t>();
	}

	// PRE CHECK IF MATS ARRAY WILL FIT
	const size_t vecArraySize = major == MajorType::Column ? columns : rows;
	const size_t vecSize = major == MajorType::Column ? rows : columns;

	size_t matAlignment = _get_array_elem_base_alignment(
		(vecSize == 3) ? (vecSize + 1) * baseAlignment : vecSize * baseAlignment);

	size_t matAlignmentOffset = _ceil_to_nearest_multiple(_currentOffset, matAlignment);

	size_t alignment = 0;
	if (mstd::mul_overflow(matAlignment, vecArraySize, matAlignment) ||
		mstd::mul_overflow(matAlignment, arraySize, alignment) ||
		mstd::add_overflow(matAlignmentOffset, alignment, matAlignmentOffset)) {
		return std::vector<size_t>();
	}

	// ADD ARRAY ELEMENTS
	std::vector<size_t> values(arraySize);
	for (size_t i = 0; i != arraySize; ++i) {
		values[i] = _add_mat(_get_array_elem_name(name, i),
			columns, rows, major, baseAlignment, baseOffset, type);
	}

	// SET ARRAY BEGIN POINTER
	_set_variable(name, values[0], std::make_shared<array_type>(type, columns, 
		rows, major, arraySize), alignment, 0);

	// RETURN
	return values;
}

size_t base_layout::_add_struct(const std::string_view& name, size_t baseAlignment, size_t baseOffset,
	const std::unordered_map<std::string, value_data>& values) {
	size_t alignmentOffset = _add(name, baseAlignment, baseOffset,
		std::make_shared<struct_type>(values, baseOffset));

	// CHECK RESULT
	if (alignmentOffset == bad_offset()) {
		return alignmentOffset;
	}

	for (const auto& [value_name, data] : values) {
		_set_variable(_get_struct_elem_name(name, value_name), 
			alignmentOffset + data.get_offset(), data.get_type(), data.get_size(), data.get_padding());
	}

	// ADD PADDING
	size_t oldCurrentOffset = _currentOffset;
	static _GLSL_STRUCT_CONSTEXPR17 const size_t offset_alignment = 16;
	_currentOffset = _ceil_to_nearest_multiple(_currentOffset, offset_alignment);

	_values.at(name.data()).set_padding(_currentOffset - oldCurrentOffset);

	return alignmentOffset;
}

std::vector<size_t> base_layout::_add_struct_array(const std::string_view& name, size_t baseAlignment, size_t baseOffset,
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
	size_t alignmentOffset = _ceil_to_nearest_multiple(_currentOffset, baseAlignment);

	size_t structEndOffset = 0;
	if (mstd::add_overflow(alignmentOffset, baseOffset, structEndOffset)) {
		return std::vector<size_t>();
	}

	static _GLSL_STRUCT_CONSTEXPR17 const size_t offset_alignment = 16;
	structEndOffset = _ceil_to_nearest_multiple(structEndOffset, offset_alignment);

	size_t structTotalSize = structEndOffset - alignmentOffset;
	size_t arrayTotalSize = 0;
	if (mstd::mul_overflow(structTotalSize, arraySize, arrayTotalSize) ||
		mstd::add_overflow(alignmentOffset, arrayTotalSize, alignmentOffset)) {
		return std::vector<size_t>();
	}

	std::vector<size_t> valuesOffsets(arraySize);
	for (size_t i = 0; i < arraySize; ++i) {
		valuesOffsets[i] = _add_struct(_get_array_elem_name(name, i), 
			baseAlignment, baseOffset, values);
	}

	// SET ARRAY BEGIN POINTER
	_set_variable(name, valuesOffsets[0], 
		std::make_shared<array_type>(values, baseOffset, arraySize), 
		arrayTotalSize, 0);

	// RETURN
	return valuesOffsets;
}

base_layout::base_layout(const base_layout& other) noexcept 
	: _values(other._values), _currentOffset(other._currentOffset), 
	_maxAlignment(other._maxAlignment) {}
base_layout::base_layout(base_layout&& other) noexcept 
	: _values(std::exchange(other._values, {})), _currentOffset(other._currentOffset), 
	_maxAlignment(other._maxAlignment) {}

base_layout& base_layout::operator=(const base_layout& other) noexcept {
	_values = other._values;
	_currentOffset = other._currentOffset;
	_maxAlignment = other._maxAlignment;
	return *this;
}
base_layout& base_layout::operator=(base_layout&& other) noexcept {
	_values = std::exchange(other._values, {});
	_currentOffset = other._currentOffset;
	_maxAlignment = other._maxAlignment;
	return *this;
}

size_t base_layout::bad_offset() noexcept {
	return ~static_cast<size_t>(0);
}

bool base_layout::contains(const std::string_view& name) const noexcept {
	#if _GLSL_STRUCT_HAS_CXX20
	return _values.contains(name.data());
	#else
	return _values.find(name.data()) != _values.end();
	#endif
}

value_data base_layout::get(const std::string_view& name) const noexcept {
	if (!contains(name)) {
		return value_data(bad_offset(), nullptr, 0, 0);
	}

	return _values.at(name.data());
}

size_t base_layout::get_offset(const std::string_view& name) const noexcept {
	return get(name).get_offset();
}

std::vector<size_t> base_layout::get_array_offsets(const std::string_view& name) const noexcept {
	if (!contains(name)) {
		return std::vector<size_t>();
	}

	const base_type_handle& varType = _values.at(name.data()).get_type();

	size_t arraySize;
	if (is_of_type<mat_type>(varType)) {
		arraySize = static_type_cast<mat_type>(varType)->get_array_count();
	}
	else if (is_of_type<array_type>(varType)) {
		arraySize = static_type_cast<array_type>(varType)->get_count();
	}
	else {
		return std::vector<size_t>({ _values.at(name.data()).get_offset() });
	}

	std::vector<size_t> values(arraySize);
	for (size_t i = 0; i != arraySize; ++i) {
		values[i] = _values.at(
_get_array_elem_name(name, i))
			.get_offset();
	}

	return values;
}

const base_type_handle& base_layout::get_type(const std::string_view& name) const noexcept {
	return get(name).get_type();
}
size_t base_layout::get_total_size(const std::string_view& name) const noexcept {
	return get(name).get_total_size();
}
size_t base_layout::get_size(const std::string_view& name) const noexcept {
	return get(name).get_size();
}
size_t base_layout::get_padding(const std::string_view& name) const noexcept {
	return get(name).get_padding();
}

std::vector<std::string> base_layout::get_names() const noexcept {
	std::vector<std::string> names;
	names.reserve(_values.size());

	for (const auto& [name, data] : _values) {
		names.push_back(name);
	}

	return names;
}

size_t base_layout::base_alignment() const noexcept {
	static _GLSL_STRUCT_CONSTEXPR17 const size_t alignment = 16;
	return _ceil_to_nearest_multiple(_maxAlignment, alignment);
}

size_t base_layout::size() const noexcept {
	static _GLSL_STRUCT_CONSTEXPR17 const size_t offset_alignment = 16;
	return _ceil_to_nearest_multiple(_currentOffset, offset_alignment);
}

size_t base_layout::padding() const noexcept {
	return size() - _currentOffset;
}

void base_layout::clear() noexcept {
	_currentOffset = 0;
	_maxAlignment = 0;
	_values.clear();
}

bool base_layout::operator==(const base_layout& other) const noexcept {
	return _currentOffset == other._currentOffset &&
		_maxAlignment == other._maxAlignment &&
		_values == other._values;
}
#if _GLSL_STRUCT_HAS_CXX20
bool base_layout::operator!=(const base_layout& other) const noexcept = default;
#else
bool base_layout::operator!=(const base_layout& other) const noexcept {
	return !(*this == other);
}
#endif

size_t std::hash<base_layout>::operator()(const base_layout& stdOff) const noexcept{
	size_t seed = mstd::hash_combine(stdOff._currentOffset, stdOff._maxAlignment);
	for (const auto& [name, data] : stdOff._values) {
		mstd::hash_append(seed, name, data);
	}
	return seed;
}

#endif