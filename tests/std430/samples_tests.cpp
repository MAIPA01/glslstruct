#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(std430_layout, offset_calculation_1) {
	size_t ret;
	std::vector<size_t> retVec;

#pragma region Rect
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
	EXPECT_EQ(ret, 72);
#pragma endregion

#pragma region Sprite
	std430_layout sprite;

	// Add uvec2 (size: 8, alignment: 8)
	ret = sprite.add<uvec2>("offset");
	EXPECT_EQ(ret, 0);
	ret = sprite.size();
	EXPECT_EQ(ret, 8);

	// Add uvec2 (size: 8, alignment: 8)
	ret = sprite.add<uvec2>("size");
	EXPECT_EQ(ret, 8);
	ret = sprite.size();
	EXPECT_EQ(ret, 16);

	// Add bool (size: 4, alignment: 4)
	ret = sprite.add<bool>("isActive");
	EXPECT_EQ(ret, 16);
	ret = sprite.size();
	EXPECT_EQ(ret, 20);
#pragma endregion

#pragma region Fill
	std430_layout fill;

	// Add uint (size: 4, alignment: 4)
	ret = fill.add<unsigned int>("type");
	EXPECT_EQ(ret, 0);
	ret = fill.size();
	EXPECT_EQ(ret, 4);

	// Add uint (size: 4, alignment: 4)
	ret = fill.add<unsigned int>("subType");
	EXPECT_EQ(ret, 4);
	ret = fill.size();
	EXPECT_EQ(ret, 8);

	// Add float (size: 4, alignment: 4)
	ret = fill.add<float>("offset");
	EXPECT_EQ(ret, 8);
	ret = fill.size();
	EXPECT_EQ(ret, 12);

	// Add float (size: 4, alignment: 4)
	ret = fill.add<float>("progress");
	EXPECT_EQ(ret, 12);
	ret = fill.size();
	EXPECT_EQ(ret, 16);

	// Add float (size: 4, alignment: 4)
	ret = fill.add<float>("rotation");
	EXPECT_EQ(ret, 16);
	ret = fill.size();
	EXPECT_EQ(ret, 20);

	// Add bool (size: 4, alignment: 4)
	ret = fill.add<bool>("isActive");
	EXPECT_EQ(ret, 20);
	ret = fill.size();
	EXPECT_EQ(ret, 24);
#pragma endregion

#pragma region UIElement
	std430_layout uiElement;

	// Add rect struct (size: 72, alignment: 16)
	ret = uiElement.add("rect", rect);
	EXPECT_EQ(ret, 0);
	ret = uiElement.size();
	EXPECT_EQ(ret, 72);

	// Add sprite struct (size: 20, alignment: 8)
	ret = uiElement.add("sprite", sprite);
	EXPECT_EQ(ret, 80);
	ret = uiElement.size();
	EXPECT_EQ(ret, 100);

	// Add fill struct (size: 24, alignment: 4)
	ret = uiElement.add("fill", fill);
	EXPECT_EQ(ret, 104);
	ret = uiElement.size();
	EXPECT_EQ(ret, 128);

	// Add vec4 (size: 16, alignment: 16)
	ret = uiElement.add<vec4>("color");
	EXPECT_EQ(ret, 128);
	ret = uiElement.size();
	EXPECT_EQ(ret, 144);

	// Add bool (size: 4, alignment: 4)
	ret = uiElement.add<bool>("isText");
	EXPECT_EQ(ret, 144);
	ret = uiElement.size();
	EXPECT_EQ(ret, 148);
#pragma endregion

#pragma region Texture
	std430_layout texture;

	// Add uvec2 (size: 8, alignment: 8)
	ret = texture.add<uvec2>("size");
	EXPECT_EQ(ret, 0);
	ret = texture.size();
	EXPECT_EQ(ret, 8);

	// Add bool (size: 4, alignment: 4)
	ret = texture.add<bool>("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.size();
	EXPECT_EQ(ret, 12);
#pragma endregion

#pragma region SSBO
	std430_layout ssbo;

	// Add uiElement struct array (count: 8, size: 1268, padding: 12, alignment: 16)
	std::vector<size_t> resultVec { 0, 160, 320, 480, 640, 800, 960, 1120 };
	retVec = ssbo.add("uiElements", uiElement, 8);
	EXPECT_EQ(retVec, resultVec);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1280);

	// Add texture struct (size: 12, alignment: 8)
	ret = ssbo.add("elementTexture", texture);
	EXPECT_EQ(ret, 1280);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1292);

	// Add int (size: 4, alignment: 4)
	ret = ssbo.add<int>("elementLayer");
	EXPECT_EQ(ret, 1296);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1300);
#pragma endregion
}

