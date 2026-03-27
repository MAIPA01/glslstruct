/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_BASE_LAYOUT_HPP_
	#define _GLSL_STRUCT_BASE_LAYOUT_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_CXX17
_GLSL_STRUCT_ERROR("This is only available for c++17 and greater!");
	#else

		#include <glslstruct/types.hpp>

		#include <glslstruct/layout_traits/layout_traits_concept.hpp>
		#include <glslstruct/type_containers/array_type.hpp>
		#include <glslstruct/type_containers/mat_type.hpp>
		#include <glslstruct/type_containers/scalar_type.hpp>
		#include <glslstruct/type_containers/struct_type.hpp>
		#include <glslstruct/type_containers/vec_type.hpp>
		#include <glslstruct/type_visitors/is_of_type_visitor.hpp>
		#include <glslstruct/utils/assert.hpp>
		#include <glslstruct/utils/ValueType.hpp>
		#include <glslstruct/var_data/var_data.hpp>
		#include <glslstruct/variable/glsl_variable.hpp>

namespace glslstruct {
		#if _GLSL_STRUCT_HAS_CXX20
	template<layout_traits Traits>
		#else
	template<class Traits, std::enable_if_t<is_layout_traits_v<Traits>, bool> >
		#endif
	class base_layout {
	public:
		using traits_type = Traits;

	private:
		friend struct std::hash<base_layout>;

		std::unordered_map<std::string, var_data> _values;
		size_t _currentOffset = 0;
		size_t _maxAlignment  = 0;

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::string _get_array_elem_name(std::string_view arrayName,
		  size_t elemIdx) {
			return fmt::format("{}[{}]", arrayName, elemIdx);
		}

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::string _get_struct_elem_name(std::string_view structName,
		  std::string_view elemName) {
			return fmt::format("{}.{}", structName, elemName);
		}

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t _ceil_to_nearest_multiple(size_t valueToRoundUp,
		  const size_t multipleValue) noexcept {
				if (const size_t modulo = valueToRoundUp % multipleValue; modulo != 0) {
					valueToRoundUp += multipleValue - modulo;
				}
			return valueToRoundUp;
		}

		#if _GLSL_STRUCT_HAS_TYPES
		var_data& _set_variable(std::string_view name, const size_t offset, const base_type_handle& type,
		  const size_t padding = 0) {
			return _values.emplace(name, var_data(offset, type, padding)).first->second;
		}
		#else
		var_data& _set_variable(std::string_view name, const size_t offset, const size_t size, const size_t padding = 0) {
			return _values.emplace(name, var_data(offset, size, padding)).first->second;
		}
		#endif

		void _remove_variable(const std::string_view name) {
				if (!contains(name)) { return; }

			_values.erase(name.data());
		}

		_GLSL_STRUCT_CONSTEXPR17 void _update_max_alignment(const size_t baseAlignment) noexcept {
			_maxAlignment = std::max(baseAlignment, _maxAlignment);
		}

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t _calculate_alignment_offset(const size_t currentOffset,
		  const size_t baseAlignment) noexcept {
			// CALCULATE ALIGNMENT
			const size_t alignmentOffset = _ceil_to_nearest_multiple(currentOffset, baseAlignment);

				// CHECK IF NOT OVERFLOWED
				if (alignmentOffset < currentOffset) {
					glsl_struct_assert(false, "Data overflow would happen!");
					return bad_offset();
				}

			return alignmentOffset;
		}

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t _calculate_padding(const size_t currentOffset,
		  const size_t baseAlignment) noexcept {
			return _calculate_alignment_offset(currentOffset, baseAlignment) - currentOffset;
		}

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 bool _move_current_offset(size_t& currentOffset,
		  const size_t alignmentOffset, const size_t baseOffset) noexcept {
				if (mstd::add_overflow(alignmentOffset, baseOffset, currentOffset)) {
					glsl_struct_assert(false, "Data overflow would happen!");
					return false;
				}
			return true;
		}

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t _add(size_t& currentOffset, const size_t baseAlignment,
		  const size_t baseOffset) noexcept {
			// GET ALIGNMENT OFFSET
			const size_t alignmentOffset = _calculate_alignment_offset(currentOffset, baseAlignment);

				if (alignmentOffset == bad_offset()) { return alignmentOffset; }

			// MOVE CURRENT OFFSET
				if (!_move_current_offset(currentOffset, alignmentOffset, baseOffset)) { return bad_offset(); }

			return alignmentOffset;
		}

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_array(size_t& currentOffset,
		  const size_t arrayBaseAlignment, const size_t elemBaseOffset, const size_t count) noexcept {
			// GET ALIGNMENT OFFSET
			size_t alignmentOffset = _calculate_alignment_offset(currentOffset, arrayBaseAlignment);

				if (alignmentOffset == bad_offset()) { return std::vector<size_t>(); }

			// CALCULATE ARRAY ELEMENTS OFFSETS
			std::vector<size_t> arrayOffsets;
			arrayOffsets.reserve(count);
				for (size_t i = 0, tempOffset = 0; i < count; ++i) {
					tempOffset = _add(alignmentOffset, arrayBaseAlignment, elemBaseOffset);
						if (tempOffset == bad_offset()) { return std::vector<size_t>(); }

					arrayOffsets.push_back(tempOffset);
				}

			// MOVE CURRENT OFFSET
				if (!_move_current_offset(currentOffset, arrayOffsets.back(), elemBaseOffset)) { return std::vector<size_t>(); }

			return arrayOffsets;
		}

