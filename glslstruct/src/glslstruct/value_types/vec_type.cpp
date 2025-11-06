#include <pch.hpp>
#include <glslstruct/value_types/vec_type.hpp>

using namespace glslstruct;

vec_type::vec_type(const ValueType& type, const size_t& length) : _type(type), _length(length) {}

[[nodiscard]] base_type* vec_type::clone() const noexcept {
	return new vec_type(*this);
}
[[nodiscard]] void vec_type::accept(base_type_visitor* const visitor) const {
	visitor->visit(*this);
}

[[nodiscard]] ValueType vec_type::getType() const noexcept {
	return _type;
}

[[nodiscard]] size_t vec_type::getLength() const noexcept {
	return _length;
}

[[nodiscard]] std::string vec_type::toString() const noexcept {
	return mstd::concat(vecTypeToString(_type), std::to_string(_length));
}

[[nodiscard]] bool vec_type::operator==(const vec_type& other) const noexcept {
	return _type == other._type && _length == other._length;
}
[[nodiscard]] bool vec_type::operator!=(const vec_type& other) const noexcept {
	return !(*this == other);
}

[[nodiscard]] static std::string glslstruct::to_string(const vec_type& value) noexcept {
	return value.toString();
}