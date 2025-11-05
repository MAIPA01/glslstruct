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

[[nodiscard]] ValutType scalar_type::getType() const noexcept {
	return _type;
}

[[nodiscard]] std::string scalar_type::toString() const noexcept {
	return to_string(_type);
}

[[nodiscard]] static std::string glslstruct::to_string(const scalar_type*& value) {
	return value->to_string();
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
	return value->to_string();
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
	return value->to_string();
}

struct_type::struct_type(const offsets_map& offsets, const names_map& names, const types_map& types) : base_type(), _offsets(offsets), _names(names) {
	for (auto& type : types) {
		_types[type.first] = type.second->Clone();
	}
}

struct_type::~struct_type() {
	_offsets.clear();
	_names.clear();
	for (auto& type : _types) {
		if (type.second == nullptr) continue;
		delete type.second;
	}
	_types.clear();
}

CLONE_FUNC_DEFINITION_ADVANCED(struct_type, STANDARD_CLONE(_offsets), STANDARD_CLONE(_names);
	for (const auto& type : _types) cloned->_types[type.first] = type.second->Clone()
)

[[nodiscard]] struct_type::offsets_map struct_type::getOffsets() const noexcept {
	return _offsets;
}

[[nodiscard]] struct_type::names_map struct_type::getNames() const noexcept {
	return _names;
}

[[nodiscard]] struct_type::types_map struct_type::getTypes() const noexcept {
	return _types;
}

[[nodiscard]] std::string struct_type::toString() const noexcept {
	return "struct";
}

[[nodiscard]] static std::string glslstruct::to_string(const struct_type*& value) noexcept {
	return value->to_string();
}

array_type::array_type(const base_type* type, const size_t& length) : base_type(type), _length(length) {}

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
	return mstd::concat(_type->to_string(), "[", std::to_string(_length), "]");
}

[[nodiscard]] static std::string glslstruct::to_string(const array_type& value) noexcept {
	return value.to_string();
}