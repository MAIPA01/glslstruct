#include "pch.hpp"
#include <glslstruct/std140_offset.hpp>

using namespace glslstruct;

std140_offset::std140_offset(std140_offset& std140off) {
	std140off.CloneTo(this);
}

std140_offset::std140_offset(const std140_offset& std140off) {
	std140off.CloneTo(this);
}

std140_offset::std140_offset(std140_offset&& std140off) {
	std140off.CloneTo(this);
}

std140_offset& std140_offset::operator=(std140_offset& std140off) {
	std140off.CloneTo(this);
	return *this;
}

std140_offset& std140_offset::operator=(const std140_offset& std140off) {
	std140off.CloneTo(this);
	return *this;
}

std140_offset& std140_offset::operator=(std140_offset&& std140off) {
	std140off.CloneTo(this);
	return *this;
}

CLONE_BASE_FUNC_DEFINITION(std140_offset, std_offset)

[[nodiscard]] size_t std140_offset::add(const std::string& name, const std140_offset& structTemplate) {
	return _addStruct(name, structTemplate.baseAligement(), structTemplate._currentOffset, structTemplate._values);
}

[[nodiscard]] std::vector<size_t> std140_offset::add(const std::string& name, const std140_offset& structTemplate, size_t size) {
	return _addStructArray(name, structTemplate.baseAligement(), structTemplate._currentOffset, structTemplate._values, size);
}
