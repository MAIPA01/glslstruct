#include <pch.hpp>
#include <glslstruct/value_types/struct_type.hpp>

using namespace glslstruct;

struct_type::struct_type(const std::unordered_map<std::string, value_data>& values, size_t size) noexcept 
	: _base_class(size), _values(values) {}
struct_type::struct_type(struct_type&& other) noexcept 
	: _base_class(other), _values(std::exchange(other._values, {})) {}

struct_type& struct_type::operator=(struct_type&& other) noexcept {
	_base_class::operator=(other);
	_values = std::exchange(other._values, {});
	return *this;
}

void struct_type::accept(base_type_visitor* const visitor) const {
	visitor->visit(*this);
}

const std::unordered_map<std::string, value_data>& struct_type::get_values() const noexcept {
	return _values;
}

std::string struct_type::to_string() const noexcept {
	return "struct";
}

bool glslstruct::operator==(const struct_type& lhs, const struct_type& rhs) noexcept {
	return lhs._values == rhs._values;
}
bool glslstruct::operator!=(const struct_type& lhs, const struct_type& rhs) noexcept {
	return !(lhs == rhs);
}

size_t std::hash<struct_type>::operator()(const struct_type& value) noexcept {
	size_t seed = 0;
	for (const auto& [name, data] : value._values) {
		mstd::hash_append(seed, name, data);
	}
	return seed;
}