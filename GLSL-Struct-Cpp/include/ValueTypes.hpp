#pragma once
#define USE_ENUMS_MACROS
#define USE_CLONE_FUNC_MACROS
#include <mstd/macros.hpp>
#include <string>

namespace glsl {
	ENUM_CLASS_BASE_STRING(VALUE_TYPE, uint8_t, OTHER, "other", BOOL, "bool", INT, "int", UINT, "uint", FLOAT, "float", DOUBLE, "double");

	template<class T>
	static VALUE_TYPE GetValueType() {
		if constexpr (std::is_same_v<T, bool>) {
			return VALUE_TYPE::BOOL;
		}
		else if constexpr (std::is_same_v<T, int>) {
			return VALUE_TYPE::INT;
		}
		else if constexpr (std::is_same_v<T, unsigned int>) {
			return VALUE_TYPE::UINT;
		}
		else if constexpr (std::is_same_v<T, float>) {
			return VALUE_TYPE::FLOAT;
		}
		else if constexpr (std::is_same_v<T, double>) {
			return VALUE_TYPE::DOUBLE;
		}
		else {
			return VALUE_TYPE::OTHER;
		}
	}

	static std::string GetVecType(const VALUE_TYPE& type);

	static std::string GetMatType(const VALUE_TYPE& type);

	class ValueType {
	protected:
		ValueType() = default;
	public:
		virtual ~ValueType() = default;

		DECLARE_ABSTRACT_CLONE_FUNC(ValueType)

		virtual std::string to_string() const = 0;
	};

	static std::string to_string(const ValueType*& value);

	class ScalarType : public ValueType {
	private:
		VALUE_TYPE _type = VALUE_TYPE::OTHER;

	protected:
		ScalarType() = default;
	public:
		ScalarType(const VALUE_TYPE& type);
		virtual ~ScalarType() = default;

		DECLARE_OVERRIDED_CLONE_FUNC(ScalarType)

		VALUE_TYPE GetType() const;

		virtual std::string to_string() const override;
	};

	static std::string to_string(const ScalarType& value);

	class VecType : public ValueType {
	private:
		VALUE_TYPE _type = VALUE_TYPE::OTHER;
		size_t _length = 0;

	protected:
		VecType() = default;

	public:
		VecType(const VALUE_TYPE& type, const size_t& length);
		virtual ~VecType() = default;

		DECLARE_OVERRIDED_CLONE_FUNC(VecType)

		VALUE_TYPE GetType() const;
		size_t GetLength() const;

		virtual std::string to_string() const override;
	};

	static std::string to_string(const VecType& value);

	class MatType : public ValueType {
	private:
		VALUE_TYPE _type = VALUE_TYPE::OTHER;
		size_t _cols = 0;
		size_t _rows = 0;

	protected:
		MatType() = default;
	public:
		MatType(const VALUE_TYPE& type, const size_t& cols, const size_t& rows);
		virtual ~MatType() = default;

		DECLARE_OVERRIDED_CLONE_FUNC(MatType)

		VALUE_TYPE GetType() const;
		size_t GetRows() const;
		size_t GetCols() const;

		virtual std::string to_string() const override;
	};

	static std::string to_string(const MatType& value);

	class StructType : public ValueType {
	private:
		using offsets_map = std::unordered_map<size_t, size_t>;
		using names_map = std::unordered_map<size_t, std::string>;
		using types_map = std::unordered_map<size_t, const ValueType*>;

		offsets_map _offsets;
		names_map _names;
		types_map _types;

	protected:
		StructType() = default;
	public:
		StructType(const offsets_map& offsets, const names_map& names, const types_map& types);
		virtual ~StructType();

		DECLARE_OVERRIDED_CLONE_FUNC(StructType)

		offsets_map GetOffsets() const;
		names_map GetNames() const;
		types_map GetTypes() const;

		virtual std::string to_string() const override;
	};

	static std::string to_string(const StructType& value);

	class ArrayType : public ValueType {
	private:
		const ValueType* _type = nullptr;
		size_t _length = 0;

	protected:
		ArrayType() = default;
	public:
		ArrayType(const ValueType* type, const size_t& length);
		virtual ~ArrayType();

		DECLARE_OVERRIDED_CLONE_FUNC(ArrayType)

		const ValueType* GetType() const;
		size_t GetLength() const;

		virtual std::string to_string() const override;
	};

	static std::string to_string(const ArrayType& value);
}