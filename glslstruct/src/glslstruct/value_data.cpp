#include <pch.hpp>
#include <glslstruct/value_data.hpp>
#include <glslstruct/value_types.hpp>

using namespace glslstruct;

bool value_data::operator==(const value_data& other) const {
	return offset == other.offset && type == other.type;
}
bool value_data::operator!=(const value_data& other) const {
	return !(*this == other);
}

size_t std::hash<glslstruct::value_data>::operator()(const value_data& value) const {
	size_t value_hash = 0;
	mstd::hash_combine(value_hash, value.offset, value.type);
	return value_hash;
}