#include "pch.h"

using namespace glsl;
using namespace glm;

namespace glm {
	using bmat2x2 = mat<2, 2, bool, packed_highp>;
	using bmat2x3 = mat<2, 3, bool, packed_highp>;
	using bmat2x4 = mat<2, 4, bool, packed_highp>;
	using bmat3x2 = mat<3, 2, bool, packed_highp>;
	using bmat3x3 = mat<3, 3, bool, packed_highp>;
	using bmat3x4 = mat<3, 4, bool, packed_highp>;
	using bmat4x2 = mat<4, 2, bool, packed_highp>;
	using bmat4x3 = mat<4, 3, bool, packed_highp>;
	using bmat4x4 = mat<4, 4, bool, packed_highp>;

	using bmat2 = bmat2x2;
	using bmat3 = bmat3x3;
	using bmat4 = bmat4x4;

	using umat2 = umat2x2;
	using umat3 = umat3x3;
	using umat4 = umat4x4;

	using imat2 = imat2x2;
	using imat3 = imat3x3;
	using imat4 = imat4x4;
}

TEST(STD140Offsets, AddScalars) {
	STD140Offsets structOffsets;
	size_t ret;

	// BOOL
	ret = structOffsets.Add<bool>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.Add<int>("Int");
	EXPECT_EQ(ret, 4);

	// UINT
	ret = structOffsets.Add<unsigned int>("Uint");
	EXPECT_EQ(ret, 8);

	// FLOAT
	ret = structOffsets.Add<float>("Float");
	EXPECT_EQ(ret, 12);

	// DOUBLE
	ret = structOffsets.Add<double>("Double");
	EXPECT_EQ(ret, 16);
}

TEST(STD140Offsets, AddScalarsArray) {
	STD140Offsets structOffsets;
	std::vector<size_t> results;
	std::vector<size_t> ret;

	// BOOL
	results = { 0, 16 };
	ret = structOffsets.Add<bool>("Bools", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 32, 48 };
	ret = structOffsets.Add<int>("Ints", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 64, 80 };
	ret = structOffsets.Add<unsigned int>("Uints", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 96, 112 };
	ret = structOffsets.Add<float>("Floats", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 128, 144 };
	ret = structOffsets.Add<double>("Doubles", 2);
	EXPECT_EQ(ret, results);
}

TEST(STD140Offsets, AddVectors) {
	STD140Offsets structOffsets;
	size_t ret;

#pragma region VEC2
	structOffsets.Clear();
	
	// BOOL
	ret = structOffsets.Add<bvec2>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.Add<ivec2>("Int");
	EXPECT_EQ(ret, 8);

	// UINT
	ret = structOffsets.Add<uvec2>("Uint");
	EXPECT_EQ(ret, 16);

	// FLOAT
	ret = structOffsets.Add<vec2>("Float");
	EXPECT_EQ(ret, 24);

	// DOUBLE
	ret = structOffsets.Add<dvec2>("Double");
	EXPECT_EQ(ret, 32);
#pragma endregion

#pragma region VEC3
	structOffsets.Clear();

	// BOOL
	ret = structOffsets.Add<bvec3>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.Add<ivec3>("Int");
	EXPECT_EQ(ret, 16);

	// UINT
	ret = structOffsets.Add<uvec3>("Uint");
	EXPECT_EQ(ret, 32);

	// FLOAT
	ret = structOffsets.Add<vec3>("Float");
	EXPECT_EQ(ret, 48);

	// DOUBLE
	ret = structOffsets.Add<dvec3>("Double");
	EXPECT_EQ(ret, 64);
#pragma endregion

#pragma region VEC4
	structOffsets.Clear();

	// BOOL
	ret = structOffsets.Add<bvec4>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.Add<ivec4>("Int1");
	EXPECT_EQ(ret, 16);

	// UINT
	ret = structOffsets.Add<uvec4>("Uint1");
	EXPECT_EQ(ret, 32);

	// FLOAT
	ret = structOffsets.Add<vec4>("Float1");
	EXPECT_EQ(ret, 48);

	// DOUBLE
	ret = structOffsets.Add<dvec4>("Double1");
	EXPECT_EQ(ret, 64);
#pragma endregion
}

