#include <pch.hpp>
#include <glslstruct/value_types/base_type.hpp>

[[nodiscard]] static std::string glslstruct::to_string(const base_type* value) noexcept {
	return value->toString();
}