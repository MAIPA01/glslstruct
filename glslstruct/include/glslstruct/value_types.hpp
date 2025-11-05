#pragma once
#define USE_ENUMS_MACROS
#define USE_CLONE_FUNC_MACROS
#include <mstd/macros.hpp>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <glslstruct/value_data.hpp>

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
		base_type(const base_type& other) = default;
		virtual ~base_type() = default;

		virtual bool visit_equal(const base_type*& other) const noexcept = 0;
		virtual bool visit_equal(const scalar_type*& other) const noexcept = 0;
		virtual bool visit_equal(const vec_type*& other) const noexcept = 0;
		virtual bool visit_equal(const mat_type*& other) const noexcept = 0;
		virtual bool visit_equal(const struct_type*& other) const noexcept = 0;
		virtual bool visit_equal(const array_type*& other) const noexcept = 0;

		[[nodiscard]] virtual std::string toString() const noexcept = 0;

		virtual bool operator==(const base_type*& other) const noexcept;
	};

	[[nodiscard]] static std::string to_string(const base_type*& value) noexcept;

	template<class _Derived>
	class value_type : base_type {
	public:
		bool visit_equal(const base_type*& other) const noexcept override {
			_Derived* this_derived = static_cast<_Derived*>(this);
			
		}

		template<class _other>
		bool visit_equal(const _other*& other) {
			if constexpr (!std::is_same_v<_other, _Derived>) {
				return false;
			}
			else {
				_Derived* this_derived = static_cast<_Derived*>(this);
				return *this_derived == *other;
			}
		}
	};

	class scalar_type : public base_type {
	private:
		ValueType _type = ValueType::Other;

	public:
		scalar_type(const ValueType& type);
		scalar_type(const scalar_type& other) = default;
		virtual ~scalar_type() = default;

		[[nodiscard]] ValueType getType() const noexcept;

		[[nodiscard]] virtual std::string toString() const noexcept override;
	};

	[[nodiscard]] static std::string to_string(const scalar_type*& value) noexcept;

	class vec_type : public base_type {
	private:
		ValueType _type = ValueType::Other;
		size_t _length = 0;

	public:
		vec_type(const ValueType& type, const size_t& length);
		vec_type(const vec_type& other) = default;
		virtual ~vec_type() = default;

		[[nodiscard]] ValueType getType() const noexcept;
		[[nodiscard]] size_t getLength() const noexcept;

		[[nodiscard]] virtual std::string toString() const noexcept override;
	};

	[[nodiscard]] static std::string to_string(const vec_type*& value) noexcept;

	class mat_type : public base_type {
	private:
		ValueType _type = ValueType::Other;
		size_t _cols = 0;
		size_t _rows = 0;

	public:
		mat_type(const ValueType& type, const size_t& cols, const size_t& rows);
		mat_type(const mat_type& other) = default;
		virtual ~mat_type() = default;

		[[nodiscard]] ValueType getType() const noexcept;
		[[nodiscard]] size_t getRows() const noexcept;
		[[nodiscard]] size_t getCols() const noexcept;

		[[nodiscard]] virtual std::string toString() const noexcept override;
	};

	[[nodiscard]] static std::string to_string(const mat_type*& value) noexcept;

	class struct_type : public base_type {
	private:
		using values_map = std::unordered_map<std::string, value_data>;

		values_map _values;

	public:
		struct_type(const values_map& values);
		struct_type(const struct_type& other);
		virtual ~struct_type();

		[[nodiscard]] values_map getValues() const noexcept;

		[[nodiscard]] virtual std::string toString() const noexcept override;
	};

	[[nodiscard]] static std::string to_string(const struct_type*& value) noexcept;

	class array_type : public base_type {
	private:
		const base_type* _type = nullptr;
		size_t _length = 0;

	public:
		array_type(const base_type* type, const size_t& length);
		array_type(const array_type& other);
		virtual ~array_type();

		[[nodiscard]] const base_type* getType() const noexcept;
		[[nodiscard]] size_t getLength() const noexcept;

		[[nodiscard]] virtual std::string toString() const noexcept override;
	};

	[[nodiscard]] static std::string to_string(const array_type*& value) noexcept;
}