		var_data& _add_variable(
		  std::string_view name, const size_t alignmentOffset, const size_t baseAlignment,
		#if _GLSL_STRUCT_HAS_TYPES
		  const base_type_handle& type
		#else
		  size_t baseOffset
		#endif
		) {
			// CHECK VARIABLE NAME
			glsl_struct_assert(!contains(name), "Layout already contains value with name {}", name);

			// UPDATE MAX ALIGNMENT
			_update_max_alignment(baseAlignment);

				// SET VARIABLE
		#if _GLSL_STRUCT_HAS_TYPES
			return _set_variable(name, alignmentOffset, type);
		#else
			return _set_variable(name, alignmentOffset, baseOffset);
		#endif
		}

		var_data& _add_array_variable(
		  const std::string_view name, const std::vector<size_t>& alignmentOffsets, const size_t arrayBaseAlignment,
		#if _GLSL_STRUCT_HAS_TYPES
		  const base_type_handle& elemType, const base_type_handle& arrayType
		#else
		  const size_t elemBaseOffset, const size_t arrayBaseOffset
		#endif
		) {
			// CHECK SIZE
			glsl_struct_assert(!alignmentOffsets.empty(), "Count cannot be 0.");

				for (size_t i = 0; i < alignmentOffsets.size(); ++i) {
		// SET ELEMENT VARIABLE
		#if _GLSL_STRUCT_HAS_TYPES
					var_data& varData =
					  _add_variable(_get_array_elem_name(name, i), alignmentOffsets[i], arrayBaseAlignment, elemType);
						if (i < alignmentOffsets.size() - 1) {
							varData.set_padding(alignmentOffsets[i + 1] - alignmentOffsets[i] - elemType->get_size());
						}
		#else
					var_data& varData =
					  _add_variable(_get_array_elem_name(name, i), alignmentOffsets[i], arrayBaseAlignment, elemBaseOffset);

						if (i < alignmentOffsets.size() - 1) {
							varData.set_padding(alignmentOffsets[i + 1] - alignmentOffsets[i] - elemBaseOffset);
						}
		#endif
				}

		// ADD ARRAY VAR
		#if _GLSL_STRUCT_HAS_TYPES
			return _add_variable(name, alignmentOffsets.front(), arrayBaseAlignment, arrayType);
		#else
			return _add_variable(name, alignmentOffsets.front(), arrayBaseAlignment, arrayBaseOffset);
		#endif
		}

		[[nodiscard]] size_t _add_scalar(std::string_view name, ValueType valueType) {
			// GET BASE OFFSET
			size_t baseOffset	   = get_value_type_size(valueType);

			// GET BASE ALIGNMENT
			size_t baseAlignment   = traits_type::get_scalar_alignment(valueType);

			// GET ALIGNMENT OFFSET
			size_t alignmentOffset = _add(_currentOffset, baseAlignment, baseOffset);

				// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			_add_variable(name, alignmentOffset, baseAlignment, std::make_shared<scalar_type>(valueType, baseOffset));
		#else
			_add_variable(name, alignmentOffset, baseAlignment, baseOffset);
		#endif

			return alignmentOffset;
		}

