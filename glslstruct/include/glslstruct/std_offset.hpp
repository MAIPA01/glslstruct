#pragma once
#include <glslstruct/templates.hpp>
#include <glslstruct/value_types.hpp>
#include <glslstruct/value_data.hpp>
#include <mstd/macros.hpp>
#include <unordered_map>

namespace std {
	template<>
	struct hash<glslstruct::std_offset> {
		size_t operator()(const glslstruct::std_offset& stdOff);
	};
}

namespace glslstruct {
	class std_offset {
	protected:
		friend struct std::hash<std_offset>;

		using values_map = std::unordered_map<std::string, value_data>;

		size_t _currentOffset = 0;
		size_t _maxAligement = 0;

		values_map _values;

		static inline const char* const _arrayElemFormat = "{}[{}]";
		static inline const char* const _subElemFormat = "{}.{}";

		void _setVariable(const std::string& name, size_t offset, const base_type* type);

		virtual size_t _add(const std::string& name, size_t baseAligement, size_t baseOffset, const base_type* type);
		virtual std::vector<size_t> _addArray(const std::string& name, size_t arraySize, size_t baseAligement,
			size_t baseOffset, const base_type* type);

		virtual size_t _addScalar(const std::string& name, size_t baseAligement, size_t baseOffset, const ValueType& type);
		virtual std::vector<size_t> _addScalarArray(const std::string& name, size_t baseAligement, size_t baseOffset,
			const ValueType& type, size_t arraySize);

		virtual size_t _addVector(const std::string& name, size_t length, size_t baseAligement, size_t baseOffset, const ValueType& type);
		virtual std::vector<size_t> _addVectorArray(const std::string& name, size_t length, size_t baseAligement, size_t baseOffset,
			const ValueType& type, size_t arraySize);

		virtual size_t _addMatrix(const std::string& name, size_t columns, size_t rows, bool column_major, size_t baseAligement,
			size_t baseOffset, const ValueType& type);
		virtual std::vector<size_t> _addMatrixArray(const std::string& name, size_t columns, size_t rows, bool column_major,
			size_t baseAligement, size_t baseOffset, const ValueType& type, size_t arraySize);

		virtual size_t _addStruct(const std::string& name, size_t baseAligement, size_t baseOffset, const values_map& values);
		virtual std::vector<size_t> _addStructArray(const std::string& name, size_t baseAligement, size_t baseOffset, 
			const values_map& values, size_t arraySize);

		void _cloneFrom(const std_offset& stdOff) noexcept;

		std_offset() = default;
		std_offset(std_offset& stdOff);
		std_offset(const std_offset& stdOff);
		std_offset(std_offset&& stdOff) noexcept;
		virtual ~std_offset();

		std_offset& operator=(std_offset& stdOff);
		std_offset& operator=(const std_offset& stdOff);
		std_offset& operator=(std_offset&& stdOff) noexcept;

	public:

		[[nodiscard]] std_offset* clone() const noexcept;

		[[nodiscard]] bool contains(const std::string& name) const;

		[[nodiscard]] virtual size_t get(const std::string& name) const;
		[[nodiscard]] virtual std::vector<size_t> getArray(const std::string& name) const;

		[[nodiscard]] const base_type* getType(const std::string& name) const;
		[[nodiscard]] std::vector<std::string> getNames() const;

		[[nodiscard]] virtual size_t baseAligement() const;
		[[nodiscard]] virtual size_t size() const;

		virtual void clear();

		bool operator==(const std_offset& stdOff) const;
		bool operator!=(const std_offset& stdOff) const;
	};
}