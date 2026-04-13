#include <pch.hpp>

using namespace glslstruct;
using namespace glm;

TEST(scalar_layout, add_scalars) {
	scalar_layout structLayout;
	size_t ret;
#if _GLSL_STRUCT_HAS_TYPES
	base_type_handle base		   = nullptr;
	scalar_type_handle temp_type   = nullptr;
	base_type_handle wrong_type	   = nullptr;
	scalar_type_handle casted_type = nullptr;
#endif

#pragma region BOOL
	ret = structLayout.add<bool>("Bool");
	EXPECT_EQ(ret, 0);
	EXPECT_EQ(structLayout.get_offset("Bool"), 0);

#if _GLSL_STRUCT_HAS_TYPES
	base = structLayout.get_type("Bool");
	EXPECT_EQ(*base, *base);
	EXPECT_EQ(std::hash<base_type>()(*base), std::hash<base_type>()(*base));

	temp_type = std::make_shared<scalar_type>(ValueType::Bool, 4);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());

	wrong_type = std::make_shared<scalar_type>(ValueType::Int, 4);
	EXPECT_NE(*base, *wrong_type);

	wrong_type = std::make_shared<vec_type>(ValueType::Bool, 2, 8);
	EXPECT_NE(*base, *wrong_type);
#endif

	EXPECT_EQ(structLayout.get_size("Bool"), 4);
	EXPECT_EQ(structLayout.get_array_count("Bool"), 1);
#pragma endregion

#pragma region INT
	ret = structLayout.add<int>("Int");
	EXPECT_EQ(ret, 4);

#if _GLSL_STRUCT_HAS_TYPES
	base = structLayout.get_type("Int");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<scalar_type>(ValueType::Int, 4);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#endif

	EXPECT_EQ(structLayout.get_size("Int"), 4);
	EXPECT_EQ(structLayout.get_array_count("Int"), 1);
#pragma endregion

#pragma region UINT
	ret = structLayout.add<unsigned int>("Uint");
	EXPECT_EQ(ret, 8);

#if _GLSL_STRUCT_HAS_TYPES
	base = structLayout.get_type("Uint");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<scalar_type>(ValueType::Uint, 4);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#endif

	EXPECT_EQ(structLayout.get_size("Uint"), 4);
	EXPECT_EQ(structLayout.get_array_count("Uint"), 1);
#pragma endregion

#pragma region FLOAT
	ret = structLayout.add<float>("Float");
	EXPECT_EQ(ret, 12);

#if _GLSL_STRUCT_HAS_TYPES
	base = structLayout.get_type("Float");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<scalar_type>(ValueType::Float, 4);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#endif

	EXPECT_EQ(structLayout.get_size("Float"), 4);
	EXPECT_EQ(structLayout.get_array_count("Float"), 1);
#pragma endregion

#pragma region DOUBLE
	ret = structLayout.add<double>("Double");
	EXPECT_EQ(ret, 16);

#if _GLSL_STRUCT_HAS_TYPES
	base = structLayout.get_type("Double");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<scalar_type>(ValueType::Double, 8);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<scalar_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<scalar_type>(base);
	EXPECT_EQ(casted_type->get_type(), temp_type->get_type());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#endif

	EXPECT_EQ(structLayout.get_size("Double"), 8);
	EXPECT_EQ(structLayout.get_array_count("Double"), 1);
#pragma endregion
}

TEST(scalar_layout, add_scalars_array) {
	scalar_layout structLayout;
	std::vector<size_t> results;
	std::vector<size_t> ret;
#if _GLSL_STRUCT_HAS_TYPES
	base_type_handle base		  = nullptr;
	array_type_handle temp_type	  = nullptr;
	array_type_handle casted_type = nullptr;
#endif

#pragma region BOOL_ARRAY
	results = { 0, 4 };
	ret		= structLayout.add<bool[2]>("Bools");
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
	base = structLayout.get_type("Bools");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Bool, 4, 2, 8);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#endif

	EXPECT_EQ(structLayout.get_size("Bools"), 8);
	EXPECT_EQ(structLayout.get_array_count("Bools"), 2);
#pragma endregion

#pragma region INT_ARRAY
	results = { 8, 12 };
	ret		= structLayout.add<std::array<int, 2>>("Ints");
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
	base = structLayout.get_type("Ints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Int, 4, 2, 8);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#endif

	EXPECT_EQ(structLayout.get_size("Ints"), 8);
	EXPECT_EQ(structLayout.get_array_count("Ints"), 2);
#pragma endregion

#pragma region UINT_ARRAY
	results = { 16, 20 };
	ret		= structLayout.add<unsigned int>("Uints", 2);
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
	base = structLayout.get_type("Uints");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Uint, 4, 2, 8);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#endif

	EXPECT_EQ(structLayout.get_size("Uints"), 8);
	EXPECT_EQ(structLayout.get_array_count("Uints"), 2);
#pragma endregion

#pragma region FLOAT_ARRAY
	results = { 24, 28 };
	ret		= structLayout.add<std::vector<float>>("Floats", 2);
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
	base = structLayout.get_type("Floats");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Float, 4, 2, 8);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#endif

	EXPECT_EQ(structLayout.get_size("Floats"), 8);
	EXPECT_EQ(structLayout.get_array_count("Floats"), 2);
#pragma endregion

#pragma region DOUBLE_ARRAY
	results = { 32, 40 };
	ret		= structLayout.add<double>("Doubles", 2);
	EXPECT_EQ(ret, results);

#if _GLSL_STRUCT_HAS_TYPES
	base = structLayout.get_type("Doubles");
	EXPECT_EQ(*base, *base);

	temp_type = std::make_shared<array_type>(ValueType::Double, 8, 2, 16);
	EXPECT_EQ(*base, *temp_type);

	casted_type = dynamic_type_cast<array_type>(base);
	EXPECT_NE(casted_type, nullptr);

	casted_type = static_type_cast<array_type>(base);
	EXPECT_EQ(*casted_type->get_type(), *temp_type->get_type());
	EXPECT_EQ(casted_type->get_count(), temp_type->get_count());
	EXPECT_EQ(casted_type->get_size(), temp_type->get_size());
#endif

	EXPECT_EQ(structLayout.get_size("Doubles"), 16);
	EXPECT_EQ(structLayout.get_array_count("Doubles"), 2);
#pragma endregion
}