#pragma once
#include <STDOffsets.h>

namespace glsl {
	class STD140Offsets : public STDOffsets {
	protected:

		template<class T, class... Ts, size_t num, size_t... nums>
		requires(sizeof...(Ts) == sizeof...(nums))
		std::enable_if_t<!std::is_same_v<T, STD430Offsets>>
		_AddMultiple(const STDVariable<T, num>& var, const STDVariable<Ts, nums>&... vars) {
			if constexpr (std::is_same_v<T, STD140Offsets>) {
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
		STD140Offsets() = default;
		STD140Offsets(STD140Offsets& std140off);
		STD140Offsets(const STD140Offsets& std140off);
		STD140Offsets(STD140Offsets&& std140off);
		template<class... Args, size_t... nums>
		requires(sizeof...(Args) == sizeof...(nums))
		STD140Offsets(const STDVariable<Args, nums>&... vars) {
			_AddMultiple(vars...);
		}
		virtual ~STD140Offsets() = default;

		STD140Offsets& operator=(STD140Offsets& std140off);
		STD140Offsets& operator=(const STD140Offsets& std140off);
		STD140Offsets& operator=(STD140Offsets&& std140off);

		DeclareCloneFunc(STD140Offsets)

#pragma region ADD_SCALAR
		template<class T>
		typename extra::scalar_enable_if_t<T, size_t>
		Add(const std::string& name) {
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _AddScalar(name, 4, 4, GetValueType<T>());
			}
			else {
				return _AddScalar(name, sizeof(T), sizeof(T), GetValueType<T>());
			}
		}

		template<class T>
		typename extra::scalar_enable_if_t<T, std::vector<size_t>>
		Add(const std::string& name, size_t size) {
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
		template<class V, class T = V::value_type, size_t L = V::length()>
		typename extra::vec_enable_if_t<T, L, size_t>
		Add(const std::string& name) {
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _AddVector(name, L, 4, 4, GetValueType<T>());
			}
			else {
				return _AddVector(name, L, sizeof(T), sizeof(T), GetValueType<T>());
			}
		}

		template<class V, class T = V::value_type, size_t L = V::length()>
		typename extra::vec_enable_if_t<T, L, std::vector<size_t>>
		Add(const std::string& name, size_t size) {
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
		template<class M, bool column_major = true, class T = M::value_type, size_t C = M::row_type::length(), size_t R = M::col_type::length()>
		typename extra::mat_enable_if_t<T, C, R, size_t>
		Add(const std::string& name) {
			if constexpr (std::is_same_v<T, bool>) {
				// sizeof(unsigned int) = 4
				return _AddMatrix(name, C, R, column_major, 4, 4, GetValueType<T>());
			}
			else {
				return _AddMatrix(name, C, R, column_major, sizeof(T), sizeof(T), GetValueType<T>());
			}
		}

		template<class M, bool column_major = true, class T = M::value_type, size_t C = M::row_type::length(), size_t R = M::col_type::length()>
		typename extra::mat_enable_if_t<T, C, R, std::vector<size_t>>
		Add(const std::string& name, size_t size) {
			if constexpr (std::is_same_v<T, bool>) {
				return _AddMatrixArray(name, C, R, column_major, 4, 4, GetValueType<T>(), size);
			}
			else {
				return _AddMatrixArray(name, C, R, column_major, sizeof(T), sizeof(T), GetValueType<T>(), size);
			}
		}
#pragma endregion

#pragma region ADD_STRUCT
		size_t Add(const std::string& name, const STD140Offsets& structTemplate);

		std::vector<size_t> Add(const std::string& name, const STD140Offsets& structTemplate, size_t size);
#pragma endregion
	};
}