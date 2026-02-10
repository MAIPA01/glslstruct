#include <pch.hpp>
#include <glslstruct/value_types/base_type.hpp>

using namespace glslstruct;

base_type::base_type(size_t size) noexcept 
	: _size(size) {}

bool base_type::operator==(const base_type& other) const noexcept {
	return _size == other._size;
}
bool base_type::operator!=(const base_type& other) const noexcept {
	return !(*this == other);
}

size_t base_type::get_size() const noexcept {
	return _size;
}

size_t glslstruct::sizeof_type(const base_type_handle& type) noexcept {
	return type->get_size();
}

std::string glslstruct::to_string(const base_type_handle& type) noexcept {
	return type->to_string();
}

size_t std::hash<base_type>::operator()(const base_type& type) noexcept {
	type_hash_visitor visitor;
	type.accept(&visitor);
	size_t result = visitor.result();
	mstd::hash_append(result, type._size);
	return result;
}