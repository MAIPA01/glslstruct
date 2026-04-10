#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(scalar_layout, offset_calculation_1) {
	size_t ret;
	std::vector<size_t> retVec;

	#pragma region Vertex
	scalar_layout vertexLayout;

	// Add vec3 (size: 12, alignment: 4)
	ret = vertexLayout.add<vec3>("position");
	EXPECT_EQ(ret, 0);
	ret = vertexLayout.size();
	EXPECT_EQ(ret, 12);

	// Add vec3 (size: 12, alignment: 4)
	ret = vertexLayout.add<vec3>("normal");
	EXPECT_EQ(ret, 12);
	ret = vertexLayout.size();
	EXPECT_EQ(ret, 24);

	// Add vec3 (size: 12, alignment: 4)
	ret = vertexLayout.add<vec3>("tangent");
	EXPECT_EQ(ret, 24);
	ret = vertexLayout.size();
	EXPECT_EQ(ret, 36);

	// Add vec3 (size: 12, alignment: 4)
	ret = vertexLayout.add<vec3>("bitangent");
	EXPECT_EQ(ret, 36);
	ret = vertexLayout.size();
	EXPECT_EQ(ret, 48);

	// Add vec2 (size: 8, alignment: 4)
	ret = vertexLayout.add<vec2>("uv");
	EXPECT_EQ(ret, 48);
	ret = vertexLayout.size();
	EXPECT_EQ(ret, 56);
	#pragma endregion

	#pragma region Buffer
	scalar_layout bufferLayout;

	// Add Vertex struct array (size: 56, alignment: 4, padding: 0, count: 4)
	const std::vector<size_t> resultVec = { 0, 56, 112, 168 };
	retVec = bufferLayout.add("vertices", vertexLayout, 4);
	EXPECT_EQ(retVec, resultVec);
	ret = bufferLayout.size();
	EXPECT_EQ(ret, 224);
	#pragma endregion
}