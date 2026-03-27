#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(std140_layout, offset_calculation_1) {
	std140_layout structLayout;
	std140_layout subStructLayout;
	size_t ret;
	std::vector<std::vector<size_t>> tempVec;
	std::vector<size_t> retVec;
	std::vector<size_t> resultVec;

	// Add float (size: 4, alignment: 4)
	ret = structLayout.add<float>("a");
	EXPECT_EQ(ret, 0);
	ret = structLayout.size();
	EXPECT_EQ(ret, 16);

	// Add vec2 (size: 8, alignment: 8)
	ret = structLayout.add<vec2>("b");
	EXPECT_EQ(ret, 8);
	ret = structLayout.size();
	EXPECT_EQ(ret, 16);

	// Add vec3 (size: 12, alignment: 16)
	ret = structLayout.add<vec3>("c");
	EXPECT_EQ(ret, 16);
	ret = structLayout.size();
	EXPECT_EQ(ret, 32);

	// Add int (size: 4, alignment: 4)
	ret = subStructLayout.add<int>("d");
	EXPECT_EQ(ret, 0);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 16);

	// Add bvec2 (size: 8, alignment: 8)
	ret = subStructLayout.add<bvec2>("e");
	EXPECT_EQ(ret, 8);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 16);
	ret = subStructLayout.base_alignment();
	EXPECT_EQ(ret, 16);

	// Add subStruct (size: 16, alignment: 16)
	ret = structLayout.add("f", subStructLayout);
	EXPECT_EQ(ret, 32);
	ret = structLayout.size();
	EXPECT_EQ(ret, 48);

	// Add float (size: 4, alignment: 4)
	ret = structLayout.add<float>("g");
	EXPECT_EQ(ret, 48);
	ret = structLayout.size();
	EXPECT_EQ(ret, 64);

	// Add float array (count: 2, size: 20, padding: 12, alignment: 16)
	resultVec = { 64, 80 };
	retVec	  = structLayout.add<float>("h", 2);
	EXPECT_EQ(retVec, resultVec);
	ret = structLayout.size();
	EXPECT_EQ(ret, 96);

	// Add mat2x3 (size: 28, padding: 4, alignment: 16)
	ret = structLayout.add<mat2x3>("i").front();
	EXPECT_EQ(ret, 96);
	ret = structLayout.size();
	EXPECT_EQ(ret, 128);

	subStructLayout.clear();

	// Add uvec3 (size: 12, alignment: 16)
	ret = subStructLayout.add<uvec3>("j");
	EXPECT_EQ(ret, 0);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 16);

	// Add vec2 (size: 8, alignment: 8)
	ret = subStructLayout.add<vec2>("k");
	EXPECT_EQ(ret, 16);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 32);

	// Add float array (count: 2, size: 20, padding: 12, alignment: 16)
	resultVec = { 32, 48 };
	retVec	  = subStructLayout.add<float>("l", 2);
	EXPECT_EQ(retVec, resultVec);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 64);

	// Add vec2 (size: 8, alignment: 8)
	ret = subStructLayout.add<vec2>("m");
	EXPECT_EQ(ret, 64);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 80);

	// Add mat3 array (count: 2, size: 92, padding: 4, alignment: 16)
	resultVec = { 80, 128 };
	tempVec	  = subStructLayout.add<mat3>("n", 2);

	retVec.clear();
	for (size_t  i = 0; i < tempVec.size(); i++) {
		retVec.push_back(tempVec[i].front());
	}

	EXPECT_EQ(retVec, resultVec);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 176);

	// Add sub struct array (count: 2, size: 352, padding: 0, alignment: 16)
	resultVec = { 128, 304 };
	retVec	  = structLayout.add("o", subStructLayout, 2);
	EXPECT_EQ(retVec, resultVec);
	ret = structLayout.size();
	EXPECT_EQ(ret, 480);
}

