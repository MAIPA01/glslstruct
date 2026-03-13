#include <pch.hpp>
#include <glslstruct/value_types/vec_type.hpp>

using namespace glslstruct;

vec_type::vec_type(ValueType type, size_t length) noexcept
	: _base_class(get_value_type_size(type) * length), _length(length), _type(type) {}

void vec_type::accept(base_type_visitor* const visitor) const {
	visitor->visit(*this);
}

[[nodiscard]] ValueType vec_type::get_type() const noexcept {
	return _type;
}
[[nodiscard]] size_t vec_type::get_length() const noexcept {
	return _length;
}

[[nodiscard]] std::string vec_type::to_string() const noexcept {
	return mstd::concat(vec_type_to_string(_type), std::to_string(_length));
}

[[nodiscard]] bool glslstruct::operator==(const vec_type& lhs, const vec_type& rhs) noexcept {
	return lhs._type == rhs._type && lhs._length == rhs._length;
}
[[nodiscard]] bool glslstruct::operator!=(const vec_type& lhs, const vec_type& rhs) noexcept {
	return !(lhs == rhs);
}

size_t std::hash<vec_type>::operator()(const vec_type& value) noexcept {
	size_t seed = static_cast<size_t>(value._type);
	mstd::hash_append(seed, value._length);
	return seed;
}