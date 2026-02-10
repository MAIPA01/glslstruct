#include "pch.hpp"
#include <glslstruct/std430_offset.hpp>

using namespace glslstruct;

size_t std430_offset::_get_array_elem_base_aligement(size_t baseAligement) const noexcept {
	return baseAligement;
}

size_t std430_offset::add(const std::string& name, const std430_offset& structTemplate) {
	return _add_struct(name, structTemplate.base_aligement(), structTemplate._currentOffset, structTemplate._values);
}

std::vector<size_t> std430_offset::add(const std::string& name, const std430_offset& structTemplate, size_t count) {
	return _add_struct_array(name, structTemplate.base_aligement(), 
		structTemplate._currentOffset, structTemplate._values, count);
}

size_t std430_offset::base_aligement() const noexcept {
	return _maxAligement;
}

#if !_GLSL_STRUCT_HAS_CXX20
bool std430_offset::operator==(const std430_offset& other) const noexcept {
	return std_offset::operator==(other);
}
bool std430_offset::operator!=(const std430_offset& other) const noexcept {
	return std_offset::operator!=(other);
}
#endif

size_t std::hash<std430_offset>::operator()(const std430_offset& std430Off) noexcept {
	return std::hash<std_offset>()(*static_cast<const std_offset*>(&std430Off));
}