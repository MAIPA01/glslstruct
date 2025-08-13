#include "pch.hpp"
#include <STD430Offsets.hpp>

using namespace glsl;
using namespace std;

vector<size_t> STD430Offsets::_AddArray(const string& name, size_t arraySize, size_t baseAligement, size_t baseOffset, const ValueType* type)
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

STD430Offsets::STD430Offsets(STD430Offsets& std430off)
{
	std430off.CloneTo(this);
}

STD430Offsets::STD430Offsets(const STD430Offsets& std430off)
{
	std430off.CloneTo(this);
}

STD430Offsets::STD430Offsets(STD430Offsets&& std430off)
{
	std430off.CloneTo(this);
}

STD430Offsets& STD430Offsets::operator=(STD430Offsets& std430off)
{
	std430off.CloneTo(this);
	return *this;
}

STD430Offsets& STD430Offsets::operator=(const STD430Offsets& std430off)
{
	std430off.CloneTo(this);
	return *this;
}

STD430Offsets& STD430Offsets::operator=(STD430Offsets&& std430off)
{
	std430off.CloneTo(this);
	return *this;
}

CLONE_BASE_FUNC_DEFINITION(STD430Offsets, STDOffsets)

size_t STD430Offsets::Add(const string& name, const STD430Offsets& structTemplate)
{
	return _AddStruct(name, structTemplate.GetBaseAligement(), structTemplate._currentOffset, structTemplate._offsets, structTemplate._names, structTemplate._types);
}

vector<size_t> STD430Offsets::Add(const string& name, const STD430Offsets& structTemplate, size_t size)
{
	return _AddStructArray(name, structTemplate.GetBaseAligement(), structTemplate._currentOffset, structTemplate._offsets, structTemplate._names, structTemplate._types, size);
}

size_t STD430Offsets::GetBaseAligement() const
{
	return _maxAligement;
}