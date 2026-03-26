#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(std140_layout, add_vectors) {
	std140_layout structLayout;
	size_t ret;
	base_type_handle base		= nullptr;
	vec_type_handle temp_type	= nullptr;
	vec_type_handle casted_type = nullptr;

#pragma region VEC2
	structLayout.clear();

#pragma region BVEC2
	ret = structLayout.add<bvec2>("Bool");
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Bool, 2, 8);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IVEC2
	ret = structLayout.add<ivec2>("Int");
	EXPECT_EQ(ret, 8);

	base = structLayout.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Int, 2, 8);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UVEC2
	ret = structLayout.add<uvec2>("Uint");
	EXPECT_EQ(ret, 16);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Uint, 2, 8);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region VEC2
	ret = structLayout.add<vec2>("Float");
	EXPECT_EQ(ret, 24);

	base = structLayout.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Float, 2, 8);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DVEC2
	ret = structLayout.add<dvec2>("Double");
	EXPECT_EQ(ret, 32);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Double, 2, 8);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region VEC3
	structLayout.clear();

#pragma region BVEC3
	ret = structLayout.add<bvec3>("Bool");
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Bool, 3, 12);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IVEC3
	ret = structLayout.add<ivec3>("Int");
	EXPECT_EQ(ret, 16);

	base = structLayout.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Int, 3, 12);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UVEC3
	ret = structLayout.add<uvec3>("Uint");
	EXPECT_EQ(ret, 32);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Uint, 3, 12);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region VEC3
	ret = structLayout.add<vec3>("Float");
	EXPECT_EQ(ret, 48);

	base = structLayout.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Float, 3, 12);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DVEC3
	ret = structLayout.add<dvec3>("Double");
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Double, 3, 12);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region VEC4
	structLayout.clear();

#pragma region BVEC4
	ret = structLayout.add<bvec4>("Bool");
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Bool, 4, 16);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IVEC4
	ret = structLayout.add<ivec4>("Int");
	EXPECT_EQ(ret, 16);

	base = structLayout.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Int, 4, 16);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UVEC4
	ret = structLayout.add<uvec4>("Uint");
	EXPECT_EQ(ret, 32);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Uint, 4, 16);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region VEC4
	ret = structLayout.add<vec4>("Float");
	EXPECT_EQ(ret, 48);

	base = structLayout.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Float, 4, 16);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DVEC4
	ret = structLayout.add<dvec4>("Double");
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<vec_type>(ValueType::Double, 4, 16);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<vec_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<vec_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_length(), temp_type->get_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion
}

TEST(std140_layout, add_vectors_array) {
	std140_layout structLayout;
	std::vector<size_t> results;
	std::vector<size_t> ret;
	base_type_handle base		  = nullptr;
	array_type_handle temp_type	  = nullptr;
	array_type_handle casted_type = nullptr;

#pragma region VEC2_ARRAY
	structLayout.clear();

#pragma region BVEC2_ARRAY
	results = { 0, 16 };
	ret		= structLayout.add<bvec2>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2, 8, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IVEC2_ARRAY
	results = { 32, 48 };
	ret		= structLayout.add<ivec2>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2, 8, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UVEC2_ARRAY
	results = { 64, 80 };
	ret		= structLayout.add<uvec2>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2, 8, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region VEC2_ARRAY
	results = { 96, 112 };
	ret		= structLayout.add<vec2>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2, 8, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DVEC2_ARRAY
	results = { 128, 144 };
	ret		= structLayout.add<dvec2>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2, 16, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region VEC2_ARRAY
	structLayout.clear();

#pragma region BVEC2_ARRAY
	results = { 0, 16 };
	ret		= structLayout.add<bvec2>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2, 8, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IVEC2_ARRAY
	results = { 32, 48 };
	ret		= structLayout.add<ivec2>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2, 8, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UVEC2_ARRAY
	results = { 64, 80 };
	ret		= structLayout.add<uvec2>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2, 8, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region VEC2_ARRAY
	results = { 96, 112 };
	ret		= structLayout.add<vec2>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2, 8, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DVEC2_ARRAY
	results = { 128, 144 };
	ret		= structLayout.add<dvec2>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2, 16, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region VEC3_ARRAY
	structLayout.clear();

#pragma region BVEC3_ARRAY
	results = { 0, 16 };
	ret		= structLayout.add<bvec3>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 3, 12, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IVEC3_ARRAY
	results = { 32, 48 };
	ret		= structLayout.add<ivec3>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 3, 12, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UVEC3_ARRAY
	results = { 64, 80 };
	ret		= structLayout.add<uvec3>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 3, 12, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region VEC3_ARRAY
	results = { 96, 112 };
	ret		= structLayout.add<vec3>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 3, 12, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DVEC3_ARRAY
	results = { 128, 160 };
	ret		= structLayout.add<dvec3>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 3, 24, 2, 64);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region VEC4_ARRAY
	structLayout.clear();

#pragma region BVEC4_ARRAY
	results = { 0, 16 };
	ret		= structLayout.add<bvec4>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 4, 16, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IVEC4_ARRAY
	results = { 32, 48 };
	ret		= structLayout.add<ivec4>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 4, 16, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UVEC4_ARRAY
	results = { 64, 80 };
	ret		= structLayout.add<uvec4>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 4, 16, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region VEC4_ARRAY
	results = { 96, 112 };
	ret		= structLayout.add<vec4>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 4, 16, 2, 32);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DVEC4_ARRAY
	results = { 128, 160 };
	ret		= structLayout.add<dvec4>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 4, 32, 2, 64);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion
}