#pragma once
#include <mstd/types.hpp>
#include <glm/glm.hpp>
#include <type_traits>

namespace glslstruct::utils {
	template<typename T> const T& unmove(T&& x) {
		return x;
	};

#pragma region SCALAR_CHECK
	template<class T>
	struct is_scalar : std::bool_constant<mstd::is_same_type_in_v<T, bool, int, unsigned int, float, double>> {};
	template<class T> static constexpr bool is_scalar_v = is_scalar<T>::value;
	template<class T, class Ret = void> using scalar_enable_if_t = std::enable_if_t<is_scalar_v<T>, Ret>;

	template<class SV>
	struct is_scalars_vector : std::false_type {};
	template<class S>
	struct is_scalars_vector<std::vector<S>> : std::bool_constant<is_scalar_v<S>> {};
	template<class SV> static constexpr bool is_scalars_vector_v = is_scalars_vector<SV>::value;
	template<class SV, class Ret = void> using scalars_vector_enable_if_t = std::enable_if_t<is_scalars_vector_v<SV>, Ret>;

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<class T> concept scalar = is_scalar_v<T>;
	template<class SV> concept scalars_vector = is_scalars_vector_v<SV>;
#endif
#pragma endregion

#pragma region VEC_CHECK
	template<class V>
	struct is_vec : std::false_type {};
	template<class T, size_t L, auto... Os>
	struct is_vec<glm::vec<L, T, Os...>> : std::conditional_t<
		is_scalar_v<T>&& mstd::is_in_range_v<L, 2, 4>,
		std::true_type,
		std::false_type
	> {
	};
	template<class V> static constexpr bool is_vec_v = is_vec<V>::value;
	template<class V, class Ret = void> using vec_enable_if_t = std::enable_if_t<is_vec_v<V>, Ret>;

	template<class VV>
	struct is_vecs_vector : std::false_type {};
	template<class V>
	struct is_vecs_vector<std::vector<V>> : std::bool_constant<is_vec_v<V>> {};
	template<class VV> static constexpr bool is_vecs_vector_v = is_vecs_vector<VV>::value;
	template<class VV, class Ret = void> using vecs_vector_enable_if_t = std::enable_if_t<is_vecs_vector_v<VV>, Ret>;

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<class V> concept vec = is_vec_v<V>;
	template<class VV> concept vecs_vector = is_vecs_vector_v<VV>;
#endif
#pragma endregion

#pragma region MAT_CHECK
	template<class M>
	struct is_mat : std::false_type {};
	template<class T, size_t C, size_t R, auto... Os>
	struct is_mat<glm::mat<C, R, T, Os...>> : std::conditional_t<
		(is_scalar_v<T>&& mstd::is_in_range_v<C, 2, 4>&& mstd::is_in_range_v<R, 2, 4>),
		std::true_type,
		std::false_type
	> {
	};
	template<class M> static constexpr bool is_mat_v = is_mat<M>::value;
	template<class M, class Ret = void> using mat_enable_if_t = std::enable_if_t<is_mat_v<M>, Ret>;

	template<class MV>
	struct is_mats_vector : std::false_type {};
	template<class M>
	struct is_mats_vector<std::vector<M>> : std::bool_constant<is_mat_v<M>> {};
	template<class MV> static constexpr bool is_mats_vector_v = is_mats_vector<MV>::value;
	template<class MV, class Ret = void> using mats_vector_enable_if_t = std::enable_if_t<is_mats_vector_v<MV>, Ret>;

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<class M> concept mat = is_mat_v<M>;
	template<class MV> concept mats_vector = is_mats_vector_v<MV>;
#endif
#pragma endregion

	template<class T> static constexpr bool is_any_standard_value_v = is_scalar_v<T> || is_vec_v<T> || is_mat_v<T>;

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<class T> concept any_standard_value = is_any_standard_value_v<T>;
#endif
}

namespace glslstruct {
	class std_offset;
	class std140_offset;
	class std430_offset;
}

namespace glslstruct::utils {
#pragma region OFFSETS_CHECK
	template<class T> static constexpr bool is_offset140_v = std::is_same_v<T, ::glslstruct::std140_offset>;
	template<class T> static constexpr bool is_offset430_v = std::is_same_v<T, ::glslstruct::std430_offset>;
	template<class T> static constexpr bool is_any_offset_v = is_offset140_v<T> || is_offset430_v<T>;

	template<class T, class Ret = void> using any_offset_enable_if_t = std::enable_if_t<is_any_offset_v<T>, Ret>;

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<class T> concept any_offset = is_any_offset_v<T>;
#endif
#pragma endregion

	template<class T> static constexpr bool is_any_standard_or_offset_value_v = is_any_standard_value_v<T> || is_any_offset_v<T>;

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<class T> concept any_standard_or_offset_value = is_any_standard_or_offset_value_v<T>;
#endif
}

namespace glslstruct {
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<utils::any_offset _Offset>
	class std_struct;
#else
	template<class _Offset, utils::any_offset_enable_if_t<_Offset, bool> = true>
	class std_struct;
#endif
}

namespace glslstruct::utils {
#pragma region STRUCT_CHECK
	template<class S, class O>
	struct is_std_struct : std::false_type {};
	template<class O, class Offset>
	struct is_std_struct<std_struct<Offset>, O> : std::bool_constant<std::is_same_v<Offset, O>> {};

	template<class S, class _Offset> static constexpr bool is_std_struct_v = is_std_struct<S, _Offset>::value;
	template<class S, class _Offset, class Ret = void> using std_struct_enable_if_t = std::enable_if_t<is_std_struct_v<S, _Offset>, Ret>;

	template<class VS, class _Offset>
	struct is_std_struct_vector : std::false_type {};
	template<class O, class _Offset>
	struct is_std_struct_vector<std::vector<std_struct<O>>, _Offset> : std::bool_constant<is_std_struct_v<std_struct<O>, _Offset>> {};
	template<class VS, class _Offset> static constexpr bool is_std_structs_vector_v = is_std_struct_vector<VS, _Offset>::value;
	template<class VS, class _Offset, class Ret = void> using std_structs_vector_enable_if_t = std::enable_if_t<is_std_structs_vector_v<VS, _Offset>, Ret>;

	template<class S>
	struct is_any_std_struct : std::false_type {};
	template<class O>
	struct is_any_std_struct<std_struct<O>> : std::true_type {};
	template<class S> static constexpr bool is_any_std_struct_v = is_any_std_struct<S>::value;

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<class S, class _Offset> concept std_struct = is_std_struct_v<S, _Offset>;
	template<class VS, class _Offset> concept std_structs_vector = is_std_structs_vector_v<VS, _Offset>;
	template<class S> concept any_std_struct = is_any_std_struct_v<S>;
#endif
#pragma endregion

	template<class T> static constexpr bool is_any_standard_or_std_struct_value_v = is_any_standard_value_v<T> || is_any_std_struct_v<T>;

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<class T> concept any_standard_or_std_struct_value = is_any_standard_or_std_struct_value_v<T>;
#endif
}

namespace glslstruct {
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
	template<utils::any_standard_or_offset_value T, size_t num = 0>
	struct std_variable;
	template<utils::any_standard_or_std_struct_value T, size_t num = 0>
	struct std_value;
#else
	template<class T, size_t num = 0,
		std::enable_if_t<(utils::is_any_offset_v<T> || utils::is_any_standard_value_v<T>), bool> = true>
	struct std_variable;
	template<class T, size_t num = 0,
		std::enable_if_t<(utils::is_any_standard_value_v<T> || utils::is_any_std_struct_v<T>), bool> = true>
	struct std_value;
#endif
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