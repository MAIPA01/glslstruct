#include "pch.hpp"

using namespace glslstruct;
using namespace glm;

TEST(std140_offset, add_scalars) {
	std140_offset structOffsets;
	size_t ret;
	base_type_handle base = nullptr;
	scalar_type_handle temp_type = nullptr;
	base_type_handle wrong_type = nullptr;
	scalar_type_handle casted_type = nullptr;

#pragma region BOOL
	ret = structOffsets.add<bool>("Bool");
	EXPECT_EQ(ret, 0);
	EXPECT_EQ(structOffsets.get_offset("Bool"), 0);

	base = structOffsets.get_type("Bool");
	EXPECT_EQ(*base, *base);
	EXPECT_EQ(std::hash<base_type>()(*base), std::hash<base_type>()(*base));

	temp_type = std::make_shared<scalar_type>(ValueType::Bool);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());

	wrong_type = std::make_shared<scalar_type>(ValueType::Int);
	EXPECT_NE(*base, *wrong_type);

	wrong_type = std::make_shared<vec_type>(ValueType::Bool, 2);
	EXPECT_NE(*base, *wrong_type);
#pragma endregion

#pragma region INT
	ret = structOffsets.add<int>("Int");
	EXPECT_EQ(ret, 4);

	base = structOffsets.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<scalar_type>(ValueType::Int);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UINT
	ret = structOffsets.add<unsigned int>("Uint");
	EXPECT_EQ(ret, 8);

	base = structOffsets.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<scalar_type>(ValueType::Uint);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region FLOAT
	ret = structOffsets.add<float>("Float");
	EXPECT_EQ(ret, 12);

	base = structOffsets.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<scalar_type>(ValueType::Float);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DOUBLE
	ret = structOffsets.add<double>("Double");
	EXPECT_EQ(ret, 16);

	base = structOffsets.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<scalar_type>(ValueType::Double);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
}

TEST(std140_offset, add_scalars_array) {
	std140_offset structOffsets;
	std::vector<size_t> results;
	std::vector<size_t> ret;
	base_type_handle base = nullptr;
	array_type_handle temp_type = nullptr;
	array_type_handle casted_type = nullptr;

#pragma region BOOL_ARRAY
	results = { 0, 16 };
	ret = structOffsets.add<bool>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region INT_ARRAY
	results = { 32, 48 };
	ret = structOffsets.add<int>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UINT_ARRAY
	results = { 64, 80 };
	ret = structOffsets.add<unsigned int>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region FLOAT_ARRAY
	results = { 96, 112 };
	ret = structOffsets.add<float>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DOUBLE_ARRAY
	results = { 128, 144 };
	ret = structOffsets.add<double>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
}