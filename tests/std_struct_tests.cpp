#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(std430_struct, add_array) {
	const std430_struct test { glsl_value<int, 3>("test", { 2, 3, 4 }) };
	EXPECT_EQ(test.get<int>("test[1]"), 3);
	EXPECT_EQ(test.get_offset("test"), 0);
}

TEST(std430_struct, set_get_test) {
	std430_struct subTest { glsl_value<bool>("a", true) };

	std430_struct test { glsl_value<int>("scalar", 0), glsl_value<int, 3>("scalar_array", { 2, 3, 4 }), glsl_value<vec2>("vec"),
		glsl_value<vec3, 2>("vec_array", { vec3(), vec3() }), glsl_value<mat2>("mat"),
		glsl_value<mat3, 2>("mat_array", { mat3(), mat3() }), glsl_value<std430_struct>("struct", subTest),
		glsl_value<std430_struct, 2>("struct_array", subTest.get_layout()) };

#pragma region SET_SCALAR
	EXPECT_EQ(test.get<int>("scalar"), 0);
	EXPECT_TRUE(test.set<int>("scalar", 2));
	EXPECT_EQ(test.get<int>("scalar"), 2);
#pragma endregion

#pragma region SET_SCALAR_ARRAY
	std::vector<int> scalar_array { 2, 3, 4 };
	EXPECT_EQ(test.get<std::vector<int>>("scalar_array"), scalar_array);
	scalar_array = { 3, 4, 5 };
	EXPECT_TRUE(test.set("scalar_array", scalar_array));
	EXPECT_EQ(test.get<std::vector<int>>("scalar_array"), scalar_array);
#pragma endregion

#pragma region SET_VEC
	EXPECT_EQ(test.get<vec2>("vec"), vec2());
	EXPECT_TRUE(test.set<vec2>("vec", vec2(1, 2)));
	EXPECT_EQ(test.get<vec2>("vec"), vec2(1, 2));
#pragma endregion

#pragma region SET_VEC_ARRAY
	std::vector<vec3> vec_array { vec3(), vec3() };
	EXPECT_EQ(test.get<std::vector<vec3>>("vec_array"), vec_array);
	vec_array = { vec3(1, 2, 3), vec3(2, 3, 4) };
	EXPECT_TRUE(test.set("vec_array", vec_array));
	EXPECT_EQ(test.get<std::vector<vec3>>("vec_array"), vec_array);
#pragma endregion

#pragma region SET_MAT
	EXPECT_EQ(test.get<mat2>("mat"), mat2());
	EXPECT_TRUE(test.set<mat2>("mat", mat2(2)));
	EXPECT_EQ(test.get<mat2>("mat"), mat2(2));
#pragma endregion

#pragma region SET_MAT_ARRAY
	std::vector<mat3> mat_array { mat3(), mat3() };
	EXPECT_EQ(test.get<std::vector<mat3>>("mat_array"), mat_array);
	mat_array = { mat3(3), mat3(4) };
	EXPECT_TRUE(test.set("mat_array", mat_array));
	EXPECT_EQ(test.get<std::vector<mat3>>("mat_array"), mat_array);
#pragma endregion

#pragma region SET_STRUCT
	EXPECT_EQ(test.get<std430_struct>("struct", subTest.get_layout()), subTest);
	subTest.set<bool>("a", false);
	EXPECT_TRUE(test.set("struct", subTest));
	EXPECT_EQ(test.get<std430_struct>("struct", subTest.get_layout()), subTest);
#pragma endregion

#pragma region SET_STRUCT_ARRAY
	std::vector<std430_struct> struct_array { std430_struct(subTest.get_layout()), std430_struct(subTest.get_layout()) };
	EXPECT_EQ(test.get<std::vector<std430_struct>>("struct_array", subTest.get_layout()), struct_array);
	struct_array[0].set<bool>("a", true);
	std::vector<std::vector<std::byte>> struct_array_data { struct_array[0].data(), struct_array[1].data() };
	EXPECT_TRUE(test.set("struct_array", subTest.get_layout(), struct_array_data));
	EXPECT_EQ(test.get<std::vector<std430_struct>>("struct_array", subTest.get_layout()), struct_array);
#pragma endregion

#pragma region GET_SUBSTRUCT_VALUE
	EXPECT_EQ(test.get<bool>("struct.a"), false);
	EXPECT_TRUE(test.set<bool>("struct.a", true));
	EXPECT_EQ(test.get<bool>("struct.a"), true);
#pragma endregion

#pragma region GET_ARRAY_ELEM_VALUE
	EXPECT_EQ(test.get<int>("scalar_array[0]"), 3);
	EXPECT_TRUE(test.set<int>("scalar_array[0]", 2));
	EXPECT_EQ(test.get<int>("scalar_array[0]"), 2);
#pragma endregion
}

TEST(std430_struct, copy_test) {
	const std430_struct test { glsl_value<int, 3>("test", { 2, 3, 4 }) };
	int value = test.get<int>("test[1]");
	EXPECT_EQ(value, 3);
	size_t ret = test.get_offset("test");
	EXPECT_EQ(ret, 0);

	const std430_struct test1 = test;
	value					  = test1.get<int>("test[1]");
	EXPECT_EQ(value, 3);
	ret = test1.get_offset("test");
	EXPECT_EQ(ret, 0);
}

TEST(std430_struct, constructor_and_get) {
	size_t ret;
	std::vector<size_t> retVec;

#pragma region RECT
	std430_struct rect { glsl_value<mat4>("transform", mat4(1.f)), glsl_value<vec2>("size") };
	auto transform = rect.get<mat4>("transform");
	EXPECT_EQ(transform, mat4(1.f));
	ret = rect.get_offset("transform");
	EXPECT_EQ(ret, 0);
	ret = rect.get_offset("size");
	EXPECT_EQ(ret, 64);
	ret = rect.size();
	EXPECT_EQ(ret, 72);
#pragma endregion

#pragma region SPRITE
	std430_struct sprite { glsl_value<uvec2>("offset"), glsl_value<uvec2>("size"), glsl_value<bool>("isActive") };
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
	std430_struct fill { glsl_value<unsigned int>("type"), glsl_value<unsigned int>("subType"), glsl_value<float>("offset"),
		glsl_value<float>("progress"), glsl_value<float>("rotation"), glsl_value<bool>("isActive") };
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
	std430_struct uiElement { glsl_value<std430_struct>("rect", rect), glsl_value<std430_struct>("sprite", sprite),
		glsl_value<std430_struct>("fill", fill), glsl_value<vec4>("color"), glsl_value<bool>("isText") };
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
	std430_struct texture { glsl_value<uvec2>("size"), glsl_value<bool>("isActive") };
	ret = texture.get_offset("size");
	EXPECT_EQ(ret, 0);
	ret = texture.get_offset("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.size();
	EXPECT_EQ(ret, 12);
#pragma endregion

#pragma region SSBO
	std430_struct ssbo { glsl_value<std430_struct, 8>("uiElements", uiElement.get_layout()),
		glsl_value<std430_struct>("elementTexture", texture), glsl_value<int>("elementLayer") };
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