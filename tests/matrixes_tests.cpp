#include "pch.hpp"

using namespace glslstruct;
using namespace glm;

TEST(std140_layout, add_matrixes) {
	std140_layout structLayout;
	size_t ret;
	base_type_handle base = nullptr;
	mat_type_handle temp_type = nullptr;
	mat_type_handle casted_type = nullptr;

#pragma region COLUMN_MAJOR

#pragma region MAT2x2
	structLayout.clear();

#pragma region BMAT2x2
	ret = structLayout.add<bmat2>("Bool");
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Bool");
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
	ret = structLayout.add<imat2>("Int");
	EXPECT_EQ(ret, 32);

	base = structLayout.get_type("Int");
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
	ret = structLayout.add<umat2>("Uint");
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Uint");
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
	ret = structLayout.add<fmat2>("Float");
	EXPECT_EQ(ret, 96);

	base = structLayout.get_type("Float");
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
	ret = structLayout.add<dmat2>("Double");
	EXPECT_EQ(ret, 128);

	base = structLayout.get_type("Double");
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
	structLayout.clear();

#pragma region BMAT2x3
	ret = structLayout.add<bmat2x3>("Bool");
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Bool");
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
	ret = structLayout.add<imat2x3>("Int");
	EXPECT_EQ(ret, 32);

	base = structLayout.get_type("Int");
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
	ret = structLayout.add<umat2x3>("Uint");
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Uint");
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
	ret = structLayout.add<fmat2x3>("Float");
	EXPECT_EQ(ret, 96);

	base = structLayout.get_type("Float");
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
	ret = structLayout.add<dmat2x3>("Double");
	EXPECT_EQ(ret, 128);

	base = structLayout.get_type("Double");
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
	structLayout.clear();

#pragma region BMAT2x4
	ret = structLayout.add<bmat2x4>("Bool");
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Bool");
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
	ret = structLayout.add<imat2x4>("Int");
	EXPECT_EQ(ret, 32);

	base = structLayout.get_type("Int");
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
	ret = structLayout.add<umat2x4>("Uint");
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Uint");
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
	ret = structLayout.add<fmat2x4>("Float");
	EXPECT_EQ(ret, 96);

	base = structLayout.get_type("Float");
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
	ret = structLayout.add<dmat2x4>("Double");
	EXPECT_EQ(ret, 128);

	base = structLayout.get_type("Double");
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
	structLayout.clear();

#pragma region BMAT2x2
	ret = structLayout.add<bmat2, MajorType::Row>("Bool");
	EXPECT_EQ(ret, 0);
	
	base = structLayout.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Bool, 2, 2, MajorType::Row);
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
	ret = structLayout.add<imat2, MajorType::Row>("Int");
	EXPECT_EQ(ret, 32);

	base = structLayout.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Int, 2, 2, MajorType::Row);
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
	ret = structLayout.add<umat2, MajorType::Row>("Uint");
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Uint, 2, 2, MajorType::Row);
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
	ret = structLayout.add<fmat2, MajorType::Row>("Float");
	EXPECT_EQ(ret, 96);

	base = structLayout.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Float, 2, 2, MajorType::Row);
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
	ret = structLayout.add<dmat2, MajorType::Row>("Double");
	EXPECT_EQ(ret, 128);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Double, 2, 2, MajorType::Row);
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
	structLayout.clear();

#pragma region BMAT2x3
	ret = structLayout.add<bmat2x3, MajorType::Row>("Bool");
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Bool, 2, 3, MajorType::Row);
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
	ret = structLayout.add<imat2x3, MajorType::Row>("Int");
	EXPECT_EQ(ret, 48);

	base = structLayout.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Int, 2, 3, MajorType::Row);
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
	ret = structLayout.add<umat2x3, MajorType::Row>("Uint");
	EXPECT_EQ(ret, 96);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Uint, 2, 3, MajorType::Row);
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
	ret = structLayout.add<fmat2x3, MajorType::Row>("Float");
	EXPECT_EQ(ret, 144);

	base = structLayout.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Float, 2, 3, MajorType::Row);
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
	ret = structLayout.add<dmat2x3, MajorType::Row>("Double");
	EXPECT_EQ(ret, 192);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Double, 2, 3, MajorType::Row);
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
	structLayout.clear();

