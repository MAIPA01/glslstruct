#pragma once
#include <STDOffsets.hpp>

namespace glsl {
	class STD430Offsets : public STDOffsets {
	protected:

		virtual std::vector<size_t> _AddArray(const std::string& name, size_t arraySize, size_t baseAligement, size_t baseOffset, const ValueType* type) override;

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<class T, class... Ts, size_t num, size_t... nums>
		requires(!utils::is_offset140_v<T>)
#else
		template<class T, class... Ts, size_t num, size_t... nums, 
			std::enable_if_t<!utils::is_offset140_v<T>, bool> = true>
#endif
		void _AddMultiple(const STDVariable<T, num>& var, const STDVariable<Ts, nums>&... vars) {
			if constexpr (var.is_offsets) {
				if (num == 0) {
					Add(var.var_name, var.struct_offsets);
				}
				else {
					Add(var.var_name, var.struct_offsets, num);
				}
			}
			else {
				if (num == 0) {
					Add<T>(var.var_name);
				}
				else {
					Add<T>(var.var_name, num);
				}
			}
			if constexpr (sizeof...(Ts) > 0 && sizeof...(nums) > 0) {
				_AddMultiple(vars...);
			}
		}

	public:
		STD430Offsets() = default;
		STD430Offsets(STD430Offsets& std430off);
		STD430Offsets(const STD430Offsets& std430off);
		STD430Offsets(STD430Offsets&& std430off);
		template<class... Args, size_t... nums>
		STD430Offsets(const STDVariable<Args, nums>&... vars) {
			_AddMultiple(vars...);
		}
		virtual ~STD430Offsets() = default;

		STD430Offsets& operator=(STD430Offsets& std430off);
		STD430Offsets& operator=(const STD430Offsets& std430off);
		STD430Offsets& operator=(STD430Offsets&& std430off);

		DECLARE_OVERRIDED_CLONE_FUNC(STD430Offsets)

#pragma region ADD_SCALAR
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		size_t Add(const std::string& name) {
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _AddScalar(name, 4, 4, GetValueType<T>());
			}
			else {
				return _AddScalar(name, sizeof(T), sizeof(T), GetValueType<T>());
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, size_t size) {
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _AddScalarArray(name, 4, 4, GetValueType<T>(), size);
			}
			else {
				return _AddScalarArray(name, sizeof(T), sizeof(T), GetValueType<T>(), size);
			}
		}
#pragma endregion

#pragma region ADD_VEC
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		size_t Add(const std::string& name) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _AddVector(name, L, 4, 4, GetValueType<T>());
			}
			else {
				return _AddVector(name, L, sizeof(T), sizeof(T), GetValueType<T>());
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, size_t size) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _AddVectorArray(name, L, 4, 4, GetValueType<T>(), size);
			}
			else {
				return _AddVectorArray(name, L, sizeof(T), sizeof(T), GetValueType<T>(), size);
			}
		}
#pragma endregion

#pragma region ADD_MAT
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		size_t Add(const std::string& name) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _AddMatrix(name, C, R, column_major, 4, 4, GetValueType<T>());
			}
			else {
				return _AddMatrix(name, C, R, column_major, sizeof(T), sizeof(T), GetValueType<T>());
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		std::vector<size_t> Add(const std::string& name, size_t size) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (std::is_same_v<T, bool>) {
				return _AddMatrixArray(name, C, R, column_major, 4, 4, GetValueType<T>(), size);
			}
			else {
				return _AddMatrixArray(name, C, R, column_major, sizeof(T), sizeof(T), GetValueType<T>(), size);
			}
		}
#pragma endregion

#pragma region ADD_STRUCT
		size_t Add(const std::string& name, const STD430Offsets& structTemplate);

		std::vector<size_t> Add(const std::string& name, const STD430Offsets& structTemplate, size_t size);
#pragma endregion

		virtual size_t GetBaseAligement() const override;
	};
}