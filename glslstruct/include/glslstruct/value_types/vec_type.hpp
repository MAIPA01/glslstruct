#pragma once
#include <glslstruct/value_types/value_type.hpp>
#include <glslstruct/value_types/ValueType.hpp>

namespace glslstruct {
	class vec_type : public value_type<vec_type> {
	private:
		ValueType _type = ValueType::Other;
		size_t _length = 0;

	public:
		vec_type(const ValueType& type, const size_t& length);
		vec_type(const vec_type& other) = default;
		virtual ~vec_type() = default;

		[[nodiscard]] base_type* clone() const noexcept override;
		[[nodiscard]] void accept(base_type_visitor* const visitor) const override;

		[[nodiscard]] ValueType type() const noexcept;
		[[nodiscard]] size_t length() const noexcept;

		[[nodiscard]] std::string toString() const noexcept override;

		[[nodiscard]] bool operator==(const vec_type& other) const noexcept;
		[[nodiscard]] bool operator!=(const vec_type& other) const noexcept;
	};

	[[nodiscard]] static std::string to_string(const vec_type& value) noexcept;

}