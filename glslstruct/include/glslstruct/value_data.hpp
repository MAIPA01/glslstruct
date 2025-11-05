#pragma once
namespace glslstruct {
	class base_type;

	struct value_data {
		size_t offset;
		const base_type* type;
	};
}