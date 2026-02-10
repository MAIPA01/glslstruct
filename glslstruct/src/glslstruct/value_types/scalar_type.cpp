#include <pch.hpp>
#include <glslstruct/value_types/scalar_type.hpp>

using namespace glslstruct;

scalar_type::scalar_type(ValueType type) noexcept 
	: _base_class(get_value_type_size(type)), _type(type) {}

void scalar_type::accept(base_type_visitor* const visitor) const {
	visitor->visit(*this);
}

[[nodiscard]] ValueType scalar_type::get_type() const noexcept {
	return _type;
}

[[nodiscard]] std::string scalar_type::to_string() const noexcept {
	return glslstruct::to_string(_type);
}

[[nodiscard]] bool glslstruct::operator==(const scalar_type& lhs, const scalar_type& rhs) noexcept {
	return lhs._type == rhs._type;
}
[[nodiscard]] bool glslstruct::operator!=(const scalar_type& lhs, const scalar_type& rhs) noexcept {
	return !(lhs == rhs);
}

size_t std::hash<scalar_type>::operator()(const scalar_type& value) noexcept {
	return static_cast<size_t>(value._type);
}
