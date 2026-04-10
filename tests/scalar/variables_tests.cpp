#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(scalar_layout, constructor_and_get) {
	size_t ret;
	std::vector<size_t> retVec;
	std::vector<size_t> resultVec;

#pragma region FStruct
	// clang-format off
	scalar_layout fStructLayout {
		glsl_variable<int>("d"),
		glsl_variable<bvec2>("e")
	};
	// clang-format on
	ret = fStructLayout.get_offset("d");
	EXPECT_EQ(ret, 0);
	ret = fStructLayout.get_offset("e");
	EXPECT_EQ(ret, 4);
	ret = fStructLayout.size();
	EXPECT_EQ(ret, 12);
#pragma endregion

#pragma region OStruct
	// clang-format off
	scalar_layout oStructLayout {
		glsl_variable<uvec3>("j"),
		glsl_variable<vec2>("k"),
		glsl_variable<float, 2>("l"),
		glsl_variable<vec2>("m"),
		glsl_variable<mat3, 2>("n")
	};
	// clang-format on
	ret = oStructLayout.get_offset("j");
	EXPECT_EQ(ret, 0);
	ret = oStructLayout.get_offset("k");
	EXPECT_EQ(ret, 12);
	resultVec = { 20, 24 };
	retVec	  = oStructLayout.get_array_offsets("l");
	EXPECT_EQ(retVec, resultVec);
	ret = oStructLayout.get_offset("m");
	EXPECT_EQ(ret, 28);
	resultVec = { 36, 72 };
	retVec	  = oStructLayout.get_array_offsets("n");
	EXPECT_EQ(retVec, resultVec);
	ret = oStructLayout.size();
	EXPECT_EQ(ret, 108);
#pragma endregion

#pragma region UBO
	// clang-format off
	scalar_layout uboLayout {
		glsl_variable<float>("a"),
		glsl_variable<vec2>("b"),
		glsl_variable<vec3>("c"),
		glsl_variable<scalar_layout>("f", fStructLayout),
		glsl_variable<float>("g"),
		glsl_variable<float, 2>("h"),
		glsl_variable<mat2x3>("i"),
		glsl_variable<scalar_layout, 2>("o", oStructLayout)
	};
	// clang-format on
	ret = uboLayout.get_offset("a");
	EXPECT_EQ(ret, 0);
	ret = uboLayout.get_offset("b");
	EXPECT_EQ(ret, 4);
	ret = uboLayout.get_offset("c");
	EXPECT_EQ(ret, 12);
	ret = uboLayout.get_offset("f");
	EXPECT_EQ(ret, 24);

	// F sub values
	ret = uboLayout.get_offset("f.d");
	EXPECT_EQ(ret, 24);
	ret = uboLayout.get_offset("f.e");
	EXPECT_EQ(ret, 28);

	ret = uboLayout.get_offset("g");
	EXPECT_EQ(ret, 36);
	resultVec = { 40, 44 };
	retVec	  = uboLayout.get_array_offsets("h");
	EXPECT_EQ(retVec, resultVec);
	ret = uboLayout.get_offset("i");
	EXPECT_EQ(ret, 48);
	resultVec = { 72, 180 };
	retVec	  = uboLayout.get_array_offsets("o");
	EXPECT_EQ(retVec, resultVec);

	// O1 sub values
	ret = uboLayout.get_offset("o[0].j");
	EXPECT_EQ(ret, 72);
	ret = uboLayout.get_offset("o[0].k");
	EXPECT_EQ(ret, 84);
	retVec	  = uboLayout.get_array_offsets("o[0].l");
	resultVec = { 92, 96 };
	EXPECT_EQ(retVec, resultVec);
	ret = uboLayout.get_offset("o[0].m");
	EXPECT_EQ(ret, 100);
	retVec	  = uboLayout.get_array_offsets("o[0].n");
	resultVec = { 108, 144 };
	EXPECT_EQ(retVec, resultVec);

	// O2 sub values
	ret = uboLayout.get_offset("o[1].j");
	EXPECT_EQ(ret, 180);
	ret = uboLayout.get_offset("o[1].k");
	EXPECT_EQ(ret, 192);
	retVec	  = uboLayout.get_array_offsets("o[1].l");
	resultVec = { 200, 204 };
	EXPECT_EQ(retVec, resultVec);
	ret = uboLayout.get_offset("o[1].m");
	EXPECT_EQ(ret, 208);
	retVec	  = uboLayout.get_array_offsets("o[1].n");
	resultVec = { 216, 252 };
	EXPECT_EQ(retVec, resultVec);

	ret = uboLayout.size();
	EXPECT_EQ(ret, 288);
#pragma endregion
}