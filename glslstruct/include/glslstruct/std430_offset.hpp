#pragma once
#include <glslstruct/std_offset.hpp>

namespace glslstruct {
	class std430_offset : public std_offset {
	protected:

		[[nodiscard]] virtual std::vector<size_t> _addArray(const std::string& name, size_t arraySize, 
			size_t baseAligement, size_t baseOffset, const base_type* type) override;

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<class T, class... Ts, size_t num, size_t... nums>
		requires(!utils::is_offset140_v<T>)
#else
		template<class T, class... Ts, size_t num, size_t... nums, 
			std::enable_if_t<!utils::is_offset140_v<T>, bool> = true>
#endif
		void _addMultiple(const std_variable<T, num>& var, const std_variable<Ts, nums>&... vars) {
			if constexpr (var.is_offsets) {
				if (num == 0) {
					add(var.var_name, var.struct_offsets);
				}
				else {
					add(var.var_name, var.struct_offsets, num);
				}
			}
			else {
				if (num == 0) {
					add<T>(var.var_name);
				}
				else {
					add<T>(var.var_name, num);
				}
			}
			if constexpr (sizeof...(Ts) > 0 && sizeof...(nums) > 0) {
				_addMultiple(vars...);
			}
		}

	public:
		std430_offset() = default;
		std430_offset(std430_offset& std430off);
		std430_offset(const std430_offset& std430off);
		std430_offset(std430_offset&& std430off);
		template<class... Args, size_t... nums>
		std430_offsets(const std_variable<Args, nums>&... vars) {
			_addMultiple(vars...);
		}
		virtual ~std430_offset() = default;

		std430_offset& operator=(std430_offset& std430off);
		std430_offset& operator=(const std430_offset& std430off);
		std430_offset& operator=(std430_offset&& std430off);

		DECLARE_OVERRIDED_CLONE_FUNC(std430_offset)

#pragma region ADD_SCALAR
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		[[nodiscard]] constexpr size_t add(const std::string& name) {
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _addScalar(name, 4, 4, getValueType<T>());
			}
			else {
				return _addScalar(name, sizeof(T), sizeof(T), getValueType<T>());
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::scalar T>
#else
		template<class T, utils::scalar_enable_if_t<T, bool> = true>
#endif
		[[nodiscard]] constexpr std::vector<size_t> add(const std::string& name, size_t size) {
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _addScalarArray(name, 4, 4, getValueType<T>(), size);
			}
			else {
				return _addScalarArray(name, sizeof(T), sizeof(T), getValueType<T>(), size);
			}
		}
#pragma endregion

#pragma region ADD_VEC
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		[[nodiscard]] constexpr size_t add(const std::string& name) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _addVector(name, L, 4, 4, getValueType<T>());
			}
			else {
				return _addVector(name, L, sizeof(T), sizeof(T), getValueType<T>());
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::vec V>
#else
		template<class V, utils::vec_enable_if_t<V, bool> = true>
#endif
		[[nodiscard]] constexpr std::vector<size_t> add(const std::string& name, size_t size) {
			using T = typename V::value_type;
			static constexpr size_t L = V::length();
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _addVectorArray(name, L, 4, 4, getValueType<T>(), size);
			}
			else {
				return _addVectorArray(name, L, sizeof(T), sizeof(T), getValueType<T>(), size);
			}
		}
#pragma endregion

#pragma region ADD_MAT
#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		[[nodiscard]] constexpr size_t add(const std::string& name) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _addMatrix(name, C, R, column_major, 4, 4, getValueType<T>());
			}
			else {
				return _addMatrix(name, C, R, column_major, sizeof(T), sizeof(T), getValueType<T>());
			}
		}

#if _HAS_CXX20 && _GLSL_STRUCT_ENABLE_CXX20
		template<utils::mat M, bool column_major = true>
#else
		template<class M, bool column_major = true, utils::mat_enable_if_t<M, bool> = true>
#endif
		[[nodiscard]] constexpr std::vector<size_t> add(const std::string& name, size_t size) {
			using T = typename M::value_type;
			static constexpr size_t C = M::row_type::length();
			static constexpr size_t R = M::col_type::length();
			if constexpr (std::is_same_v<T, bool>) {
				return _addMatrixArray(name, C, R, column_major, 4, 4, getValueType<T>(), size);
			}
			else {
				return _addMatrixArray(name, C, R, column_major, sizeof(T), sizeof(T), getValueType<T>(), size);
			}
		}
#pragma endregion

#pragma region ADD_STRUCT
		[[nodiscard]] size_t add(const std::string& name, const std430_offset& structTemplate);

		[[nodiscard]] std::vector<size_t> add(const std::string& name, const std430_offset& structTemplate, size_t size);
#pragma endregion

		[[nodiscard]] virtual size_t getBaseAligement() const override;
	};
}