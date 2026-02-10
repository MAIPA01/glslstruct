#include <pch.hpp>
#include <mstd/hash.hpp>
#include <glslstruct/value_types/visitors/type_hash_visitor.hpp>
#include <glslstruct/value_types/scalar_type.hpp>
#include <glslstruct/value_types/vec_type.hpp>
#include <glslstruct/value_types/mat_type.hpp>
#include <glslstruct/value_types/struct_type.hpp>
#include <glslstruct/value_types/array_type.hpp>

using namespace glslstruct;

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