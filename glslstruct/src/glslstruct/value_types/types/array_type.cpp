#include <pch.hpp>
#include <glslstruct/value_types/array_type.hpp>
#include <glslstruct/value_types.hpp>

using namespace glslstruct;

size_t array_type::_calculate_array_size(const base_type_handle& type, size_t count) noexcept {
	size_t typeSize = type->get_size();
	if (typeSize % 16 != 0) {
		typeSize += (16 - (typeSize % 16));
	}
	return typeSize * count;
}

array_type::array_type(ValueType type, size_t count) noexcept 
	: array_type(std::make_shared<scalar_type>(type), count) {}
array_type::array_type(ValueType type, size_t length, size_t count) noexcept
	: array_type(std::make_shared<vec_type>(type, length), count) {}
array_type::array_type(ValueType type, size_t cols, size_t rows, bool columnMajor, size_t count) noexcept 
	: array_type(std::make_shared<mat_type>(type, cols, rows, columnMajor), count) { }
array_type::array_type(ValueType type, size_t cols, size_t rows, size_t count) noexcept
	: array_type(type, cols, rows, true, count) {}
array_type::array_type(const std::unordered_map<std::string, value_data>& values, size_t size, size_t count) noexcept 
	: array_type(std::make_shared<struct_type>(values, size), count) { }
array_type::array_type(const base_type_handle& type, size_t count) noexcept 
	: _base_class(_calculate_array_size(type, count)), _type(type), _count(count) {}
array_type::array_type(array_type&& other) noexcept 
	: _base_class(other),
	_type(std::exchange(other._type, nullptr)), 
	_count(std::exchange(other._count, 0)) {}

array_type& array_type::operator=(array_type&& other) noexcept {
	_base_class::operator=(other);
	_type = std::exchange(other._type, nullptr);
	_count = std::exchange(other._count, 0);
	return *this;
}

void array_type::accept(base_type_visitor* const visitor) const {
	visitor->visit(*this);
}

const base_type_handle& array_type::get_type() const noexcept {
	return _type;
}
size_t array_type::get_count() const noexcept {
	return _count;
}

std::string array_type::to_string() const noexcept {
	return mstd::concat(_type->to_string(), "[", std::to_string(_count), "]");
}

bool glslstruct::operator==(const array_type& lhs, const array_type& rhs) noexcept {
	return *lhs._type == *rhs._type && lhs._count == rhs._count;
}
bool glslstruct::operator!=(const array_type& lhs, const array_type& rhs) noexcept {
	return !(lhs == rhs);
}

size_t std::hash<array_type>::operator()(const array_type& value) noexcept {
	return mstd::hash_combine(value._count, *value._type);
}