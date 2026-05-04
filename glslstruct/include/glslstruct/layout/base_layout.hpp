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

		#include <glslstruct/layout/traits/concept.hpp>
		#if _GLSL_STRUCT_HAS_TYPES
			#include <glslstruct/type/containers/array_type.hpp>
			#include <glslstruct/type/containers/mat_type.hpp>
			#include <glslstruct/type/containers/scalar_type.hpp>
			#include <glslstruct/type/containers/struct_type.hpp>
			#include <glslstruct/type/containers/vec_type.hpp>
			#include <glslstruct/type/visitors/is_of_type_visitor.hpp>
		#endif
		#include <glslstruct/utils/assert.hpp>
		#include <glslstruct/utils/functions.hpp>
		#include <glslstruct/utils/hash.hpp>
		#include <glslstruct/utils/ValueType.hpp>
		#include <glslstruct/var_data/var_data.hpp>
		#include <glslstruct/variable/glsl_variable.hpp>

namespace glslstruct {
	namespace utils {
		/**
		 * @brief Context container for layout
		 * @ingroup utils
		 * @tparam Traits layout traits type
		 */
		template<class Traits>
		struct layout_with_context {
			/// @brief context type
			using context_type = _GLSL_STRUCT_TYPENAME17 Traits::context_type;

		protected:
			/// @brief context value
			context_type _context;

		public:
				/// @brief default constructor
		#if !_GLSL_STRUCT_HAS_CXX20
			template<class C = context_type,
			  std::enable_if_t<std::is_default_constructible_v<context_type> && std::is_same_v<C, context_type>, bool> = true>
		#endif
			_GLSL_STRUCT_CONSTEXPR17 layout_with_context() noexcept _GLSL_STRUCT_REQUIRES(
			  std::is_default_constructible_v<context_type>
			)
				: _context() {
			}

			/// @brief constructor with context value
			_GLSL_STRUCT_CONSTEXPR17 explicit layout_with_context(const context_type& ctx) : _context(ctx) {}
		};

		/**
		 * @brief empty container for layout
		 * @ingroup utils
		 */
		struct _GLSL_STRUCT_EXPORT layout_without_context {};
	} // namespace utils

