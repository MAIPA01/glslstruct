#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(std140_layout, add_matrixes) {
	std140_layout structLayout;
	size_t ret;
	base_type_handle base	   = nullptr;
	mat_type_handle tempType   = nullptr;
	mat_type_handle castedType = nullptr;

#pragma region MAT2x2
	structLayout.clear();

#pragma region BMAT2x2
	ret = structLayout.add<bmat2>("Bool").front();
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Bool");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Bool, 2, 2, 24);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region IMAT2x2
	ret = structLayout.add<imat2>("Int").front();
	EXPECT_EQ(ret, 32);

	base = structLayout.get_type("Int");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Int, 2, 2, 24);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region UMAT2x2
	ret = structLayout.add<umat2>("Uint").front();
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Uint, 2, 2, 24);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region MAT2x2
	ret = structLayout.add<fmat2>("Float").front();
	EXPECT_EQ(ret, 96);

	base = structLayout.get_type("Float");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Float, 2, 2, 24);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT2x2
	ret = structLayout.add<dmat2>("Double").front();
	EXPECT_EQ(ret, 128);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Double, 2, 2, 32);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x3
	structLayout.clear();

#pragma region BMAT2x3
	ret = structLayout.add<bmat2x3>("Bool").front();
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Bool");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Bool, 2, 3, 28);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region IMAT2x3
	ret = structLayout.add<imat2x3>("Int").front();
	EXPECT_EQ(ret, 32);

	base = structLayout.get_type("Int");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Int, 2, 3, 28);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region UMAT2x3
	ret = structLayout.add<umat2x3>("Uint").front();
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Uint, 2, 3, 28);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region MAT2x3
	ret = structLayout.add<fmat2x3>("Float").front();
	EXPECT_EQ(ret, 96);

	base = structLayout.get_type("Float");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Float, 2, 3, 28);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT2x3
	ret = structLayout.add<dmat2x3>("Double").front();
	EXPECT_EQ(ret, 128);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Double, 2, 3, 56);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x4
	structLayout.clear();

#pragma region BMAT2x4
	ret = structLayout.add<bmat2x4>("Bool").front();
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Bool");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Bool, 2, 4, 32);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region IMAT2x4
	ret = structLayout.add<imat2x4>("Int").front();
	EXPECT_EQ(ret, 32);

	base = structLayout.get_type("Int");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Int, 2, 4, 32);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region UMAT2x4
	ret = structLayout.add<umat2x4>("Uint").front();
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Uint, 2, 4, 32);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region MAT2x4
	ret = structLayout.add<fmat2x4>("Float").front();
	EXPECT_EQ(ret, 96);

	base = structLayout.get_type("Float");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Float, 2, 4, 32);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT2x4
	ret = structLayout.add<dmat2x4>("Double").front();
	EXPECT_EQ(ret, 128);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Double, 2, 4, 64);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT3x2
	structLayout.clear();

#pragma region UMAT3x2
	ret = structLayout.add<umat3x2>("Uint").front();
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Uint, 3, 2, 40);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT3x2
	ret = structLayout.add<dmat3x2>("Double").front();
	EXPECT_EQ(ret, 48);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Double, 3, 2, 48);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT3x3
	structLayout.clear();

#pragma region UMAT3x3
	ret = structLayout.add<umat3>("Uint").front();
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Uint, 3, 3, 44);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT3x3
	ret = structLayout.add<dmat3>("Double").front();
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Double, 3, 3, 88);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT3x4
	structLayout.clear();

#pragma region UMAT3x4
	ret = structLayout.add<umat3x4>("Uint").front();
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Uint, 3, 4, 48);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT3x4
	ret = structLayout.add<dmat3x4>("Double").front();
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Double, 3, 4, 96);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT4x2
	structLayout.clear();

#pragma region UMAT4x2
	ret = structLayout.add<umat4x2>("Uint").front();
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Uint, 4, 2, 56);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT4x2
	ret = structLayout.add<dmat4x2>("Double").front();
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Double, 4, 2, 64);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT4x3
	structLayout.clear();

#pragma region UMAT4x3
	ret = structLayout.add<umat4x3>("Uint").front();
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Uint, 4, 3, 60);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT4x3
	ret = structLayout.add<dmat4x3>("Double").front();
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Double, 4, 3, 120);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT4x4
	structLayout.clear();

#pragma region UMAT4x4
	ret = structLayout.add<umat4>("Uint").front();
	EXPECT_EQ(ret, 0);

	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Uint, 4, 4, 64);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT4x4
	ret = structLayout.add<dmat4>("Double").front();
	EXPECT_EQ(ret, 64);

	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<mat_type>(ValueType::Double, 4, 4, 128);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<mat_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<mat_type>(base);
	EXPECT_EQ(castedType->get_type(), tempType->get_type());
	EXPECT_EQ(castedType->get_cols(), tempType->get_cols());
	EXPECT_EQ(castedType->get_rows(), tempType->get_rows());
	EXPECT_EQ(castedType->get_array_count(), tempType->get_array_count());
	EXPECT_EQ(castedType->get_vec_length(), tempType->get_vec_length());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion
}