TEST(std430_layout, offset_calculation_2) {
	size_t ret;
	std::vector<size_t> retVec;

#pragma region PointLight
	std430_layout pointLight;

	// Add vec3 (size: 12, alignment: 16)
	ret = pointLight.add<vec3>("position");
	EXPECT_EQ(ret, 0);
	ret = pointLight.size();
	EXPECT_EQ(ret, 12);

	// Add vec3 (size: 12, alignment: 16)
	ret = pointLight.add<vec3>("color");
	EXPECT_EQ(ret, 16);
	ret = pointLight.size();
	EXPECT_EQ(ret, 28);

	// Add float (size: 4, alignment: 4)
	ret = pointLight.add<float>("power");
	EXPECT_EQ(ret, 28);
	ret = pointLight.size();
	EXPECT_EQ(ret, 32);

	// Add float (size: 4, alignment: 4)
	ret = pointLight.add<float>("constant");
	EXPECT_EQ(ret, 32);
	ret = pointLight.size();
	EXPECT_EQ(ret, 36);

	// Add float (size: 4, alignment: 4)
	ret = pointLight.add<float>("linear");
	EXPECT_EQ(ret, 36);
	ret = pointLight.size();
	EXPECT_EQ(ret, 40);

	// Add float (size: 4, alignment: 4)
	ret = pointLight.add<float>("quadratic");
	EXPECT_EQ(ret, 40);
	ret = pointLight.size();
	EXPECT_EQ(ret, 44);
#pragma endregion

#pragma region SpotLight
	std430_layout spotLight;

	// Add vec3 (size: 12, alignment: 16)
	ret = spotLight.add<vec3>("position");
	EXPECT_EQ(ret, 0);
	ret = spotLight.size();
	EXPECT_EQ(ret, 12);

	// Add vec3 (size: 12, alignment: 16)
	ret = spotLight.add<vec3>("direction");
	EXPECT_EQ(ret, 16);
	ret = spotLight.size();
	EXPECT_EQ(ret, 28);

	// Add float (size: 4, alignment: 4)
	ret = spotLight.add<float>("power");
	EXPECT_EQ(ret, 28);
	ret = spotLight.size();
	EXPECT_EQ(ret, 32);

	// Add vec3 (size: 12, alignment: 16)
	ret = spotLight.add<vec3>("color");
	EXPECT_EQ(ret, 32);
	ret = spotLight.size();
	EXPECT_EQ(ret, 44);

	// Add float (size: 4, alignment: 4)
	ret = spotLight.add<float>("cutOff");
	EXPECT_EQ(ret, 44);
	ret = spotLight.size();
	EXPECT_EQ(ret, 48);

	// Add float (size: 4, alignment: 4)
	ret = spotLight.add<float>("outerCutOff");
	EXPECT_EQ(ret, 48);
	ret = spotLight.size();
	EXPECT_EQ(ret, 52);

	// Add float (size: 4, alignment: 4)
	ret = spotLight.add<float>("constant");
	EXPECT_EQ(ret, 52);
	ret = spotLight.size();
	EXPECT_EQ(ret, 56);

	// Add float (size: 4, alignment: 4)
	ret = spotLight.add<float>("linear");
	EXPECT_EQ(ret, 56);
	ret = spotLight.size();
	EXPECT_EQ(ret, 60);

	// Add float (size: 4, alignment: 4)
	ret = spotLight.add<float>("quadratic");
	EXPECT_EQ(ret, 60);
	ret = spotLight.size();
	EXPECT_EQ(ret, 64);
#pragma endregion

#pragma region DirLight
	std430_layout dirLight;

	// Add vec3 (size: 12, alignment: 16)
	ret = dirLight.add<vec3>("direction");
	EXPECT_EQ(ret, 0);
	ret = dirLight.size();
	EXPECT_EQ(ret, 12);

	// Add vec3 (size: 12, alignment: 16)
	ret = dirLight.add<vec3>("color");
	EXPECT_EQ(ret, 16);
	ret = dirLight.size();
	EXPECT_EQ(ret, 28);

	// Add mat4 (size: 64, alignment: 16)
	ret = dirLight.add<mat4>("lightSpaceMatrix").front();
	EXPECT_EQ(ret, 32);
	ret = dirLight.size();
	EXPECT_EQ(ret, 96);

	// Add float (size: 4, alignment: 4)
	ret = dirLight.add<float>("power");
	EXPECT_EQ(ret, 96);
	ret = dirLight.size();
	EXPECT_EQ(ret, 100);
#pragma endregion

#pragma region SSBO
	std430_layout ssbo;
	std::vector<size_t> resultVec;

	// Add uint (size: 4, alignment: 4)
	ret = ssbo.add<unsigned int>("numberOfPointLights");
	EXPECT_EQ(ret, 0);
	ret = ssbo.size();
	EXPECT_EQ(ret, 4);

	// Add uint (size: 4, alignment: 4)
	ret = ssbo.add<unsigned int>("numberOfSpotLights");
	EXPECT_EQ(ret, 4);
	ret = ssbo.size();
	EXPECT_EQ(ret, 8);

	// Add uint (size: 4, alignment: 4)
	ret = ssbo.add<unsigned int>("numberOfDirLights");
	EXPECT_EQ(ret, 8);
	ret = ssbo.size();
	EXPECT_EQ(ret, 12);

	// Add PointLight struct array (size: 380, alignment: 16, padding: 4, count: 8)
	retVec	  = ssbo.add("pointLights", pointLight, 8);
	resultVec = { 16, 64, 112, 160, 208, 256, 304, 352 };
	EXPECT_EQ(retVec, resultVec);
	ret = ssbo.size();
	EXPECT_EQ(ret, 400);

	// Add SpotLight struct array (size: 512, alignment: 16, padding: 0, count: 8)
	retVec	  = ssbo.add("spotLights", spotLight, 8);
	resultVec = { 400, 464, 528, 592, 656, 720, 784, 848 };
	EXPECT_EQ(retVec, resultVec);
	ret = ssbo.size();
	EXPECT_EQ(ret, 912);

	// Add DirLight struct array (size: 448, alignment: 16, padding: 0, count: 4)
	retVec	  = ssbo.add("dirLights", dirLight, 4);
	resultVec = { 912, 1024, 1136, 1248 };
	EXPECT_EQ(retVec, resultVec);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1360);
#pragma endregion
}