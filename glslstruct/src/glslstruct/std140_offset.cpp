#include "pch.hpp"
#include <glslstruct/std140_offset.hpp>

using namespace glslstruct;

size_t std140_offset::add(const std::string& name, const std140_offset& structOffsets) {
	return _add_struct(name, structOffsets.base_aligement(), 
		structOffsets._currentOffset, structOffsets._values);
}

std::vector<size_t> std140_offset::add(const std::string& name, const std140_offset& structOffsets, size_t count) {
	return _add_struct_array(name, structOffsets.base_aligement(), structOffsets._currentOffset, 
		structOffsets._values, count);
}

#if !_GLSL_STRUCT_HAS_CXX20
bool std140_offset::operator==(const std140_offset& other) const noexcept {
	return std_offset::operator==(other);
}

bool std140_offset::operator!=(const std140_offset& other) const noexcept {
	return std_offset::operator!=(other);
}
#endif

size_t std::hash<std140_offset>::operator()(const std140_offset& std140Off) {
	return std::hash<std_offset>()(*static_cast<const std_offset*>(&std140Off));
}