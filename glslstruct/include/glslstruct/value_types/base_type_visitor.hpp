#pragma once

namespace glslstruct {
	class scalar_type;
	class vec_type;
	class mat_type;
	class struct_type;
	class array_type;

	class base_type_visitor {
	public:
		virtual void visit(const scalar_type& value) = 0;
		virtual void visit(const vec_type& value) = 0;
		virtual void visit(const mat_type& value) = 0;
		virtual void visit(const struct_type& value) = 0;
		virtual void visit(const array_type& value) = 0;
	};
}