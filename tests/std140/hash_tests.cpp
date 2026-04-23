#include <pch.hpp>

using namespace glslstruct;

TEST(std140_layout, hash) {
	std140_layout layout;
	layout.add<int>("a");

	const size_t hash = std::hash<std140_layout> {}(layout);
	ASSERT_NE(hash, 0);
}

TEST(std140_struct, hash) {
	std140_struct structure;
	structure.add<int>("a");

	const size_t hash = std::hash<std140_struct> {}(structure);
	ASSERT_NE(hash, 0);
}