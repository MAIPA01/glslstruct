#include "pch.hpp"

using namespace glslstruct;
using namespace glm;

TEST(std140_offset, offset_calculation_1) {
	std140_offset structOffsets;
	std140_offset subStructOffsets;
	size_t ret;
	std::vector<size_t> retVec;
	std::vector<size_t> resultVec;

	ret = structOffsets.add<float>("a");
	EXPECT_EQ(ret, 0);
	ret = structOffsets.add<vec2>("b");
	EXPECT_EQ(ret, 8);
	ret = structOffsets.add<vec3>("c");
	EXPECT_EQ(ret, 16);

	ret = subStructOffsets.add<int>("d");
	EXPECT_EQ(ret, 0);
	ret = subStructOffsets.add<bvec2>("e");
	EXPECT_EQ(ret, 8);
	ret = subStructOffsets.size();
	EXPECT_EQ(ret, 16);

	ret = structOffsets.add("f", subStructOffsets);
	EXPECT_EQ(ret, 32);
	ret = structOffsets.add<float>("g");
	EXPECT_EQ(ret, 48);
	resultVec = { 64, 80 };
	retVec = structOffsets.add<float>("h", 2);
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.add<mat2x3>("i");
	EXPECT_EQ(ret, 96);

	subStructOffsets.clear();
	ret = subStructOffsets.add<uvec3>("j");
	EXPECT_EQ(ret, 0);
	ret = subStructOffsets.add<vec2>("k");
	EXPECT_EQ(ret, 16);
	resultVec = { 32, 48 };
	retVec = subStructOffsets.add<float>("l", 2);
	EXPECT_EQ(retVec, resultVec);
	ret = subStructOffsets.add<vec2>("m");
	EXPECT_EQ(ret, 64);
	resultVec = { 80, 128 };
	retVec = subStructOffsets.add<mat3>("n", 2);
	EXPECT_EQ(retVec, resultVec);
	ret = subStructOffsets.size();
	EXPECT_EQ(ret, 176);

	resultVec = { 128, 304 };
	retVec = structOffsets.add("o", subStructOffsets, 2);
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.size();
	EXPECT_EQ(ret, 480);
}

TEST(std140_offset, offset_calculation_2) {
	std140_offset structOffsets;
	std140_offset subStructOffsets;
	size_t ret;

	ret = subStructOffsets.add<bool>("has_diffuse_texture");
	EXPECT_EQ(ret, 0);
	ret = subStructOffsets.add<bool>("has_specular_texture");
	EXPECT_EQ(ret, 4);
	ret = subStructOffsets.add<vec3>("color");
	EXPECT_EQ(ret, 16);
	ret = subStructOffsets.add<float>("shininess");
	EXPECT_EQ(ret, 28);
	ret = subStructOffsets.add<unsigned int>("diffuse_toon_borders");
	EXPECT_EQ(ret, 32);
	ret = subStructOffsets.add<unsigned int>("specular_toon_borders");
	EXPECT_EQ(ret, 36);
	ret = subStructOffsets.add<vec2>("highlight_translate");
	EXPECT_EQ(ret, 40);
	ret = subStructOffsets.add<vec2>("highlight_rotation");
	EXPECT_EQ(ret, 48);
	ret = subStructOffsets.add<vec2>("highlight_scale");
	EXPECT_EQ(ret, 56);
	ret = subStructOffsets.add<vec2>("highlight_split");
	EXPECT_EQ(ret, 64);
	ret = subStructOffsets.add<int>("highlight_square_n");
	EXPECT_EQ(ret, 72);
	ret = subStructOffsets.add<float>("highlight_square_x");
	EXPECT_EQ(ret, 76);
	ret = subStructOffsets.size();
	EXPECT_EQ(ret, 80);

	std::vector<size_t> resultVec{ 0, 80, 160, 240, 320, 400, 480, 560 };
	std::vector<size_t> retVec = structOffsets.add("materialInputs", subStructOffsets, 8);
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.size();
	EXPECT_EQ(ret, 640);
}

TEST(std140_offset, offset_calculation_3) {
	std140_offset structOffsets;
	size_t ret;

	ret = structOffsets.add<vec2>("windowSize");
	EXPECT_EQ(ret, 0);
	ret = structOffsets.add<float>("nearPlane");
	EXPECT_EQ(ret, 8);
	ret = structOffsets.add<float>("farPlane");
	EXPECT_EQ(ret, 12);
	ret = structOffsets.add<float>("gamma");
	EXPECT_EQ(ret, 16);
	ret = structOffsets.size();
	EXPECT_EQ(ret, 32);
}

