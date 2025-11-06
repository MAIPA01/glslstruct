#include "pch.hpp"

using namespace glslstruct;
using namespace glm;

TEST(std140_offset, add_matrixes) {
	std140_offset structOffsets;
	size_t ret;

#pragma region COLUMN_MAJOR

#pragma region MAT2x2
	structOffsets.clear();

	// BOOL
	ret = structOffsets.add<bmat2>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.add<imat2>("Int");
	EXPECT_EQ(ret, 32);

	// UINT
	ret = structOffsets.add<umat2>("Uint");
	EXPECT_EQ(ret, 64);

	// FLOAT
	ret = structOffsets.add<fmat2>("Float");
	EXPECT_EQ(ret, 96);

	// DOUBLE
	ret = structOffsets.add<dmat2>("Double");
	EXPECT_EQ(ret, 128);
#pragma endregion

#pragma region MAT2x3
	structOffsets.clear();

	// BOOL
	ret = structOffsets.add<bmat2x3>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.add<imat2x3>("Int");
	EXPECT_EQ(ret, 32);

	// UINT
	ret = structOffsets.add<umat2x3>("Uint");
	EXPECT_EQ(ret, 64);

	// FLOAT
	ret = structOffsets.add<fmat2x3>("Float");
	EXPECT_EQ(ret, 96);

	// DOUBLE
	ret = structOffsets.add<dmat2x3>("Double");
	EXPECT_EQ(ret, 128);
#pragma endregion

#pragma region MAT2x4
	structOffsets.clear();

	// BOOL
	ret = structOffsets.add<bmat2x4>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.add<imat2x4>("Int");
	EXPECT_EQ(ret, 32);

	// UINT
	ret = structOffsets.add<umat2x4>("Uint");
	EXPECT_EQ(ret, 64);

	// FLOAT
	ret = structOffsets.add<fmat2x4>("Float");
	EXPECT_EQ(ret, 96);

	// DOUBLE
	ret = structOffsets.add<dmat2x4>("Double");
	EXPECT_EQ(ret, 128);
#pragma endregion

#pragma endregion

#pragma region ROW_MAJOR

#pragma region MAT2x2
	structOffsets.clear();

	// BOOL
	ret = structOffsets.add<bmat2, false>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.add<imat2, false>("Int");
	EXPECT_EQ(ret, 32);

	// UINT
	ret = structOffsets.add<umat2, false>("Uint");
	EXPECT_EQ(ret, 64);

	// FLOAT
	ret = structOffsets.add<fmat2, false>("Float");
	EXPECT_EQ(ret, 96);

	// DOUBLE
	ret = structOffsets.add<dmat2, false>("Double");
	EXPECT_EQ(ret, 128);
#pragma endregion

#pragma region MAT2x3
	structOffsets.clear();

	// BOOL
	ret = structOffsets.add<bmat2x3, false>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.add<imat2x3, false>("Int");
	EXPECT_EQ(ret, 48);

	// UINT
	ret = structOffsets.add<umat2x3, false>("Uint");
	EXPECT_EQ(ret, 96);

	// FLOAT
	ret = structOffsets.add<fmat2x3, false>("Float");
	EXPECT_EQ(ret, 144);

	// DOUBLE
	ret = structOffsets.add<dmat2x3, false>("Double");
	EXPECT_EQ(ret, 192);
#pragma endregion

#pragma region MAT2x4
	structOffsets.clear();

	// BOOL
	ret = structOffsets.add<bmat2x4, false>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.add<imat2x4, false>("Int");
	EXPECT_EQ(ret, 64);

	// UINT
	ret = structOffsets.add<umat2x4, false>("Uint");
	EXPECT_EQ(ret, 128);

	// FLOAT
	ret = structOffsets.add<fmat2x4, false>("Float");
	EXPECT_EQ(ret, 192);

	// DOUBLE
	ret = structOffsets.add<dmat2x4, false>("Double");
	EXPECT_EQ(ret, 256);
#pragma endregion

#pragma endregion
}

