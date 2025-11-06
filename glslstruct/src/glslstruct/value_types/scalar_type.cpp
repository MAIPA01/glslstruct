#include <pch.hpp>
#include <glslstruct/value_types/scalar_type.hpp>

using namespace glslstruct;

scalar_type::scalar_type(const ValueType& type) : _type(type) {}

[[nodiscard]] base_type* scalar_type::clone() const noexcept {
	return new scalar_type(*this);
}
[[nodiscard]] void scalar_type::accept(base_type_visitor* const visitor) const {
	visitor->visit(*this);
}

[[nodiscard]] ValueType scalar_type::getType() const noexcept {
	return _type;
}

[[nodiscard]] std::string scalar_type::toString() const noexcept {
	return to_string(_type);
}

[[nodiscard]] bool scalar_type::operator==(const scalar_type& other) const noexcept {
	return _type == other._type;
}
[[nodiscard]] bool scalar_type::operator!=(const scalar_type& other) const noexcept {
	return !(*this == other);
}

[[nodiscard]] static std::string glslstruct::to_string(const scalar_type& value) noexcept {
	return value.toString();
}