		[[nodiscard]] std::vector<size_t> _add_scalar_array(std::string_view name, ValueType valueType, size_t count) {
			// GET BASE OFFSET
			size_t baseOffset					 = get_value_type_size(valueType);

			// GET BASE ALIGNMENT
			size_t baseAlignment				 = traits_type::get_scalar_alignment(valueType);

			// GET ARRAY ALIGNMENT
			size_t arrayBaseAlignment			 = traits_type::get_array_alignment(baseAlignment);

			// GET ALIGNMENT OFFSETS
			std::vector<size_t> alignmentOffsets = _add_array(_currentOffset, arrayBaseAlignment, baseOffset, count);

			// GET ARRAY SIZE
			const size_t arraySize				 = alignmentOffsets.back() + baseOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t arrayPadding			 = _calculate_padding(_currentOffset, arrayBaseAlignment);

			// APPLY PADDING TO CURRENT OFFSET
			glsl_struct_assert(_move_current_offset(_currentOffset, alignmentOffsets.back(), arrayBaseAlignment),
			  "Data overflow!");

				// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			const auto scalarType = std::make_shared<scalar_type>(valueType, baseOffset);
			const auto arrayType	= std::make_shared<array_type>(scalarType, count, arraySize);

			_add_array_variable(name, alignmentOffsets, arrayBaseAlignment, scalarType, arrayType)
			  .set_padding(arrayPadding);
		#else
			_add_array_variable(name, alignmentOffset, arrayBaseAlignment, baseOffset, arraySize, count)
			  .set_padding(arrayPadding);
		#endif

			return alignmentOffsets;
		}

		[[nodiscard]] size_t _add_vec(std::string_view name, size_t length, ValueType valueType) {
			glsl_struct_assert(length >= 2 && length <= 4, "vec length should be in range <2, 4>!");

			// GET BASE OFFSET
			size_t baseOffset	   = get_value_type_size(valueType) * length;

			// GET BASE ALIGNMENT
			size_t baseAlignment   = traits_type::get_vec_alignment(valueType, length);

			// GET ALIGNMENT OFFSET
			size_t alignmentOffset = _add(_currentOffset, baseAlignment, baseOffset);

				// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			_add_variable(name, alignmentOffset, baseAlignment, std::make_shared<vec_type>(valueType, length, baseOffset))
			  .set_padding(baseAlignment - baseOffset);
		#else
			_add_variable(name, alignmentOffset, baseAlignment, baseOffset).set_padding(baseAlignment - baseOffset);
		#endif

			return alignmentOffset;
		}

		[[nodiscard]] std::vector<size_t> _add_vec_array(std::string_view name, size_t length, ValueType valueType,
		  size_t count) {
			glsl_struct_assert(length >= 2 && length <= 4, "vec length should be in range <2, 4>!");

			// GET BASE OFFSET
			size_t baseOffset					 = get_value_type_size(valueType) * length;

			// GET BASE ALIGNMENT
			size_t baseAlignment				 = traits_type::get_vec_alignment(valueType, length);

			// GET ARRAY ALIGNMENT
			size_t arrayBaseAlignment			 = traits_type::get_array_alignment(baseAlignment);

			// GET ALIGNMENT OFFSETS
			std::vector<size_t> alignmentOffsets = _add_array(_currentOffset, arrayBaseAlignment, baseOffset, count);

			// GET ARRAY SIZE
			const size_t arraySize				 = alignmentOffsets.back() + baseOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t arrayPadding			 = _calculate_padding(_currentOffset, arrayBaseAlignment);

			// APPLY PADDING TO CURRENT OFFSET
			glsl_struct_assert(_move_current_offset(_currentOffset, alignmentOffsets.back(), arrayBaseAlignment),
			  "Data overflow!");

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			const auto vecType	  = std::make_shared<vec_type>(valueType, length, baseOffset);
			const auto arrayType	  = std::make_shared<array_type>(vecType, count, arraySize);

			var_data& varData = _add_array_variable(name, alignmentOffsets, arrayBaseAlignment, vecType, arrayType);
		#else
			var_data& varData = _add_array_variable(name, alignmentOffset, arrayBaseAlignment, baseOffset, arraySize);
		#endif

			// APPLY PADDING TO VAR DATA
			varData.set_padding(arrayPadding);

			return alignmentOffsets;
		}

