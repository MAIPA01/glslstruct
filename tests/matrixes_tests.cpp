#include "pch.hpp"

using namespace glslstruct;
using namespace glm;

TEST(std140_offset, add_matrixes) {
	std140_offset structOffsets;
	size_t ret;
	base_type_handle base = nullptr;
	mat_type_handle temp_type = nullptr;
	mat_type_handle casted_type = nullptr;

#pragma region COLUMN_MAJOR

#pragma region MAT2x2
	structOffsets.clear();

#pragma region BMAT2x2
	ret = structOffsets.add<bmat2>("Bool");
	EXPECT_EQ(ret, 0);

	base = structOffsets.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Bool, 2, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x2
	ret = structOffsets.add<imat2>("Int");
	EXPECT_EQ(ret, 32);

	base = structOffsets.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Int, 2, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x2
	ret = structOffsets.add<umat2>("Uint");
	EXPECT_EQ(ret, 64);

	base = structOffsets.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Uint, 2, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x2
	ret = structOffsets.add<fmat2>("Float");
	EXPECT_EQ(ret, 96);

	base = structOffsets.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Float, 2, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x2
	ret = structOffsets.add<dmat2>("Double");
	EXPECT_EQ(ret, 128);

	base = structOffsets.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Double, 2, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x3
	structOffsets.clear();

#pragma region BMAT2x3
	ret = structOffsets.add<bmat2x3>("Bool");
	EXPECT_EQ(ret, 0);

	base = structOffsets.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Bool, 2, 3);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x3
	ret = structOffsets.add<imat2x3>("Int");
	EXPECT_EQ(ret, 32);

	base = structOffsets.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Int, 2, 3);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x3
	ret = structOffsets.add<umat2x3>("Uint");
	EXPECT_EQ(ret, 64);

	base = structOffsets.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Uint, 2, 3);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x3
	ret = structOffsets.add<fmat2x3>("Float");
	EXPECT_EQ(ret, 96);

	base = structOffsets.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Float, 2, 3);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x3
	ret = structOffsets.add<dmat2x3>("Double");
	EXPECT_EQ(ret, 128);

	base = structOffsets.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Double, 2, 3);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x4
	structOffsets.clear();

#pragma region BMAT2x4
	ret = structOffsets.add<bmat2x4>("Bool");
	EXPECT_EQ(ret, 0);

	base = structOffsets.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Bool, 2, 4);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x4
	ret = structOffsets.add<imat2x4>("Int");
	EXPECT_EQ(ret, 32);

	base = structOffsets.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Int, 2, 4);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x4
	ret = structOffsets.add<umat2x4>("Uint");
	EXPECT_EQ(ret, 64);

	base = structOffsets.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Uint, 2, 4);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x4
	ret = structOffsets.add<fmat2x4>("Float");
	EXPECT_EQ(ret, 96);

	base = structOffsets.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Float, 2, 4);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x4
	ret = structOffsets.add<dmat2x4>("Double");
	EXPECT_EQ(ret, 128);

	base = structOffsets.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Double, 2, 4);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma endregion

#pragma region ROW_MAJOR

#pragma region MAT2x2
	structOffsets.clear();

#pragma region BMAT2x2
	ret = structOffsets.add<bmat2, false>("Bool");
	EXPECT_EQ(ret, 0);
	
	base = structOffsets.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Bool, 2, 2, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x2
	ret = structOffsets.add<imat2, false>("Int");
	EXPECT_EQ(ret, 32);

	base = structOffsets.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Int, 2, 2, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x2
	ret = structOffsets.add<umat2, false>("Uint");
	EXPECT_EQ(ret, 64);

	base = structOffsets.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Uint, 2, 2, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x2
	ret = structOffsets.add<fmat2, false>("Float");
	EXPECT_EQ(ret, 96);

	base = structOffsets.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Float, 2, 2, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x2
	ret = structOffsets.add<dmat2, false>("Double");
	EXPECT_EQ(ret, 128);

	base = structOffsets.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Double, 2, 2, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x3
	structOffsets.clear();

#pragma region BMAT2x3
	ret = structOffsets.add<bmat2x3, false>("Bool");
	EXPECT_EQ(ret, 0);

	base = structOffsets.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Bool, 2, 3, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x3
	ret = structOffsets.add<imat2x3, false>("Int");
	EXPECT_EQ(ret, 48);

	base = structOffsets.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Int, 2, 3, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x3
	ret = structOffsets.add<umat2x3, false>("Uint");
	EXPECT_EQ(ret, 96);

	base = structOffsets.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Uint, 2, 3, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x3
	ret = structOffsets.add<fmat2x3, false>("Float");
	EXPECT_EQ(ret, 144);

	base = structOffsets.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Float, 2, 3, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x3
	ret = structOffsets.add<dmat2x3, false>("Double");
	EXPECT_EQ(ret, 192);

	base = structOffsets.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Double, 2, 3, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x4
	structOffsets.clear();

