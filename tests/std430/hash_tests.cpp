#include <pch.hpp>

using namespace glslstruct;

TEST(std430_layout, hash) {
	std430_layout layout;
	layout.add<int>("a");

	const size_t hash = std::hash<std430_layout> {}(layout);
	ASSERT_NE(hash, 0);
}

TEST(std430_struct, hash) {
	std430_struct structure;
	structure.add<int>("a");

	const size_t hash = std::hash<std430_struct> {}(structure);
	ASSERT_NE(hash, 0);
}