		[[nodiscard]] std::vector<size_t> _add_mat(const std::string_view name, size_t columns, size_t rows,
		  ValueType valueType) {
			glsl_struct_assert(columns >= 2 && columns <= 4, "mat columns should be in range <2, 4>!");
			glsl_struct_assert(rows >= 2 && rows <= 4, "mat rows should be in range <2, 4>!");

			// GET BASE OFFSET
			size_t vecBaseOffset				 = get_value_type_size(valueType) * rows;

			// GET BASE ALIGNMENT
			size_t vecBaseAlignment				 = traits_type::get_vec_alignment(valueType, rows);

			// GET MAT ALIGNMENT
			size_t matBaseAlignment				 = traits_type::get_array_alignment(vecBaseAlignment);

			// GET ALIGNMENT OFFSETS
			std::vector<size_t> alignmentOffsets = _add_array(_currentOffset, matBaseAlignment, vecBaseOffset, columns);

			// GET MAT SIZE
			const size_t matSize				 = alignmentOffsets.back() + vecBaseOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t matPadding				 = _calculate_padding(_currentOffset, matBaseAlignment);

			// APPLY PADDING TO CURRENT OFFSET
			glsl_struct_assert(_move_current_offset(_currentOffset, alignmentOffsets.back(), matBaseAlignment),
			  "Data overflow!");

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			const auto vecType = std::make_shared<vec_type>(valueType, rows, vecBaseOffset);
			const auto matType = std::make_shared<mat_type>(valueType, columns, rows, matSize);

			_add_array_variable(name, alignmentOffsets, matBaseAlignment, vecType, matType).set_padding(matPadding);
		#else
			_add_array_variable(name, alignmentOffset, matBaseAlignment, vecBaseOffset, matSize, columns).set_padding(matPadding);
		#endif

			return alignmentOffsets;
		}

		[[nodiscard]] std::vector<std::vector<size_t> > _add_mat_array(std::string_view name, size_t columns, size_t rows,
		  ValueType valueType, size_t count) {
			glsl_struct_assert(columns >= 2 && columns <= 4, "mat columns should be in range <2, 4>!");
			glsl_struct_assert(rows >= 2 && rows <= 4, "mat rows should be in range <2, 4>!");

			// GET BASE OFFSET
			size_t vecBaseOffset				 = get_value_type_size(valueType) * rows;

			// GET BASE ALIGNMENT
			size_t vecBaseAlignment				 = traits_type::get_vec_alignment(valueType, rows);

			// GET ARRAY ALIGNMENT
			size_t matBaseAlignment				 = traits_type::get_array_alignment(vecBaseAlignment);

			// GET MAT SIZE
			size_t tempOffset					 = 0;
			std::vector<size_t> alignmentOffsets = _add_array(tempOffset, matBaseAlignment, vecBaseOffset, columns);
			const size_t matSize				 = alignmentOffsets.back() + vecBaseOffset - alignmentOffsets.front();

		#if _GLSL_STRUCT_HAS_TYPES
			const auto vecType = std::make_shared<vec_type>(valueType, rows, vecBaseOffset);
			const auto matType = std::make_shared<mat_type>(valueType, columns, rows, matSize);
		#endif

			std::vector<std::vector<size_t> > alignmentOffsetsPerMat;
			alignmentOffsetsPerMat.reserve(count);

			std::vector<size_t> matsOffsets;
			matsOffsets.reserve(count);
			size_t matPadding = 0;
				for (size_t i = 0; i < count; ++i) {
					// GET VEC ALIGNMENT OFFSETS
					alignmentOffsets = _add_array(_currentOffset, matBaseAlignment, vecBaseOffset, columns);

					// GET ARRAY PADDING
					matPadding		 = _calculate_padding(_currentOffset, matBaseAlignment);

					// APPLY PADDING TO CURRENT OFFSET
					glsl_struct_assert(_move_current_offset(_currentOffset, alignmentOffsets.back(), matBaseAlignment),
					  "Data overflow!");

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
					var_data& varData = _add_array_variable(_get_array_elem_name(name, i), alignmentOffsets, matBaseAlignment,
					  vecType, matType);
		#else
					var_data& varData = _add_array_variable(_get_array_elem_name(name, i), alignmentOffsets, matBaseAlignment,
					  vecBaseOffset, matSize);
		#endif

						if (i < count - 1) { varData.set_padding(matPadding); }

					matsOffsets.push_back(alignmentOffsets.front());

					alignmentOffsetsPerMat.push_back(alignmentOffsets);
				}

			// GET ARRAY SIZE
			const size_t arraySize = matsOffsets.back() + matSize - matsOffsets.front();

			// APPLY PADDING TO CURRENT OFFSET
			glsl_struct_assert(_move_current_offset(_currentOffset, matsOffsets.back(), matSize + matPadding), "Data overflow!");

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			const auto arrayType = std::make_shared<array_type>(matType, count, arraySize);

			_add_variable(name, matsOffsets.front(), matBaseAlignment, arrayType).set_padding(matPadding);
		#else
			_add_variable(name, matsOffsets.front(), matBaseAlignment, arraySize).set_padding(matPadding);
		#endif

			return alignmentOffsetsPerMat;
		}

