#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(std430_layout, add_matrixes) {
	std430_layout structLayout;
	size_t ret;
#if _GLSL_STRUCT_HAS_TYPES
	base_type_handle base	   = nullptr;
	mat_type_handle tempType   = nullptr;
	mat_type_handle castedType = nullptr;
#endif

#pragma region MAT2x2
	structLayout.clear();

#pragma region BMAT2x2
	ret = structLayout.add<bmat2>("Bool").front();
	EXPECT_EQ(ret, 0);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Bool"), 24);
	EXPECT_EQ(structLayout.get_array_count("Bool"), 2);
#pragma endregion

#pragma region IMAT2x2
	ret = structLayout.add<imat2>("Int").front();
	EXPECT_EQ(ret, 32);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Int"), 24);
	EXPECT_EQ(structLayout.get_array_count("Int"), 2);
#pragma endregion

#pragma region UMAT2x2
	ret = structLayout.add<umat2>("Uint").front();
	EXPECT_EQ(ret, 64);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uint"), 24);
	EXPECT_EQ(structLayout.get_array_count("Uint"), 2);
#pragma endregion

#pragma region MAT2x2
	ret = structLayout.add<fmat2>("Float").front();
	EXPECT_EQ(ret, 96);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Float"), 24);
	EXPECT_EQ(structLayout.get_array_count("Float"), 2);
#pragma endregion

#pragma region DMAT2x2
	ret = structLayout.add<dmat2>("Double").front();
	EXPECT_EQ(ret, 128);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Double"), 32);
	EXPECT_EQ(structLayout.get_array_count("Double"), 2);
#pragma endregion
#pragma endregion

#pragma region MAT2x3
	structLayout.clear();

#pragma region BMAT2x3
	ret = structLayout.add<bmat2x3>("Bool").front();
	EXPECT_EQ(ret, 0);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Bool"), 28);
	EXPECT_EQ(structLayout.get_array_count("Bool"), 2);
#pragma endregion

#pragma region IMAT2x3
	ret = structLayout.add<imat2x3>("Int").front();
	EXPECT_EQ(ret, 32);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Int"), 28);
	EXPECT_EQ(structLayout.get_array_count("Int"), 2);
#pragma endregion

#pragma region UMAT2x3
	ret = structLayout.add<umat2x3>("Uint").front();
	EXPECT_EQ(ret, 64);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uint"), 28);
	EXPECT_EQ(structLayout.get_array_count("Uint"), 2);
#pragma endregion

#pragma region MAT2x3
	ret = structLayout.add<fmat2x3>("Float").front();
	EXPECT_EQ(ret, 96);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Float"), 28);
	EXPECT_EQ(structLayout.get_array_count("Float"), 2);
#pragma endregion

#pragma region DMAT2x3
	ret = structLayout.add<dmat2x3>("Double").front();
	EXPECT_EQ(ret, 128);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Double"), 56);
	EXPECT_EQ(structLayout.get_array_count("Double"), 2);
#pragma endregion
#pragma endregion

#pragma region MAT2x4
	structLayout.clear();

#pragma region BMAT2x4
	ret = structLayout.add<bmat2x4>("Bool").front();
	EXPECT_EQ(ret, 0);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Bool"), 32);
	EXPECT_EQ(structLayout.get_array_count("Bool"), 2);
#pragma endregion

#pragma region IMAT2x4
	ret = structLayout.add<imat2x4>("Int").front();
	EXPECT_EQ(ret, 32);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Int"), 32);
	EXPECT_EQ(structLayout.get_array_count("Int"), 2);
#pragma endregion

#pragma region UMAT2x4
	ret = structLayout.add<umat2x4>("Uint").front();
	EXPECT_EQ(ret, 64);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uint"), 32);
	EXPECT_EQ(structLayout.get_array_count("Uint"), 2);
#pragma endregion

#pragma region MAT2x4
	ret = structLayout.add<fmat2x4>("Float").front();
	EXPECT_EQ(ret, 96);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Float"), 32);
	EXPECT_EQ(structLayout.get_array_count("Float"), 2);
#pragma endregion

#pragma region DMAT2x4
	ret = structLayout.add<dmat2x4>("Double").front();
	EXPECT_EQ(ret, 128);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Double"), 64);
	EXPECT_EQ(structLayout.get_array_count("Double"), 2);
#pragma endregion
#pragma endregion

#pragma region MAT3x2
	structLayout.clear();

#pragma region UMAT3x2
	ret = structLayout.add<umat3x2>("Uint").front();
	EXPECT_EQ(ret, 0);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uint"), 40);
	EXPECT_EQ(structLayout.get_array_count("Uint"), 3);
#pragma endregion

