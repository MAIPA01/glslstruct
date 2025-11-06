#pragma once
#include <glslstruct/value_types/value_type.hpp>
#include <glslstruct/value_types/ValueType.hpp>

namespace glslstruct {
	class array_type : public value_type<array_type> {
	private:
		const base_type* _type = nullptr;
		size_t _length = 0;

	public:
		array_type(const base_type* type, const size_t& length);
		array_type(const array_type& other);
		virtual ~array_type();

		[[nodiscard]] base_type* clone() const noexcept;
		[[nodiscard]] void accept(base_type_visitor* const visitor) const override;

		[[nodiscard]] const base_type* type() const noexcept;
		[[nodiscard]] size_t length() const noexcept;

		[[nodiscard]] std::string toString() const noexcept override;

		[[nodiscard]] bool operator==(const array_type& other) const noexcept;
		[[nodiscard]] bool operator!=(const array_type& other) const noexcept;
	};

	[[nodiscard]] static std::string to_string(const array_type& value) noexcept;
}