TEST(std140_layout, offset_calculation_2) {
	std140_layout structLayout;
	std140_layout subStructLayout;
	size_t ret;

	// Add bool (size: 4, alignment: 4)
	ret = subStructLayout.add<bool>("has_diffuse_texture");
	EXPECT_EQ(ret, 0);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 16);

	// Add bool (size: 4, alignment: 4)
	ret = subStructLayout.add<bool>("has_specular_texture");
	EXPECT_EQ(ret, 4);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 16);

	// Add vec3 (size: 12, alignment: 16)
	ret = subStructLayout.add<vec3>("color");
	EXPECT_EQ(ret, 16);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 32);

	// Add float (size: 4, alignment: 4)
	ret = subStructLayout.add<float>("shininess");
	EXPECT_EQ(ret, 28);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 32);

	// Add uint (size: 4, alignment: 4)
	ret = subStructLayout.add<unsigned int>("diffuse_toon_borders");
	EXPECT_EQ(ret, 32);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 48);

	// Add uint (size: 4, alignment: 4)
	ret = subStructLayout.add<unsigned int>("specular_toon_borders");
	EXPECT_EQ(ret, 36);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 48);

	// Add vec2 (size: 8, alignment: 8)
	ret = subStructLayout.add<vec2>("highlight_translate");
	EXPECT_EQ(ret, 40);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 48);

	// Add vec2 (size: 8, alignment: 8)
	ret = subStructLayout.add<vec2>("highlight_rotation");
	EXPECT_EQ(ret, 48);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 64);

	// Add vec2 (size: 8, alignment: 8)
	ret = subStructLayout.add<vec2>("highlight_scale");
	EXPECT_EQ(ret, 56);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 64);

	// Add vec2 (size: 8, alignment: 8)
	ret = subStructLayout.add<vec2>("highlight_split");
	EXPECT_EQ(ret, 64);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 80);

	// Add int (size: 4, alignment: 4)
	ret = subStructLayout.add<int>("highlight_square_n");
	EXPECT_EQ(ret, 72);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 80);

	// Add float (size: 4, alignment: 4)
	ret = subStructLayout.add<float>("highlight_square_x");
	EXPECT_EQ(ret, 76);
	ret = subStructLayout.size();
	EXPECT_EQ(ret, 80);

	std::vector<size_t> resultVec { 0, 80, 160, 240, 320, 400, 480, 560 };
	std::vector<size_t> retVec = structLayout.add("materialInputs", subStructLayout, 8);
	EXPECT_EQ(retVec, resultVec);
	ret = structLayout.size();
	EXPECT_EQ(ret, 640);
}

TEST(std140_layout, offset_calculation_3) {
	std140_layout structLayout;

	// Add vec2 (size: 8, alignment: 8)
	size_t ret = structLayout.add<vec2>("windowSize");
	EXPECT_EQ(ret, 0);
	ret = structLayout.size();
	EXPECT_EQ(ret, 16);

	// Add float (size: 4, alignment: 4)
	ret = structLayout.add<float>("nearPlane");
	EXPECT_EQ(ret, 8);
	ret = structLayout.size();
	EXPECT_EQ(ret, 16);

	// Add float (size: 4, alignment: 4)
	ret = structLayout.add<float>("farPlane");
	EXPECT_EQ(ret, 12);
	ret = structLayout.size();
	EXPECT_EQ(ret, 16);

	// Add float (size: 4, alignment: 4)
	ret = structLayout.add<float>("gamma");
	EXPECT_EQ(ret, 16);
	ret = structLayout.size();
	EXPECT_EQ(ret, 32);
}

