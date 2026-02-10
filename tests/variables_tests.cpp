#include "pch.hpp"

using namespace glslstruct;
using namespace glm;

TEST(std140_offset, constructor_and_get) {
	size_t ret;
	std::vector<size_t> retVec;
	std::vector<size_t> resultVec;

	std140_offset subStruct1{
		std_variable<int>("d"),
		std_variable<bvec2>("e")
	};
	ret = subStruct1.get_offset("d");
	EXPECT_EQ(ret, 0);
	ret = subStruct1.get_offset("e");
	EXPECT_EQ(ret, 8);
	ret = subStruct1.size();
	EXPECT_EQ(ret, 16);

	std140_offset subStruct2{
		std_variable<uvec3>("j"),
		std_variable<vec2>("k"),
		std_variable<float, 2>("l"),
		std_variable<vec2>("m"),
		std_variable<mat3, 2>("n")
	};
	ret = subStruct2.get_offset("j");
	EXPECT_EQ(ret, 0);
	ret = subStruct2.get_offset("k");
	EXPECT_EQ(ret, 16);
	resultVec = { 32, 48 };
	retVec = subStruct2.get_array_offsets("l");
	EXPECT_EQ(retVec, resultVec);
	ret = subStruct2.get_offset("m");
	EXPECT_EQ(ret, 64);
	resultVec = { 80, 128 };
	retVec = subStruct2.get_array_offsets("n");
	EXPECT_EQ(retVec, resultVec);
	ret = subStruct2.size();
	EXPECT_EQ(ret, 176);

	std140_offset structOffsets{
		std_variable<float>("a"),
		std_variable<vec2>("b"),
		std_variable<vec3>("c"),
		std_variable<std140_offset>("f", subStruct1),
		std_variable<float>("g"),
		std_variable<float, 2>("h"),
		std_variable<mat2x3>("i"),
		std_variable<std140_offset, 2>("o", subStruct2)
	};
	ret = structOffsets.get_offset("a");
	EXPECT_EQ(ret, 0);
	ret = structOffsets.get_offset("b");
	EXPECT_EQ(ret, 8);
	ret = structOffsets.get_offset("c");
	EXPECT_EQ(ret, 16);
	ret = structOffsets.get_offset("f");
	EXPECT_EQ(ret, 32);

	// F sub values
	ret = structOffsets.get_offset("f.d");
	EXPECT_EQ(ret, 32);
	ret = structOffsets.get_offset("f.e");
	EXPECT_EQ(ret, 40);

	ret = structOffsets.get_offset("g");
	EXPECT_EQ(ret, 48);
	resultVec = { 64, 80 };
	retVec = structOffsets.get_array_offsets("h");
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.get_offset("i");
	EXPECT_EQ(ret, 96);
	resultVec = { 128, 304 };
	retVec = structOffsets.get_array_offsets("o");
	EXPECT_EQ(retVec, resultVec);

	// O1 sub values
	ret = structOffsets.get_offset("o[0].j");
	EXPECT_EQ(ret, 128);
	ret = structOffsets.get_offset("o[0].k");
	EXPECT_EQ(ret, 144);
	retVec = structOffsets.get_array_offsets("o[0].l");
	resultVec = { 160, 176 };
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.get_offset("o[0].m");
	EXPECT_EQ(ret, 192);
	retVec = structOffsets.get_array_offsets("o[0].n");
	resultVec = { 208, 256 };
	EXPECT_EQ(retVec, resultVec);

	// O2 sub values
	ret = structOffsets.get_offset("o[1].j");
	EXPECT_EQ(ret, 304);
	ret = structOffsets.get_offset("o[1].k");
	EXPECT_EQ(ret, 320);
	retVec = structOffsets.get_array_offsets("o[1].l");
	resultVec = { 336, 352 };
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.get_offset("o[1].m");
	EXPECT_EQ(ret, 368);
	retVec = structOffsets.get_array_offsets("o[1].n");
	resultVec = { 384, 432 };
	EXPECT_EQ(retVec, resultVec);

	ret = structOffsets.size();
	EXPECT_EQ(ret, 480);
}