TEST(STD140Offsets, AddVectorsArray) {
	STD140Offsets structOffsets;
	std::vector<size_t> results;
	std::vector<size_t> ret;

#pragma region VEC2
	structOffsets.Clear();

	// BOOL
	results = { 0, 16 };
	ret = structOffsets.Add<bvec2>("Bools", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 32, 48 };
	ret = structOffsets.Add<ivec2>("Ints", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 64, 80 };
	ret = structOffsets.Add<uvec2>("Uints", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 96, 112 };
	ret = structOffsets.Add<vec2>("Floats", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 128, 144 };
	ret = structOffsets.Add<dvec2>("Doubles", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma region VEC3
	structOffsets.Clear();

	// BOOL
	results = { 0, 16 };
	ret = structOffsets.Add<bvec3>("Bools", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 32, 48 };
	ret = structOffsets.Add<ivec3>("Ints", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 64, 80 };
	ret = structOffsets.Add<uvec3>("Uints", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 96, 112 };
	ret = structOffsets.Add<vec3>("Floats", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 128, 160 };
	ret = structOffsets.Add<dvec3>("Doubles", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma region VEC4
	structOffsets.Clear();

	// BOOL
	results = { 0, 16 };
	ret = structOffsets.Add<bvec4>("Bools", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 32, 48 };
	ret = structOffsets.Add<ivec4>("Ints", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 64, 80 };
	ret = structOffsets.Add<uvec4>("Uints", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 96, 112 };
	ret = structOffsets.Add<vec4>("Floats", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 128, 160 };
	ret = structOffsets.Add<dvec4>("Doubles", 2);
	EXPECT_EQ(ret, results);
#pragma endregion
}

TEST(STD140Offsets, AddMatrixes) {
	STD140Offsets structOffsets;
	size_t ret;

#pragma region COLUMN_MAJOR

#pragma region MAT2x2
	structOffsets.Clear();

	// BOOL
	ret = structOffsets.Add<bmat2>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.Add<imat2>("Int");
	EXPECT_EQ(ret, 32);

	// UINT
	ret = structOffsets.Add<umat2>("Uint");
	EXPECT_EQ(ret, 64);

	// FLOAT
	ret = structOffsets.Add<fmat2>("Float");
	EXPECT_EQ(ret, 96);

	// DOUBLE
	ret = structOffsets.Add<dmat2>("Double");
	EXPECT_EQ(ret, 128);
#pragma endregion

#pragma region MAT2x3
	structOffsets.Clear();

	// BOOL
	ret = structOffsets.Add<bmat2x3>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.Add<imat2x3>("Int");
	EXPECT_EQ(ret, 32);

	// UINT
	ret = structOffsets.Add<umat2x3>("Uint");
	EXPECT_EQ(ret, 64);

	// FLOAT
	ret = structOffsets.Add<fmat2x3>("Float");
	EXPECT_EQ(ret, 96);

	// DOUBLE
	ret = structOffsets.Add<dmat2x3>("Double");
	EXPECT_EQ(ret, 128);
#pragma endregion

#pragma region MAT2x4
	structOffsets.Clear();

	// BOOL
	ret = structOffsets.Add<bmat2x4>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.Add<imat2x4>("Int");
	EXPECT_EQ(ret, 32);

	// UINT
	ret = structOffsets.Add<umat2x4>("Uint");
	EXPECT_EQ(ret, 64);

	// FLOAT
	ret = structOffsets.Add<fmat2x4>("Float");
	EXPECT_EQ(ret, 96);

	// DOUBLE
	ret = structOffsets.Add<dmat2x4>("Double");
	EXPECT_EQ(ret, 128);
#pragma endregion

#pragma endregion

#pragma region ROW_MAJOR

#pragma region MAT2x2
	structOffsets.Clear();

	// BOOL
	ret = structOffsets.Add<bmat2, false>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.Add<imat2, false>("Int");
	EXPECT_EQ(ret, 32);

	// UINT
	ret = structOffsets.Add<umat2, false>("Uint");
	EXPECT_EQ(ret, 64);

	// FLOAT
	ret = structOffsets.Add<fmat2, false>("Float");
	EXPECT_EQ(ret, 96);

	// DOUBLE
	ret = structOffsets.Add<dmat2, false>("Double");
	EXPECT_EQ(ret, 128);
#pragma endregion

#pragma region MAT2x3
	structOffsets.Clear();

	// BOOL
	ret = structOffsets.Add<bmat2x3, false>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.Add<imat2x3, false>("Int");
	EXPECT_EQ(ret, 48);

	// UINT
	ret = structOffsets.Add<umat2x3, false>("Uint");
	EXPECT_EQ(ret, 96);

	// FLOAT
	ret = structOffsets.Add<fmat2x3, false>("Float");
	EXPECT_EQ(ret, 144);

	// DOUBLE
	ret = structOffsets.Add<dmat2x3, false>("Double");
	EXPECT_EQ(ret, 192);
#pragma endregion

#pragma region MAT2x4
	structOffsets.Clear();

	// BOOL
	ret = structOffsets.Add<bmat2x4, false>("Bool");
	EXPECT_EQ(ret, 0);

	// INT
	ret = structOffsets.Add<imat2x4, false>("Int");
	EXPECT_EQ(ret, 64);

	// UINT
	ret = structOffsets.Add<umat2x4, false>("Uint");
	EXPECT_EQ(ret, 128);

	// FLOAT
	ret = structOffsets.Add<fmat2x4, false>("Float");
	EXPECT_EQ(ret, 192);

	// DOUBLE
	ret = structOffsets.Add<dmat2x4, false>("Double");
	EXPECT_EQ(ret, 256);
#pragma endregion

#pragma endregion
}

TEST(STD140Offsets, AddMarixesArray) {
	STD140Offsets structOffsets;
	std::vector<size_t> results;
	std::vector<size_t> ret;

#pragma region COLUMN_MAJOR

#pragma region MAT2x2
	structOffsets.Clear();

	// BOOL
	results = { 0, 32 };
	ret = structOffsets.Add<bmat2>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 64, 96 };
	ret = structOffsets.Add<imat2>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 128, 160 };
	ret = structOffsets.Add<umat2>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 192, 224 };
	ret = structOffsets.Add<fmat2>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 256, 288 };
	ret = structOffsets.Add<dmat2>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma region MAT2x3
	structOffsets.Clear();

	// BOOL
	results = { 0, 32 };
	ret = structOffsets.Add<bmat2x3>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 64, 96 };
	ret = structOffsets.Add<imat2x3>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 128, 160 };
	ret = structOffsets.Add<umat2x3>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 192, 224 };
	ret = structOffsets.Add<fmat2x3>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 256, 320 };
	ret = structOffsets.Add<dmat2x3>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma region MAT2x4
	structOffsets.Clear();

	// BOOL
	results = { 0, 32 };
	ret = structOffsets.Add<bmat2x4>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 64, 96 };
	ret = structOffsets.Add<imat2x4>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 128, 160 };
	ret = structOffsets.Add<umat2x4>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 192, 224 };
	ret = structOffsets.Add<fmat2x4>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 256, 320 };
	ret = structOffsets.Add<dmat2x4>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma endregion

