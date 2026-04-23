#include <pch.hpp>

using namespace glslstruct;

TEST(scalar_layout, hash) {
	scalar_layout layout;
	layout.add<int>("a");

	const size_t hash = std::hash<scalar_layout> {}(layout);
	ASSERT_NE(hash, 0);
}

TEST(scalar_struct, hash) {
	scalar_struct structure;
	structure.add<int>("a");

	const size_t hash = std::hash<scalar_struct> {}(structure);
	ASSERT_NE(hash, 0);
}