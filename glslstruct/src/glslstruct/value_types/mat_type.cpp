#include <pch.hpp>
#include <glslstruct/value_types/mat_type.hpp>

using namespace glslstruct;

mat_type::mat_type(const ValueType& type, const size_t& cols, const size_t& rows) 
	: _type(type), _cols(cols), _rows(rows) {}

[[nodiscard]] base_type* mat_type::clone() const noexcept {
	return new mat_type(*this);
}
[[nodiscard]] void mat_type::accept(base_type_visitor* const visitor) const {
	visitor->visit(*this);
}

[[nodiscard]] ValueType mat_type::getType() const noexcept {
	return _type;
}

[[nodiscard]] size_t mat_type::getRows() const noexcept {
	return _rows;
}

[[nodiscard]] size_t mat_type::getCols() const noexcept {
	return _cols;
}

[[nodiscard]] std::string mat_type::toString() const noexcept {
	std::string out = matTypeToString(_type);
	if (_cols == _rows) mstd::concat_to(out, std::to_string(_cols));
	else mstd::concat_to(out, std::to_string(_cols), "x", std::to_string(_rows));
	return out;
}

[[nodiscard]] bool mat_type::operator==(const mat_type& other) const noexcept {
	return _type == other._type && _cols == other._cols && _rows == other._rows;
}
[[nodiscard]] bool mat_type::operator!=(const mat_type& other) const noexcept {
	return !(*this == other);
}

[[nodiscard]] static std::string glslstruct::to_string(const mat_type& value) noexcept {
	return value.toString();
}