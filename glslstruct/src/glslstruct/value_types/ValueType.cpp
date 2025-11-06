#include <pch.hpp>
#include <glslstruct/value_types/ValueType.hpp>

[[nodiscard]] inline std::string glslstruct::vecTypeToString(const ValueType& type) noexcept {
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

[[nodiscard]] inline std::string glslstruct::matTypeToString(const ValueType& type) noexcept {
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