#pragma region ROW_MAJOR

#pragma region MAT2x2
	structOffsets.Clear();

	// BOOL
	results = { 0, 32 };
	ret = structOffsets.Add<bmat2, false>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 64, 96 };
	ret = structOffsets.Add<imat2, false>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 128, 160 };
	ret = structOffsets.Add<umat2, false>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 192, 224 };
	ret = structOffsets.Add<fmat2, false>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 256, 288 };
	ret = structOffsets.Add<dmat2, false>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma region MAT2x3
	structOffsets.Clear();

	// BOOL
	results = { 0, 48 };
	ret = structOffsets.Add<bmat2x3, false>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 96, 144 };
	ret = structOffsets.Add<imat2x3, false>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 192, 240 };
	ret = structOffsets.Add<umat2x3, false>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 288, 336 };
	ret = structOffsets.Add<fmat2x3, false>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 384, 432 };
	ret = structOffsets.Add<dmat2x3, false>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma region MAT2x4
	structOffsets.Clear();

	// BOOL
	results = { 0, 64 };
	ret = structOffsets.Add<bmat2x4, false>("Bool", 2);
	EXPECT_EQ(ret, results);

	// INT
	results = { 128, 192 };
	ret = structOffsets.Add<imat2x4, false>("Int", 2);
	EXPECT_EQ(ret, results);

	// UINT
	results = { 256, 320 };
	ret = structOffsets.Add<umat2x4, false>("Uint", 2);
	EXPECT_EQ(ret, results);

	// FLOAT
	results = { 384, 448 };
	ret = structOffsets.Add<fmat2x4, false>("Float", 2);
	EXPECT_EQ(ret, results);

	// DOUBLE
	results = { 512, 576 };
	ret = structOffsets.Add<dmat2x4, false>("Double", 2);
	EXPECT_EQ(ret, results);
#pragma endregion

#pragma endregion
}

TEST(STD140Offsets, AddStruct) {
	STD140Offsets structOffsets;
}

TEST(STD140Offsets, AddStructArray) {
	STD140Offsets structOffsets;
}

TEST(STD140Offsets, OffsetCalculation1) {
	STD140Offsets structOffsets;
	STD140Offsets subStructOffsets;
	size_t ret;
	std::vector<size_t> retVec;
	std::vector<size_t> resultVec;

	ret = structOffsets.Add<float>("a");
	EXPECT_EQ(ret, 0);
	ret = structOffsets.Add<vec2>("b");
	EXPECT_EQ(ret, 8);
	ret = structOffsets.Add<vec3>("c");
	EXPECT_EQ(ret, 16);

	ret = subStructOffsets.Add<int>("d");
	EXPECT_EQ(ret, 0);
	ret = subStructOffsets.Add<bvec2>("e");
	EXPECT_EQ(ret, 8);
	ret = subStructOffsets.GetSize();
	EXPECT_EQ(ret, 16);

	ret = structOffsets.Add("f", subStructOffsets);
	EXPECT_EQ(ret, 32);
	ret = structOffsets.Add<float>("g");
	EXPECT_EQ(ret, 48);
	resultVec = { 64, 80 };
	retVec = structOffsets.Add<float>("h", 2);
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.Add<mat2x3>("i");
	EXPECT_EQ(ret, 96);

	subStructOffsets.Clear();
	ret = subStructOffsets.Add<uvec3>("j");
	EXPECT_EQ(ret, 0);
	ret = subStructOffsets.Add<vec2>("k");
	EXPECT_EQ(ret, 16);
	resultVec = { 32, 48 };
	retVec = subStructOffsets.Add<float>("l", 2);
	EXPECT_EQ(retVec, resultVec);
	ret = subStructOffsets.Add<vec2>("m");
	EXPECT_EQ(ret, 64);
	resultVec = { 80, 128 };
	retVec = subStructOffsets.Add<mat3>("n", 2);
	EXPECT_EQ(retVec, resultVec);
	ret = subStructOffsets.GetSize();
	EXPECT_EQ(ret, 176);

	resultVec = { 128, 304 };
	retVec = structOffsets.Add("o", subStructOffsets, 2);
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.GetSize();
	EXPECT_EQ(ret, 480);
}