#pragma region DMAT3x2
	ret = structLayout.add<dmat3x2>("Double").front();
	EXPECT_EQ(ret, 48);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Double"), 48);
	EXPECT_EQ(structLayout.get_array_count("Double"), 3);
#pragma endregion
#pragma endregion

#pragma region MAT3x3
	structLayout.clear();

#pragma region UMAT3x3
	ret = structLayout.add<umat3>("Uint").front();
	EXPECT_EQ(ret, 0);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uint"), 44);
	EXPECT_EQ(structLayout.get_array_count("Uint"), 3);
#pragma endregion

#pragma region DMAT3x3
	ret = structLayout.add<dmat3>("Double").front();
	EXPECT_EQ(ret, 64);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Double"), 88);
	EXPECT_EQ(structLayout.get_array_count("Double"), 3);
#pragma endregion
#pragma endregion

#pragma region MAT3x4
	structLayout.clear();

#pragma region UMAT3x4
	ret = structLayout.add<umat3x4>("Uint").front();
	EXPECT_EQ(ret, 0);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uint"), 48);
	EXPECT_EQ(structLayout.get_array_count("Uint"), 3);
#pragma endregion

#pragma region DMAT3x4
	ret = structLayout.add<dmat3x4>("Double").front();
	EXPECT_EQ(ret, 64);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Double"), 96);
	EXPECT_EQ(structLayout.get_array_count("Double"), 3);
#pragma endregion
#pragma endregion

#pragma region MAT4x2
	structLayout.clear();

#pragma region UMAT4x2
	ret = structLayout.add<umat4x2>("Uint").front();
	EXPECT_EQ(ret, 0);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uint"), 56);
	EXPECT_EQ(structLayout.get_array_count("Uint"), 4);
#pragma endregion

#pragma region DMAT4x2
	ret = structLayout.add<dmat4x2>("Double").front();
	EXPECT_EQ(ret, 64);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Double"), 64);
	EXPECT_EQ(structLayout.get_array_count("Double"), 4);
#pragma endregion
#pragma endregion

#pragma region MAT4x3
	structLayout.clear();

#pragma region UMAT4x3
	ret = structLayout.add<umat4x3>("Uint").front();
	EXPECT_EQ(ret, 0);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uint"), 60);
	EXPECT_EQ(structLayout.get_array_count("Uint"), 4);
#pragma endregion

#pragma region DMAT4x3
	ret = structLayout.add<dmat4x3>("Double").front();
	EXPECT_EQ(ret, 64);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Double"), 120);
	EXPECT_EQ(structLayout.get_array_count("Double"), 4);
#pragma endregion
#pragma endregion

#pragma region MAT4x4
	structLayout.clear();

#pragma region UMAT4x4
	ret = structLayout.add<umat4>("Uint").front();
	EXPECT_EQ(ret, 0);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uint"), 64);
	EXPECT_EQ(structLayout.get_array_count("Uint"), 4);
#pragma endregion

#pragma region DMAT4x4
	ret = structLayout.add<dmat4>("Double").front();
	EXPECT_EQ(ret, 64);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Double"), 128);
	EXPECT_EQ(structLayout.get_array_count("Uint"), 4);
#pragma endregion
#pragma endregion
}