TEST(std430_layout, offset_calculation_1) {
	size_t ret;
	std::vector<size_t> retVec;

	std430_layout rect;

	// Add mat4 (size: 64, alignment: 16)
	ret = rect.add<mat4>("transform").front();
	EXPECT_EQ(ret, 0);
	ret = rect.size();
	EXPECT_EQ(ret, 64);

	// Add vec2 (size: 8, alignment: 8)
	ret = rect.add<vec2>("size");
	EXPECT_EQ(ret, 64);
	ret = rect.size();
	EXPECT_EQ(ret, 80);

	std430_layout sprite;

	// Add uvec2 (size: 8, alignment: 8)
	ret = sprite.add<uvec2>("offset");
	EXPECT_EQ(ret, 0);
	ret = sprite.size();
	EXPECT_EQ(ret, 16);

	// Add uvec2 (size: 8, alignment: 8)
	ret = sprite.add<uvec2>("size");
	EXPECT_EQ(ret, 8);
	ret = sprite.size();
	EXPECT_EQ(ret, 16);

	// Add bool (size: 4, alignment: 4)
	ret = sprite.add<bool>("isActive");
	EXPECT_EQ(ret, 16);
	ret = sprite.size();
	EXPECT_EQ(ret, 32);

	std430_layout fill;

	// Add uint (size: 4, alignment: 4)
	ret = fill.add<unsigned int>("type");
	EXPECT_EQ(ret, 0);
	ret = fill.size();
	EXPECT_EQ(ret, 16);

	// Add uint (size: 4, alignment: 4)
	ret = fill.add<unsigned int>("subType");
	EXPECT_EQ(ret, 4);
	ret = fill.size();
	EXPECT_EQ(ret, 16);

	// Add float (size: 4, alignment: 4)
	ret = fill.add<float>("offset");
	EXPECT_EQ(ret, 8);
	ret = fill.size();
	EXPECT_EQ(ret, 16);

	// Add float (size: 4, alignment: 4)
	ret = fill.add<float>("progress");
	EXPECT_EQ(ret, 12);
	ret = fill.size();
	EXPECT_EQ(ret, 16);

	// Add float (size: 4, alignment: 4)
	ret = fill.add<float>("rotation");
	EXPECT_EQ(ret, 16);
	ret = fill.size();
	EXPECT_EQ(ret, 32);

	// Add bool (size: 4, alignment: 4)
	ret = fill.add<bool>("isActive");
	EXPECT_EQ(ret, 20);
	ret = fill.size();
	EXPECT_EQ(ret, 32);

	std430_layout uiElement;

	// Add rect struct (size: 80, alignment: 16)
	ret = uiElement.add("rect", rect);
	EXPECT_EQ(ret, 0);
	ret = uiElement.size();
	EXPECT_EQ(ret, 80);

	// Add sprite struct (size: 32, alignment: 8)
	ret = uiElement.add("sprite", sprite);
	EXPECT_EQ(ret, 80);
	ret = uiElement.size();
	EXPECT_EQ(ret, 112);

	// Add fill struct (size: 32, alignment: 4)
	ret = uiElement.add("fill", fill);
	EXPECT_EQ(ret, 112);
	ret = uiElement.size();
	EXPECT_EQ(ret, 144);

	// Add vec4 (size: 16, alignment: 16)
	ret = uiElement.add<vec4>("color");
	EXPECT_EQ(ret, 144);
	ret = uiElement.size();
	EXPECT_EQ(ret, 160);

	// Add bool (size: 4, alignment: 4)
	ret = uiElement.add<bool>("isText");
	EXPECT_EQ(ret, 160);
	ret = uiElement.size();
	EXPECT_EQ(ret, 176);

	std430_layout texture;

	// Add uvec2 (size: 8, alignment: 8)
	ret = texture.add<uvec2>("size");
	EXPECT_EQ(ret, 0);
	ret = uiElement.size();
	EXPECT_EQ(ret, 16);

	// Add bool (size: 4, alignment: 4)
	ret = texture.add<bool>("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.size();
	EXPECT_EQ(ret, 16);

	std430_layout ssbo;

	// Add uiElement struct array (count: 8, size: 1408, alignment: 16)
	std::vector<size_t> resultVec { 0, 176, 352, 528, 704, 880, 1056, 1232 };
	retVec = ssbo.add("uiElements", uiElement, 8);
	EXPECT_EQ(retVec, resultVec);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1408);

	// Add texture struct (size: 16, alignment: 8)
	ret = ssbo.add("elementTexture", texture);
	EXPECT_EQ(ret, 1408);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1424);

	// Add int (size: 4, alignment: 4)
	ret = ssbo.add<int>("elementLayer");
	EXPECT_EQ(ret, 1424);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1440);
}

TEST(std430_layout, offset_calculation_2) {
	size_t ret;
	std::vector<size_t> retVec;

	std430_layout pointLight;
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

	std430_layout spotLight;
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

	std430_layout dirLight;
	ret = dirLight.add<vec3>("direction");
	EXPECT_EQ(ret, 0);
	ret = dirLight.add<vec3>("color");
	EXPECT_EQ(ret, 16);
	ret = dirLight.add<mat4>("lightSpaceMatrix").front();
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

	std430_layout ssbo;
	std::vector<size_t> resultVec;
	ret = ssbo.add<unsigned int>("numberOfPointLights");
	EXPECT_EQ(ret, 0);
	ret = ssbo.add<unsigned int>("numberOfSpotLights");
	EXPECT_EQ(ret, 4);
	ret = ssbo.add<unsigned int>("numberOfDirLights");
	EXPECT_EQ(ret, 8);
	retVec	  = ssbo.add("pointLights", pointLight, 8);
	resultVec = { 16, 64, 112, 160, 208, 256, 304, 352 };
	EXPECT_EQ(retVec, resultVec);
	retVec	  = ssbo.add("spotLights", spotLight, 8);
	resultVec = { 400, 464, 528, 592, 656, 720, 784, 848 };
	EXPECT_EQ(retVec, resultVec);
	retVec	  = ssbo.add("dirLights", dirLight, 4);
	resultVec = { 912, 1024, 1136, 1248 };
	EXPECT_EQ(retVec, resultVec);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1360);
}