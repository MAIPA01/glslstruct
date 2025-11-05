#pragma once
#include <glslstruct/value_types/value_type.hpp>
#include <glslstruct/value_types/ValueType.hpp>

namespace glslstruct {
	class scalar_type : public value_type<scalar_type> {
	private:
		ValueType _type = ValueType::Other;

	public:
		scalar_type(const ValueType& type);
		scalar_type(const scalar_type& other) = default;
		virtual ~scalar_type() = default;

		[[nodiscard]] base_type* clone() const noexcept;

		[[nodiscard]] ValueType getType() const noexcept;

		[[nodiscard]] std::string toString() const noexcept override;

		[[nodiscard]] bool operator==(const scalar_type& other) const noexcept {
			return _type == other._type;
		}
		[[nodiscard]] bool operator!=(const scalar_type& other) const noexcept {
			return !(*this == other);
		}
	};

	[[nodiscard]] static std::string to_string(const scalar_type*& value) noexcept;
}