		[[nodiscard]] size_t _add_struct(std::string_view name, size_t baseAlignment, size_t baseOffset,
		  const std::unordered_map<std::string, var_data>& values) {
			// GET ALIGNMENT OFFSET
			const size_t alignmentOffset = _add(_currentOffset, baseAlignment, baseOffset);

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			var_data& varData =
			  _add_variable(name, alignmentOffset, baseAlignment, std::make_shared<struct_type>(values, baseOffset));
		#else
			var_data& varData = _add_variable(name, alignmentOffset, baseAlignment, baseOffset);
		#endif

			// ADD STRUCTURE VARIABLES DATA
				for (const auto& [value_name, data] : values) {
		#if _GLSL_STRUCT_HAS_TYPES
					var_data& localVarData = _add_variable(_get_struct_elem_name(name, value_name),
					  alignmentOffset + data.get_offset(), 0, data.get_type());
		#else
					var_data& localVarData = _add_variable(_get_struct_elem_name(name, value_name),
					  alignmentOffset + data.get_offset(), 0, data.get_size());
		#endif

					localVarData.set_padding(data.get_padding());
				}

			// GET ARRAY PADDING
			const size_t structPadding = _calculate_padding(_currentOffset, baseAlignment);

			// APPLY PADDING TO CURRENT OFFSET
			glsl_struct_assert(_move_current_offset(_currentOffset, alignmentOffset, baseOffset + structPadding),
			  "Data overflow!");

			// APPLY PADDING TO VAR DATA
			varData.set_padding(structPadding);

			return alignmentOffset;
		}

		[[nodiscard]] std::vector<size_t> _add_struct_array(std::string_view name, size_t elemBaseAlignment,
		  size_t elemBaseOffset, const std::unordered_map<std::string, var_data>& values, size_t count) {
			// GET ARRAY ALIGNMENT
			size_t arrayBaseAlignment			 = traits_type::get_array_alignment(elemBaseAlignment);

			// GET ALIGNMENT OFFSETS
			std::vector<size_t> alignmentOffsets = _add_array(_currentOffset, arrayBaseAlignment, elemBaseOffset, count);

			// GET ARRAY SIZE
			const size_t arraySize				 = alignmentOffsets.back() + elemBaseOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t arrayPadding			 = _calculate_padding(_currentOffset, arrayBaseAlignment);

			// APPLY PADDING TO CURRENT OFFSET
			glsl_struct_assert(_move_current_offset(_currentOffset, _currentOffset, arrayPadding),
			  "Data overflow!");

				for (size_t i = 0; i < count; ++i) {
					std::string arrayElemName = _get_array_elem_name(name, i);

						// ADD STRUCTURE VARIABLES DATA
						for (const auto& [value_name, data] : values) {
		#if _GLSL_STRUCT_HAS_TYPES
							var_data& localVarData = _add_variable(_get_struct_elem_name(arrayElemName, value_name),
							  alignmentOffsets[i] + data.get_offset(), 0, data.get_type());
		#else
							var_data& localVarData = _add_variable(_get_struct_elem_name(name, value_name),
							  alignmentOffsets[i] + data.get_offset(), 0, data.get_size());
		#endif

							localVarData.set_padding(data.get_padding());
						}
				}

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			const auto structType = std::make_shared<struct_type>(values, elemBaseOffset);
			const auto arrayType	= std::make_shared<array_type>(structType, count, arraySize);

			_add_array_variable(name, alignmentOffsets, arrayBaseAlignment, structType, arrayType)
			  .set_padding(arrayPadding);
		#else
			_add_array_variable(name, alignmentOffset, arrayBaseAlignment, elemBaseOffset, arraySize)
			  .set_padding(arrayPadding);
		#endif

			return alignmentOffsets;
		}

