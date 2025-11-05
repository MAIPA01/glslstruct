#pragma once
#include <glslstruct/value_types/value_type.hpp>
#include <glslstruct/value_types/ValueType.hpp>

namespace glslstruct {
	class mat_type : public value_type<mat_type> {
	private:
		ValueType _type = ValueType::Other;
		size_t _cols = 0;
		size_t _rows = 0;

	public:
		mat_type(const ValueType& type, const size_t& cols, const size_t& rows);
		mat_type(const mat_type& other) = default;
		virtual ~mat_type() = default;

		[[nodiscard]] base_type* clone() const noexcept;

		[[nodiscard]] ValueType getType() const noexcept;
		[[nodiscard]] size_t getRows() const noexcept;
		[[nodiscard]] size_t getCols() const noexcept;

		[[nodiscard]] std::string toString() const noexcept override;

		[[nodiscard]] bool operator==(const mat_type& other) const noexcept {
			return _type == other._type && _cols == other._cols && _rows == other._rows;
		}
		[[nodiscard]] bool operator!=(const mat_type& other) const noexcept {
			return !(*this == other);
		}
	};

	[[nodiscard]] static std::string to_string(const mat_type*& value) noexcept;

}