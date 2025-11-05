#pragma once
#define USE_ENUMS_MACROS
#define USE_CLONE_FUNC_MACROS
#include <mstd/macros.hpp>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace glslstruct {
	ENUM_CLASS_BASE_STRING(ValueType, uint8_t, Other, "other", Bool, "bool", Int, "int", Uint, "uint", Float, "float", Double, "double");

	template<class T>
	[[nodicard]] static constexpr ValueType getValueType() noexcept {
		if constexpr (std::is_same_v<T, bool>) {
			return ValueType::Bool;
		}
		else if constexpr (std::is_same_v<T, int>) {
			return ValueType::Int;
		}
		else if constexpr (std::is_same_v<T, unsigned int>) {
			return ValueType::Uint;
		}
		else if constexpr (std::is_same_v<T, float>) {
			return ValueType::Float;
		}
		else if constexpr (std::is_same_v<T, double>) {
			return ValueType::Double;
		}
		else {
			return ValueType::Other;
		}
	}

	[[nodiscard]] static std::string vecTypeToString(const ValueType& type) noexcept;

	[[nodiscard]] static std::string matTypeToString(const ValueType& type) noexcept;

	class base_type {
	protected:
		base_type() = default;
	public:
		virtual ~base_type() = default;

		DECLARE_ABSTRACT_CLONE_FUNC(base_type)

		[[nodiscard]] virtual std::string toString() const noexcept = 0;
	};

	[[nodiscard]] static std::string to_string(const base_type*& value) noexcept;

	class scalar_type : public base_type {
	private:
		ValueType _type = ValueType::Other;

	protected:
		scalar_type() = default;
	public:
		scalar_type(const ValueType& type);
		virtual ~scalar_type() = default;

		DECLARE_OVERRIDED_CLONE_FUNC(scalar_type)

		[[nodiscard]] ValueType getType() const noexcept;

		[[nodiscard]] virtual std::string toString() const noexcept override;
	};

	[[nodiscard]] static std::string to_string(const scalar_type*& value) noexcept;

	class vec_type : public base_type {
	private:
		ValueType _type = ValueType::Other;
		size_t _length = 0;

	protected:
		vec_type() = default;

	public:
		vec_type(const ValueType& type, const size_t& length);
		virtual ~vec_type() = default;

		DECLARE_OVERRIDED_CLONE_FUNC(vec_type)

		[[nodiscard]] ValueType getType() const noexcept;
		[[nodiscard]] size_t getLength() const noexcept;

		virtual std::string toString() const override;
	};

	[[nodiscard]] static std::string to_string(const vec_type*& value) noexcept;

	class mat_type : public base_type {
	private:
		ValueType _type = ValueType::Other;
		size_t _cols = 0;
		size_t _rows = 0;

	protected:
		mat_type() = default;
	public:
		mat_type(const ValueType& type, const size_t& cols, const size_t& rows);
		virtual ~mat_type() = default;

		DECLARE_OVERRIDED_CLONE_FUNC(mat_type)

		[[nodiscard]] ValueType getType() const noexcept;
		[[nodiscard]] size_t getRows() const noexcept;
		[[nodiscard]] size_t getCols() const noexcept;

		virtual std::string toString() const override;
	};

	[[nodiscard]] static std::string to_string(const mat_type*& value) noexcept;

	class struct_type : public base_type {
	private:
		using offsets_map = std::unordered_map<size_t, size_t>;
		using names_map = std::unordered_map<size_t, std::string>;
		using types_map = std::unordered_map<size_t, const base_type*>;

		offsets_map _offsets;
		names_map _names;
		types_map _types;

	protected:
		struct_type() = default;
	public:
		struct_type(const offsets_map& offsets, const names_map& names, const types_map& types);
		virtual ~struct_type();

		DECLARE_OVERRIDED_CLONE_FUNC(struct_type)

		[[nodiscard]] offsets_map getOffsets() const noexcept;
		[[nodiscard]] names_map getNames() const noexcept;
		[[nodiscard]] types_map getTypes() const noexcept;

		virtual std::string toString() const override;
	};

	[[nodiscard]] static std::string to_string(const struct_type*& value) noexcept;

	class array_type : public base_type {
	private:
		const base_type* _type = nullptr;
		size_t _length = 0;

	protected:
		array_type() = default;
	public:
		array_type(const base_type* type, const size_t& length);
		virtual ~array_type();

		DECLARE_OVERRIDED_CLONE_FUNC(array_type)

		[[nodiscard]] const base_type* getType() const noexcept;
		[[nodiscard]] size_t getLength() const noexcept;

		virtual std::string toString() const override;
	};

	[[nodiscard]] static std::string to_string(const array_type*& value) noexcept;
}