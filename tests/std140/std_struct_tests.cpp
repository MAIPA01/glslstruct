#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(std140_struct, add_array) {
	const std140_struct test { glsl_value<int, 3>("test", { 2, 3, 4 }) };
	EXPECT_EQ(test.get<int>("test[1]"), 3);
	EXPECT_EQ(test.get_offset("test"), 0);
}

TEST(std140_struct, set_get_test) {
	// clang-format off
	std140_struct subTest {
		glsl_value<bool>("a", true)
	};

	std140_struct test {
		glsl_value<int>("scalar", 0),
		glsl_value<int, 3>("scalar_array", { 2, 3, 4 }),
		glsl_value<vec2>("vec"),
		glsl_value<vec3, 2>("vec_array"),
		glsl_value<mat2>("mat"),
		glsl_value<mat3, 2>("mat_array"),
		glsl_value<std140_struct>("struct", subTest),
		glsl_value<std140_struct, 2>("struct_array", subTest.get_layout())
	};
	// clang-format on

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
	EXPECT_EQ(test.get<std140_struct>("struct", subTest.get_layout()), subTest);
	subTest.set<bool>("a", false);
	EXPECT_TRUE(test.set("struct", subTest));
	EXPECT_EQ(test.get<std140_struct>("struct", subTest.get_layout()), subTest);
#pragma endregion

#pragma region SET_STRUCT_ARRAY
	std::vector<std140_struct> struct_array { std140_struct(subTest.get_layout()), std140_struct(subTest.get_layout()) };
	EXPECT_EQ(test.get<std::vector<std140_struct>>("struct_array", subTest.get_layout()), struct_array);
	struct_array[0].set<bool>("a", true);
	std::vector<std::vector<std::byte>> struct_array_data { struct_array[0].data(), struct_array[1].data() };
	EXPECT_TRUE(test.set("struct_array", subTest.get_layout(), struct_array_data));
	EXPECT_EQ(test.get<std::vector<std140_struct>>("struct_array", subTest.get_layout()), struct_array);
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

#if _GLSL_STRUCT_HAS_TYPES
TEST(std140_struct, writer) {
	// clang-format off
	std140_struct subSubTest {
		glsl_value<int>("b")
	};

	std140_struct subTest {
		glsl_value<bool>("a", true),
		glsl_value<std140_struct>("c", subSubTest)
	};

	std140_struct test {
		glsl_value<int>("scalar", 0),
		glsl_value<int, 3>("scalar_array", { 2, 3, 4 }),
		glsl_value<vec2>("vec"),
		glsl_value<vec3, 2>("vec_array"),
		glsl_value<mat2>("mat"),
		glsl_value<mat3, 2>("mat_array"),
		glsl_value<std140_struct>("struct", subTest),
		glsl_value<std140_struct, 2>("struct_array", subTest.get_layout())
	};
	// clang-format on

	glsl_opengl_writer glWriter;
	glWriter.append_struct("SubTest", *test.get_type<struct_type>("struct"));
	glWriter.append_shader_storage_buffer(0, "Test", test);
	// std::cout << "OPENGL: \n" << glWriter.to_string() << std::endl << std::endl << std::endl;

	glsl_vulkan_writer vkWriter;
	vkWriter.append_struct("SubTest", *test.get_type<struct_type>("struct"));
	vkWriter.append_shader_storage_buffer(0, 0, "Test", "ssbo", test, "readonly");
	// std::cout << "VULKAN: \n" << vkWriter.to_string() << std::endl;
}
#endif

TEST(std140_struct, parser) {
	std140_parser parser;

	// Add variable test
	std140_struct test;
	parser.add_variable(test, "bool", "a", 2);
	parser.add_variable(test, "dvec2", "b");
	parser.add_variable(test, "int c");
	parser.add_variable(test, "bmat3 d[];");
	parser.add_variable(test, "imat2x4 e[2]");

	ASSERT_TRUE(test.contains("a"));
	ASSERT_TRUE(test.contains("a[0]"));
	ASSERT_TRUE(test.contains("b"));
	ASSERT_TRUE(test.contains("c"));
	ASSERT_TRUE(test.contains("d"));
	ASSERT_TRUE(test.contains("d[0]"));
	ASSERT_TRUE(test.contains("e"));
	ASSERT_TRUE(test.contains("e[0]"));

	// Add variables test
	parser.add_variables(test, "float f;" "int g;" "dvec2 h; bmat3 i;" "imat2x4 j");
	ASSERT_TRUE(test.contains("f"));
	ASSERT_TRUE(test.contains("g"));
	ASSERT_TRUE(test.contains("h"));
	ASSERT_TRUE(test.contains("i"));
	ASSERT_TRUE(test.contains("j"));

	// Create struct test
	test = parser.create_struct("float a; int b; dvec2 c; bmat3 d; imat2x4 e");
	ASSERT_TRUE(test.contains("a"));
	ASSERT_TRUE(test.contains("b"));
	ASSERT_TRUE(test.contains("c"));
	ASSERT_TRUE(test.contains("d"));
	ASSERT_TRUE(test.contains("e"));

	// Add struct definition
	parser.add_struct_definition("SubTest", "bool f");
	parser.add_variable(test, "SubTest g");
	ASSERT_TRUE(test.contains("g"));
	ASSERT_TRUE(test.contains("g.f"));

	// Add structs
	parser.add_structs_definitions(
	  "struct SubTest2 { bool g; }; struct Test { float a; int c; dvec2 b; bmat3 z; imat2x4 d; SubTest h; SubTest2 i; };"
	);
	test = parser.get_struct("Test");
	ASSERT_TRUE(test.contains("a"));
	ASSERT_TRUE(test.contains("c"));
	ASSERT_TRUE(test.contains("b"));
	ASSERT_TRUE(test.contains("z"));
	ASSERT_TRUE(test.contains("d"));
	ASSERT_TRUE(test.contains("h"));
	ASSERT_TRUE(test.contains("h.f"));
	ASSERT_TRUE(test.contains("i"));
	ASSERT_TRUE(test.contains("i.g"));

#if _GLSL_STRUCT_HAS_TYPES
	// From writer to parser test
	glsl_opengl_writer glWriter;
	glWriter.append_shader_storage_buffer(0, "SSBO", test);

	parser.add_structs_definitions(glWriter.to_string());
	test = parser.get_struct("SSBO");
	ASSERT_TRUE(test.contains("a"));
	ASSERT_TRUE(test.contains("c"));
	ASSERT_TRUE(test.contains("b"));
	ASSERT_TRUE(test.contains("z"));
	ASSERT_TRUE(test.contains("d"));
	ASSERT_TRUE(test.contains("h"));
	ASSERT_TRUE(test.contains("h.f"));
	ASSERT_TRUE(test.contains("i"));
	ASSERT_TRUE(test.contains("i.g"));

	// std::cout << glWriter.to_string() << std::endl;
#endif
}

TEST(std140_struct, copy_test) {
	std140_struct test { glsl_value<int, 3>("test", { 2, 3, 4 }) };
	int value = test.get<int>("test[1]");
	EXPECT_EQ(value, 3);
	size_t ret = test.get_offset("test");
	EXPECT_EQ(ret, 0);

	const std140_struct test1 = test;
	value					  = test1.get<int>("test[1]");
	EXPECT_EQ(value, 3);
	ret = test1.get_offset("test");
	EXPECT_EQ(ret, 0);
}

TEST(std140_struct, constructor_and_get) {
	size_t ret;
	std::vector<size_t> retVec;

#pragma region RECT
	// clang-format off
	std140_struct rect {
		glsl_value<mat4>("transform", mat4(1.f)),
		glsl_value<vec2>("size")
	};
	// clang-format on
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
	// clang-format off
	std140_struct sprite {
		glsl_value<uvec2>("offset"),
		glsl_value<uvec2>("size"),
		glsl_value<bool>("isActive")
	};
	// clang-format on
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
	// clang-format off
	std140_struct fill {
		glsl_value<unsigned int>("type"),
		glsl_value<unsigned int>("subType"),
		glsl_value<float>("offset"),
		glsl_value<float>("progress"),
		glsl_value<float>("rotation"),
		glsl_value<bool>("isActive")
	};
	// clang-format on
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
	// clang-format off
	std140_struct uiElement {
		glsl_value<std140_struct>("rect", rect),
		glsl_value<std140_struct>("sprite", sprite),
		glsl_value<std140_struct>("fill", fill),
		glsl_value<vec4>("color"),
		glsl_value<bool>("isText")
	};
	// clang-format on
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
	EXPECT_EQ(ret, 164);
#pragma endregion

#pragma region TEXTURE
	// clang-format off
	std140_struct texture {
		glsl_value<uvec2>("size"),
		glsl_value<bool>("isActive")
	};
	// clang-format on
	ret = texture.get_offset("size");
	EXPECT_EQ(ret, 0);
	ret = texture.get_offset("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.size();
	EXPECT_EQ(ret, 12);
#pragma endregion

#pragma region SSBO
	// clang-format off
	std140_struct ssbo {
		glsl_value<std140_struct, 8>("uiElements", uiElement.get_layout()),
		glsl_value<std140_struct>("elementTexture", texture),
		glsl_value<int>("elementLayer")
	};
	// clang-format on
	std::vector<size_t> resultVec { 0, 176, 352, 528, 704, 880, 1056, 1232 };
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
	EXPECT_EQ(ret, 1428);
#pragma endregion
}