TEST(std140_offset, add_marixes_array) {
	std140_offset structOffsets;
	std::vector<size_t> results;
	std::vector<size_t> ret;

#pragma region COLUMN_MAJOR

#pragma region MAT2x2
	structOffsets.clear();

	// BOOL
	results = { 0, 32 };
	ret = structOffsets.add<bmat2>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 64, 96 };
	ret = structOffsets.add<imat2>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 128, 160 };
	ret = structOffsets.add<umat2>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 192, 224 };
	ret = structOffsets.add<fmat2>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 256, 288 };
	ret = structOffsets.add<dmat2>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma region MAT2x3
	structOffsets.clear();

	// BOOL
	results = { 0, 32 };
	ret = structOffsets.add<bmat2x3>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 64, 96 };
	ret = structOffsets.add<imat2x3>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 128, 160 };
	ret = structOffsets.add<umat2x3>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 192, 224 };
	ret = structOffsets.add<fmat2x3>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 256, 320 };
	ret = structOffsets.add<dmat2x3>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma region MAT2x4
	structOffsets.clear();

	// BOOL
	results = { 0, 32 };
	ret = structOffsets.add<bmat2x4>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 64, 96 };
	ret = structOffsets.add<imat2x4>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 128, 160 };
	ret = structOffsets.add<umat2x4>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 192, 224 };
	ret = structOffsets.add<fmat2x4>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 256, 320 };
	ret = structOffsets.add<dmat2x4>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma endregion

#pragma region ROW_MAJOR

#pragma region MAT2x2
	structOffsets.clear();

	// BOOL
	results = { 0, 32 };
	ret = structOffsets.add<bmat2, false>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 64, 96 };
	ret = structOffsets.add<imat2, false>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 128, 160 };
	ret = structOffsets.add<umat2, false>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 192, 224 };
	ret = structOffsets.add<fmat2, false>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 256, 288 };
	ret = structOffsets.add<dmat2, false>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma region MAT2x3
	structOffsets.clear();

	// BOOL
	results = { 0, 48 };
	ret = structOffsets.add<bmat2x3, false>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 96, 144 };
	ret = structOffsets.add<imat2x3, false>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 192, 240 };
	ret = structOffsets.add<umat2x3, false>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 288, 336 };
	ret = structOffsets.add<fmat2x3, false>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 384, 432 };
	ret = structOffsets.add<dmat2x3, false>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma region MAT2x4
	structOffsets.clear();

	// BOOL
	results = { 0, 64 };
	ret = structOffsets.add<bmat2x4, false>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 128, 192 };
	ret = structOffsets.add<imat2x4, false>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 256, 320 };
	ret = structOffsets.add<umat2x4, false>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 384, 448 };
	ret = structOffsets.add<fmat2x4, false>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 512, 576 };
	ret = structOffsets.add<dmat2x4, false>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma endregion
}

TEST(std140_offset, add_struct) {
	std140_offset structOffsets;
}

TEST(std140_offset, add_struct_array) {
	std140_offset structOffsets;
}

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

