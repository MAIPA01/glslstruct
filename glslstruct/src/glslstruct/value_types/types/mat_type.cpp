#include <pch.hpp>
#include <glslstruct/value_types/mat_type.hpp>

using namespace glslstruct;

size_t mat_type::_calculate_mat_size(ValueType type, size_t cols, size_t rows, bool columnMajor) noexcept {
	const size_t arrayCount = columnMajor ? cols : rows;
	size_t vecSize = (columnMajor ? rows : cols) * get_value_type_size(type);
	if (vecSize % 16 != 0) {
		vecSize += 16 - (vecSize % 16);
	}
	return vecSize * arrayCount;
}

mat_type::mat_type(ValueType type, size_t cols, size_t rows, bool columnMajor) noexcept
	: _base_class(_calculate_mat_size(type, cols, rows, columnMajor)), 
	_cols(cols), _rows(rows), _type(type), _columnMajor(columnMajor) {}

void mat_type::accept(base_type_visitor* const visitor) const {
	visitor->visit(*this);
}

ValueType mat_type::get_type() const noexcept {
	return _type;
}
size_t mat_type::get_rows() const noexcept {
	return _rows;
}
size_t mat_type::get_cols() const noexcept {
	return _cols;
}
size_t mat_type::get_vec_length() const noexcept {
	return _columnMajor ? _rows : _cols;
}
size_t mat_type::get_array_count() const noexcept {
	return _columnMajor ? _cols : _rows;
}

bool mat_type::is_column_major() const noexcept {
	return _columnMajor;
}

std::string mat_type::to_string() const noexcept {
	std::string out = mat_type_to_string(_type);
	if (_cols == _rows) mstd::concat_to(out, std::to_string(_cols));
	else mstd::concat_to(out, std::to_string(_cols), "x", std::to_string(_rows));
	return out;
}

bool glslstruct::operator==(const mat_type& lhs, const mat_type& rhs) noexcept {
	return lhs._type == rhs._type && lhs._cols == rhs._cols && lhs._rows == rhs._rows;
}
bool glslstruct::operator!=(const mat_type& lhs, const mat_type& rhs) noexcept {
	return !(lhs == rhs);
}

size_t std::hash<mat_type>::operator()(const mat_type& value) noexcept {
	size_t seed = static_cast<size_t>(value._type);
	mstd::hash_append(seed, value._cols, value._rows);
	return seed;
}