#pragma region BMAT2x4
	ret = structLayout.add<bmat2x4, MajorType::Row>("Bool");
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Bool");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Bool, 2, 4, MajorType::Row);
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
	ret = structLayout.add<imat2x4, MajorType::Row>("Int");
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Int, 2, 4, MajorType::Row);
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
	ret = structLayout.add<umat2x4, MajorType::Row>("Uint");
	EXPECT_EQ(ret, 128);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Uint, 2, 4, MajorType::Row);
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
	ret = structLayout.add<fmat2x4, MajorType::Row>("Float");
	EXPECT_EQ(ret, 192);

	base = structLayout.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Float, 2, 4, MajorType::Row);
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
	ret = structLayout.add<dmat2x4, MajorType::Row>("Double");
	EXPECT_EQ(ret, 256);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<mat_type>(ValueType::Double, 2, 4, MajorType::Row);
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

TEST(std140_layout, add_marixes_array) {
	std140_layout structLayout;
	std::vector<size_t> results;
	std::vector<size_t> ret;
	base_type_handle base = nullptr;
	array_type_handle temp_type = nullptr;
	array_type_handle casted_type = nullptr;

#pragma region COLUMN_MAJOR

#pragma region MAT2x2_ARRAY
	structLayout.clear();

#pragma region BMAT2x2_ARRAY
	results = { 0, 32 };
	ret = structLayout.add<bmat2>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
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
	ret = structLayout.add<imat2>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
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
	ret = structLayout.add<umat2>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
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
	ret = structLayout.add<fmat2>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
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
	ret = structLayout.add<dmat2>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
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
	structLayout.clear();

#pragma region BMAT2x3_ARRAY
	results = { 0, 32 };
	ret = structLayout.add<bmat2x3>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
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
	ret = structLayout.add<imat2x3>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
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
	ret = structLayout.add<umat2x3>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
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
	ret = structLayout.add<fmat2x3>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
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
	ret = structLayout.add<dmat2x3>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
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
	structLayout.clear();

#pragma region BMAT2x4_ARRAY
	results = { 0, 32 };
	ret = structLayout.add<bmat2x4>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
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
	ret = structLayout.add<imat2x4>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
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
	ret = structLayout.add<umat2x4>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
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
	ret = structLayout.add<fmat2x4>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
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
	ret = structLayout.add<dmat2x4>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
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
	structLayout.clear();

#pragma region BMAT2x2_ARRAY
	results = { 0, 32 };
	ret = structLayout.add<bmat2, MajorType::Row>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2, 2, MajorType::Row, 2);
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
	ret = structLayout.add<imat2, MajorType::Row>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2, 2, MajorType::Row, 2);
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
	ret = structLayout.add<umat2, MajorType::Row>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2, 2, MajorType::Row, 2);
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
	ret = structLayout.add<fmat2, MajorType::Row>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2, 2, MajorType::Row, 2);
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
	ret = structLayout.add<dmat2, MajorType::Row>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2, 2, MajorType::Row, 2);
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
	structLayout.clear();

#pragma region BMAT2x3_ARRAY
	results = { 0, 48 };
	ret = structLayout.add<bmat2x3, MajorType::Row>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2, 3, MajorType::Row, 2);
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
	ret = structLayout.add<imat2x3, MajorType::Row>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2, 3, MajorType::Row, 2);
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
	ret = structLayout.add<umat2x3, MajorType::Row>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2, 3, MajorType::Row, 2);
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
	ret = structLayout.add<fmat2x3, MajorType::Row>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2, 3, MajorType::Row, 2);
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
	ret = structLayout.add<dmat2x3, MajorType::Row>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2, 3, MajorType::Row, 2);
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
	structLayout.clear();

#pragma region BMAT2x4_ARRAY
	results = { 0, 64 };
	ret = structLayout.add<bmat2x4, MajorType::Row>("Bools", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 2, 4, MajorType::Row, 2);
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
	ret = structLayout.add<imat2x4, MajorType::Row>("Ints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 2, 4, MajorType::Row, 2);
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
	ret = structLayout.add<umat2x4, MajorType::Row>("Uints", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 2, 4, MajorType::Row, 2);
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
	ret = structLayout.add<fmat2x4, MajorType::Row>("Floats", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 2, 4, MajorType::Row, 2);
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
	ret = structLayout.add<dmat2x4, MajorType::Row>("Doubles", 2);
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 2, 4, MajorType::Row, 2);
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