TEST(std140_offset, constructor_and_get) {
	size_t ret;
	std::vector<size_t> retVec;
	std::vector<size_t> resultVec;

	std140_offset subStruct1{
		std_variable<int>("d"),
		std_variable<bvec2>("e")
	};
	ret = subStruct1.get("d");
	EXPECT_EQ(ret, 0);
	ret = subStruct1.get("e");
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
	ret = subStruct2.get("j");
	EXPECT_EQ(ret, 0);
	ret = subStruct2.get("k");
	EXPECT_EQ(ret, 16);
	resultVec = { 32, 48 };
	retVec = subStruct2.getArray("l");
	EXPECT_EQ(retVec, resultVec);
	ret = subStruct2.get("m");
	EXPECT_EQ(ret, 64);
	resultVec = { 80, 128 };
	retVec = subStruct2.getArray("n");
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
	ret = structOffsets.get("a");
	EXPECT_EQ(ret, 0);
	ret = structOffsets.get("b");
	EXPECT_EQ(ret, 8);
	ret = structOffsets.get("c");
	EXPECT_EQ(ret, 16);
	ret = structOffsets.get("f");
	EXPECT_EQ(ret, 32);
	
	// F sub values
	ret = structOffsets.get("f.d");
	EXPECT_EQ(ret, 32);
	ret = structOffsets.get("f.e");
	EXPECT_EQ(ret, 40);

	ret = structOffsets.get("g");
	EXPECT_EQ(ret, 48);
	resultVec = { 64, 80 };
	retVec = structOffsets.getArray("h");
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.get("i");
	EXPECT_EQ(ret, 96);
	resultVec = { 128, 304 };
	retVec = structOffsets.getArray("o");
	EXPECT_EQ(retVec, resultVec);

	// O1 sub values
	ret = structOffsets.get("o[0].j");
	EXPECT_EQ(ret, 128);
	ret = structOffsets.get("o[0].k");
	EXPECT_EQ(ret, 144);
	retVec = structOffsets.getArray("o[0].l");
	resultVec = { 160, 176 };
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.get("o[0].m");
	EXPECT_EQ(ret, 192);
	retVec = structOffsets.getArray("o[0].n");
	resultVec = { 208, 256 };
	EXPECT_EQ(retVec, resultVec);

	// O2 sub values
	ret = structOffsets.get("o[1].j");
	EXPECT_EQ(ret, 304);
	ret = structOffsets.get("o[1].k");
	EXPECT_EQ(ret, 320);
	retVec = structOffsets.getArray("o[1].l");
	resultVec = { 336, 352 };
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.get("o[1].m");
	EXPECT_EQ(ret, 368);
	retVec = structOffsets.getArray("o[1].n");
	resultVec = { 384, 432 };
	EXPECT_EQ(retVec, resultVec);

	ret = structOffsets.size();
	EXPECT_EQ(ret, 480);
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
	std::vector<size_t> resultVec{0, 176, 352, 528, 704, 880, 1056, 1232};
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

TEST(std430_offset, constructor_and_get) {
	size_t ret;
	std::vector<size_t> retVec;

#pragma region RECT
	std430_offset rect{
		std_variable<mat4>("transform"),
		std_variable<vec2>("size")
	};
	ret = rect.get("transform");
	EXPECT_EQ(ret, 0);
	ret = rect.get("size");
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
	ret = sprite.get("offset");
	EXPECT_EQ(ret, 0);
	ret = sprite.get("size");
	EXPECT_EQ(ret, 8);
	ret = sprite.get("isActive");
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
	ret = fill.get("type");
	EXPECT_EQ(ret, 0);
	ret = fill.get("subType");
	EXPECT_EQ(ret, 4);
	ret = fill.get("offset");
	EXPECT_EQ(ret, 8);
	ret = fill.get("progress");
	EXPECT_EQ(ret, 12);
	ret = fill.get("rotation");
	EXPECT_EQ(ret, 16);
	ret = fill.get("isActive");
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
	ret = uiElement.get("rect");
	EXPECT_EQ(ret, 0);

	// RECT sub Values
	ret = uiElement.get("rect.transform");
	EXPECT_EQ(ret, 0);
	ret = uiElement.get("rect.size");
	EXPECT_EQ(ret, 64);

	ret = uiElement.get("sprite");
	EXPECT_EQ(ret, 80);

	// SPRITE sub Values
	ret = uiElement.get("sprite.offset");
	EXPECT_EQ(ret, 80);
	ret = uiElement.get("sprite.size");
	EXPECT_EQ(ret, 88);
	ret = uiElement.get("sprite.isActive");
	EXPECT_EQ(ret, 96);

	ret = uiElement.get("fill");
	EXPECT_EQ(ret, 112);

	// FILL sub Values
	ret = uiElement.get("fill.type");
	EXPECT_EQ(ret, 112);
	ret = uiElement.get("fill.subType");
	EXPECT_EQ(ret, 116);
	ret = uiElement.get("fill.offset");
	EXPECT_EQ(ret, 120);
	ret = uiElement.get("fill.progress");
	EXPECT_EQ(ret, 124);
	ret = uiElement.get("fill.rotation");
	EXPECT_EQ(ret, 128);
	ret = uiElement.get("fill.isActive");
	EXPECT_EQ(ret, 132);

	ret = uiElement.get("color");
	EXPECT_EQ(ret, 144);
	ret = uiElement.get("isText");
	EXPECT_EQ(ret, 160);
	ret = uiElement.size();
	EXPECT_EQ(ret, 176);
#pragma endregion

#pragma region TEXTURE
	std430_offset texture{
		std_variable<uvec2>("size"),
		std_variable<bool>("isActive")
	};
	ret = texture.get("size");
	EXPECT_EQ(ret, 0);
	ret = texture.get("isActive");
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
	retVec = ssbo.getArray("uiElements");
	EXPECT_EQ(retVec, resultVec);

	// UI FIRST ELEMENT SUB VALUES TEST
	ret = ssbo.get("uiElements[0].rect");
	EXPECT_EQ(ret, 0);

	// RECT sub Values
	ret = ssbo.get("uiElements[0].rect.transform");
	EXPECT_EQ(ret, 0);
	ret = ssbo.get("uiElements[0].rect.size");
	EXPECT_EQ(ret, 64);

	ret = ssbo.get("uiElements[0].sprite");
	EXPECT_EQ(ret, 80);

	// SPRITE sub Values
	ret = ssbo.get("uiElements[0].sprite.offset");
	EXPECT_EQ(ret, 80);
	ret = ssbo.get("uiElements[0].sprite.size");
	EXPECT_EQ(ret, 88);
	ret = ssbo.get("uiElements[0].sprite.isActive");
	EXPECT_EQ(ret, 96);

	ret = ssbo.get("uiElements[0].fill");
	EXPECT_EQ(ret, 112);

	// FILL sub Values
	ret = ssbo.get("uiElements[0].fill.type");
	EXPECT_EQ(ret, 112);
	ret = ssbo.get("uiElements[0].fill.subType");
	EXPECT_EQ(ret, 116);
	ret = ssbo.get("uiElements[0].fill.offset");
	EXPECT_EQ(ret, 120);
	ret = ssbo.get("uiElements[0].fill.progress");
	EXPECT_EQ(ret, 124);
	ret = ssbo.get("uiElements[0].fill.rotation");
	EXPECT_EQ(ret, 128);
	ret = ssbo.get("uiElements[0].fill.isActive");
	EXPECT_EQ(ret, 132);

	ret = ssbo.get("uiElements[0].color");
	EXPECT_EQ(ret, 144);
	ret = ssbo.get("uiElements[0].isText");
	EXPECT_EQ(ret, 160);

	ret = ssbo.get("elementTexture");
	EXPECT_EQ(ret, 1408);

	// TEXTURE SUB VALUES
	ret = ssbo.get("elementTexture.size");
	EXPECT_EQ(ret, 1408);
	ret = ssbo.get("elementTexture.isActive");
	EXPECT_EQ(ret, 1416);

	ret = ssbo.get("elementLayer");
	EXPECT_EQ(ret, 1424);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1440);
#pragma endregion
}

