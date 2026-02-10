#include <pch.hpp>
#include <glslstruct/value_data.hpp>
#include <glslstruct/value_types.hpp>

using namespace glslstruct;

value_data::value_data(size_t offset, const base_type_handle& type, size_t size, size_t padding) noexcept 
	: _type(type), _offset(offset), _size(size), _padding(padding) {}
value_data::value_data(value_data&& other) noexcept 
	: _type(std::exchange(other._type, nullptr)), _offset(other._offset), 
	_size(other._size), _padding(other._padding) {}

value_data& value_data::operator=(value_data&& other) noexcept {
	_offset = other._offset;
	_type = std::exchange(other._type, nullptr);
	return *this;
}

void value_data::change_type(const base_type_handle& newType) noexcept {
	_type = newType;
}
void value_data::set_padding(size_t padding) noexcept {
	_padding = padding;
}

[[nodiscard]] const base_type_handle& value_data::get_type() const noexcept {
	return _type;
}
[[nodiscard]] size_t value_data::get_offset() const noexcept {
	return _offset;
}
[[nodiscard]] size_t value_data::get_total_size() const noexcept {
	return _size + _padding;
}
[[nodiscard]] size_t value_data::get_size() const noexcept {
	return _size;
}
[[nodiscard]] size_t value_data::get_padding() const noexcept {
	return _padding;
}

bool value_data::operator==(const value_data& other) const noexcept {
	return _offset == other._offset && *_type == *other._type && 
		_size == other._size && _padding == other._padding;
}
#if !_GLSL_STRUCT_HAS_CXX20
bool value_data::operator!=(const value_data& other) const noexcept {
	return !(*this == other);
}
#endif

size_t std::hash<glslstruct::value_data>::operator()(const value_data& value) const noexcept {
	return mstd::hash_combine(value._offset, *value._type, 
		value._size, value._padding);
}