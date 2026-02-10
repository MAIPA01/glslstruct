/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <glslstruct/config.hpp>
#include <glslstruct/libraries.hpp>

namespace glslstruct::utils {
#pragma region SCALAR_CHECK
	template<class T> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_scalar_v = mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double>;

	template<class SV>
	struct is_glsl_scalars_vector : std::false_type {};
	template<class S>
	struct is_glsl_scalars_vector<std::vector<S>> : std::bool_constant<is_glsl_scalar_v<S>> {};
	template<class SV> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_scalars_vector_v = is_glsl_scalars_vector<SV>::value;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T> concept glsl_scalar = is_glsl_scalar_v<T>;
	template<class SV> concept glsl_scalars_vector = is_glsl_scalars_vector_v<SV>;
#endif
#pragma endregion

#pragma region VEC_CHECK
	template<class V>
	struct is_glsl_vec : std::false_type {};
	template<class T, size_t L, glm::qualifier Q>
	struct is_glsl_vec<glm::vec<L, T, Q>> : 
		std::bool_constant<(is_glsl_scalar_v<T> && mstd::is_in_range_v<L, 1, 4>)> {};
	template<class V> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_vec_v = is_glsl_vec<V>::value;

	template<class VV>
	struct is_glsl_vecs_vector : std::false_type {};
	template<class V>
	struct is_glsl_vecs_vector<std::vector<V>> : std::bool_constant<is_glsl_vec_v<V>> {};
	template<class VV> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_vecs_vector_v = is_glsl_vecs_vector<VV>::value;

#if _GLSL_STRUCT_HAS_CXX20
	template<class V> concept glsl_vec = is_glsl_vec_v<V>;
	template<class VV> concept glsl_vecs_vector = is_glsl_vecs_vector_v<VV>;
#endif
#pragma endregion

#pragma region MAT_CHECK
	template<class M>
	struct is_glsl_mat : std::false_type {};
	template<class T, size_t C, size_t R, glm::qualifier Q>
	struct is_glsl_mat<glm::mat<C, R, T, Q>> 
		: std::bool_constant<(is_glsl_scalar_v<T> && mstd::is_in_range_v<C, 2, 4> && mstd::is_in_range_v<R, 2, 4>)> {};
	template<class M> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_mat_v = is_glsl_mat<M>::value;

	template<class MV>
	struct is_glsl_mats_vector : std::false_type {};
	template<class M>
	struct is_glsl_mats_vector<std::vector<M>> : std::bool_constant<is_glsl_mat_v<M>> {};
	template<class MV> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_mats_vector_v = is_glsl_mats_vector<MV>::value;

#if _GLSL_STRUCT_HAS_CXX20
	template<class M> concept glsl_mat = is_glsl_mat_v<M>;
	template<class MV> concept glsl_mats_vector = is_glsl_mats_vector_v<MV>;
#endif
#pragma endregion

	template<class T> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_simple_value_v = is_glsl_scalar_v<T> || is_glsl_vec_v<T> || is_glsl_mat_v<T>;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T> concept glsl_simple_value = is_glsl_simple_value_v<T>;
#endif
}

namespace glslstruct {
	class std_offset;
	class std140_offset;
	class std430_offset;
}

namespace glslstruct::utils {
#pragma region OFFSETS_CHECK
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_offset_v = std::is_base_of_v<std_offset, T> && !std::is_same_v<std_offset, T>;
	template<class T>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_offset140_v = std::is_same_v<T, std140_offset>;
	template<class T> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_offset430_v = std::is_same_v<T, std430_offset>;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T> concept glsl_offset = is_glsl_offset_v<T>;
#endif
#pragma endregion

	template<class T> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_simple_or_offset_value_v = is_glsl_simple_value_v<T> || is_glsl_offset_v<T>;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T> concept glsl_simple_or_offset_value = is_glsl_simple_or_offset_value_v<T>;
#endif
}

namespace glslstruct {
	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(_Offset, utils::glsl_offset, utils::is_glsl_offset_v<_Offset>, = true)
	class std_struct;
}

namespace glslstruct::utils {
#pragma region STRUCT_CHECK
	template<class S, class O>
	struct is_glsl_struct_with_offset : std::false_type {};
	template<class O, class Offset>
	struct is_glsl_struct_with_offset<glslstruct::std_struct<Offset>, O> : std::bool_constant<std::is_same_v<Offset, O>> {};

	template<class S, class _Offset> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_struct_with_offset_v = is_glsl_struct_with_offset<S, _Offset>::value;

	template<class VS, class _Offset>
	struct is_glsl_structs_vector_with_offset : std::false_type {};
	template<class O, class _Offset>
	struct is_glsl_structs_vector_with_offset<std::vector<glslstruct::std_struct<O>>, _Offset> 
		: std::bool_constant<is_glsl_struct_with_offset_v<std_struct<O>, _Offset>> {};
	template<class VS, class _Offset> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_structs_vector_with_offset_v = is_glsl_structs_vector_with_offset<VS, _Offset>::value;

	template<class S>
	struct is_glsl_struct : std::false_type {};
	template<class O>
	struct is_glsl_struct<glslstruct::std_struct<O>> : std::true_type {};
	template<class S> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_struct_v = is_glsl_struct<S>::value;

#if _GLSL_STRUCT_HAS_CXX20
	template<class S, class _Offset> concept glsl_struct_with_offset = is_glsl_struct_with_offset_v<S, _Offset>;
	template<class VS, class _Offset> concept glsl_structs_vector_with_offset = is_glsl_structs_vector_with_offset_v<VS, _Offset>;
	template<class S> concept glsl_struct = is_glsl_struct_v<S>;
#endif
#pragma endregion

	template<class T> 
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_simple_or_struct_value_v = is_glsl_simple_value_v<T> || is_glsl_struct_v<T>;

	template<class T, class _Offset>
	static _GLSL_STRUCT_CONSTEXPR17 const bool is_glsl_simple_or_struct_with_offset_value_v = is_glsl_simple_value_v<T> || 
		is_glsl_struct_with_offset_v<T, _Offset>;

#if _GLSL_STRUCT_HAS_CXX20
	template<class T> concept glsl_simple_or_struct_value = is_glsl_simple_or_struct_value_v<T>;
	template<class T, class _Offset> concept glsl_simple_or_struct_with_offset_value = is_glsl_simple_or_struct_with_offset_value_v<T, _Offset>;
#endif
}

namespace glslstruct {
	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_simple_or_offset_value, utils::is_glsl_simple_or_offset_value_v<T>, = true, size_t num = 0)
	struct std_variable;
	_GLSL_STRUCT_ONE_CLASS_TEMPLATE(T, utils::glsl_simple_or_struct_value, utils::is_glsl_simple_or_struct_value_v<T>, = true, size_t num = 0)
	struct std_value;
}

#pragma region SOME_ADDITIONAL_GLM_TYPES
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
#pragma endregion