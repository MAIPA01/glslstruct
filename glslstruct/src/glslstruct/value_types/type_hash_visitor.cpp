#include <pch.hpp>
#include <mstd/hash.hpp>
#include <glslstruct/value_types/type_hash_visitor.hpp>
#include <glslstruct/value_types/scalar_type.hpp>
#include <glslstruct/value_types/vec_type.hpp>
#include <glslstruct/value_types/mat_type.hpp>
#include <glslstruct/value_types/struct_type.hpp>
#include <glslstruct/value_types/array_type.hpp>

using namespace glslstruct;

size_t std::hash<base_type>::operator()(const base_type& value) {
	type_hash_visitor visitor;
	value.accept(&visitor);
	return visitor.result();
}
size_t std::hash<scalar_type>::operator()(const scalar_type& value) {
	return static_cast<size_t>(value._type);
}
size_t std::hash<vec_type>::operator()(const vec_type& value) {
	size_t seed = static_cast<size_t>(value._type);
	mstd::hash_append(seed, value._length);
	return seed;
}
size_t std::hash<mat_type>::operator()(const mat_type& value) {
	size_t seed = static_cast<size_t>(value._type);
	mstd::hash_append(seed, value._cols, value._rows);
	return seed;
}
size_t std::hash<struct_type>::operator()(const struct_type& value) {
	size_t seed = 0;
	for (const auto& [name, data] : value._values) {
		mstd::hash_append(seed, name, data.offset, *data.type);
	}
	return seed;
}
size_t std::hash<array_type>::operator()(const array_type& value) {
	return mstd::hash_combine(value._length, *value._type);
}

void type_hash_visitor::visit(const scalar_type& value) {
	_result = std::hash<scalar_type>()(value);
}
void type_hash_visitor::visit(const vec_type& value) {
	_result = std::hash<vec_type>()(value);
}
void type_hash_visitor::visit(const mat_type& value) {
	_result = std::hash<mat_type>()(value);
}
void type_hash_visitor::visit(const struct_type& value) {
	_result = std::hash<struct_type>()(value);
}
void type_hash_visitor::visit(const array_type& value) {
	_result = std::hash<array_type>()(value);
}

size_t type_hash_visitor::result() const noexcept {
	return _result;
}