TEST(std140_layout, add_marixes_array) {
	std140_layout structLayout;
	std::vector<size_t> results;
	std::vector<size_t> ret;
	std::vector<std::vector<size_t>> tempRet;
	base_type_handle base		 = nullptr;
	array_type_handle tempType	 = nullptr;
	array_type_handle castedType = nullptr;

#pragma region MAT2x2_ARRAY
	structLayout.clear();

#pragma region BMAT2x2_ARRAY
	results = { 0, 32 };
	tempRet = structLayout.add<bmat2>("Bools", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Bool, 2, 2, 24, 2, 56);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region IMAT2x2_ARRAY
	results = { 64, 96 };
	tempRet = structLayout.add<imat2>("Ints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Int, 2, 2, 24, 2, 56);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region UMAT2x2_ARRAY
	results = { 128, 160 };
	tempRet = structLayout.add<umat2>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Uint, 2, 2, 24, 2, 56);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region MAT2x2_ARRAY
	results = { 192, 224 };
	tempRet = structLayout.add<fmat2>("Floats", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Float, 2, 2, 24, 2, 56);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT2x2_ARRAY
	results = { 256, 288 };
	tempRet = structLayout.add<dmat2>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Double, 2, 2, 32, 2, 64);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x3_ARRAY
	structLayout.clear();

#pragma region BMAT2x3_ARRAY
	results = { 0, 32 };
	tempRet = structLayout.add<bmat2x3>("Bools", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Bool, 2, 3, 28, 2, 60);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region IMAT2x3_ARRAY
	results = { 64, 96 };
	tempRet = structLayout.add<imat2x3>("Ints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Int, 2, 3, 28, 2, 60);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region UMAT2x3_ARRAY
	results = { 128, 160 };
	tempRet = structLayout.add<umat2x3>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Uint, 2, 3, 28, 2, 60);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region MAT2x3_ARRAY
	results = { 192, 224 };
	tempRet = structLayout.add<fmat2x3>("Floats", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Float, 2, 3, 28, 2, 60);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT2x3_ARRAY
	results = { 256, 320 };
	tempRet = structLayout.add<dmat2x3>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Double, 2, 3, 56, 2, 120);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT2x4_ARRAY
	structLayout.clear();

#pragma region BMAT2x4_ARRAY
	results = { 0, 32 };
	tempRet = structLayout.add<bmat2x4>("Bools", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Bools");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Bool, 2, 4, 32, 2, 64);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region IMAT2x4_ARRAY
	results = { 64, 96 };
	tempRet = structLayout.add<imat2x4>("Ints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Ints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Int, 2, 4, 32, 2, 64);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region UMAT2x4_ARRAY
	results = { 128, 160 };
	tempRet = structLayout.add<umat2x4>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Uint, 2, 4, 32, 2, 64);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region MAT2x4_ARRAY
	results = { 192, 224 };
	tempRet = structLayout.add<fmat2x4>("Floats", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Floats");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Float, 2, 4, 32, 2, 64);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT2x4_ARRAY
	results = { 256, 320 };
	tempRet = structLayout.add<dmat2x4>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Double, 2, 4, 64, 2, 128);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT3x2_ARRAY
	structLayout.clear();

#pragma region UMAT3x2_ARRAY
	results = { 0, 48 };
	tempRet = structLayout.add<umat3x2>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Uint, 3, 2, 40, 2, 88);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT3x2_ARRAY
	results = { 96, 144 };
	tempRet = structLayout.add<dmat3x2>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Double, 3, 2, 48, 2, 96);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT3x3_ARRAY
	structLayout.clear();

#pragma region UMAT3x3_ARRAY
	results = { 0, 48 };
	tempRet = structLayout.add<umat3>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Uint, 3, 3, 44, 2, 92);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT3x3_ARRAY
	results = { 96, 192 };
	tempRet = structLayout.add<dmat3>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Double, 3, 3, 88, 2, 184);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT3x4_ARRAY
	structLayout.clear();

#pragma region UMAT3x4_ARRAY
	results = { 0, 48 };
	tempRet = structLayout.add<umat3x4>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Uint, 3, 4, 48, 2, 96);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT3x4_ARRAY
	results = { 96, 192 };
	tempRet = structLayout.add<dmat3x4>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Double, 3, 4, 96, 2, 192);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT4x2_ARRAY
	structLayout.clear();

#pragma region UMAT4x2_ARRAY
	results = { 0, 64 };
	tempRet = structLayout.add<umat4x2>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Uint, 4, 2, 56, 2, 120);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT4x2_ARRAY
	results = { 128, 192 };
	tempRet = structLayout.add<dmat4x2>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Double, 4, 2, 64, 2, 128);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT4x3_ARRAY
	structLayout.clear();

#pragma region UMAT4x3_ARRAY
	results = { 0, 64 };
	tempRet = structLayout.add<umat4x3>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Uint, 4, 3, 60, 2, 124);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT4x3_ARRAY
	results = { 128, 256 };
	tempRet = structLayout.add<dmat4x3>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Double, 4, 3, 120, 2, 248);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion

#pragma region MAT4x4_ARRAY
	structLayout.clear();

#pragma region UMAT4x4_ARRAY
	results = { 0, 64 };
	tempRet = structLayout.add<umat4>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Uint, 4, 4, 64, 2, 128);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion

#pragma region DMAT4x4_ARRAY
	results = { 128, 256 };
	tempRet = structLayout.add<dmat4>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	tempType = std::make_shared<array_type>(ValueType::Double, 4, 4, 128, 2, 256);
	EXPECT_EQ(*base, *tempType);

	castedType = dynamic_type_cast<array_type>(base);
	EXPECT_NE(castedType, nullptr);

	castedType = static_type_cast<array_type>(base);
	EXPECT_EQ(*castedType->get_type(), *tempType->get_type());
	EXPECT_EQ(castedType->get_count(), tempType->get_count());
	EXPECT_EQ(castedType->get_size(), tempType->get_size());
#pragma endregion
#pragma endregion
}