		/**
		 * @brief base layout container
		 * @ingroup glslstruct
		 * @tparam Traits layout traits type
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<layout_traits Traits>
		#else
	template<class Traits, std::enable_if_t<is_layout_traits_v<Traits>, bool> >
		#endif
	class base_layout : public std::conditional_t<utils::has_layout_traits_context_v<Traits>,
											  utils::layout_with_context<Traits>, utils::layout_without_context> {
	public:
		/// @brief traits type
		using traits_type								 = Traits;

		/// @brief value indicating if traits has context defined
		static _GLSL_STRUCT_CONSTEXPR17 bool has_context = utils::has_layout_traits_context_v<traits_type>;

	private:
		friend struct std::hash<base_layout>;

		/// @brief base layout struct
		using base_struct =
		  std::conditional_t<has_context, utils::layout_with_context<traits_type>, utils::layout_without_context>;

		/// @brief variables data
		mstd::ordered_map<std::string, var_data> _variables;
		/// @brief current offset
		size_t _currentOffset = 0;

		#pragma region VARIABLE_SET

		/// @brief sets variable data
		_GLSL_STRUCT_CONSTEXPR17 var_data& _set_variable(const std::string_view name, const size_t offset,
		#if _GLSL_STRUCT_HAS_TYPES
		  const base_type_handle& type,
		#else
		  const size_t size,
		#endif
		  const bool isTopLevel, const size_t padding = 0) {
			return _variables.emplace_back(name.data(), var_data(offset,
		#if _GLSL_STRUCT_HAS_TYPES
														  type,
		#else
														  size,
		#endif
														  isTopLevel, padding));
		}

		#pragma endregion

		/// @brief calculates alignment offset
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t _calculate_alignment_offset(const size_t currentOffset,
		  const size_t baseAlignment) noexcept {
			// CALCULATE ALIGNMENT
			const size_t alignmentOffset = ceil_to_nearest_multiple(currentOffset, baseAlignment);

				// CHECK IF NOT OVERFLOWED
				if (alignmentOffset < currentOffset) {
					glsl_struct_assert(false, "Data overflow would happen!");
					return bad_offset();
				}

			return alignmentOffset;
		}

		/// @brief returns padding value
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t _calculate_padding(const size_t currentOffset,
		  const size_t baseAlignment) noexcept {
			return _calculate_alignment_offset(currentOffset, baseAlignment) - currentOffset;
		}

		/// @brief moves current offset to end of variable
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 bool _move_current_offset(size_t& currentOffset,
		  const size_t distance) noexcept {
				if (mstd::add_overflow(currentOffset, distance, currentOffset)) {
					glsl_struct_assert(false, "Data overflow would happen!");
					return false;
				}
			return true;
		}

		#pragma region COMMON_CHECKS

		/// @brief checks if vec length is between 2 and 4
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 bool _is_vec_length_good(const size_t length) noexcept {
				if (length < 2 || length > 4) {
					glsl_struct_assert(false, "value should be in range <2, 4>!");
					return false;
				}
			return true;
		}

		/// @brief checks if array count is not zero
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 bool _is_array_count_good(const size_t count) {
				if (count == 0) {
					glsl_struct_assert(false, "array count cannot be zero!!");
					return false;
				}
			return true;
		}

		/// @brief checks if struct is not empty
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 bool _is_struct_not_empty(
		  const mstd::ordered_map<std::string, var_data>& values
		) noexcept {
				if (values.empty()) {
					glsl_struct_assert(false, "struct cannot be empty!!");
					return false;
				}
			return true;
		}

		#pragma endregion

		#pragma region STANDARD_ADD

		/// @brief returns alignment offset of variable and moves current offset
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t _add(size_t& currentOffset, const size_t baseAlignment,
		  const size_t baseOffset) noexcept {
			// GET ALIGNMENT OFFSET
			const size_t alignmentOffset = _calculate_alignment_offset(currentOffset, baseAlignment);

				if (alignmentOffset == bad_offset()) { return alignmentOffset; }

			// CHECK OFFSET BOUNDARIES
			size_t tempOffset = alignmentOffset;
				if (!_move_current_offset(tempOffset, baseOffset)) { return bad_offset(); }

			// MOVE CURRENT OFFSET
			currentOffset = tempOffset;
			return alignmentOffset;
		}

		/// @brief returns alignment offsets of array variables and moves current offset
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_array(size_t& currentOffset,
		  const size_t arrayBaseAlignment, const size_t elemBaseOffset, const size_t count) noexcept {
			// CALCULATE ARRAY ELEMENTS OFFSETS
			size_t tempOffset = currentOffset;

			std::vector<size_t> arrayOffsets;
			arrayOffsets.reserve(count);
				for (size_t i = 0, elemOffset; i < count; ++i) {
					elemOffset = _add(tempOffset, arrayBaseAlignment, elemBaseOffset);

						if (elemOffset == bad_offset()) { return std::vector<size_t>(); }

					arrayOffsets.push_back(elemOffset);
				}

			// MOVE CURRENT OFFSET
			currentOffset = tempOffset;
			return arrayOffsets;
		}

		#pragma endregion

		#pragma region VARIABLE_ADD

		/// @brief sets variable data
		_GLSL_STRUCT_CONSTEXPR17 var_data& _add_variable(
		  const std::string_view name, const size_t alignmentOffset, const bool isTopLevel,
		#if _GLSL_STRUCT_HAS_TYPES
		  const base_type_handle& type
		#else
		  const size_t typeSize
		#endif
		) {
			// CHECK VARIABLE NAME
			glsl_struct_assert(!contains(name), "Layout already contains value with name {}", name);

			// SET VARIABLE

			return _set_variable(name, alignmentOffset,
		#if _GLSL_STRUCT_HAS_TYPES
			  type,
		#else
			  typeSize,
		#endif
			  isTopLevel);
		}

		/// @brief sets array variables data
		_GLSL_STRUCT_CONSTEXPR17 var_data& _add_array_variable(
		  const std::string_view name, const std::vector<size_t>& alignmentOffsets, const bool isTopLevel,
		#if _GLSL_STRUCT_HAS_TYPES
		  const base_type_handle& elemType, const base_type_handle& arrayType
		#else
		  const size_t elemBaseOffset, const size_t arrayBaseOffset
		#endif
		) {
		#if _GLSL_STRUCT_HAS_TYPES
			size_t elemSize = elemType->get_size();
		#else
			size_t elemSize = elemBaseOffset;
		#endif

				for (size_t i = 0; i < alignmentOffsets.size(); ++i) {
					// SET ELEMENT VARIABLE
					var_data& varData = _add_variable(
					  get_array_elem_name(name, i), alignmentOffsets[i], false,
		#if _GLSL_STRUCT_HAS_TYPES
					  elemType
		#else
					  elemSize
		#endif
					);

						if (i < alignmentOffsets.size() - 1) {
							varData.set_padding(alignmentOffsets[i + 1] - alignmentOffsets[i] - elemSize);
						}
				}

			// ADD ARRAY VAR
			return _add_variable(
			  name, alignmentOffsets.front(), isTopLevel,
		#if _GLSL_STRUCT_HAS_TYPES
			  arrayType
		#else
			  arrayBaseOffset
		#endif
			);
		}

		#if _GLSL_STRUCT_HAS_TYPES
		/// @brief sets standard array variable
		_GLSL_STRUCT_CONSTEXPR17 var_data& _add_array_variable(const std::string_view name,
		  const std::vector<size_t>& alignmentOffsets, const bool isTopLevel, const size_t count,
		  const base_type_handle& elemType, const size_t arrayBaseOffset) {
			return _add_array_variable(name, alignmentOffsets, isTopLevel, elemType,
			  std::make_shared<array_type>(elemType, count, arrayBaseOffset));
		}
		#endif

		/// @brief sets mat variable
		_GLSL_STRUCT_CONSTEXPR17 var_data& _add_mat_variable(const std::string_view name,
		  const std::vector<size_t>& alignmentOffsets, const bool isTopLevel,
		#if _GLSL_STRUCT_HAS_TYPES
		  const ValueType valueType, const size_t columns, const size_t rows,
		#endif
		  const size_t vecBaseOffset, const size_t matBaseOffset) {
			return _add_array_variable(
			  name, alignmentOffsets, isTopLevel,
		#if _GLSL_STRUCT_HAS_TYPES
			  std::make_shared<vec_type>(valueType, rows, vecBaseOffset),
			  std::make_shared<mat_type>(valueType, columns, rows, matBaseOffset)
		#else
			  vecBaseOffset, matBaseOffset
		#endif
			);
		}

		/// @brief sets array of mats variable
		var_data& _add_mat_array_variable(const std::string_view name, const std::vector<std::vector<size_t> >& alignmentOffsets,
		  const bool isTopLevel, const size_t count,
		#if _GLSL_STRUCT_HAS_TYPES
		  const ValueType valueType, const size_t columns, const size_t rows,
		#endif
		  const size_t vecBaseOffset, const size_t matBaseOffset, const size_t arrayBaseOffset, const size_t matPadding) {
		#if _GLSL_STRUCT_HAS_TYPES
			const auto vecType = std::make_shared<vec_type>(valueType, rows, vecBaseOffset);
			const auto matType = std::make_shared<mat_type>(valueType, columns, rows, matBaseOffset);
		#endif

				for (size_t i = 0; i < alignmentOffsets.size(); ++i) {
					const auto& matOffsets = alignmentOffsets[i];

					// ADD SINGLE MAT VARIABLE DATA
					var_data& varData	   = _add_array_variable(
					  get_array_elem_name(name, i), matOffsets, false,
		#if _GLSL_STRUCT_HAS_TYPES
					  vecType, matType
		#else
					  vecBaseOffset, matBaseOffset
		#endif
					);

						if (i < count - 1) { varData.set_padding(matPadding); }
				}

			// ADD ARRAY TYPE POINTER TO FIRST MAT
			var_data& arrayVarData = _add_variable(
			  name, alignmentOffsets.front().front(), isTopLevel,
		#if _GLSL_STRUCT_HAS_TYPES
			  std::make_shared<array_type>(matType, count, arrayBaseOffset)
		#else
			  arrayBaseOffset
		#endif
			);

			arrayVarData.set_padding(matPadding);

			return arrayVarData;
		}

		#pragma endregion

		#pragma region BEFORE_ADD_EVENTS

		/// @brief calls before add event
		_GLSL_STRUCT_CONSTEXPR17 void _before_add() {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add(_currentOffset); }
				}
		}

		/// @brief calls before add array event
		_GLSL_STRUCT_CONSTEXPR17 void _before_add_array() {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_array_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_array(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_array(_currentOffset); }
				}
				else { _before_add(); }
		}

		/// @brief calls before add scalar event
		_GLSL_STRUCT_CONSTEXPR17 void _before_add_scalar() {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_scalar_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_scalar(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_scalar(_currentOffset); }
				}
				else { _before_add(); }
		}

		/// @brief calls before add vec event
		_GLSL_STRUCT_CONSTEXPR17 void _before_add_vec() {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_vec_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_vec(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_vec(_currentOffset); }
				}
				else { _before_add(); }
		}

		/// @brief calls before add mat event
		_GLSL_STRUCT_CONSTEXPR17 void _before_add_mat() {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_mat_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_mat(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_mat(_currentOffset); }
				}
				else { _before_add(); }
		}

		/// @brief calls before add struct event
		_GLSL_STRUCT_CONSTEXPR17 void _before_add_struct() {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_struct_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_struct(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_struct(_currentOffset); }
				}
				else { _before_add(); }
		}

		#pragma endregion

		#pragma region AFTER_ADD_EVENTS

		/// @brief calls after add event
		_GLSL_STRUCT_CONSTEXPR17 void _after_add(const size_t baseOffset, const size_t baseAlignment) {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
						}
						else { traits_type::after_add(_currentOffset, baseOffset, baseAlignment); }
				}
		}

		/// @brief calls after add array event
		_GLSL_STRUCT_CONSTEXPR17 void _after_add_array(const size_t baseOffset, const size_t baseAlignment) {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_array_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_array(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_array(_currentOffset, baseOffset, baseAlignment); }
				}
				else { _after_add(baseOffset, baseAlignment); }
		}

		/// @brief calls after add scalar event
		_GLSL_STRUCT_CONSTEXPR17 void _after_add_scalar(const size_t baseOffset, const size_t baseAlignment) {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_scalar_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_scalar(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_scalar(_currentOffset, baseOffset, baseAlignment); }
				}
				else { _after_add(baseOffset, baseAlignment); }
		}

		/// @brief calls after add vec event
		_GLSL_STRUCT_CONSTEXPR17 void _after_add_vec(const size_t baseOffset, const size_t baseAlignment) {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_vec_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_vec(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_vec(_currentOffset, baseOffset, baseAlignment); }
				}
				else { _after_add(baseOffset, baseAlignment); }
		}

		/// @brief calls after add mat event
		_GLSL_STRUCT_CONSTEXPR17 void _after_add_mat(const size_t baseOffset, const size_t baseAlignment) {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_mat_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_mat(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_mat(_currentOffset, baseOffset, baseAlignment); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_v<traits_type>) {
					_after_add(baseOffset, baseAlignment);
				}
		}

		/// @brief calls after add struct event
		_GLSL_STRUCT_CONSTEXPR17 void _after_add_struct(const size_t baseOffset, const size_t baseAlignment) {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_struct_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_struct(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_struct(_currentOffset, baseOffset, baseAlignment); }
				}
				else { _after_add(baseOffset, baseAlignment); }
		}

		#pragma endregion

		#pragma region GET_ALIGNMENT

		/// @brief returns array of elements alignment
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t _get_array_alignment(const size_t elemBaseAlignment) {
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
					return traits_type::get_array_alignment(elemBaseAlignment, base_struct::_context);
				}
				else { return traits_type::get_array_alignment(elemBaseAlignment); }
		}

		/// @brief returns scalar alignment
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t _get_scalar_alignment(const ValueType valueType) {
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
					return traits_type::get_scalar_alignment(valueType, base_struct::_context);
				}
				else { return traits_type::get_scalar_alignment(valueType); }
		}

		/// @brief returns array of scalars alignment
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t _get_scalar_array_alignment(const ValueType valueType) {
			return _get_array_alignment(_get_scalar_alignment(valueType));
		}

		/// @brief returns vec alignment
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t _get_vec_alignment(const ValueType valueType, const size_t length) {
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
					return traits_type::get_vec_alignment(valueType, length, base_struct::_context);
				}
				else { return traits_type::get_vec_alignment(valueType, length); }
		}

		/// @brief returns array of vecs alignment
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t _get_vec_array_alignment(const ValueType valueType, const size_t length) {
			return _get_array_alignment(_get_vec_alignment(valueType, length));
		}

		/// @brief returns mat alignment
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t _get_mat_alignment(const ValueType valueType, const size_t rows) {
			return _get_vec_array_alignment(valueType, rows);
		}

		#pragma endregion

		#pragma region SPECIALIZED_ADD

		/// @brief adds scalar
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t _add_scalar(const std::string_view name, const ValueType valueType) {
			// BEFORE ADD SCALAR
			_before_add_scalar();

			// GET BASE OFFSET
			const size_t baseOffset		 = get_value_type_size(valueType);

			// GET BASE ALIGNMENT
			const size_t baseAlignment	 = _get_scalar_alignment(valueType);

			// GET ALIGNMENT OFFSET
			const size_t alignmentOffset = _add(_currentOffset, baseAlignment, baseOffset);

				// CHECK ADD
				if (alignmentOffset == bad_offset()) { return alignmentOffset; }

			// ADD VARIABLE DATA
			_add_variable(
			  name, alignmentOffset, true,
		#if _GLSL_STRUCT_HAS_TYPES
			  std::make_shared<scalar_type>(valueType, baseOffset)
		#else
			  baseOffset
		#endif
			);

			// AFTER ADD SCALAR
			_after_add_scalar(baseOffset, baseAlignment);

			return alignmentOffset;
		}

		/// @brief adds array of scalars
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_scalar_array(const std::string_view name,
		  const ValueType valueType, const size_t count) {
			// CHECK COUNT
				if (!_is_array_count_good(count)) { return std::vector<size_t>(); }

			// BEFORE ADD SCALAR ARRAY
			_before_add_array();

			// GET BASE OFFSET
			const size_t baseOffset					   = get_value_type_size(valueType);

			// GET ARRAY ALIGNMENT
			const size_t arrayBaseAlignment			   = _get_scalar_array_alignment(valueType);

			// GET ALIGNMENT OFFSETS
			size_t tempOffset						   = _currentOffset;
			const std::vector<size_t> alignmentOffsets = _add_array(tempOffset, arrayBaseAlignment, baseOffset, count);

			// CHECK ADD
				if (alignmentOffsets.empty()) { return alignmentOffsets; }

			// GET ARRAY SIZE
			const size_t arraySize	  = tempOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t arrayPadding = _calculate_padding(tempOffset, arrayBaseAlignment);

				// APPLY PADDING TO CURRENT OFFSET
				if (!_move_current_offset(tempOffset, arrayPadding)) { return std::vector<size_t>(); }
			_currentOffset = tempOffset;

			// ADD VARIABLE DATA
			_add_array_variable(name, alignmentOffsets, true,
		#if _GLSL_STRUCT_HAS_TYPES
			  count, std::make_shared<scalar_type>(valueType, baseOffset),
		#else
			  baseOffset,
		#endif
			  arraySize)
			  .set_padding(arrayPadding);

			// AFTER ADD SCALAR ARRAY
			_after_add_array(arraySize, arrayBaseAlignment);

			return alignmentOffsets;
		}

		/// @brief adds vec
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t _add_vec(const std::string_view name, const size_t length,
		  const ValueType valueType) {
				// CHECK VEC LENGTH
				if (!_is_vec_length_good(length)) { return bad_offset(); }

			// BEFORE ADD VEC
			_before_add_vec();

			// GET BASE OFFSET
			const size_t baseOffset		 = get_value_type_size(valueType) * length;

			// GET BASE ALIGNMENT
			const size_t baseAlignment	 = _get_vec_alignment(valueType, length);

			// GET ALIGNMENT OFFSET
			const size_t alignmentOffset = _add(_currentOffset, baseAlignment, baseOffset);

				// CHECK ADD
				if (alignmentOffset == bad_offset()) { return alignmentOffset; }

			// ADD VARIABLE DATA
			_add_variable(
			  name, alignmentOffset, true,
		#if _GLSL_STRUCT_HAS_TYPES
			  std::make_shared<vec_type>(valueType, length, baseOffset)
		#else
			  baseOffset
		#endif
			);

			// AFTER ADD VEC
			_after_add_vec(baseOffset, baseAlignment);

			return alignmentOffset;
		}

		/// @brief adds array of vecs
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_vec_array(const std::string_view name,
		  const size_t length, const ValueType valueType, const size_t count) {
			// CHECK COUNT
				if (!_is_array_count_good(count)) { return std::vector<size_t>(); }

			// CHECK VEC LENGTH
				if (!_is_vec_length_good(length)) { return std::vector<size_t>(); }

			// BEFORE ADD VEC ARRAY
			_before_add_array();

			// GET BASE OFFSET
			const size_t baseOffset					   = get_value_type_size(valueType) * length;

			// GET ARRAY ALIGNMENT
			const size_t arrayBaseAlignment			   = _get_vec_array_alignment(valueType, length);

			// GET ALIGNMENT OFFSETS
			size_t tempOffset						   = _currentOffset;
			const std::vector<size_t> alignmentOffsets = _add_array(tempOffset, arrayBaseAlignment, baseOffset, count);

				// CHECK ADD
				if (alignmentOffsets.empty()) { return alignmentOffsets; }

			// GET ARRAY SIZE
			const size_t arraySize	  = tempOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t arrayPadding = _calculate_padding(tempOffset, arrayBaseAlignment);

				// APPLY PADDING TO CURRENT OFFSET
				if (!_move_current_offset(tempOffset, arrayPadding)) { return std::vector<size_t>(); }
			_currentOffset = tempOffset;

			// ADD VARIABLE DATA
			_add_array_variable(name, alignmentOffsets, true,
		#if _GLSL_STRUCT_HAS_TYPES
			  count, std::make_shared<vec_type>(valueType, length, baseOffset),
		#else
			  baseOffset,
		#endif
			  arraySize)
			  .set_padding(arrayPadding);

			// AFTER ADD VEC ARRAY
			_after_add_array(arraySize, arrayBaseAlignment);

			return alignmentOffsets;
		}

		/// @brief adds mat
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_mat(const std::string_view name, const size_t columns,
		  const size_t rows, const ValueType valueType) {
			// CHECK MAT COLUMNS
				if (!_is_vec_length_good(columns)) { return std::vector<size_t>(); }

			// CHECK MAT ROWS
				if (!_is_vec_length_good(rows)) { return std::vector<size_t>(); }

			// BEFORE ADD MAT
			_before_add_mat();

			// GET BASE OFFSET
			const size_t vecBaseOffset				   = get_value_type_size(valueType) * rows;

			// GET MAT ALIGNMENT
			const size_t matBaseAlignment			   = _get_mat_alignment(valueType, rows);

			// GET ALIGNMENT OFFSETS
			size_t tempOffset						   = _currentOffset;
			const std::vector<size_t> alignmentOffsets = _add_array(tempOffset, matBaseAlignment, vecBaseOffset, columns);

				// CHECK ADD
				if (alignmentOffsets.empty()) { return alignmentOffsets; }

			// GET MAT SIZE
			const size_t matSize	= tempOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t matPadding = _calculate_padding(tempOffset, matBaseAlignment);

				// APPLY PADDING TO CURRENT OFFSET
				if (!_move_current_offset(tempOffset, matPadding)) { return std::vector<size_t>(); }
			_currentOffset = tempOffset;

			// ADD VARIABLE DATA
			_add_mat_variable(name, alignmentOffsets, true,
		#if _GLSL_STRUCT_HAS_TYPES
			  valueType, columns, rows,
		#endif
			  vecBaseOffset, matSize)
			  .set_padding(matPadding);

			// AFTER ADD MAT
			_after_add_mat(matSize, matBaseAlignment);

			return alignmentOffsets;
		}

		/// @brief adds array of mats
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<std::vector<size_t> > _add_mat_array(const std::string_view name,
		  const size_t columns, const size_t rows, const ValueType valueType, const size_t count) {
			// CHECK COUNT
				if (!_is_array_count_good(count)) { return std::vector<std::vector<size_t> >(); }

			// CHECK MAT COLUMNS
				if (!_is_vec_length_good(columns)) { return std::vector<std::vector<size_t> >(); }

				// CHECK MAT ROWS
				if (!_is_vec_length_good(rows)) { return std::vector<std::vector<size_t> >(); }

			// BEFORE ADD MAT ARRAY
			_before_add_array();

			// GET BASE OFFSET
			const size_t vecBaseOffset	  = get_value_type_size(valueType) * rows;

			// GET ARRAY ALIGNMENT
			const size_t matBaseAlignment = _get_mat_alignment(valueType, rows);

			// GET MATS ALIGNMENT OFFSETS
			size_t tempOffset			  = _currentOffset;

			size_t matSize				  = 0;
			size_t matPadding			  = 0;

			std::vector<std::vector<size_t> > alignmentOffsetsPerMat;
			alignmentOffsetsPerMat.reserve(count);
				for (size_t i = 0; i < count; ++i) {
					// GET VEC ALIGNMENT OFFSETS
					const std::vector<size_t> alignmentOffsets = _add_array(tempOffset, matBaseAlignment, vecBaseOffset, columns);

						// CHECK ADD
						if (alignmentOffsets.empty()) { return std::vector<std::vector<size_t> >(); }

						// CALCULATE SIZE AND PADDING ONCE
						if (matSize == 0) {
							// CALCULATE SIZE
							matSize	   = tempOffset - alignmentOffsets.front();

							// CALCULATE PADDING
							matPadding = _calculate_padding(tempOffset, matBaseAlignment);
						}

					// APPLY PADDING TO CURRENT OFFSET
						if (i != count - 1) {
								if (!_move_current_offset(tempOffset, matPadding)) { return std::vector<std::vector<size_t> >(); }
						}

					alignmentOffsetsPerMat.push_back(alignmentOffsets);
				}

			// GET ARRAY SIZE
			const size_t arraySize = tempOffset - alignmentOffsetsPerMat.front().front();

				// APPLY PADDING TO CURRENT OFFSET
				if (!_move_current_offset(tempOffset, matPadding)) { return std::vector<std::vector<size_t> >(); }
			_currentOffset = tempOffset;

			// ADD VARIABLE DATA
			_add_mat_array_variable(name, alignmentOffsetsPerMat, true, count,
		#if _GLSL_STRUCT_HAS_TYPES
			  valueType, columns, rows,
		#endif
			  vecBaseOffset, matSize, arraySize, matPadding);

			// AFTER ADD SCALAR ARRAY
			_after_add_array(arraySize, matBaseAlignment);

			return alignmentOffsetsPerMat;
		}

		/// @brief adds struct
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 size_t _add_struct(const std::string_view name, const size_t baseAlignment,
		  const size_t baseOffset, const mstd::ordered_map<std::string, var_data>& values) {
				// CHECK VALUES
				if (!_is_struct_not_empty(values)) { return bad_offset(); }

			// BEFORE ADD STRUCT
			_before_add_struct();

			// GET ALIGNMENT OFFSET
			const size_t alignmentOffset = _add(_currentOffset, baseAlignment, baseOffset);

				// CHECK ADD
				if (alignmentOffset == bad_offset()) { return alignmentOffset; }

			// ADD VARIABLE DATA
			_add_variable(
			  name, alignmentOffset, true,
		#if _GLSL_STRUCT_HAS_TYPES
			  std::make_shared<struct_type>(values, baseOffset)
		#else
			  baseOffset
		#endif
			);

			// ADD STRUCTURE VARIABLES DATA
				for (const auto& [value_name, data] : values) {
					_add_variable(
					  get_struct_elem_name(name, value_name), alignmentOffset + data.get_offset(), false,
		#if _GLSL_STRUCT_HAS_TYPES
					  data.get_type()
		#else
					  data.get_size()
		#endif
					)
					  .set_padding(data.get_padding());
				}

			// AFTER ADD STRUCT
			_after_add_struct(baseOffset, baseAlignment);

			return alignmentOffset;
		}

		/// @brief adds array of structs
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> _add_struct_array(const std::string_view name,
		  const size_t baseAlignment, const size_t baseOffset, const mstd::ordered_map<std::string, var_data>& values,
		  const size_t count) {
			// CHECK COUNT
				if (!_is_array_count_good(count)) { return std::vector<size_t>(); }

			// CHECK VALUES
				if (!_is_struct_not_empty(values)) { return std::vector<size_t>(); }

			// BEFORE ADD STRUCTS ARRAY
			_before_add_array();

			// GET ARRAY ALIGNMENT
			const size_t arrayBaseAlignment			   = _get_array_alignment(baseAlignment);

			// GET ALIGNMENT OFFSETS
			size_t tempOffset						   = _currentOffset;
			const std::vector<size_t> alignmentOffsets = _add_array(tempOffset, arrayBaseAlignment, baseOffset, count);

				// CHECK ADD
				if (alignmentOffsets.empty()) { return alignmentOffsets; }

			// GET ARRAY SIZE
			const size_t arraySize	  = tempOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t arrayPadding = _calculate_padding(tempOffset, arrayBaseAlignment);

				// APPLY PADDING TO CURRENT OFFSET
				if (!_move_current_offset(tempOffset, arrayPadding)) { return std::vector<size_t>(); }
			_currentOffset = tempOffset;

			// Add variables
				for (size_t i = 0; i < count; ++i) {
					const std::string arrayElemName = get_array_elem_name(name, i);
					const size_t alignmentOffset	= alignmentOffsets[i];

						// ADD STRUCTURE VARIABLES DATA
						for (const auto& [value_name, data] : values) {
							_add_variable(
							  get_struct_elem_name(arrayElemName, value_name), alignmentOffset + data.get_offset(), false,
		#if _GLSL_STRUCT_HAS_TYPES
							  data.get_type()
		#else
							  data.get_size()
		#endif
							)
							  .set_padding(data.get_padding());
						}
				}

			// ADD VARIABLE DATA
			_add_array_variable(name, alignmentOffsets, true,
		#if _GLSL_STRUCT_HAS_TYPES
			  count, std::make_shared<struct_type>(values, baseOffset),
		#else
			  baseOffset,
		#endif
			  arraySize)
			  .set_padding(arrayPadding);

			// AFTER ADD SCALAR ARRAY
			_after_add_array(arraySize, arrayBaseAlignment);

			return alignmentOffsets;
		}

		/// @brief adds multiple variables
		template<class T, class... Ts, size_t Num, size_t... Nums>
		_GLSL_STRUCT_CONSTEXPR17 void _add_variables(const glsl_variable<T, Num>& var,
		  const glsl_variable<Ts, Nums>&... vars) noexcept {
			using VarType = glsl_variable<T, Num>;

				if _GLSL_STRUCT_CONSTEXPR17 (VarType::is_layout) {
						if _GLSL_STRUCT_CONSTEXPR17 (VarType::is_array) { add(var.varName, var.layout, Num); }
						else { add(var.varName, var.layout); }
				}
				else {
						if _GLSL_STRUCT_CONSTEXPR17 (VarType::is_array) { add<T>(var.varName, Num); }
						else { add<T>(var.varName); }
				}
				if _GLSL_STRUCT_CONSTEXPR17 (sizeof...(Ts) > 0 && sizeof...(Nums) > 0) { _add_variables(vars...); }
		}

		#pragma endregion

	public:
		#pragma region CONSTRUCTORS_WITHOUT_CONTEXT
		/// @brief default constructor without providing context
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class T = traits_type,
		  std::enable_if_t<(!has_context || (has_context && std::is_default_constructible_v<base_struct>)) &&
							 std::is_same_v<T, traits_type>,
			bool>		 = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 base_layout() noexcept _GLSL_STRUCT_REQUIRES(!has_context ||
																			  (has_context &&
																				std::is_default_constructible_v<base_struct>))
			: base_struct() {
		}

		/// @brief constructor for multiple variables
		#if _GLSL_STRUCT_HAS_CXX20
		template<class... Ts, size_t... Nums>
		#else
		template<class... Ts, size_t... Nums,
		  std::enable_if_t<!has_context || (has_context && std::is_default_constructible_v<base_struct>), bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 explicit base_layout(
		  const glsl_variable<Ts, Nums>&... vars
		) noexcept _GLSL_STRUCT_REQUIRES(!has_context || (has_context && std::is_default_constructible_v<base_struct>))
			: base_struct() {
			_add_variables(vars...);
		}

		#pragma endregion

		#pragma region CONSTRUCTORS_WITH_CONTEXT
		/// @brief constructor with provided context
		#if !_GLSL_STRUCT_HAS_CXX20
		template<class T = traits_type, std::enable_if_t<has_context && std::is_same_v<T, traits_type>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 explicit base_layout(
		  const _GLSL_STRUCT_TYPENAME17 base_struct::context_type& ctx
		) noexcept _GLSL_STRUCT_REQUIRES(has_context)
			: base_struct(ctx) {
		}

		/// @brief constructor for multiple variables and with provided context
		#if _GLSL_STRUCT_HAS_CXX20
		template<class... Ts, size_t... Nums>
		#else
		template<class... Ts, size_t... Nums, std::enable_if_t<has_context, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 explicit base_layout(const glsl_variable<Ts, Nums>&... vars,
		  const _GLSL_STRUCT_TYPENAME17 base_struct::context_type& ctx) noexcept _GLSL_STRUCT_REQUIRES(has_context)
			: base_struct(ctx) {
			_add_variables(vars...);
		}

		#pragma endregion

		/// @brief default copy constructor
		_GLSL_STRUCT_CONSTEXPR17 base_layout(const base_layout& other) noexcept			   = default;

		/// @brief default move constructor
		_GLSL_STRUCT_CONSTEXPR17 base_layout(base_layout&& other) noexcept				   = default;

		/// @brief default destructor
		_GLSL_STRUCT_CONSTEXPR20 ~base_layout() noexcept								   = default;

		/// @brief default copy assign operator
		_GLSL_STRUCT_CONSTEXPR17 base_layout& operator=(const base_layout& other) noexcept = default;

		/// @brief default move assign operator
		_GLSL_STRUCT_CONSTEXPR17 base_layout& operator=(base_layout&& other) noexcept	   = default;

		/// @brief value indicating error while returning offset
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t bad_offset() noexcept { return std::numeric_limits<size_t>::max(); }

		/// @brief returns true if layout contains variable with given name
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool contains(const std::string_view name) const noexcept {
			return _variables.contains(name.data());
		}

		/// @brief returns variable data
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 const var_data& get(const std::string_view name) const noexcept {
			glsl_struct_assert(contains(name), "The variable name doesn't exists!");
			return _variables.at(name.data());
		}

		/// @brief returns array count (if single value then returns 1 else if value doesn't exist returns 0 else array count)
		[[nodiscard]]
		#if _GLSL_STRUCT_HAS_TYPES || _GLSL_STRUCT_HAS_CXX20
		_GLSL_STRUCT_CONSTEXPR17
		#endif
		  size_t get_array_count(const std::string_view name) const noexcept {
				if (!contains(name)) { return 0; }

		#if _GLSL_STRUCT_HAS_TYPES
			const base_type_handle& varType = get_type(name);

				if (is_of_type<mat_type>(varType)) { return static_type_cast<mat_type>(varType)->get_array_count(); }
				if (is_of_type<array_type>(varType)) { return static_type_cast<array_type>(varType)->get_count(); }
		#else
			size_t i = 0;
				while (true) {
					const std::string valueName = get_array_elem_name(name, i);

						if (!contains(valueName)) { break; }
					++i;
				}

				if (i != 0) { return i; }
		#endif
			return 1;
		}

		/// @brief returns elem size of array
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_array_elem_size(const std::string_view name) const noexcept {
			const size_t arrayCount = get_array_count(name);

				if (arrayCount == 0) { return 0; }

				if (arrayCount == 1) { return _variables.at(name.data()).get_size(); }

			return _variables.at(get_array_elem_name(name, 0)).get_size();
		}

		/// @brief returns offset of variable with given name
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_offset(const std::string_view name) const noexcept {
			return get(name).get_offset();
		}

		/// @brief returns offsets of array elements
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> get_array_offsets(const std::string_view name) const noexcept {
			const size_t arrayCount = get_array_count(name);

				if (arrayCount == 0) { return {}; }

				if (arrayCount == 1) { return { _variables.at(name.data()).get_offset() }; }

			std::vector<size_t> values;
			values.reserve(arrayCount);
				for (size_t i = 0; i != arrayCount; ++i) {
					values.push_back(_variables.at(get_array_elem_name(name, i)).get_offset());
				}
			return values;
		}

		#if _GLSL_STRUCT_HAS_TYPES
		/// @brief returns type of variable
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 const base_type_handle& get_type(const std::string_view name) const noexcept {
			return get(name).get_type();
		}

				/// @brief returns type of variable with casting to given type (dynamic casting occurs)
			#if _GLSL_STRUCT_HAS_CXX20
		template<glsl_type Type>
			#else
		template<class Type, std::enable_if_t<is_glsl_type_v<Type>, bool> = true>
			#endif
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 std::shared_ptr<Type> get_type(const std::string_view name) const noexcept {
			return dynamic_type_cast<Type>(get_type(name));
		}
		#endif

		/// @brief returns total size of variable (size + padding)
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_total_size(const std::string_view name) const noexcept {
			return get(name).get_total_size();
		}

		/// @brief returns size of variable
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_size(const std::string_view name) const noexcept {
			return get(name).get_size();
		}

		/// @brief returns padding of variable
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t get_padding(const std::string_view name) const noexcept {
			return get(name).get_padding();
		}

		/// @brief returns all variables names
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 std::vector<std::string> get_names() const noexcept {
			std::vector<std::string> names;
			names.reserve(_variables.size());

		#if _GLSL_STRUCT_HAS_CXX20
				for (const auto& name : _variables | std::views::keys) { names.push_back(name); }
		#else
				for (const auto& [name, data] : _variables) { names.push_back(name); }
		#endif

			return names;
		}

		/// @brief returns all variables and their data
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR20 const mstd::ordered_map<std::string, var_data>& get_variables() const noexcept {
			return _variables;
		}

		/// @brief returns all top level variables and their data
		[[nodiscard]] mstd::ordered_map<std::string, var_data> get_top_level_variables() const noexcept {
			mstd::ordered_map<std::string, var_data> variables;
				for (const auto& [name, data] : _variables) {
						if (!data.is_top_level()) { continue; }
					variables.emplace_back(name, data);
				}
			return variables;
		}

		/// @brief returns base alignment of layout
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t base_alignment() const noexcept {
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) { return traits_type::get_struct_alignment(base_struct::_context); }
				else { return traits_type::get_struct_alignment(); }
		}

		/// @brief returns size of layout
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t size() const noexcept { return _currentOffset; }

		/// @brief clears layout and all variables
		_GLSL_STRUCT_CONSTEXPR17 void clear() noexcept {
			_currentOffset = 0;
			_variables.clear();
		}

		#pragma region ADD_SCALAR
		/// @brief adds scalar
		#if _GLSL_STRUCT_HAS_CXX20
		template<glsl_scalar S>
		#else
		template<class S, std::enable_if_t<is_glsl_scalar_v<S>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view name) {
			return _add_scalar(name, get_scalar_value_type<S>());
		}

		#pragma endregion

		#pragma region ADD_SCALARS_ARRAY

		/// @brief adds array of scalars
		#if _GLSL_STRUCT_HAS_CXX20
		template<glsl_scalar S>
		#else
		template<class S, std::enable_if_t<is_glsl_scalar_v<S>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const size_t count) {
			return _add_scalar_array(name, get_scalar_value_type<S>(), count);
		}

		/// @brief adds array of scalars
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_array SA>
		#else
		template<class SA, std::enable_if_t<utils::is_glsl_scalars_array_v<SA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const size_t count) {
			using S = utils::array_value_type_t<SA>;
			return add<S>(name, count);
		}

		/// @brief adds array of scalars
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_static_size_array SA>
		#else
		template<class SA, std::enable_if_t<utils::is_glsl_scalars_static_size_array_v<SA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name) {
			return add<SA>(name, utils::array_static_size_v<SA>);
		}

		#pragma endregion

		#pragma region ADD_VEC
		/// @brief adds vec
		#if _GLSL_STRUCT_HAS_CXX20
		template<glsl_vec V>
		#else
		template<class V, std::enable_if_t<is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view name) {
			return _add_vec(name, get_vec_length<V>(), get_vec_value_type<V>());
		}

		#pragma endregion

		#pragma region ADD_VECS_ARRAY

		/// @brief adds array of vecs
		#if _GLSL_STRUCT_HAS_CXX20
		template<glsl_vec V>
		#else
		template<class V, std::enable_if_t<is_glsl_vec_v<V>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const size_t count) {
			return _add_vec_array(name, get_vec_length<V>(), get_vec_value_type<V>(), count);
		}

		/// @brief adds array of vecs
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_array VA>
		#else
		template<class VA, std::enable_if_t<utils::is_glsl_vecs_array_v<VA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const size_t count) {
			using V = utils::array_value_type_t<VA>;
			return add<V>(name, count);
		}

		/// @brief adds array of vecs
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_static_size_array VA>
		#else
		template<class VA, std::enable_if_t<utils::is_glsl_vecs_static_size_array_v<VA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name) {
			return add<VA>(name, utils::array_static_size_v<VA>);
		}

		#pragma endregion

		#pragma region ADD_MAT
		/// @brief adds mat
		#if _GLSL_STRUCT_HAS_CXX20
		template<glsl_mat M>
		#else
		template<class M, std::enable_if_t<is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name) {
			return _add_mat(name, get_mat_columns<M>(), get_mat_rows<M>(), get_mat_value_type<M>());
		}

		#pragma endregion

		#pragma region ADD_MATS_ARRAY

		/// @brief adds array of mats
		#if _GLSL_STRUCT_HAS_CXX20
		template<glsl_mat M>
		#else
		template<class M, std::enable_if_t<is_glsl_mat_v<M>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<std::vector<size_t> > add(const std::string_view name, const size_t count) {
			return _add_mat_array(name, get_mat_columns<M>(), get_mat_rows<M>(), get_mat_value_type<M>(), count);
		}

		/// @brief adds array of mats
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_array MA>
		#else
		template<class MA, std::enable_if_t<utils::is_glsl_mats_array_v<MA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<std::vector<size_t> > add(const std::string_view name, const size_t count) {
			using M = utils::array_value_type_t<MA>;
			return add<M>(name, count);
		}

		/// @brief adds array of mats
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_static_size_array MA>
		#else
		template<class MA, std::enable_if_t<utils::is_glsl_mats_static_size_array_v<MA>, bool> = true>
		#endif
		_GLSL_STRUCT_CONSTEXPR20 std::vector<std::vector<size_t> > add(const std::string_view name) {
			return add<MA>(name, utils::array_static_size_v<MA>);
		}

		#pragma endregion

		#pragma region ADD_STRUCT

		/// @brief adds struct with given layout
		_GLSL_STRUCT_CONSTEXPR17 size_t add(const std::string_view name, const base_layout& layout) {
			return _add_struct(name, layout.base_alignment(), layout._currentOffset, layout._variables);
		}

		#pragma endregion

		#pragma region ADD_STRUCTS_ARRAY

		/// @brief adds array of structs with given layout
		_GLSL_STRUCT_CONSTEXPR20 std::vector<size_t> add(const std::string_view name, const base_layout& layout,
		  const size_t count) {
			return _add_struct_array(name, layout.base_alignment(), layout._currentOffset, layout._variables, count);
		}

		#pragma endregion

		/// @brief checks equality of layouts
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator==(const base_layout& other) const noexcept {
			return _currentOffset == other._currentOffset && _variables == other._variables;
		}

		/// @brief default not equal operator
		[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 bool operator!=(const base_layout& other) const noexcept
		#if _GLSL_STRUCT_HAS_CXX20
		  = default;
		#else
		{
			return !(*this == other);
		}
		#endif
	};
} // namespace glslstruct

/**
 * @brief std::hash overload for base_layout
 * @ingroup glslstruct
 * @tparam Traits layout traits type
 */
template<class Traits>
struct std::hash<glslstruct::base_layout<Traits> > {
	[[nodiscard]] _GLSL_STRUCT_CONSTEXPR17 size_t operator()(const glslstruct::base_layout<Traits>& layout) const noexcept {
		size_t seed = std::hash<size_t> {}(layout._currentOffset);
			if _GLSL_STRUCT_CONSTEXPR17 (layout.has_context &&
										 glslstruct::utils::is_hashable_v<typename glslstruct::base_layout<Traits>::context_type>) {
				mstd::hash_append(seed, layout._context);
			}
			for (const auto& [name, data] : layout._variables) { mstd::hash_append(seed, name, data); }
		return seed;
	}
};
	#endif
#endif