		template<class T, class... Ts, size_t num, size_t... nums>
		_GLSL_STRUCT_CONSTEXPR17 void _add_variables(const glsl_variable<T, num>& var,
		  const glsl_variable<Ts, nums>&... vars) noexcept {
				if _GLSL_STRUCT_CONSTEXPR17 (var.is_layout) {
						if _GLSL_STRUCT_CONSTEXPR17 (num == 0) { add(var.var_name, var.layout); }
						else { add(var.var_name, var.layout, num); }
				}
				else {
						if _GLSL_STRUCT_CONSTEXPR17 (num == 0) { add<T>(var.var_name); }
						else { add<T>(var.var_name, num); }
				}
				if _GLSL_STRUCT_CONSTEXPR17 (sizeof...(Ts) > 0 && sizeof...(nums) > 0) { _add_variables(vars...); }
		}

	public:
		base_layout() noexcept = default;

		template<class... Ts, size_t... nums>
		explicit base_layout(const glsl_variable<Ts, nums>&... vars) noexcept {
			_add_variables(vars...);
		}

		base_layout(const base_layout& other) noexcept			  = default;

		base_layout(base_layout&& other) noexcept				  = default;

		~base_layout() noexcept									  = default;

		base_layout& operator=(const base_layout& other) noexcept = default;

		base_layout& operator=(base_layout&& other) noexcept	  = default;

		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t bad_offset() noexcept { return std::numeric_limits<size_t>::max(); }

		[[nodiscard]] bool contains(const std::string_view name) const noexcept {
		#if _GLSL_STRUCT_HAS_CXX20
			return _values.contains(name.data());
		#else
			return _values.find(name.data()) != _values.end();
		#endif
		}

		[[nodiscard]] const var_data& get(const std::string_view name) const noexcept {
			glsl_struct_assert(contains(name), "The variable name doesn't exists!");
			return _values.at(name.data());
		}

		[[nodiscard]] size_t get_offset(const std::string_view name) const noexcept { return get(name).get_offset(); }

		[[nodiscard]] std::vector<size_t> get_array_offsets(const std::string_view name) const noexcept {
			const base_type_handle& varType = get_type(name);

			size_t arraySize;
				if (is_of_type<mat_type>(varType)) { arraySize = static_type_cast<mat_type>(varType)->get_array_count(); }
				else if (is_of_type<array_type>(varType)) { arraySize = static_type_cast<array_type>(varType)->get_count(); }
				else { return { _values.at(name.data()).get_offset() }; }

			std::vector<size_t> values;
			values.reserve(arraySize);
				for (size_t i = 0; i != arraySize; ++i) {
					values.push_back(_values.at(_get_array_elem_name(name, i)).get_offset());
				}

			return values;
		}

		#if _GLSL_STRUCT_HAS_TYPES
		[[nodiscard]] const base_type_handle& get_type(const std::string_view name) const noexcept {
			return get(name).get_type();
		}
			#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_type Type>
			#else
		template<class Type, std::enable_if_t<utils::is_glsl_type_v<Type>, bool> = true>
			#endif
		[[nodiscard]] std::shared_ptr<Type> get_type(const std::string_view name) const noexcept {
			return dynamic_type_cast<Type>(get_type(name));
		}
		#endif