#pragma region BMAT2x4
	ret = structOffsets.add<bmat2x4, false>("Bool");
	EXPECT_EQ(ret, 0);

	base = structOffsets.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Bool, 2, 4, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x4
	ret = structOffsets.add<imat2x4, false>("Int");
	EXPECT_EQ(ret, 64);

	base = structOffsets.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Int, 2, 4, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x4
	ret = structOffsets.add<umat2x4, false>("Uint");
	EXPECT_EQ(ret, 128);

	base = structOffsets.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Uint, 2, 4, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x4
	ret = structOffsets.add<fmat2x4, false>("Float");
	EXPECT_EQ(ret, 192);

	base = structOffsets.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Float, 2, 4, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x4
	ret = structOffsets.add<dmat2x4, false>("Double");
	EXPECT_EQ(ret, 256);

	base = structOffsets.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Double, 2, 4, false);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<mat_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_cols(), temp_type->get_cols());
	EXPECT_EQ(casted_type->get_rows(), temp_type->get_rows());
	EXPECT_EQ(casted_type->get_array_count(), temp_type->get_array_count());
	EXPECT_EQ(casted_type->get_vec_length(), temp_type->get_vec_length());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma endregion
}

TEST(std140_offset, add_marixes_array) {
	std140_offset structOffsets;
	std::vector<size_t> results;
	std::vector<size_t> ret;
	base_type_handle base = nullptr;
	array_type_handle temp_type = nullptr;
	array_type_handle casted_type = nullptr;

#pragma region COLUMN_MAJOR

#pragma region MAT2x2_ARRAY
	structOffsets.clear();

#pragma region BMAT2x2_ARRAY
	results = { 0, 32 };
	ret = structOffsets.add<bmat2>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2, 2, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x2_ARRAY
	results = { 64, 96 };
	ret = structOffsets.add<imat2>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2, 2, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x2_ARRAY
	results = { 128, 160 };
	ret = structOffsets.add<umat2>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2, 2, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x2_ARRAY
	results = { 192, 224 };
	ret = structOffsets.add<fmat2>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2, 2, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x2_ARRAY
	results = { 256, 288 };
	ret = structOffsets.add<dmat2>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2, 2, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x3_ARRAY
	structOffsets.clear();

#pragma region BMAT2x3_ARRAY
	results = { 0, 32 };
	ret = structOffsets.add<bmat2x3>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2, 3, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x3_ARRAY
	results = { 64, 96 };
	ret = structOffsets.add<imat2x3>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2, 3, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x3_ARRAY
	results = { 128, 160 };
	ret = structOffsets.add<umat2x3>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2, 3, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x3_ARRAY
	results = { 192, 224 };
	ret = structOffsets.add<fmat2x3>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2, 3, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x3_ARRAY
	results = { 256, 320 };
	ret = structOffsets.add<dmat2x3>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2, 3, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x4_ARRAY
	structOffsets.clear();

#pragma region BMAT2x4_ARRAY
	results = { 0, 32 };
	ret = structOffsets.add<bmat2x4>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2, 4, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x4_ARRAY
	results = { 64, 96 };
	ret = structOffsets.add<imat2x4>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2, 4, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x4_ARRAY
	results = { 128, 160 };
	ret = structOffsets.add<umat2x4>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2, 4, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x4_ARRAY
	results = { 192, 224 };
	ret = structOffsets.add<fmat2x4>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2, 4, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x4_ARRAY
	results = { 256, 320 };
	ret = structOffsets.add<dmat2x4>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2, 4, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma endregion

#pragma region ROW_MAJOR

#pragma region MAT2x2_ARRAY
	structOffsets.clear();

#pragma region BMAT2x2_ARRAY
	results = { 0, 32 };
	ret = structOffsets.add<bmat2, false>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2, 2, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x2_ARRAY
	results = { 64, 96 };
	ret = structOffsets.add<imat2, false>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2, 2, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x2_ARRAY
	results = { 128, 160 };
	ret = structOffsets.add<umat2, false>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2, 2, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x2_ARRAY
	results = { 192, 224 };
	ret = structOffsets.add<fmat2, false>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2, 2, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x2_ARRAY
	results = { 256, 288 };
	ret = structOffsets.add<dmat2, false>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2, 2, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x3_ARRAY
	structOffsets.clear();

#pragma region BMAT2x3_ARRAY
	results = { 0, 48 };
	ret = structOffsets.add<bmat2x3, false>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2, 3, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x3_ARRAY
	results = { 96, 144 };
	ret = structOffsets.add<imat2x3, false>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2, 3, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x3_ARRAY
	results = { 192, 240 };
	ret = structOffsets.add<umat2x3, false>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2, 3, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x3_ARRAY
	results = { 288, 336 };
	ret = structOffsets.add<fmat2x3, false>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2, 3, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x3_ARRAY
	results = { 384, 432 };
	ret = structOffsets.add<dmat2x3, false>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2, 3, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x4_ARRAY
	structOffsets.clear();

#pragma region BMAT2x4_ARRAY
	results = { 0, 64 };
	ret = structOffsets.add<bmat2x4, false>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2, 4, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region IMAT2x4_ARRAY
	results = { 128, 192 };
	ret = structOffsets.add<imat2x4, false>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2, 4, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region UMAT2x4_ARRAY
	results = { 256, 320 };
	ret = structOffsets.add<umat2x4, false>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2, 4, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region MAT2x4_ARRAY
	results = { 384, 448 };
	ret = structOffsets.add<fmat2x4, false>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2, 4, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion

#pragma region DMAT2x4_ARRAY
	results = { 512, 576 };
	ret = structOffsets.add<dmat2x4, false>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structOffsets.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2, 4, false, 2);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#pragma endregion
#pragma endregion

#pragma endregion
}