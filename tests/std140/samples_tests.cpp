#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(std140_layout, offset_calculation_1) {
	size_t ret;
	std::vector<std::vector<size_t>> tempVec;
	std::vector<size_t> retVec;
	std::vector<size_t> resultVec;

#pragma region fStructLayout
	std140_layout fStructLayout;

	// Add int (size: 4, alignment: 4)
	ret = fStructLayout.add<int>("d");
	EXPECT_EQ(ret, 0);
	ret = fStructLayout.size();
	EXPECT_EQ(ret, 4);

	// Add bvec2 (size: 8, alignment: 8)
	ret = fStructLayout.add<bvec2>("e");
	EXPECT_EQ(ret, 8);
	ret = fStructLayout.size();
	EXPECT_EQ(ret, 16);
	ret = fStructLayout.base_alignment();
	EXPECT_EQ(ret, 16);
#pragma endregion

#pragma region oStructLayout
	std140_layout oStructLayout;

	// Add uvec3 (size: 12, alignment: 16)
	ret = oStructLayout.add<uvec3>("j");
	EXPECT_EQ(ret, 0);
	ret = oStructLayout.size();
	EXPECT_EQ(ret, 12);

	// Add vec2 (size: 8, alignment: 8)
	ret = oStructLayout.add<vec2>("k");
	EXPECT_EQ(ret, 16);
	ret = oStructLayout.size();
	EXPECT_EQ(ret, 24);

	// Add float array (count: 2, size: 20, padding: 12, alignment: 16)
	resultVec = { 32, 48 };
	retVec	  = oStructLayout.add<float>("l", 2);
	EXPECT_EQ(retVec, resultVec);
	ret = oStructLayout.size();
	EXPECT_EQ(ret, 64);

	// Add vec2 (size: 8, alignment: 8)
	ret = oStructLayout.add<vec2>("m");
	EXPECT_EQ(ret, 64);
	ret = oStructLayout.size();
	EXPECT_EQ(ret, 72);

	// Add mat3 array (count: 2, size: 92, padding: 4, alignment: 16)
	resultVec = { 80, 128 };
	tempVec	  = oStructLayout.add<mat3>("n", 2);

	retVec.clear();
		for (size_t i = 0; i < tempVec.size(); i++) { retVec.push_back(tempVec[i].front()); }

	EXPECT_EQ(retVec, resultVec);
	ret = oStructLayout.size();
	EXPECT_EQ(ret, 176);
#pragma endregion

#pragma region UBO
	std140_layout uboLayout;

	// Add float (size: 4, alignment: 4)
	ret = uboLayout.add<float>("a");
	EXPECT_EQ(ret, 0);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 4);

	// Add vec2 (size: 8, alignment: 8)
	ret = uboLayout.add<vec2>("b");
	EXPECT_EQ(ret, 8);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 16);

	// Add vec3 (size: 12, alignment: 16)
	ret = uboLayout.add<vec3>("c");
	EXPECT_EQ(ret, 16);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 28);

	// Add subStruct (size: 16, alignment: 16)
	ret = uboLayout.add("f", fStructLayout);
	EXPECT_EQ(ret, 32);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 48);

	// Add float (size: 4, alignment: 4)
	ret = uboLayout.add<float>("g");
	EXPECT_EQ(ret, 48);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 52);

	// Add float array (count: 2, size: 20, padding: 12, alignment: 16)
	resultVec = { 64, 80 };
	retVec	  = uboLayout.add<float>("h", 2);
	EXPECT_EQ(retVec, resultVec);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 96);

	// Add mat2x3 (size: 28, padding: 4, alignment: 16)
	ret = uboLayout.add<mat2x3>("i").front();
	EXPECT_EQ(ret, 96);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 128);

	// Add sub struct array (count: 2, size: 352, padding: 0, alignment: 16)
	resultVec = { 128, 304 };
	retVec	  = uboLayout.add("o", oStructLayout, 2);
	EXPECT_EQ(retVec, resultVec);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 480);
#pragma endregion
}

TEST(std140_layout, offset_calculation_2) {
	size_t ret;

#pragma region Material
	std140_layout materialLayout;

	// Add bool (size: 4, alignment: 4)
	ret = materialLayout.add<bool>("has_diffuse_texture");
	EXPECT_EQ(ret, 0);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 4);

	// Add bool (size: 4, alignment: 4)
	ret = materialLayout.add<bool>("has_specular_texture");
	EXPECT_EQ(ret, 4);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 8);

	// Add vec3 (size: 12, alignment: 16)
	ret = materialLayout.add<vec3>("color");
	EXPECT_EQ(ret, 16);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 28);

	// Add float (size: 4, alignment: 4)
	ret = materialLayout.add<float>("shininess");
	EXPECT_EQ(ret, 28);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 32);

	// Add uint (size: 4, alignment: 4)
	ret = materialLayout.add<unsigned int>("diffuse_toon_borders");
	EXPECT_EQ(ret, 32);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 36);

	// Add uint (size: 4, alignment: 4)
	ret = materialLayout.add<unsigned int>("specular_toon_borders");
	EXPECT_EQ(ret, 36);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 40);

	// Add vec2 (size: 8, alignment: 8)
	ret = materialLayout.add<vec2>("highlight_translate");
	EXPECT_EQ(ret, 40);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 48);

	// Add vec2 (size: 8, alignment: 8)
	ret = materialLayout.add<vec2>("highlight_rotation");
	EXPECT_EQ(ret, 48);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 56);

	// Add vec2 (size: 8, alignment: 8)
	ret = materialLayout.add<vec2>("highlight_scale");
	EXPECT_EQ(ret, 56);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 64);

	// Add vec2 (size: 8, alignment: 8)
	ret = materialLayout.add<vec2>("highlight_split");
	EXPECT_EQ(ret, 64);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 72);

	// Add int (size: 4, alignment: 4)
	ret = materialLayout.add<int>("highlight_square_n");
	EXPECT_EQ(ret, 72);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 76);

	// Add float (size: 4, alignment: 4)
	ret = materialLayout.add<float>("highlight_square_x");
	EXPECT_EQ(ret, 76);
	ret = materialLayout.size();
	EXPECT_EQ(ret, 80);
#pragma endregion

	std140_layout uboLayout;
	std::vector<size_t> resultVec { 0, 80, 160, 240, 320, 400, 480, 560 };
	std::vector<size_t> retVec = uboLayout.add("materialInputs", materialLayout, 8);
	EXPECT_EQ(retVec, resultVec);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 640);
}

TEST(std140_layout, offset_calculation_3) {
	std140_layout uboLayout;

	// Add vec2 (size: 8, alignment: 8)
	size_t ret = uboLayout.add<vec2>("windowSize");
	EXPECT_EQ(ret, 0);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 8);

	// Add float (size: 4, alignment: 4)
	ret = uboLayout.add<float>("nearPlane");
	EXPECT_EQ(ret, 8);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 12);

	// Add float (size: 4, alignment: 4)
	ret = uboLayout.add<float>("farPlane");
	EXPECT_EQ(ret, 12);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 16);

	// Add float (size: 4, alignment: 4)
	ret = uboLayout.add<float>("gamma");
	EXPECT_EQ(ret, 16);
	ret = uboLayout.size();
	EXPECT_EQ(ret, 20);
}