TEST(STD140Offsets, OffsetCalculation2) {
	STD140Offsets structOffsets;
	STD140Offsets subStructOffsets;
	size_t ret;

	ret = subStructOffsets.Add<bool>("has_diffuse_texture");
	EXPECT_EQ(ret, 0);
	ret = subStructOffsets.Add<bool>("has_specular_texture");
	EXPECT_EQ(ret, 4);
	ret = subStructOffsets.Add<vec3>("color");
	EXPECT_EQ(ret, 16);
	ret = subStructOffsets.Add<float>("shininess");
	EXPECT_EQ(ret, 28);
	ret = subStructOffsets.Add<unsigned int>("diffuse_toon_borders");
	EXPECT_EQ(ret, 32);
	ret = subStructOffsets.Add<unsigned int>("specular_toon_borders");
	EXPECT_EQ(ret, 36);
	ret = subStructOffsets.Add<vec2>("highlight_translate");
	EXPECT_EQ(ret, 40);
	ret = subStructOffsets.Add<vec2>("highlight_rotation");
	EXPECT_EQ(ret, 48);
	ret = subStructOffsets.Add<vec2>("highlight_scale");
	EXPECT_EQ(ret, 56);
	ret = subStructOffsets.Add<vec2>("highlight_split");
	EXPECT_EQ(ret, 64);
	ret = subStructOffsets.Add<int>("highlight_square_n");
	EXPECT_EQ(ret, 72);
	ret = subStructOffsets.Add<float>("highlight_square_x");
	EXPECT_EQ(ret, 76);
	ret = subStructOffsets.GetSize();
	EXPECT_EQ(ret, 80);

	std::vector<size_t> resultVec{ 0, 80, 160, 240, 320, 400, 480, 560 };
	std::vector<size_t> retVec = structOffsets.Add("materialInputs", subStructOffsets, 8);
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.GetSize();
	EXPECT_EQ(ret, 640);
}

TEST(STD140Offsets, OffsetCalculation3) {
	STD140Offsets structOffsets;
	size_t ret;

	ret = structOffsets.Add<vec2>("windowSize");
	EXPECT_EQ(ret, 0);
	ret = structOffsets.Add<float>("nearPlane");
	EXPECT_EQ(ret, 8);
	ret = structOffsets.Add<float>("farPlane");
	EXPECT_EQ(ret, 12);
	ret = structOffsets.Add<float>("gamma");
	EXPECT_EQ(ret, 16);
	ret = structOffsets.GetSize();
	EXPECT_EQ(ret, 32);
}

TEST(STD140Offsets, ConstructorAndGet) {
	size_t ret;
	std::vector<size_t> retVec;
	std::vector<size_t> resultVec;

	STD140Offsets subStruct1{
		STDVariable<int>("d"),
		STDVariable<bvec2>("e")
	};
	ret = subStruct1.Get("d");
	EXPECT_EQ(ret, 0);
	ret = subStruct1.Get("e");
	EXPECT_EQ(ret, 8);
	ret = subStruct1.GetSize();
	EXPECT_EQ(ret, 16);

	STD140Offsets subStruct2{
		STDVariable<uvec3>("j"),
		STDVariable<vec2>("k"),
		STDVariable<float, 2>("l"),
		STDVariable<vec2>("m"),
		STDVariable<mat3, 2>("n")
	};
	ret = subStruct2.Get("j");
	EXPECT_EQ(ret, 0);
	ret = subStruct2.Get("k");
	EXPECT_EQ(ret, 16);
	resultVec = { 32, 48 };
	retVec = subStruct2.GetArray("l");
	EXPECT_EQ(retVec, resultVec);
	ret = subStruct2.Get("m");
	EXPECT_EQ(ret, 64);
	resultVec = { 80, 128 };
	retVec = subStruct2.GetArray("n");
	EXPECT_EQ(retVec, resultVec);
	ret = subStruct2.GetSize();
	EXPECT_EQ(ret, 176);

	STD140Offsets structOffsets{
		STDVariable<float>("a"),
		STDVariable<vec2>("b"),
		STDVariable<vec3>("c"),
		STDVariable<STD140Offsets>("f", subStruct1),
		STDVariable<float>("g"),
		STDVariable<float, 2>("h"),
		STDVariable<mat2x3>("i"),
		STDVariable<STD140Offsets, 2>("o", subStruct2)
	};
	ret = structOffsets.Get("a");
	EXPECT_EQ(ret, 0);
	ret = structOffsets.Get("b");
	EXPECT_EQ(ret, 8);
	ret = structOffsets.Get("c");
	EXPECT_EQ(ret, 16);
	ret = structOffsets.Get("f");
	EXPECT_EQ(ret, 32);
	
	// F sub values
	ret = structOffsets.Get("f.d");
	EXPECT_EQ(ret, 32);
	ret = structOffsets.Get("f.e");
	EXPECT_EQ(ret, 40);

	ret = structOffsets.Get("g");
	EXPECT_EQ(ret, 48);
	resultVec = { 64, 80 };
	retVec = structOffsets.GetArray("h");
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.Get("i");
	EXPECT_EQ(ret, 96);
	resultVec = { 128, 304 };
	retVec = structOffsets.GetArray("o");
	EXPECT_EQ(retVec, resultVec);

	// O1 sub values
	ret = structOffsets.Get("o[0].j");
	EXPECT_EQ(ret, 128);
	ret = structOffsets.Get("o[0].k");
	EXPECT_EQ(ret, 144);
	retVec = structOffsets.GetArray("o[0].l");
	resultVec = { 160, 176 };
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.Get("o[0].m");
	EXPECT_EQ(ret, 192);
	retVec = structOffsets.GetArray("o[0].n");
	resultVec = { 208, 256 };
	EXPECT_EQ(retVec, resultVec);

	// O2 sub values
	ret = structOffsets.Get("o[1].j");
	EXPECT_EQ(ret, 304);
	ret = structOffsets.Get("o[1].k");
	EXPECT_EQ(ret, 320);
	retVec = structOffsets.GetArray("o[1].l");
	resultVec = { 336, 352 };
	EXPECT_EQ(retVec, resultVec);
	ret = structOffsets.Get("o[1].m");
	EXPECT_EQ(ret, 368);
	retVec = structOffsets.GetArray("o[1].n");
	resultVec = { 384, 432 };
	EXPECT_EQ(retVec, resultVec);

	ret = structOffsets.GetSize();
	EXPECT_EQ(ret, 480);
}