TEST(std430_layout, add_marixes_array) {
	std430_layout structLayout;
	std::vector<size_t> results;
	std::vector<size_t> ret;
	std::vector<std::vector<size_t>> tempRet;
#if _GLSL_STRUCT_HAS_TYPES
	base_type_handle base		 = nullptr;
	array_type_handle tempType	 = nullptr;
	array_type_handle castedType = nullptr;
#endif

#pragma region MAT2x2_ARRAY
	structLayout.clear();

#pragma region BMAT2x2_ARRAY
	results = { 0, 32 };
	tempRet = structLayout.add<bmat2>("Bools", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Bools"), 56);
	EXPECT_EQ(structLayout.get_array_count("Bools"), 2);
#pragma endregion

#pragma region IMAT2x2_ARRAY
	results = { 64, 96 };
	tempRet = structLayout.add<imat2>("Ints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Ints"), 56);
	EXPECT_EQ(structLayout.get_array_count("Ints"), 2);
#pragma endregion

#pragma region UMAT2x2_ARRAY
	results = { 128, 160 };
	tempRet = structLayout.add<umat2>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uints"), 56);
	EXPECT_EQ(structLayout.get_array_count("Uints"), 2);
#pragma endregion

#pragma region MAT2x2_ARRAY
	results = { 192, 224 };
	tempRet = structLayout.add<fmat2>("Floats", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Floats"), 56);
	EXPECT_EQ(structLayout.get_array_count("Floats"), 2);
#pragma endregion

#pragma region DMAT2x2_ARRAY
	results = { 256, 288 };
	tempRet = structLayout.add<dmat2>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Doubles"), 64);
	EXPECT_EQ(structLayout.get_array_count("Doubles"), 2);
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

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Bools"), 60);
	EXPECT_EQ(structLayout.get_array_count("Bools"), 2);
#pragma endregion

#pragma region IMAT2x3_ARRAY
	results = { 64, 96 };
	tempRet = structLayout.add<imat2x3>("Ints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Ints"), 60);
	EXPECT_EQ(structLayout.get_array_count("Ints"), 2);
#pragma endregion

#pragma region UMAT2x3_ARRAY
	results = { 128, 160 };
	tempRet = structLayout.add<umat2x3>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uints"), 60);
	EXPECT_EQ(structLayout.get_array_count("Uints"), 2);
#pragma endregion

#pragma region MAT2x3_ARRAY
	results = { 192, 224 };
	tempRet = structLayout.add<fmat2x3>("Floats", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Floats"), 60);
	EXPECT_EQ(structLayout.get_array_count("Floats"), 2);
#pragma endregion

#pragma region DMAT2x3_ARRAY
	results = { 256, 320 };
	tempRet = structLayout.add<dmat2x3>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Doubles"), 120);
	EXPECT_EQ(structLayout.get_array_count("Doubles"), 2);
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

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Bools"), 64);
	EXPECT_EQ(structLayout.get_array_count("Bools"), 2);
#pragma endregion

#pragma region IMAT2x4_ARRAY
	results = { 64, 96 };
	tempRet = structLayout.add<imat2x4>("Ints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Ints"), 64);
	EXPECT_EQ(structLayout.get_array_count("Ints"), 2);
#pragma endregion

#pragma region UMAT2x4_ARRAY
	results = { 128, 160 };
	tempRet = structLayout.add<umat2x4>("Uints", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uints"), 64);
	EXPECT_EQ(structLayout.get_array_count("Uints"), 2);
#pragma endregion

#pragma region MAT2x4_ARRAY
	results = { 192, 224 };
	tempRet = structLayout.add<fmat2x4>("Floats", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Floats"), 64);
	EXPECT_EQ(structLayout.get_array_count("Floats"), 2);
#pragma endregion

#pragma region DMAT2x4_ARRAY
	results = { 256, 320 };
	tempRet = structLayout.add<dmat2x4>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Doubles"), 128);
	EXPECT_EQ(structLayout.get_array_count("Doubles"), 2);
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

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uints"), 88);
	EXPECT_EQ(structLayout.get_array_count("Uints"), 2);
#pragma endregion

#pragma region DMAT3x2_ARRAY
	results = { 96, 144 };
	tempRet = structLayout.add<dmat3x2>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Doubles"), 96);
	EXPECT_EQ(structLayout.get_array_count("Doubles"), 2);
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

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uints"), 92);
	EXPECT_EQ(structLayout.get_array_count("Uints"), 2);
#pragma endregion

#pragma region DMAT3x3_ARRAY
	results = { 96, 192 };
	tempRet = structLayout.add<dmat3>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Doubles"), 184);
	EXPECT_EQ(structLayout.get_array_count("Doubles"), 2);
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

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uints"), 96);
	EXPECT_EQ(structLayout.get_array_count("Uints"), 2);
#pragma endregion

#pragma region DMAT3x4_ARRAY
	results = { 96, 192 };
	tempRet = structLayout.add<dmat3x4>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Doubles"), 192);
	EXPECT_EQ(structLayout.get_array_count("Doubles"), 2);
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

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uints"), 120);
	EXPECT_EQ(structLayout.get_array_count("Uints"), 2);
#pragma endregion

#pragma region DMAT4x2_ARRAY
	results = { 128, 192 };
	tempRet = structLayout.add<dmat4x2>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Doubles"), 128);
	EXPECT_EQ(structLayout.get_array_count("Doubles"), 2);
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

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uints"), 124);
	EXPECT_EQ(structLayout.get_array_count("Uints"), 2);
#pragma endregion

#pragma region DMAT4x3_ARRAY
	results = { 128, 256 };
	tempRet = structLayout.add<dmat4x3>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Doubles"), 248);
	EXPECT_EQ(structLayout.get_array_count("Doubles"), 2);
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

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Uints"), 128);
	EXPECT_EQ(structLayout.get_array_count("Uints"), 2);
#pragma endregion

#pragma region DMAT4x4_ARRAY
	results = { 128, 256 };
	tempRet = structLayout.add<dmat4>("Doubles", 2);

	ret.clear();
	ret.reserve(tempRet.size());
		for (size_t i = 0; i < tempRet.size(); i++) { ret.push_back(tempRet[i].front()); }
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
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
#endif

	EXPECT_EQ(structLayout.get_size("Doubles"), 256);
	EXPECT_EQ(structLayout.get_array_count("Doubles"), 2);
#pragma endregion
#pragma endregion
}