#pragma once
#include <functional>
#include <mstd/hash.hpp>

namespace glslstruct {
	class base_type;

	struct value_data {
		size_t offset;
		const base_type* type;

		bool operator==(const value_data& other) const;
		bool operator!=(const value_data& other) const;
	};
}

namespace std {
	template<>
	struct hash<glslstruct::value_data> {
		size_t operator()(const glslstruct::value_data& value) const;
	};
}