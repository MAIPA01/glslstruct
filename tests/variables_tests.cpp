#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(std140_offset, constructor_and_get) {
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

TEST(std430_offset, constructor_and_get) {
	size_t ret;
	std::vector<size_t> retVec;

#pragma region RECT
	std430_layout rect { glsl_variable<mat4>("transform"), glsl_variable<vec2>("size") };
	ret = rect.get_offset("transform");
	EXPECT_EQ(ret, 0);
	ret = rect.get_offset("size");
	EXPECT_EQ(ret, 64);
	ret = rect.size();
	EXPECT_EQ(ret, 72);
#pragma endregion

#pragma region SPRITE
	std430_layout sprite { glsl_variable<uvec2>("offset"), glsl_variable<uvec2>("size"), glsl_variable<bool>("isActive") };
	ret = sprite.get_offset("offset");
	EXPECT_EQ(ret, 0);
	ret = sprite.get_offset("size");
	EXPECT_EQ(ret, 8);
	ret = sprite.get_offset("isActive");
	EXPECT_EQ(ret, 16);
	ret = sprite.size();
	EXPECT_EQ(ret, 20);
#pragma endregion

#pragma region FILL
	std430_layout fill { glsl_variable<unsigned int>("type"), glsl_variable<unsigned int>("subType"),
		glsl_variable<float>("offset"), glsl_variable<float>("progress"), glsl_variable<float>("rotation"),
		glsl_variable<bool>("isActive") };
	ret = fill.get_offset("type");
	EXPECT_EQ(ret, 0);
	ret = fill.get_offset("subType");
	EXPECT_EQ(ret, 4);
	ret = fill.get_offset("offset");
	EXPECT_EQ(ret, 8);
	ret = fill.get_offset("progress");
	EXPECT_EQ(ret, 12);
	ret = fill.get_offset("rotation");
	EXPECT_EQ(ret, 16);
	ret = fill.get_offset("isActive");
	EXPECT_EQ(ret, 20);
	ret = fill.size();
	EXPECT_EQ(ret, 24);
#pragma endregion

#pragma region UIElement
	std430_layout uiElement { glsl_variable<std430_layout>("rect", rect), glsl_variable<std430_layout>("sprite", sprite),
		glsl_variable<std430_layout>("fill", fill), glsl_variable<vec4>("color"), glsl_variable<bool>("isText") };
	ret = uiElement.get_offset("rect");
	EXPECT_EQ(ret, 0);

	// RECT sub Values
	ret = uiElement.get_offset("rect.transform");
	EXPECT_EQ(ret, 0);
	ret = uiElement.get_offset("rect.size");
	EXPECT_EQ(ret, 64);

	ret = uiElement.get_offset("sprite");
	EXPECT_EQ(ret, 80);

	// SPRITE sub Values
	ret = uiElement.get_offset("sprite.offset");
	EXPECT_EQ(ret, 80);
	ret = uiElement.get_offset("sprite.size");
	EXPECT_EQ(ret, 88);
	ret = uiElement.get_offset("sprite.isActive");
	EXPECT_EQ(ret, 96);

	ret = uiElement.get_offset("fill");
	EXPECT_EQ(ret, 104);

	// FILL sub Values
	ret = uiElement.get_offset("fill.type");
	EXPECT_EQ(ret, 104);
	ret = uiElement.get_offset("fill.subType");
	EXPECT_EQ(ret, 108);
	ret = uiElement.get_offset("fill.offset");
	EXPECT_EQ(ret, 112);
	ret = uiElement.get_offset("fill.progress");
	EXPECT_EQ(ret, 116);
	ret = uiElement.get_offset("fill.rotation");
	EXPECT_EQ(ret, 120);
	ret = uiElement.get_offset("fill.isActive");
	EXPECT_EQ(ret, 124);

	ret = uiElement.get_offset("color");
	EXPECT_EQ(ret, 128);
	ret = uiElement.get_offset("isText");
	EXPECT_EQ(ret, 144);
	ret = uiElement.size();
	EXPECT_EQ(ret, 148);
#pragma endregion

#pragma region TEXTURE
	std430_layout texture { glsl_variable<uvec2>("size"), glsl_variable<bool>("isActive") };
	ret = texture.get_offset("size");
	EXPECT_EQ(ret, 0);
	ret = texture.get_offset("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.size();
	EXPECT_EQ(ret, 12);
#pragma endregion

#pragma region SSBO
	std430_layout ssbo { glsl_variable<std430_layout, 8>("uiElements", uiElement),
		glsl_variable<std430_layout>("elementTexture", texture), glsl_variable<int>("elementLayer") };
	std::vector<size_t> resultVec { 0, 160, 320, 480, 640, 800, 960, 1120 };
	retVec = ssbo.get_array_offsets("uiElements");
	EXPECT_EQ(retVec, resultVec);

	// UI FIRST ELEMENT SUB VALUES TEST
	ret = ssbo.get_offset("uiElements[0].rect");
	EXPECT_EQ(ret, 0);

	// RECT sub Values
	ret = ssbo.get_offset("uiElements[0].rect.transform");
	EXPECT_EQ(ret, 0);
	ret = ssbo.get_offset("uiElements[0].rect.size");
	EXPECT_EQ(ret, 64);

	ret = ssbo.get_offset("uiElements[0].sprite");
	EXPECT_EQ(ret, 80);

	// SPRITE sub Values
	ret = ssbo.get_offset("uiElements[0].sprite.offset");
	EXPECT_EQ(ret, 80);
	ret = ssbo.get_offset("uiElements[0].sprite.size");
	EXPECT_EQ(ret, 88);
	ret = ssbo.get_offset("uiElements[0].sprite.isActive");
	EXPECT_EQ(ret, 96);

	ret = ssbo.get_offset("uiElements[0].fill");
	EXPECT_EQ(ret, 104);

	// FILL sub Values
	ret = ssbo.get_offset("uiElements[0].fill.type");
	EXPECT_EQ(ret, 104);
	ret = ssbo.get_offset("uiElements[0].fill.subType");
	EXPECT_EQ(ret, 108);
	ret = ssbo.get_offset("uiElements[0].fill.offset");
	EXPECT_EQ(ret, 112);
	ret = ssbo.get_offset("uiElements[0].fill.progress");
	EXPECT_EQ(ret, 116);
	ret = ssbo.get_offset("uiElements[0].fill.rotation");
	EXPECT_EQ(ret, 120);
	ret = ssbo.get_offset("uiElements[0].fill.isActive");
	EXPECT_EQ(ret, 124);

	ret = ssbo.get_offset("uiElements[0].color");
	EXPECT_EQ(ret, 128);
	ret = ssbo.get_offset("uiElements[0].isText");
	EXPECT_EQ(ret, 144);

	ret = ssbo.get_offset("elementTexture");
	EXPECT_EQ(ret, 1280);

	// TEXTURE SUB VALUES
	ret = ssbo.get_offset("elementTexture.size");
	EXPECT_EQ(ret, 1280);
	ret = ssbo.get_offset("elementTexture.isActive");
	EXPECT_EQ(ret, 1288);

	ret = ssbo.get_offset("elementLayer");
	EXPECT_EQ(ret, 1296);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1300);
#pragma endregion
}