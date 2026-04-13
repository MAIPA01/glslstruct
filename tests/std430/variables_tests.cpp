#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(std430_layout, constructor_and_get) {
	size_t ret;
	std::vector<size_t> retVec;
	std::vector<size_t> resultVec;

#pragma region FStruct
	std140_layout fStructLayout { glsl_variable<int>("d"), glsl_variable<bvec2>("e") };
	ret = fStructLayout.get_offset("d");
	EXPECT_EQ(ret, 0);
	ret = fStructLayout.get_offset("e");
	EXPECT_EQ(ret, 8);
	ret = fStructLayout.size();
	EXPECT_EQ(ret, 16);
#pragma endregion

#pragma region OStruct
	std140_layout oStructLayout { glsl_variable<uvec3>("j"), glsl_variable<vec2>("k"), glsl_variable<float, 2>("l"),
		glsl_variable<vec2>("m"), glsl_variable<mat3, 2>("n") };
	ret = oStructLayout.get_offset("j");
	EXPECT_EQ(ret, 0);
	ret = oStructLayout.get_offset("k");
	EXPECT_EQ(ret, 16);
	resultVec = { 32, 48 };
	retVec	  = oStructLayout.get_array_offsets("l");
	EXPECT_EQ(retVec, resultVec);
	ret = oStructLayout.get_offset("m");
	EXPECT_EQ(ret, 64);
	resultVec = { 80, 128 };
	retVec	  = oStructLayout.get_array_offsets("n");
	EXPECT_EQ(retVec, resultVec);
	ret = oStructLayout.size();
	EXPECT_EQ(ret, 176);
#pragma endregion

#pragma region UBO
	std140_layout uboLayout { glsl_variable<float>("a"), glsl_variable<vec2>("b"), glsl_variable<vec3>("c"),
		glsl_variable<std140_layout>("f", fStructLayout), glsl_variable<float>("g"), glsl_variable<float, 2>("h"),
		glsl_variable<mat2x3>("i"), glsl_variable<std140_layout, 2>("o", oStructLayout) };
	ret = uboLayout.get_offset("a");
	EXPECT_EQ(ret, 0);
	ret = uboLayout.get_offset("b");
	EXPECT_EQ(ret, 8);
	ret = uboLayout.get_offset("c");
	EXPECT_EQ(ret, 16);
	ret = uboLayout.get_offset("f");
	EXPECT_EQ(ret, 32);

	// F sub values
	ret = uboLayout.get_offset("f.d");
	EXPECT_EQ(ret, 32);
	ret = uboLayout.get_offset("f.e");
	EXPECT_EQ(ret, 40);

	ret = uboLayout.get_offset("g");
	EXPECT_EQ(ret, 48);
	resultVec = { 64, 80 };
	retVec	  = uboLayout.get_array_offsets("h");
	EXPECT_EQ(retVec, resultVec);
	ret = uboLayout.get_offset("i");
	EXPECT_EQ(ret, 96);
	resultVec = { 128, 304 };
	retVec	  = uboLayout.get_array_offsets("o");
	EXPECT_EQ(retVec, resultVec);

	// O1 sub values
	ret = uboLayout.get_offset("o[0].j");
	EXPECT_EQ(ret, 128);
	ret = uboLayout.get_offset("o[0].k");
	EXPECT_EQ(ret, 144);
	retVec	  = uboLayout.get_array_offsets("o[0].l");
	resultVec = { 160, 176 };
	EXPECT_EQ(retVec, resultVec);
	ret = uboLayout.get_offset("o[0].m");
	EXPECT_EQ(ret, 192);
	retVec	  = uboLayout.get_array_offsets("o[0].n");
	resultVec = { 208, 256 };
	EXPECT_EQ(retVec, resultVec);

	// O2 sub values
	ret = uboLayout.get_offset("o[1].j");
	EXPECT_EQ(ret, 304);
	ret = uboLayout.get_offset("o[1].k");
	EXPECT_EQ(ret, 320);
	retVec	  = uboLayout.get_array_offsets("o[1].l");
	resultVec = { 336, 352 };
	EXPECT_EQ(retVec, resultVec);
	ret = uboLayout.get_offset("o[1].m");
	EXPECT_EQ(ret, 368);
	retVec	  = uboLayout.get_array_offsets("o[1].n");
	resultVec = { 384, 432 };
	EXPECT_EQ(retVec, resultVec);

	ret = uboLayout.size();
	EXPECT_EQ(ret, 480);
#pragma endregion
}