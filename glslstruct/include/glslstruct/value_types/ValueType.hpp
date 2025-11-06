#pragma once
#define USE_ENUMS_MACROS
#include <mstd/macros.hpp>
#include <string>
#include <cstdint>

namespace glslstruct {
	ENUM_CLASS_BASE_STRING(ValueType, uint8_t, Other, "other", Bool, "bool", Int, "int", Uint, "uint", Float, "float", Double, "double");

	template<class T>
	[[nodicard]] static constexpr ValueType getValueType() noexcept {
		if constexpr (std::is_same_v<T, bool>) {
			return ValueType::Bool;
		}
		else if constexpr (std::is_same_v<T, int>) {
			return ValueType::Int;
		}
		else if constexpr (std::is_same_v<T, unsigned int>) {
			return ValueType::Uint;
		}
		else if constexpr (std::is_same_v<T, float>) {
			return ValueType::Float;
		}
		else if constexpr (std::is_same_v<T, double>) {
			return ValueType::Double;
		}
		else {
			return ValueType::Other;
		}
	}

	[[nodiscard]] extern inline std::string vecTypeToString(const ValueType& type) noexcept;

	[[nodiscard]] extern inline std::string matTypeToString(const ValueType& type) noexcept;
}