TEST(std_struct, add_array) {
	std430_struct test{
		std_value<int, 3>("test", { 2, 3, 4 })
	};
	int value = test.get<int>("test[1]");
	EXPECT_EQ(value, 3);
	size_t ret = test.getOffset("test");
	EXPECT_EQ(ret, 0);
}

TEST(std_struct, copy_test) {
	std430_struct test{
		std_value<int, 3>("test", { 2, 3, 4 })
	};
	int value = test.get<int>("test[1]");
	EXPECT_EQ(value, 3);
	size_t ret = test.getOffset("test");
	EXPECT_EQ(ret, 0);

	std430_struct test1 = test;
	value = test1.get<int>("test[1]");
	EXPECT_EQ(value, 3);
	ret = test1.getOffset("test");
	EXPECT_EQ(ret, 0);

	std430_struct* test2 = test.Clone();
	value = test2->get<int>("test[1]");
	EXPECT_EQ(value, 3);
	ret = test2->getOffset("test");
	EXPECT_EQ(ret, 0);
	delete test2;
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
	ret = rect.getOffset("transform");
	EXPECT_EQ(ret, 0);
	ret = rect.getOffset("size");
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
	ret = sprite.getOffset("offset");
	EXPECT_EQ(ret, 0);
	ret = sprite.getOffset("size");
	EXPECT_EQ(ret, 8);
	ret = sprite.getOffset("isActive");
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
	ret = fill.getOffset("type");
	EXPECT_EQ(ret, 0);
	ret = fill.getOffset("subType");
	EXPECT_EQ(ret, 4);
	ret = fill.getOffset("offset");
	EXPECT_EQ(ret, 8);
	ret = fill.getOffset("progress");
	EXPECT_EQ(ret, 12);
	ret = fill.getOffset("rotation");
	EXPECT_EQ(ret, 16);
	ret = fill.getOffset("isActive");
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
	ret = uiElement.getOffset("rect");
	EXPECT_EQ(ret, 0);

	// RECT sub Values
	ret = uiElement.getOffset("rect.transform");
	EXPECT_EQ(ret, 0);
	ret = uiElement.getOffset("rect.size");
	EXPECT_EQ(ret, 64);

	ret = uiElement.getOffset("sprite");
	EXPECT_EQ(ret, 80);

	// SPRITE sub Values
	ret = uiElement.getOffset("sprite.offset");
	EXPECT_EQ(ret, 80);
	ret = uiElement.getOffset("sprite.size");
	EXPECT_EQ(ret, 88);
	ret = uiElement.getOffset("sprite.isActive");
	EXPECT_EQ(ret, 96);

	ret = uiElement.getOffset("fill");
	EXPECT_EQ(ret, 112);

	// FILL sub Values
	ret = uiElement.getOffset("fill.type");
	EXPECT_EQ(ret, 112);
	ret = uiElement.getOffset("fill.subType");
	EXPECT_EQ(ret, 116);
	ret = uiElement.getOffset("fill.offset");
	EXPECT_EQ(ret, 120);
	ret = uiElement.getOffset("fill.progress");
	EXPECT_EQ(ret, 124);
	ret = uiElement.getOffset("fill.rotation");
	EXPECT_EQ(ret, 128);
	ret = uiElement.getOffset("fill.isActive");
	EXPECT_EQ(ret, 132);

	ret = uiElement.getOffset("color");
	EXPECT_EQ(ret, 144);
	ret = uiElement.getOffset("isText");
	EXPECT_EQ(ret, 160);
	ret = uiElement.size();
	EXPECT_EQ(ret, 176);
#pragma endregion

#pragma region TEXTURE
	std430_struct texture{
		std_value<uvec2>("size"),
		std_value<bool>("isActive")
	};
	ret = texture.getOffset("size");
	EXPECT_EQ(ret, 0);
	ret = texture.getOffset("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.size();
	EXPECT_EQ(ret, 16);
#pragma endregion

#pragma region SSBO
	std430_struct ssbo{
		std_value<std430_struct, 8>("uiElements", uiElement.getOffsets()),
		std_value<std430_struct>("elementTexture", texture),
		std_value<int>("elementLayer")
	};
	std::vector<size_t> resultVec{ 0, 176, 352, 528, 704, 880, 1056, 1232 };
	retVec = ssbo.getArrayOffsets("uiElements");
	EXPECT_EQ(retVec, resultVec);

	// UI FIRST ELEMENT SUB VALUES TEST
	ret = ssbo.getOffset("uiElements[0].rect");
	EXPECT_EQ(ret, 0);

	// RECT sub Values
	ret = ssbo.getOffset("uiElements[0].rect.transform");
	EXPECT_EQ(ret, 0);
	ret = ssbo.getOffset("uiElements[0].rect.size");
	EXPECT_EQ(ret, 64);

	ret = ssbo.getOffset("uiElements[0].sprite");
	EXPECT_EQ(ret, 80);

	// SPRITE sub Values
	ret = ssbo.getOffset("uiElements[0].sprite.offset");
	EXPECT_EQ(ret, 80);
	ret = ssbo.getOffset("uiElements[0].sprite.size");
	EXPECT_EQ(ret, 88);
	ret = ssbo.getOffset("uiElements[0].sprite.isActive");
	EXPECT_EQ(ret, 96);

	ret = ssbo.getOffset("uiElements[0].fill");
	EXPECT_EQ(ret, 112);

	// FILL sub Values
	ret = ssbo.getOffset("uiElements[0].fill.type");
	EXPECT_EQ(ret, 112);
	ret = ssbo.getOffset("uiElements[0].fill.subType");
	EXPECT_EQ(ret, 116);
	ret = ssbo.getOffset("uiElements[0].fill.offset");
	EXPECT_EQ(ret, 120);
	ret = ssbo.getOffset("uiElements[0].fill.progress");
	EXPECT_EQ(ret, 124);
	ret = ssbo.getOffset("uiElements[0].fill.rotation");
	EXPECT_EQ(ret, 128);
	ret = ssbo.getOffset("uiElements[0].fill.isActive");
	EXPECT_EQ(ret, 132);

	ret = ssbo.getOffset("uiElements[0].color");
	EXPECT_EQ(ret, 144);
	ret = ssbo.getOffset("uiElements[0].isText");
	EXPECT_EQ(ret, 160);

	ret = ssbo.getOffset("elementTexture");
	EXPECT_EQ(ret, 1408);

	// TEXTURE SUB VALUES
	ret = ssbo.getOffset("elementTexture.size");
	EXPECT_EQ(ret, 1408);
	ret = ssbo.getOffset("elementTexture.isActive");
	EXPECT_EQ(ret, 1416);

	ret = ssbo.getOffset("elementLayer");
	EXPECT_EQ(ret, 1424);
	ret = ssbo.size();
	EXPECT_EQ(ret, 1440);
#pragma endregion
}