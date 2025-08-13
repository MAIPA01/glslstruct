#include "pch.hpp"
#include <STD140Offsets.hpp>

using namespace glsl;
using namespace std;

STD140Offsets::STD140Offsets(STD140Offsets& std140off)
{
	std140off.CloneTo(this);
}

STD140Offsets::STD140Offsets(const STD140Offsets& std140off)
{
	std140off.CloneTo(this);
}

STD140Offsets::STD140Offsets(STD140Offsets&& std140off)
{
	std140off.CloneTo(this);
}

STD140Offsets& STD140Offsets::operator=(STD140Offsets& std140off)
{
	std140off.CloneTo(this);
	return *this;
}

STD140Offsets& STD140Offsets::operator=(const STD140Offsets& std140off)
{
	std140off.CloneTo(this);
	return *this;
}

STD140Offsets& STD140Offsets::operator=(STD140Offsets&& std140off)
{
	std140off.CloneTo(this);
	return *this;
}

CLONE_BASE_FUNC_DEFINITION(STD140Offsets, STDOffsets)

size_t STD140Offsets::Add(const string& name, const STD140Offsets& structTemplate)
{
	return _AddStruct(name, structTemplate.GetBaseAligement(), structTemplate._currentOffset, structTemplate._offsets, structTemplate._names, structTemplate._types);
}

vector<size_t> STD140Offsets::Add(const string& name, const STD140Offsets& structTemplate, size_t size)
{
	return _AddStructArray(name, structTemplate.GetBaseAligement(), structTemplate._currentOffset, structTemplate._offsets, structTemplate._names, structTemplate._types, size);
}
