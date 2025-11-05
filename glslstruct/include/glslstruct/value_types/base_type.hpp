#pragma once
#include <string>

namespace glslstruct {
	class scalar_type;
	class vec_type;
	class mat_type;
	class struct_type;
	class array_type;

	class base_type {
	protected:
		base_type() = default;
	public:
		base_type(const base_type& other) = default;
		virtual ~base_type() = default;

		[[nodiscard]] virtual base_type* clone() const noexcept = 0;

		[[nodiscard]] virtual bool visit_equal(const base_type*& other) const noexcept = 0;
		[[nodiscard]] virtual bool visit_equal(const scalar_type*& other) const noexcept = 0;
		[[nodiscard]] virtual bool visit_equal(const vec_type*& other) const noexcept = 0;
		[[nodiscard]] virtual bool visit_equal(const mat_type*& other) const noexcept = 0;
		[[nodiscard]] virtual bool visit_equal(const struct_type*& other) const noexcept = 0;
		[[nodiscard]] virtual bool visit_equal(const array_type*& other) const noexcept = 0;

		[[nodiscard]] virtual std::string toString() const noexcept = 0;

		[[nodiscard]] virtual bool operator==(const base_type* other) const noexcept = 0;
		[[nodiscard]] virtual bool operator!=(const base_type* other) const noexcept = 0;
	};

	[[nodiscard]] static std::string to_string(const base_type*& value) noexcept;
}