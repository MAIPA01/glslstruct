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

		[[nodiscard]] base_type* clone() const noexcept;

		[[nodiscard]] ValueType getType() const noexcept;
		[[nodiscard]] size_t getLength() const noexcept;

		[[nodiscard]] std::string toString() const noexcept override;

		[[nodiscard]] bool operator==(const vec_type& other) const noexcept {
			return _type == other._type && _length == other._length;
		}
		[[nodiscard]] bool operator!=(const vec_type& other) const noexcept {
			return !(*this == other);
		}
	};

	[[nodiscard]] static std::string to_string(const vec_type*& value) noexcept;

}