TEST(std430_offset, constructor_and_get) {
	size_t ret;
	std::vector<size_t> retVec;

#pragma region RECT
	std430_offset rect{
		std_variable<mat4>("transform"),
		std_variable<vec2>("size")
	};
	ret = rect.get_offset("transform");
	EXPECT_EQ(ret, 0);
	ret = rect.get_offset("size");
	EXPECT_EQ(ret, 64);
	ret = rect.size();
	EXPECT_EQ(ret, 80);
#pragma endregion

#pragma region SPRITE
	std430_offset sprite{
		std_variable<uvec2>("offset"),
		std_variable<uvec2>("size"),
		std_variable<bool>("isActive")
	};
	ret = sprite.get_offset("offset");
	EXPECT_EQ(ret, 0);
	ret = sprite.get_offset("size");
	EXPECT_EQ(ret, 8);
	ret = sprite.get_offset("isActive");
	EXPECT_EQ(ret, 16);
	ret = sprite.size();
	EXPECT_EQ(ret, 32);
#pragma endregion

#pragma region FILL
	std430_offset fill{
		std_variable<unsigned int>("type"),
		std_variable<unsigned int>("subType"),
		std_variable<float>("offset"),
		std_variable<float>("progress"),
		std_variable<float>("rotation"),
		std_variable<bool>("isActive")
	};
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
	EXPECT_EQ(ret, 32);
#pragma endregion

#pragma region UIElement
	std430_offset uiElement{
		std_variable<std430_offset>("rect", rect),
		std_variable<std430_offset>("sprite", sprite),
		std_variable<std430_offset>("fill", fill),
		std_variable<vec4>("color"),
		std_variable<bool>("isText")
	};
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
	EXPECT_EQ(ret, 112);

	// FILL sub Values
	ret = uiElement.get_offset("fill.type");
	EXPECT_EQ(ret, 112);
	ret = uiElement.get_offset("fill.subType");
	EXPECT_EQ(ret, 116);
	ret = uiElement.get_offset("fill.offset");
	EXPECT_EQ(ret, 120);
	ret = uiElement.get_offset("fill.progress");
	EXPECT_EQ(ret, 124);
	ret = uiElement.get_offset("fill.rotation");
	EXPECT_EQ(ret, 128);
	ret = uiElement.get_offset("fill.isActive");
	EXPECT_EQ(ret, 132);

	ret = uiElement.get_offset("color");
	EXPECT_EQ(ret, 144);
	ret = uiElement.get_offset("isText");
	EXPECT_EQ(ret, 160);
	ret = uiElement.size();
	EXPECT_EQ(ret, 176);
#pragma endregion

#pragma region TEXTURE
	std430_offset texture{
		std_variable<uvec2>("size"),
		std_variable<bool>("isActive")
	};
	ret = texture.get_offset("size");
	EXPECT_EQ(ret, 0);
	ret = texture.get_offset("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.size();
	EXPECT_EQ(ret, 16);
#pragma endregion

#pragma region SSBO
	std430_offset ssbo{
		std_variable<std430_offset, 8>("uiElements", uiElement),
		std_variable<std430_offset>("elementTexture", texture),
		std_variable<int>("elementLayer")
	};
	std::vector<size_t> resultVec{ 0, 176, 352, 528, 704, 880, 1056, 1232 };
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
	EXPECT_EQ(ret, 112);

	// FILL sub Values
	ret = ssbo.get_offset("uiElements[0].fill.type");
	EXPECT_EQ(ret, 112);
	ret = ssbo.get_offset("uiElements[0].fill.subType");
	EXPECT_EQ(ret, 116);
	ret = ssbo.get_offset("uiElements[0].fill.offset");
	EXPECT_EQ(ret, 120);
	ret = ssbo.get_offset("uiElements[0].fill.progress");
	EXPECT_EQ(ret, 124);
	ret = ssbo.get_offset("uiElements[0].fill.rotation");
	EXPECT_EQ(ret, 128);
	ret = ssbo.get_offset("uiElements[0].fill.isActive");
	EXPECT_EQ(ret, 132);

	ret = ssbo.get_offset("uiElements[0].color");
	EXPECT_EQ(ret, 144);
	ret = ssbo.get_offset("uiElements[0].isText");
	EXPECT_EQ(ret, 160);

	ret = ssbo.get_offset("elementTexture");
	EXPECT_EQ(ret, 1408);

	// TEXTURE SUB VALUES
	ret = ssbo.get_offset("elementTexture.size");
	EXPECT_EQ(ret, 1408);
	ret = ssbo.get_offset("elementTexture.isActive");
	EXPECT_EQ(ret, 1416);

	ret = ssbo.get_offset("elementLayer");
	EXPECT_EQ(ret, 1424);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1440);
#pragma endregion
}