#include "pch.hpp"

using namespace glslstruct;
using namespace glm;

TEST(std_struct, add_array) {
	std430_struct test{
		std_value<int, 3>("test", { 2, 3, 4 })
	};
	int value = test.get<int>("test[1]");
	EXPECT_EQ(value, 3);
	size_t ret = test.get_offset("test");
	EXPECT_EQ(ret, 0);
}

TEST(std_struct, copy_test) {
	std430_struct test{
		std_value<int, 3>("test", { 2, 3, 4 })
	};
	int value = test.get<int>("test[1]");
	EXPECT_EQ(value, 3);
	size_t ret = test.get_offset("test");
	EXPECT_EQ(ret, 0);

	std430_struct test1 = test;
	value = test1.get<int>("test[1]");
	EXPECT_EQ(value, 3);
	ret = test1.get_offset("test");
	EXPECT_EQ(ret, 0);
}

TEST(std_struct, constructor_and_get) {
	size_t ret;
	std::vector<size_t> retVec;

#pragma region RECT
	std430_struct rect{
		std_value<mat4>("transform", mat4(1.f)),
		std_value<vec2>("size")
	};
	mat4 transform = rect.get<mat4>("transform");
	EXPECT_EQ(transform, mat4(1.f));
	ret = rect.get_offset("transform");
	EXPECT_EQ(ret, 0);
	ret = rect.get_offset("size");
	EXPECT_EQ(ret, 64);
	ret = rect.size();
	EXPECT_EQ(ret, 80);
#pragma endregion

#pragma region SPRITE
	std430_struct sprite{
		std_value<uvec2>("offset"),
		std_value<uvec2>("size"),
		std_value<bool>("isActive")
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
	std430_struct fill{
		std_value<unsigned int>("type"),
		std_value<unsigned int>("subType"),
		std_value<float>("offset"),
		std_value<float>("progress"),
		std_value<float>("rotation"),
		std_value<bool>("isActive")
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
	std430_struct uiElement{
		std_value<std430_struct>("rect", rect),
		std_value<std430_struct>("sprite", sprite),
		std_value<std430_struct>("fill", fill),
		std_value<vec4>("color"),
		std_value<bool>("isText")
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
	std430_struct texture{
		std_value<uvec2>("size"),
		std_value<bool>("isActive")
	};
	ret = texture.get_offset("size");
	EXPECT_EQ(ret, 0);
	ret = texture.get_offset("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.size();
	EXPECT_EQ(ret, 16);
#pragma endregion

#pragma region SSBO
	std430_struct ssbo{
		std_value<std430_struct, 8>("uiElements", uiElement.get_offsets()),
		std_value<std430_struct>("elementTexture", texture),
		std_value<int>("elementLayer")
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