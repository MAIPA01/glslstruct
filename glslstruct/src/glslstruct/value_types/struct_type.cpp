#include <pch.hpp>
#include <glslstruct/value_types/struct_type.hpp>

using namespace glslstruct;

struct_type::struct_type(const values_map& values) {
	for (const auto& [name, data] : values) {
		_values[name] = { data.offset, data.type->clone() };
	}
}

struct_type::struct_type(const struct_type& other) {
	for (const auto& [name, data] : other._values) {
		_values[name] = { data.offset, data.type->clone() };
	}
}

struct_type::~struct_type() {
	for (const auto& value : _values) {
		if (value.second.type != nullptr)
			delete value.second.type;
	}
	_values.clear();
}

[[nodiscard]] base_type* struct_type::clone() const noexcept {
	return new struct_type(*this);
}

[[nodiscard]] struct_type::values_map struct_type::getValues() const noexcept {
	return _values;
}

[[nodiscard]] std::string struct_type::toString() const noexcept {
	return "struct";
}

[[nodiscard]] bool struct_type::operator==(const struct_type& other) const noexcept {
	return _values == other._values;
}
[[nodiscard]] bool struct_type::operator!=(const struct_type& other) const noexcept {
	return !(*this == other);
}

[[nodiscard]] static std::string glslstruct::to_string(const struct_type& value) noexcept {
	return value.toString();
}