		[[nodiscard]] size_t get_total_size(const std::string_view name) const noexcept { return get(name).get_total_size(); }

		[[nodiscard]] size_t get_size(const std::string_view name) const noexcept { return get(name).get_size(); }

		[[nodiscard]] size_t get_padding(const std::string_view name) const noexcept { return get(name).get_padding(); }

		[[nodiscard]] std::vector<std::string> get_names() const noexcept {
			std::vector<std::string> names;
			names.reserve(_values.size());

				for (const auto& [name, data] : _values) { names.push_back(name); }

			return names;
		}

		[[nodiscard]] size_t base_alignment() const noexcept { return traits_type::get_struct_alignment(_maxAlignment); }

		[[nodiscard]] size_t size() const noexcept { return traits_type::get_struct_size(_currentOffset); }

		[[nodiscard]] size_t padding() const noexcept { return size() - _currentOffset; }

		void clear() noexcept {
			_currentOffset = 0;
			_maxAlignment  = 0;
			_values.clear();
		}

		#pragma region ADD_SCALAR
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		[[nodiscard]] size_t add(const std::string_view name) {
			static ValueType valueType = get_scalar_value_type<T>();
			return _add_scalar(name, valueType);
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>
		#endif
		[[nodiscard]] std::vector<size_t> add(const std::string_view name, const size_t count) {
			static ValueType valueType = get_scalar_value_type<T>();
			return _add_scalar_array(name, valueType, count);
		}

		#pragma endregion

		#pragma region ADD_VEC
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		[[nodiscard]] size_t add(const std::string_view name) {
			static ValueType valueType = get_vec_value_type<V>();
			static size_t length		= get_vec_length<V>();
			return _add_vec(name, length, valueType);
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		[[nodiscard]] std::vector<size_t> add(const std::string_view name, const size_t count) {
			static ValueType valueType = get_vec_value_type<V>();
			static size_t length		= get_vec_length<V>();
			return _add_vec_array(name, length, valueType, count);
		}

		#pragma endregion

		#pragma region ADD_MAT
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		[[nodiscard]] std::vector<size_t> add(const std::string_view name) {
			static ValueType valueType = get_mat_value_type<M>();
			static size_t columns		= get_mat_columns<M>();
			static size_t rows			= get_mat_rows<M>();
			return _add_mat(name, columns, rows, valueType);
		}

		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		[[nodiscard]] std::vector<std::vector<size_t> > add(const std::string_view name, const size_t count) {
			static ValueType valueType = get_mat_value_type<M>();
			static size_t columns		= get_mat_columns<M>();
			static size_t rows			= get_mat_rows<M>();
			return _add_mat_array(name, columns, rows, valueType, count);
		}

		#pragma endregion

		#pragma region ADD_STRUCT

		[[nodiscard]] size_t add(const std::string_view name, const base_layout& layout) {
			return _add_struct(name, layout.base_alignment(), layout._currentOffset, layout._values);
		}

		[[nodiscard]] std::vector<size_t> add(const std::string_view name, const base_layout& layout, const size_t count) {
			return _add_struct_array(name, layout.base_alignment(), layout._currentOffset, layout._values, count);
		}

		#pragma endregion

		[[nodiscard]] bool operator==(const base_layout& other) const noexcept {
			return _currentOffset == other._currentOffset && _maxAlignment == other._maxAlignment && _values == other._values;
		}

		#if _GLSL_STRUCT_HAS_CXX20
		[[nodiscard]] bool operator!=(const base_layout& other) const noexcept = default;
		#else
		[[nodiscard]] bool operator!=(const base_layout& other) const noexcept { return !(*this == other); }
		#endif
	};
} // namespace glslstruct

template<class T>
struct std::hash<glslstruct::base_layout<T> > {
	[[nodiscard]] size_t operator()(const glslstruct::base_layout<T>& layout) const noexcept {
		size_t seed = mstd::hash_combine(layout._currentOffset, layout._maxAlignment);
			for (const auto& [name, data] : layout._values) { mstd::hash_append(seed, name, data); }
		return seed;
	}
};
	#endif
#endif