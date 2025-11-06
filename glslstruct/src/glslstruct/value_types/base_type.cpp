#include <pch.hpp>
#include <glslstruct/value_types/base_type.hpp>

using namespace glslstruct;

[[nodiscard]] bool base_type::operator!=(const base_type& other) const noexcept {
	return !(*this == other);
}

[[nodiscard]] bool base_type::operator!=(const scalar_type& other) const noexcept {
	return !(*this == other);
}

[[nodiscard]] bool base_type::operator!=(const vec_type& other) const noexcept {
	return !(*this == other);
}

[[nodiscard]] bool base_type::operator!=(const mat_type& other) const noexcept {
	return !(*this == other);
}

[[nodiscard]] bool base_type::operator!=(const struct_type& other) const noexcept {
	return !(*this == other);
}

[[nodiscard]] bool base_type::operator!=(const array_type& other) const noexcept {
	return !(*this == other);
}

[[nodiscard]] static std::string glslstruct::to_string(const base_type* value) noexcept {
	return value->toString();
}