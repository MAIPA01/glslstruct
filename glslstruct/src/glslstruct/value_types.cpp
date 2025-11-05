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

scalar_type::scalar_type(const ValueType& type) : base_type(), _type(type) {}

CLONE_FUNC_DEFINITION(scalar_type, _type)

[[nodiscard]] ValueType scalar_type::getType() const noexcept {
	return _type;
}

[[nodiscard]] std::string scalar_type::toString() const noexcept {
	return to_string(_type);
}

[[nodiscard]] static std::string glslstruct::to_string(const scalar_type*& value) noexcept {
	return value->toString();
}

vec_type::vec_type(const ValueType& type, const size_t& length) : base_type(), _type(type), _length(length) {}

CLONE_FUNC_DEFINITION(vec_type, _type, _length)

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

mat_type::mat_type(const ValueType& type, const size_t& cols, const size_t& rows) : base_type(), _type(type), _cols(cols), _rows(rows) {}

CLONE_FUNC_DEFINITION(mat_type, _type, _cols, _rows)

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

struct_type::struct_type(const values_map& values) : base_type() {
	for (auto& value : values) {
		_values[value.first] = { value.second.offset, value.second.type->Clone() };
	}
}

struct_type::~struct_type() {
	for (const auto& value : _values) {
		if (value.second.type != nullptr)
			delete value.second.type;
	}
	_values.clear();
}

struct_type* struct_type::Clone() const {
	struct_type* cloned = new struct_type();
	CloneTo(cloned);
	return cloned;
}
void struct_type::CloneTo(struct_type* cloned) const {
	for (auto& value : _values) {
		cloned->_values[value.first] = { value.second.offset, value.second.type->Clone() };
	}
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

array_type::array_type(const base_type* type, const size_t& length) : base_type(), _type(type), _length(length) {}

array_type::~array_type() {
	delete _type;
}

CLONE_FUNC_DEFINITION_ADVANCED(array_type, _type, _type->Clone(), STANDARD_CLONE(_length))

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