TEST(std430_offset, offset_calculation_1) {
	size_t ret;
	std::vector<size_t> retVec;

	std430_offset rect;
	ret = rect.add<mat4>("transform");
	EXPECT_EQ(ret, 0);
	ret = rect.add<vec2>("size");
	EXPECT_EQ(ret, 64);
	ret = rect.size();
	EXPECT_EQ(ret, 80);

	std430_offset sprite;
	ret = sprite.add<uvec2>("offset");
	EXPECT_EQ(ret, 0);
	ret = sprite.add<uvec2>("size");
	EXPECT_EQ(ret, 8);
	ret = sprite.add<bool>("isActive");
	EXPECT_EQ(ret, 16);
	ret = sprite.size();
	EXPECT_EQ(ret, 32);

	std430_offset fill;
	ret = fill.add<unsigned int>("type");
	EXPECT_EQ(ret, 0);
	ret = fill.add<unsigned int>("subType");
	EXPECT_EQ(ret, 4);
	ret = fill.add<float>("offset");
	EXPECT_EQ(ret, 8);
	ret = fill.add<float>("progress");
	EXPECT_EQ(ret, 12);
	ret = fill.add<float>("rotation");
	EXPECT_EQ(ret, 16);
	ret = fill.add<bool>("isActive");
	EXPECT_EQ(ret, 20);
	ret = fill.size();
	EXPECT_EQ(ret, 32);

	std430_offset uiElement;
	ret = uiElement.add("rect", rect);
	EXPECT_EQ(ret, 0);
	ret = uiElement.add("sprite", sprite);
	EXPECT_EQ(ret, 80);
	ret = uiElement.add("fill", fill);
	EXPECT_EQ(ret, 112);
	ret = uiElement.add<vec4>("color");
	EXPECT_EQ(ret, 144);
	ret = uiElement.add<bool>("isText");
	EXPECT_EQ(ret, 160);
	ret = uiElement.size();
	EXPECT_EQ(ret, 176);

	std430_offset texture;
	ret = texture.add<uvec2>("size");
	EXPECT_EQ(ret, 0);
	ret = texture.add<bool>("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.size();
	EXPECT_EQ(ret, 16);

	std430_offset ssbo;
	std::vector<size_t> resultVec{ 0, 176, 352, 528, 704, 880, 1056, 1232 };
	retVec = ssbo.add("uiElements", uiElement, 8);
	EXPECT_EQ(retVec, resultVec);
	ret = ssbo.add("elementTexture", texture);
	EXPECT_EQ(ret, 1408);
	ret = ssbo.add<int>("elementLayer");
	EXPECT_EQ(ret, 1424);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1440);
}

TEST(std430_offset, offset_calculation_2) {
	size_t ret;
	std::vector<size_t> retVec;

	std430_offset pointLight;
	ret = pointLight.add<vec3>("position");
	EXPECT_EQ(ret, 0);
	ret = pointLight.add<vec3>("color");
	EXPECT_EQ(ret, 16);
	ret = pointLight.add<float>("power");
	EXPECT_EQ(ret, 28);
	ret = pointLight.add<float>("constant");
	EXPECT_EQ(ret, 32);
	ret = pointLight.add<float>("linear");
	EXPECT_EQ(ret, 36);
	ret = pointLight.add<float>("quadratic");
	EXPECT_EQ(ret, 40);
	ret = pointLight.size();
	EXPECT_EQ(ret, 48);

	std430_offset spotLight;
	ret = spotLight.add<vec3>("position");
	EXPECT_EQ(ret, 0);
	ret = spotLight.add<vec3>("direction");
	EXPECT_EQ(ret, 16);
	ret = spotLight.add<float>("power");
	EXPECT_EQ(ret, 28);
	ret = spotLight.add<vec3>("color");
	EXPECT_EQ(ret, 32);
	ret = spotLight.add<float>("cutOff");
	EXPECT_EQ(ret, 44);
	ret = spotLight.add<float>("outerCutOff");
	EXPECT_EQ(ret, 48);
	ret = spotLight.add<float>("constant");
	EXPECT_EQ(ret, 52);
	ret = spotLight.add<float>("linear");
	EXPECT_EQ(ret, 56);
	ret = spotLight.add<float>("quadratic");
	EXPECT_EQ(ret, 60);
	ret = spotLight.size();
	EXPECT_EQ(ret, 64);

	std430_offset dirLight;
	ret = dirLight.add<vec3>("direction");
	EXPECT_EQ(ret, 0);
	ret = dirLight.add<vec3>("color");
	EXPECT_EQ(ret, 16);
	ret = dirLight.add<mat4>("lightSpaceMatrix");
	EXPECT_EQ(ret, 32);
	ret = dirLight.add<float>("power");
	EXPECT_EQ(ret, 96);
	ret = dirLight.add<float>("padding1");
	EXPECT_EQ(ret, 100);
	ret = dirLight.add<float>("padding2");
	EXPECT_EQ(ret, 104);
	ret = dirLight.add<float>("padding3");
	EXPECT_EQ(ret, 108);
	ret = dirLight.size();
	EXPECT_EQ(ret, 112);

	std430_offset ssbo;
	std::vector<size_t> resultVec;
	ret = ssbo.add<unsigned int>("numberOfPointLights");
	EXPECT_EQ(ret, 0);
	ret = ssbo.add<unsigned int>("numberOfSpotLights");
	EXPECT_EQ(ret, 4);
	ret = ssbo.add<unsigned int>("numberOfDirLights");
	EXPECT_EQ(ret, 8);
	retVec = ssbo.add("pointLights", pointLight, 8);
	resultVec = { 16, 64, 112, 160, 208, 256, 304, 352 };
	EXPECT_EQ(retVec, resultVec);
	retVec = ssbo.add("spotLights", spotLight, 8);
	resultVec = { 400, 464, 528, 592, 656, 720, 784, 848 };
	EXPECT_EQ(retVec, resultVec);
	retVec = ssbo.add("dirLights", dirLight, 4);
	resultVec = { 912, 1024, 1136, 1248 };
	EXPECT_EQ(retVec, resultVec);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1360);
}