#pragma once
#include <glslstruct/value_types/base_type_visitor.hpp>
#include <glslstruct/value_types/base_type.hpp>
#include <functional>

namespace std {
	template<>
	struct hash<glslstruct::base_type> {
		size_t operator()(const glslstruct::base_type& value);
	};

	template<>
	struct hash<glslstruct::scalar_type> {
		size_t operator()(const glslstruct::scalar_type& value);
	};

	template<>
	struct hash<glslstruct::vec_type> {
		size_t operator()(const glslstruct::vec_type& value);
	};

	template<>
	struct hash<glslstruct::mat_type> {
		size_t operator()(const glslstruct::mat_type& value);
	};

	template<>
	struct hash<glslstruct::struct_type> {
		size_t operator()(const glslstruct::struct_type& value);
	};

	template<>
	struct hash<glslstruct::array_type> {
		size_t operator()(const glslstruct::array_type& value);
	};
}

namespace glslstruct {
	class type_hash_visitor : public base_type_visitor {
	private:
		size_t _result = 0;

	public:
		type_hash_visitor() = default;
		virtual ~type_hash_visitor() = default;

		void visit(const scalar_type& value) override;
		void visit(const vec_type& value) override;
		void visit(const mat_type& value) override;
		void visit(const struct_type& value) override;
		void visit(const array_type& value) override;

		size_t result() const noexcept;
	};
}