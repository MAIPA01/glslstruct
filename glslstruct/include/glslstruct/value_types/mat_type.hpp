#pragma once
#include <glslstruct/value_types/value_type.hpp>
#include <glslstruct/value_types/ValueType.hpp>

namespace glslstruct {
	class mat_type : public value_type<mat_type> {
	private:
		friend struct std::hash<mat_type>;

		ValueType _type = ValueType::Other;
		size_t _cols = 0;
		size_t _rows = 0;

	public:
		mat_type(const ValueType& type, const size_t& cols, const size_t& rows);
		mat_type(const mat_type& other) = default;
		virtual ~mat_type() = default;

		[[nodiscard]] base_type* clone() const noexcept override;
		[[nodiscard]] void accept(base_type_visitor* const visitor) const override;

		[[nodiscard]] ValueType type() const noexcept;
		[[nodiscard]] size_t rows() const noexcept;
		[[nodiscard]] size_t cols() const noexcept;

		[[nodiscard]] std::string toString() const noexcept override;

		[[nodiscard]] bool operator==(const mat_type& other) const noexcept;
		[[nodiscard]] bool operator!=(const mat_type& other) const noexcept;
	};

	[[nodiscard]] static std::string to_string(const mat_type& value) noexcept;

}