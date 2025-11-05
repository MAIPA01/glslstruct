#include "pch.hpp"
#include <glslstruct/value_types.hpp>
#include <glslstruct/std140_offset.hpp>

using namespace glslstruct;
using namespace glslstruct::utils;

[[nodiscard]] static std::string glslstruct::vecTypeToString(const ValueType& type) noexcept {
	switch (type) {
	case ValueType::Other: return "other";
	case ValueType::Bool: return "bvec";
	case ValueType::Int: return "ivec";
	case ValueType::Uint: return "uvec";
	case ValueType::Float: return "vec";
	case ValueType::Double: return "dvec";
	default: return "UNKNOWN";
	}
}

[[nodiscard]] static std::string glslstruct::matTypeToString(const ValueType& type) noexcept {
	switch (type) {
	case ValueType::Other: return "other";
	case ValueType::Bool: return "bmat";
	case ValueType::Int: return "imat";
	case ValueType::Uint: return "umat";
	case ValueType::Float: return "mat";
	case ValueType::Double: return "dmat";
	default: return "UNKNOWN";
	}
}

[[nodiscard]] static std::string glslstruct::to_string(const base_type*& value) noexcept {
	return value->toString();
}

scalar_type::scalar_type(const ValueType& type) : _type(type) {}

[[nodiscard]] ValueType scalar_type::getType() const noexcept {
	return _type;
}

[[nodiscard]] std::string scalar_type::toString() const noexcept {
	return to_string(_type);
}

[[nodiscard]] static std::string glslstruct::to_string(const scalar_type*& value) noexcept {
	return value->toString();
}

vec_type::vec_type(const ValueType& type, const size_t& length) : _type(type), _length(length) {}

[[nodiscard]] ValueType vec_type::getType() const noexcept {
	return _type;
}

[[nodiscard]] size_t vec_type::getLength() const noexcept {
	return _length;
}

[[nodiscard]] std::string vec_type::toString() const noexcept {
	return mstd::concat(vecTypeToString(_type), std::to_string(_length));
}

[[nodiscard]] static std::string glslstruct::to_string(const vec_type*& value) noexcept {
	return value->toString();
}

mat_type::mat_type(const ValueType& type, const size_t& cols, const size_t& rows) : _type(type), _cols(cols), _rows(rows) {}

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

[[nodiscard]] static std::string glslstruct::to_string(const mat_type*& value) noexcept {
	return value->toString();
}

struct_type::struct_type(const values_map& values) {
	for (auto& value : values) {
		_values[value.first] = { value.second.offset, value.second.type };
	}
}

struct_type::~struct_type() {
	for (const auto& value : _values) {
		if (value.second.type != nullptr)
			delete value.second.type;
	}
	_values.clear();
}

[[nodiscard]] struct_type::values_map struct_type::getValues() const noexcept {
	return _values;
}

[[nodiscard]] std::string struct_type::toString() const noexcept {
	return "struct";
}

[[nodiscard]] static std::string glslstruct::to_string(const struct_type*& value) noexcept {
	return value->toString();
}

array_type::array_type(const base_type* type, const size_t& length) : _type(type), _length(length) {}

array_type::~array_type() {
	delete _type;
}

[[nodiscard]] const base_type* array_type::getType() const noexcept {
	return _type;
}

[[nodiscard]] size_t array_type::getLength() const noexcept {
	return _length;
}

[[nodiscard]] std::string array_type::toString() const noexcept {
	return mstd::concat(_type->toString(), "[", std::to_string(_length), "]");
}

[[nodiscard]] static std::string glslstruct::to_string(const array_type*& value) noexcept {
	return value->toString();
}