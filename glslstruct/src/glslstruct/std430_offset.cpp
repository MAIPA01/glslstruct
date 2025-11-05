#include "pch.hpp"
#include <glslstruct/std430_offset.hpp>

using namespace glslstruct;

[[nodiscard]] std::vector<size_t> std430_offset::_addArray(const std::string& name, size_t arraySize, size_t baseAligement, 
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
		_setVariable(valueName, valueAligementOffset, type->Clone());
	}

	// UPDATE SIZE
	_currentOffset = aligementOffset + arraySize * baseAligement;

	// SET ARRAY BEGIN POINTER
	_setVariable(name, arrayElemOffsets[0], new array_type(type, arraySize));

	return arrayElemOffsets;
}

std430_offset::std430_offset(std430_offset& std430off) {
	std430off.CloneTo(this);
}

std430_offset::std430_offset(const std430_offset& std430off) {
	std430off.CloneTo(this);
}

std430_offset::std430_offset(std430_offset&& std430off) {
	std430off.CloneTo(this);
}

std430_offset& std430_offset::operator=(std430_offset& std430off) {
	std430off.CloneTo(this);
	return *this;
}

std430_offset& std430_offset::operator=(const std430_offset& std430off) {
	std430off.CloneTo(this);
	return *this;
}

std430_offset& std430_offset::operator=(std430_offset&& std430off) {
	std430off.CloneTo(this);
	return *this;
}

CLONE_BASE_FUNC_DEFINITION(std430_offset, std_offset)

[[nodiscard]] size_t std430_offset::add(const std::string& name, const std430_offset& structTemplate) {
	return _addStruct(name, structTemplate.baseAligement(), structTemplate._currentOffset, structTemplate._values);
}

[[nodiscard]] std::vector<size_t> std430_offset::add(const std::string& name, const std430_offset& structTemplate, size_t size) {
	return _addStructArray(name, structTemplate.baseAligement(), structTemplate._currentOffset, structTemplate._values, size);
}

[[nodiscard]] size_t std430_offset::baseAligement() const {
	return _maxAligement;
}