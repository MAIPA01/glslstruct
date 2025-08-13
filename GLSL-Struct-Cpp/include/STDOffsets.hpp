#pragma once
#include <templates.hpp>
#include <ValueTypes.hpp>
#include <mstd/macros.hpp>

namespace glsl {
	class STDOffsets {
	protected:
		using offsets_map = std::unordered_map<size_t, size_t>;
		using names_map = std::unordered_map<size_t, std::string>;
		using types_map = std::unordered_map<size_t, const ValueType*>;

		size_t _currentOffset = 0;
		size_t _maxAligement = 0;

		offsets_map _offsets;
		names_map _names;
		types_map _types;

		static std::hash<std::string> _hasher;

		static const char* const _arrayElemFormat;
		static const char* const _subElemFormat;

		bool _CheckVariable(const std::string& name) const;

		void _SetVariable(const std::string& name, size_t offset, const ValueType* type);

		virtual size_t _Add(const std::string& name, size_t baseAligement, size_t baseOffset, const ValueType* type);
		virtual std::vector<size_t> _AddArray(const std::string& name, size_t arraySize, size_t baseAligement, size_t baseOffset, const ValueType* type);

		virtual size_t _AddScalar(const std::string& name, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type);
		virtual std::vector<size_t> _AddScalarArray(const std::string& name, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type, size_t arraySize);

		virtual size_t _AddVector(const std::string& name, size_t length, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type);
		virtual std::vector<size_t> _AddVectorArray(const std::string& name, size_t length, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type, size_t arraySize);

		virtual size_t _AddMatrix(const std::string& name, size_t columns, size_t rows, bool column_major, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type);
		virtual std::vector<size_t> _AddMatrixArray(const std::string& name, size_t columns, size_t rows, bool column_major, size_t baseAligement, size_t baseOffset, const VALUE_TYPE& type, size_t arraySize);

		virtual size_t _AddStruct(const std::string& name, size_t baseAligement, size_t baseOffset, const offsets_map& offsets, const names_map& names, const types_map& types);
		virtual std::vector<size_t> _AddStructArray(const std::string& name, size_t baseAligement, size_t baseOffset, const offsets_map& offsets, const names_map& names, const types_map& types, size_t arraySize);

		STDOffsets() = default;
		STDOffsets(STDOffsets& stdOff);
		STDOffsets(const STDOffsets& stdOff);
		STDOffsets(STDOffsets&& stdOff);
		virtual ~STDOffsets();

		STDOffsets& operator=(STDOffsets& stdOff);
		STDOffsets& operator=(const STDOffsets& stdOff);
		STDOffsets& operator=(STDOffsets&& stdOff);

	public:

		DECLARE_CLONE_FUNC(STDOffsets)

		bool Contains(const std::string& name) const;

		virtual size_t Get(const std::string& name) const;
		virtual std::vector<size_t> GetArray(const std::string& name) const;

		const ValueType* GetType(const std::string& name) const;
		std::vector<std::string> GetNames() const;

		virtual size_t GetBaseAligement() const;
		virtual size_t GetSize() const;

		virtual void Clear();
	};
}