#include "pch.hpp"

using namespace glslstruct;
using namespace glm;

TEST(std140_offset, add_scalars) {
	std140_offset structOffsets;
	size_t ret;
	const base_type* base = nullptr;
	const scalar_type* temp_type = nullptr;
	const scalar_type* casted_type = nullptr;

#pragma region BOOL
	ret = structOffsets.add<bool>("Bool");
	EXPECT_EQ(ret, 0);
	EXPECT_EQ(structOffsets.get("Bool"), 0);

	base = structOffsets.getType("Bool");
	EXPECT_EQ(*base, *base);
	EXPECT_EQ(std::hash<base_type>()(*base), std::hash<base_type>()(*base));

	temp_type = new scalar_type(ValueType::Bool);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->type(), temp_type->type());
	delete temp_type;
#pragma endregion

#pragma region INT
	ret = structOffsets.add<int>("Int");
	EXPECT_EQ(ret, 4);

	base = structOffsets.getType("Int");
	EXPECT_EQ(*base, *base);

	temp_type = new scalar_type(ValueType::Int);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->type(), temp_type->type());
	delete temp_type;
#pragma endregion

#pragma region UINT
	ret = structOffsets.add<unsigned int>("Uint");
	EXPECT_EQ(ret, 8);

	base = structOffsets.getType("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = new scalar_type(ValueType::Uint);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->type(), temp_type->type());
	delete temp_type;
#pragma endregion

#pragma region FLOAT
	ret = structOffsets.add<float>("Float");
	EXPECT_EQ(ret, 12);

	base = structOffsets.getType("Float");
	EXPECT_EQ(*base, *base);

	temp_type = new scalar_type(ValueType::Float);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->type(), temp_type->type());
	delete temp_type;
#pragma endregion

#pragma region DOUBLE
	ret = structOffsets.add<double>("Double");
	EXPECT_EQ(ret, 16);

	base = structOffsets.getType("Double");
	EXPECT_EQ(*base, *base);

	temp_type = new scalar_type(ValueType::Double);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->type(), temp_type->type());
	delete temp_type;
#pragma endregion
}

TEST(std140_offset, add_scalars_array) {
	std140_offset structOffsets;
	std::vector<size_t> results;
	std::vector<size_t> ret;
	const base_type* base = nullptr;
	const array_type* temp_type = nullptr;
	const array_type* casted_type = nullptr;

#pragma region BOOL_ARRAY
	results = { 0, 16 };
	ret = structOffsets.add<bool>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.getType("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = new array_type(new scalar_type(ValueType::Bool), 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->type(), *temp_type->type());
	EXPECT_EQ(casted_type->length(), temp_type->length());
	delete temp_type;
#pragma endregion

#pragma region INT_ARRAY
	results = { 32, 48 };
	ret = structOffsets.add<int>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.getType("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = new array_type(new scalar_type(ValueType::Int), 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->type(), *temp_type->type());
	EXPECT_EQ(casted_type->length(), temp_type->length());
	delete temp_type;
#pragma endregion

#pragma region UINT_ARRAY
	results = { 64, 80 };
	ret = structOffsets.add<unsigned int>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.getType("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = new array_type(new scalar_type(ValueType::Uint), 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->type(), *temp_type->type());
	EXPECT_EQ(casted_type->length(), temp_type->length());
	delete temp_type;
#pragma endregion

#pragma region FLOAT_ARRAY
	results = { 96, 112 };
	ret = structOffsets.add<float>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.getType("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = new array_type(new scalar_type(ValueType::Float), 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->type(), *temp_type->type());
	EXPECT_EQ(casted_type->length(), temp_type->length());
	delete temp_type;
#pragma endregion

#pragma region DOUBLE_ARRAY
	results = { 128, 144 };
	ret = structOffsets.add<double>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.getType("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = new array_type(new scalar_type(ValueType::Double), 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->type(), *temp_type->type());
	EXPECT_EQ(casted_type->length(), temp_type->length());
	delete temp_type;
#pragma endregion
}