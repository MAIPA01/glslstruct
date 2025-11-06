#include <pch.hpp>
#include <glslstruct/value_types/array_type.hpp>

using namespace glslstruct;

array_type::array_type(const base_type* type, const size_t& length) : _type(type), _length(length) {}

array_type::array_type(const array_type& other) 
	: _type(other._type->clone()), _length(other._length) {}

array_type::~array_type() {
	delete _type;
}

[[nodiscard]] base_type* array_type::clone() const noexcept {
	return new array_type(*this);
}
[[nodiscard]] void array_type::accept(base_type_visitor* const visitor) const {
	visitor->visit(*this);
}

[[nodiscard]] const base_type* array_type::type() const noexcept {
	return _type;
}

[[nodiscard]] size_t array_type::length() const noexcept {
	return _length;
}

[[nodiscard]] std::string array_type::toString() const noexcept {
	return mstd::concat(_type->toString(), "[", std::to_string(_length), "]");
}

[[nodiscard]] bool array_type::operator==(const array_type& other) const noexcept {
	return *_type == *other._type && _length == other._length;
}
[[nodiscard]] bool array_type::operator!=(const array_type& other) const noexcept {
	return !(*this == other);
}

[[nodiscard]] static std::string glslstruct::to_string(const array_type& value) noexcept {
	return value.toString();
}