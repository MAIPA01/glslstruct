#pragma once
#include <string>
#include <glslstruct/value_types/base_type_visitor.hpp>

namespace glslstruct {
	class base_type {
	protected:
		base_type() = default;
	public:
		base_type(const base_type& other) = default;
		virtual ~base_type() = default;

		[[nodiscard]] virtual base_type* clone() const noexcept = 0;
		[[nodiscard]] virtual void accept(base_type_visitor* const visitor) const = 0;

		[[nodiscard]] virtual bool operator==(const base_type& other) const noexcept = 0;
		[[nodiscard]] virtual bool operator!=(const base_type& other) const noexcept;

		[[nodiscard]] virtual bool operator==(const scalar_type& other) const noexcept = 0;
		[[nodiscard]] virtual bool operator!=(const scalar_type& other) const noexcept;

		[[nodiscard]] virtual bool operator==(const vec_type& other) const noexcept = 0;
		[[nodiscard]] virtual bool operator!=(const vec_type& other) const noexcept;

		[[nodiscard]] virtual bool operator==(const mat_type& other) const noexcept = 0;
		[[nodiscard]] virtual bool operator!=(const mat_type& other) const noexcept;

		[[nodiscard]] virtual bool operator==(const struct_type& other) const noexcept = 0;
		[[nodiscard]] virtual bool operator!=(const struct_type& other) const noexcept;

		[[nodiscard]] virtual bool operator==(const array_type& other) const noexcept = 0;
		[[nodiscard]] virtual bool operator!=(const array_type& other) const noexcept;

		[[nodiscard]] virtual std::string toString() const noexcept = 0;
	};

	[[nodiscard]] static std::string to_string(const base_type* value) noexcept;
}