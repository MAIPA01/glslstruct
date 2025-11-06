#include "pch.hpp"
#include <glslstruct/std140_offset.hpp>

using namespace glslstruct;

size_t std::hash<std140_offset>::operator()(const std140_offset& std140Off) {
	return std::hash<std_offset>()(*static_cast<const std_offset*>(&std140Off));
}

void std140_offset::_cloneFrom(const std140_offset& std140off) noexcept {
	std_offset::_cloneFrom(*static_cast<const std_offset*>(&std140off));
}

std140_offset::std140_offset(std140_offset& std140off) {
	_cloneFrom(std140off);
}

std140_offset::std140_offset(const std140_offset& std140off) {
	_cloneFrom(std140off);
}

std140_offset::std140_offset(std140_offset&& std140off) noexcept {
	_cloneFrom(std140off);
}

std140_offset& std140_offset::operator=(std140_offset& std140off) {
	clear();
	_cloneFrom(std140off);
	return *this;
}

std140_offset& std140_offset::operator=(const std140_offset& std140off) {
	clear();
	_cloneFrom(std140off);
	return *this;
}

std140_offset& std140_offset::operator=(std140_offset&& std140off) noexcept {
	clear();
	_cloneFrom(std140off);
	return *this;
}

[[nodiscard]] std140_offset* std140_offset::clone() const noexcept {
	return new std140_offset(*this);
}

size_t std140_offset::add(const std::string& name, const std140_offset& structTemplate) {
	return _addStruct(name, structTemplate.baseAligement(), structTemplate._currentOffset, structTemplate._values);
}

std::vector<size_t> std140_offset::add(const std::string& name, const std140_offset& structTemplate, size_t size) {
	return _addStructArray(name, structTemplate.baseAligement(), structTemplate._currentOffset, structTemplate._values, size);
}

bool std140_offset::operator==(const std140_offset& std140off) const {
	return std_offset::operator==(*static_cast<const std_offset*>(&std140off));
}
bool std140_offset::operator!=(const std140_offset& std140off) const {
	return !(*this == std140off);
}