#pragma once
#include <unordered_map>
#include <glslstruct/value_types/value_type.hpp>
#include <glslstruct/value_types/ValueType.hpp>

namespace glslstruct {
	class struct_type : public value_type<struct_type> {
	private:
		using values_map = std::unordered_map<std::string, value_data>;

		values_map _values;

	public:
		struct_type(const values_map& values);
		struct_type(const struct_type& other);
		virtual ~struct_type();

		[[nodiscard]] base_type* clone() const noexcept;

		[[nodiscard]] values_map getValues() const noexcept;

		[[nodiscard]] std::string toString() const noexcept override;

		[[nodiscard]] bool operator==(const struct_type& other) const noexcept {
			return _values == other._values;
		}
		[[nodiscard]] bool operator!=(const struct_type& other) const noexcept {
			return !(*this == other);
		}
	};

	[[nodiscard]] static std::string to_string(const struct_type*& value) noexcept;

}