TEST(STD430Offsets, OffsetCalculation1) {
	size_t ret;
	std::vector<size_t> retVec;

	STD430Offsets rect;
	ret = rect.Add<mat4>("transform");
	EXPECT_EQ(ret, 0);
	ret = rect.Add<vec2>("size");
	EXPECT_EQ(ret, 64);
	ret = rect.GetSize();
	EXPECT_EQ(ret, 80);

	STD430Offsets sprite;
	ret = sprite.Add<uvec2>("offset");
	EXPECT_EQ(ret, 0);
	ret = sprite.Add<uvec2>("size");
	EXPECT_EQ(ret, 8);
	ret = sprite.Add<bool>("isActive");
	EXPECT_EQ(ret, 16);
	ret = sprite.GetSize();
	EXPECT_EQ(ret, 32);

	STD430Offsets fill;
	ret = fill.Add<unsigned int>("type");
	EXPECT_EQ(ret, 0);
	ret = fill.Add<unsigned int>("subType");
	EXPECT_EQ(ret, 4);
	ret = fill.Add<float>("offset");
	EXPECT_EQ(ret, 8);
	ret = fill.Add<float>("progress");
	EXPECT_EQ(ret, 12);
	ret = fill.Add<float>("rotation");
	EXPECT_EQ(ret, 16);
	ret = fill.Add<bool>("isActive");
	EXPECT_EQ(ret, 20);
	ret = fill.GetSize();
	EXPECT_EQ(ret, 32);

	STD430Offsets uiElement;
	ret = uiElement.Add("rect", rect);
	EXPECT_EQ(ret, 0);
	ret = uiElement.Add("sprite", sprite);
	EXPECT_EQ(ret, 80);
	ret = uiElement.Add("fill", fill);
	EXPECT_EQ(ret, 112);
	ret = uiElement.Add<vec4>("color");
	EXPECT_EQ(ret, 144);
	ret = uiElement.Add<bool>("isText");
	EXPECT_EQ(ret, 160);
	ret = uiElement.GetSize();
	EXPECT_EQ(ret, 176);

	STD430Offsets texture;
	ret = texture.Add<uvec2>("size");
	EXPECT_EQ(ret, 0);
	ret = texture.Add<bool>("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.GetSize();
	EXPECT_EQ(ret, 16);

	STD430Offsets ssbo;
	std::vector<size_t> resultVec{0, 176, 352, 528, 704, 880, 1056, 1232};
	retVec = ssbo.Add("uiElements", uiElement, 8);
	EXPECT_EQ(retVec, resultVec);
	ret = ssbo.Add("elementTexture", texture);
	EXPECT_EQ(ret, 1408);
	ret = ssbo.Add<int>("elementLayer");
	EXPECT_EQ(ret, 1424);
	ret = ssbo.GetSize();
	EXPECT_EQ(ret, 1440);
}

TEST(STD430Offsets, OffsetCalculation2) {
	size_t ret;
	std::vector<size_t> retVec;

	STD430Offsets pointLight;
	ret = pointLight.Add<vec3>("position");
	EXPECT_EQ(ret, 0);
	ret = pointLight.Add<vec3>("color");
	EXPECT_EQ(ret, 16);
	ret = pointLight.Add<float>("power");
	EXPECT_EQ(ret, 28);
	ret = pointLight.Add<float>("constant");
	EXPECT_EQ(ret, 32);
	ret = pointLight.Add<float>("linear");
	EXPECT_EQ(ret, 36);
	ret = pointLight.Add<float>("quadratic");
	EXPECT_EQ(ret, 40);
	ret = pointLight.GetSize();
	EXPECT_EQ(ret, 48);

	STD430Offsets spotLight;
	ret = spotLight.Add<vec3>("position");
	EXPECT_EQ(ret, 0);
	ret = spotLight.Add<vec3>("direction");
	EXPECT_EQ(ret, 16);
	ret = spotLight.Add<float>("power");
	EXPECT_EQ(ret, 28);
	ret = spotLight.Add<vec3>("color");
	EXPECT_EQ(ret, 32);
	ret = spotLight.Add<float>("cutOff");
	EXPECT_EQ(ret, 44);
	ret = spotLight.Add<float>("outerCutOff");
	EXPECT_EQ(ret, 48);
	ret = spotLight.Add<float>("constant");
	EXPECT_EQ(ret, 52);
	ret = spotLight.Add<float>("linear");
	EXPECT_EQ(ret, 56);
	ret = spotLight.Add<float>("quadratic");
	EXPECT_EQ(ret, 60);
	ret = spotLight.GetSize();
	EXPECT_EQ(ret, 64);

	STD430Offsets dirLight;
	ret = dirLight.Add<vec3>("direction");
	EXPECT_EQ(ret, 0);
	ret = dirLight.Add<vec3>("color");
	EXPECT_EQ(ret, 16);
	ret = dirLight.Add<mat4>("lightSpaceMatrix");
	EXPECT_EQ(ret, 32);
	ret = dirLight.Add<float>("power");
	EXPECT_EQ(ret, 96);
	ret = dirLight.Add<float>("padding1");
	EXPECT_EQ(ret, 100);
	ret = dirLight.Add<float>("padding2");
	EXPECT_EQ(ret, 104);
	ret = dirLight.Add<float>("padding3");
	EXPECT_EQ(ret, 108);
	ret = dirLight.GetSize();
	EXPECT_EQ(ret, 112);

	STD430Offsets ssbo;
	std::vector<size_t> resultVec;
	ret = ssbo.Add<unsigned int>("numberOfPointLights");
	EXPECT_EQ(ret, 0);
	ret = ssbo.Add<unsigned int>("numberOfSpotLights");
	EXPECT_EQ(ret, 4);
	ret = ssbo.Add<unsigned int>("numberOfDirLights");
	EXPECT_EQ(ret, 8);
	retVec = ssbo.Add("pointLights", pointLight, 8);
	resultVec = { 16, 64, 112, 160, 208, 256, 304, 352 };
	EXPECT_EQ(retVec, resultVec);
	retVec = ssbo.Add("spotLights", spotLight, 8);
	resultVec = { 400, 464, 528, 592, 656, 720, 784, 848 };
	EXPECT_EQ(retVec, resultVec);
	retVec = ssbo.Add("dirLights", dirLight, 4);
	resultVec = { 912, 1024, 1136, 1248 };
	EXPECT_EQ(retVec, resultVec);
	ret = ssbo.GetSize();
	EXPECT_EQ(ret, 1360);
}

TEST(STD430Offsets, ConstructorAndGet) {
	size_t ret;
	std::vector<size_t> retVec;

#pragma region RECT
	STD430Offsets rect{
		STDVariable<mat4>("transform"),
		STDVariable<vec2>("size")
	};
	ret = rect.Get("transform");
	EXPECT_EQ(ret, 0);
	ret = rect.Get("size");
	EXPECT_EQ(ret, 64);
	ret = rect.GetSize();
	EXPECT_EQ(ret, 80);
#pragma endregion

#pragma region SPRITE
	STD430Offsets sprite{
		STDVariable<uvec2>("offset"),
		STDVariable<uvec2>("size"),
		STDVariable<bool>("isActive")
	};
	ret = sprite.Get("offset");
	EXPECT_EQ(ret, 0);
	ret = sprite.Get("size");
	EXPECT_EQ(ret, 8);
	ret = sprite.Get("isActive");
	EXPECT_EQ(ret, 16);
	ret = sprite.GetSize();
	EXPECT_EQ(ret, 32);
#pragma endregion

#pragma region FILL
	STD430Offsets fill{
		STDVariable<unsigned int>("type"),
		STDVariable<unsigned int>("subType"),
		STDVariable<float>("offset"),
		STDVariable<float>("progress"),
		STDVariable<float>("rotation"),
		STDVariable<bool>("isActive")
	};
	ret = fill.Get("type");
	EXPECT_EQ(ret, 0);
	ret = fill.Get("subType");
	EXPECT_EQ(ret, 4);
	ret = fill.Get("offset");
	EXPECT_EQ(ret, 8);
	ret = fill.Get("progress");
	EXPECT_EQ(ret, 12);
	ret = fill.Get("rotation");
	EXPECT_EQ(ret, 16);
	ret = fill.Get("isActive");
	EXPECT_EQ(ret, 20);
	ret = fill.GetSize();
	EXPECT_EQ(ret, 32);
#pragma endregion

#pragma region UIElement
	STD430Offsets uiElement{
		STDVariable<STD430Offsets>("rect", rect),
		STDVariable<STD430Offsets>("sprite", sprite),
		STDVariable<STD430Offsets>("fill", fill),
		STDVariable<vec4>("color"),
		STDVariable<bool>("isText")
	};
	ret = uiElement.Get("rect");
	EXPECT_EQ(ret, 0);

	// RECT sub Values
	ret = uiElement.Get("rect.transform");
	EXPECT_EQ(ret, 0);
	ret = uiElement.Get("rect.size");
	EXPECT_EQ(ret, 64);

	ret = uiElement.Get("sprite");
	EXPECT_EQ(ret, 80);

	// SPRITE sub Values
	ret = uiElement.Get("sprite.offset");
	EXPECT_EQ(ret, 80);
	ret = uiElement.Get("sprite.size");
	EXPECT_EQ(ret, 88);
	ret = uiElement.Get("sprite.isActive");
	EXPECT_EQ(ret, 96);

	ret = uiElement.Get("fill");
	EXPECT_EQ(ret, 112);

	// FILL sub Values
	ret = uiElement.Get("fill.type");
	EXPECT_EQ(ret, 112);
	ret = uiElement.Get("fill.subType");
	EXPECT_EQ(ret, 116);
	ret = uiElement.Get("fill.offset");
	EXPECT_EQ(ret, 120);
	ret = uiElement.Get("fill.progress");
	EXPECT_EQ(ret, 124);
	ret = uiElement.Get("fill.rotation");
	EXPECT_EQ(ret, 128);
	ret = uiElement.Get("fill.isActive");
	EXPECT_EQ(ret, 132);

	ret = uiElement.Get("color");
	EXPECT_EQ(ret, 144);
	ret = uiElement.Get("isText");
	EXPECT_EQ(ret, 160);
	ret = uiElement.GetSize();
	EXPECT_EQ(ret, 176);
#pragma endregion

#pragma region TEXTURE
	STD430Offsets texture{
		STDVariable<uvec2>("size"),
		STDVariable<bool>("isActive")
	};
	ret = texture.Get("size");
	EXPECT_EQ(ret, 0);
	ret = texture.Get("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.GetSize();
	EXPECT_EQ(ret, 16);
#pragma endregion

#pragma region SSBO
	STD430Offsets ssbo{
		STDVariable<STD430Offsets, 8>("uiElements", uiElement),
		STDVariable<STD430Offsets>("elementTexture", texture),
		STDVariable<int>("elementLayer")
	};
	std::vector<size_t> resultVec{ 0, 176, 352, 528, 704, 880, 1056, 1232 };
	retVec = ssbo.GetArray("uiElements");
	EXPECT_EQ(retVec, resultVec);

	// UI FIRST ELEMENT SUB VALUES TEST
	ret = ssbo.Get("uiElements[0].rect");
	EXPECT_EQ(ret, 0);

	// RECT sub Values
	ret = ssbo.Get("uiElements[0].rect.transform");
	EXPECT_EQ(ret, 0);
	ret = ssbo.Get("uiElements[0].rect.size");
	EXPECT_EQ(ret, 64);

	ret = ssbo.Get("uiElements[0].sprite");
	EXPECT_EQ(ret, 80);

	// SPRITE sub Values
	ret = ssbo.Get("uiElements[0].sprite.offset");
	EXPECT_EQ(ret, 80);
	ret = ssbo.Get("uiElements[0].sprite.size");
	EXPECT_EQ(ret, 88);
	ret = ssbo.Get("uiElements[0].sprite.isActive");
	EXPECT_EQ(ret, 96);

	ret = ssbo.Get("uiElements[0].fill");
	EXPECT_EQ(ret, 112);

	// FILL sub Values
	ret = ssbo.Get("uiElements[0].fill.type");
	EXPECT_EQ(ret, 112);
	ret = ssbo.Get("uiElements[0].fill.subType");
	EXPECT_EQ(ret, 116);
	ret = ssbo.Get("uiElements[0].fill.offset");
	EXPECT_EQ(ret, 120);
	ret = ssbo.Get("uiElements[0].fill.progress");
	EXPECT_EQ(ret, 124);
	ret = ssbo.Get("uiElements[0].fill.rotation");
	EXPECT_EQ(ret, 128);
	ret = ssbo.Get("uiElements[0].fill.isActive");
	EXPECT_EQ(ret, 132);

	ret = ssbo.Get("uiElements[0].color");
	EXPECT_EQ(ret, 144);
	ret = ssbo.Get("uiElements[0].isText");
	EXPECT_EQ(ret, 160);

	ret = ssbo.Get("elementTexture");
	EXPECT_EQ(ret, 1408);

	// TEXTURE SUB VALUES
	ret = ssbo.Get("elementTexture.size");
	EXPECT_EQ(ret, 1408);
	ret = ssbo.Get("elementTexture.isActive");
	EXPECT_EQ(ret, 1416);

	ret = ssbo.Get("elementLayer");
	EXPECT_EQ(ret, 1424);
	ret = ssbo.GetSize();
	EXPECT_EQ(ret, 1440);
#pragma endregion
}

TEST(STDStruct, ConstructorAndGet) {
	size_t ret;
	std::vector<size_t> retVec;

#pragma region RECT
	STD430Struct rect{
		STDValue<mat4>("transform", mat4(1.f)),
		STDValue<vec2>("size")
	};
	mat4 transform = rect.Get<mat4>("transform");
	EXPECT_EQ(transform, mat4(1.f));
	ret = rect.GetOffset("transform");
	EXPECT_EQ(ret, 0);
	ret = rect.GetOffset("size");
	EXPECT_EQ(ret, 64);
	ret = rect.GetSize();
	EXPECT_EQ(ret, 80);
#pragma endregion

#pragma region SPRITE
	STD430Struct sprite{
		STDVariable<uvec2>("offset"),
		STDVariable<uvec2>("size"),
		STDVariable<bool>("isActive")
	};
	ret = sprite.GetOffset("offset");
	EXPECT_EQ(ret, 0);
	ret = sprite.GetOffset("size");
	EXPECT_EQ(ret, 8);
	ret = sprite.GetOffset("isActive");
	EXPECT_EQ(ret, 16);
	ret = sprite.GetSize();
	EXPECT_EQ(ret, 32);
#pragma endregion

#pragma region FILL
	STD430Struct fill{
		STDValue<unsigned int>("type"),
		STDValue<unsigned int>("subType"),
		STDValue<float>("offset"),
		STDValue<float>("progress"),
		STDValue<float>("rotation"),
		STDValue<bool>("isActive")
	};
	ret = fill.GetOffset("type");
	EXPECT_EQ(ret, 0);
	ret = fill.GetOffset("subType");
	EXPECT_EQ(ret, 4);
	ret = fill.GetOffset("offset");
	EXPECT_EQ(ret, 8);
	ret = fill.GetOffset("progress");
	EXPECT_EQ(ret, 12);
	ret = fill.GetOffset("rotation");
	EXPECT_EQ(ret, 16);
	ret = fill.GetOffset("isActive");
	EXPECT_EQ(ret, 20);
	ret = fill.GetSize();
	EXPECT_EQ(ret, 32);
#pragma endregion

#pragma region UIElement
	STD430Struct uiElement{
		STDValue<STD430Struct>("rect", rect),
		STDValue<STD430Offsets>("sprite", sprite.GetOffsets()),
		STDValue<STD430Struct>("fill", fill),
		STDValue<vec4>("color"),
		STDValue<bool>("isText")
	};
	ret = uiElement.GetOffset("rect");
	EXPECT_EQ(ret, 0);

	// RECT sub Values
	ret = uiElement.GetOffset("rect.transform");
	EXPECT_EQ(ret, 0);
	ret = uiElement.GetOffset("rect.size");
	EXPECT_EQ(ret, 64);

	ret = uiElement.GetOffset("sprite");
	EXPECT_EQ(ret, 80);

	// SPRITE sub Values
	ret = uiElement.GetOffset("sprite.offset");
	EXPECT_EQ(ret, 80);
	ret = uiElement.GetOffset("sprite.size");
	EXPECT_EQ(ret, 88);
	ret = uiElement.GetOffset("sprite.isActive");
	EXPECT_EQ(ret, 96);

	ret = uiElement.GetOffset("fill");
	EXPECT_EQ(ret, 112);

	// FILL sub Values
	ret = uiElement.GetOffset("fill.type");
	EXPECT_EQ(ret, 112);
	ret = uiElement.GetOffset("fill.subType");
	EXPECT_EQ(ret, 116);
	ret = uiElement.GetOffset("fill.offset");
	EXPECT_EQ(ret, 120);
	ret = uiElement.GetOffset("fill.progress");
	EXPECT_EQ(ret, 124);
	ret = uiElement.GetOffset("fill.rotation");
	EXPECT_EQ(ret, 128);
	ret = uiElement.GetOffset("fill.isActive");
	EXPECT_EQ(ret, 132);

	ret = uiElement.GetOffset("color");
	EXPECT_EQ(ret, 144);
	ret = uiElement.GetOffset("isText");
	EXPECT_EQ(ret, 160);
	ret = uiElement.GetSize();
	EXPECT_EQ(ret, 176);
#pragma endregion

#pragma region TEXTURE
	STD430Struct texture{
		STDValue<uvec2>("size"),
		STDValue<bool>("isActive")
	};
	ret = texture.GetOffset("size");
	EXPECT_EQ(ret, 0);
	ret = texture.GetOffset("isActive");
	EXPECT_EQ(ret, 8);
	ret = texture.GetSize();
	EXPECT_EQ(ret, 16);
#pragma endregion

#pragma region SSBO
	STD430Struct ssbo{
		STDValue<STD430Struct, 8>("uiElements", uiElement.GetOffsets()),
		STDValue<STD430Struct>("elementTexture", texture),
		STDValue<int>("elementLayer")
	};
	std::vector<size_t> resultVec{ 0, 176, 352, 528, 704, 880, 1056, 1232 };
	retVec = ssbo.GetArrayOffsets("uiElements");
	EXPECT_EQ(retVec, resultVec);

	// UI FIRST ELEMENT SUB VALUES TEST
	ret = ssbo.GetOffset("uiElements[0].rect");
	EXPECT_EQ(ret, 0);

	// RECT sub Values
	ret = ssbo.GetOffset("uiElements[0].rect.transform");
	EXPECT_EQ(ret, 0);
	ret = ssbo.GetOffset("uiElements[0].rect.size");
	EXPECT_EQ(ret, 64);

	ret = ssbo.GetOffset("uiElements[0].sprite");
	EXPECT_EQ(ret, 80);

	// SPRITE sub Values
	ret = ssbo.GetOffset("uiElements[0].sprite.offset");
	EXPECT_EQ(ret, 80);
	ret = ssbo.GetOffset("uiElements[0].sprite.size");
	EXPECT_EQ(ret, 88);
	ret = ssbo.GetOffset("uiElements[0].sprite.isActive");
	EXPECT_EQ(ret, 96);

	ret = ssbo.GetOffset("uiElements[0].fill");
	EXPECT_EQ(ret, 112);

	// FILL sub Values
	ret = ssbo.GetOffset("uiElements[0].fill.type");
	EXPECT_EQ(ret, 112);
	ret = ssbo.GetOffset("uiElements[0].fill.subType");
	EXPECT_EQ(ret, 116);
	ret = ssbo.GetOffset("uiElements[0].fill.offset");
	EXPECT_EQ(ret, 120);
	ret = ssbo.GetOffset("uiElements[0].fill.progress");
	EXPECT_EQ(ret, 124);
	ret = ssbo.GetOffset("uiElements[0].fill.rotation");
	EXPECT_EQ(ret, 128);
	ret = ssbo.GetOffset("uiElements[0].fill.isActive");
	EXPECT_EQ(ret, 132);

	ret = ssbo.GetOffset("uiElements[0].color");
	EXPECT_EQ(ret, 144);
	ret = ssbo.GetOffset("uiElements[0].isText");
	EXPECT_EQ(ret, 160);

	ret = ssbo.GetOffset("elementTexture");
	EXPECT_EQ(ret, 1408);

	// TEXTURE SUB VALUES
	ret = ssbo.GetOffset("elementTexture.size");
	EXPECT_EQ(ret, 1408);
	ret = ssbo.GetOffset("elementTexture.isActive");
	EXPECT_EQ(ret, 1416);

	ret = ssbo.GetOffset("elementLayer");
	EXPECT_EQ(ret, 1424);
	ret = ssbo.GetSize();
	EXPECT_EQ(ret, 1440);
#pragma endregion
}