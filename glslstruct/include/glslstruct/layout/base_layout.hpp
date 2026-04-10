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

		#include <glslstruct/layout/traits/layout_traits_concept.hpp>
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
			layout_with_context() noexcept _GLSL_STRUCT_REQUIRES(std::is_default_constructible_v<context_type>) : _context() {
			}

			/// @brief constructor with context value
			explicit layout_with_context(const context_type& ctx) : _context(ctx) {}

			/// @brief default destructor
			~layout_with_context() = default;
		};

		/**
		 * @brief empty container for layout
		 * @ingroup utils
		 */
		struct layout_without_context {};
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
	class base_layout : public std::conditional_t<utils::has_layout_traits_context_v<Traits>, utils::layout_with_context<Traits>,
						  utils::layout_without_context> {
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
		#if _GLSL_STRUCT_HAS_TYPES
		var_data& _set_variable(const std::string_view name, const size_t offset, const base_type_handle& type,
		  const bool isTopLevel, const size_t padding = 0) {
			return _variables.emplace_back(name.data(), var_data(offset, type, isTopLevel, padding));
		}
		#else
		var_data& _set_variable(const std::string_view name, const size_t offset, const size_t size, const bool isTopLevel,
		  const size_t padding = 0) {
			return _variables.emplace_back(name.data(), var_data(offset, size, isTopLevel, padding));
		}
		#endif
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
		  const size_t alignmentOffset, const size_t baseOffset) noexcept {
				if (mstd::add_overflow(alignmentOffset, baseOffset, currentOffset)) {
					glsl_struct_assert(false, "Data overflow would happen!");
					return false;
				}
			return true;
		}

		#pragma region STANDARD_ADD

		/// @brief returns alignment offset of variable and moves current offset
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t _add(size_t& currentOffset, const size_t baseAlignment,
		  const size_t baseOffset) noexcept {
			// GET ALIGNMENT OFFSET
			const size_t alignmentOffset = _calculate_alignment_offset(currentOffset, baseAlignment);

				if (alignmentOffset == bad_offset()) { return alignmentOffset; }

			// MOVE CURRENT OFFSET
				if (!_move_current_offset(currentOffset, alignmentOffset, baseOffset)) { return bad_offset(); }

			return alignmentOffset;
		}

		/// @brief returns alignment offsets of array variables and moves current offset
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

		#pragma endregion

		#pragma region VARIABLE_ADD

		/// @brief sets variable data
		var_data& _add_variable(
		  const std::string_view name, const size_t alignmentOffset, const bool isTopLevel,
		#if _GLSL_STRUCT_HAS_TYPES
		  const base_type_handle& type
		#else
		  const size_t baseOffset
		#endif
		) {
			// CHECK VARIABLE NAME
			glsl_struct_assert(!contains(name), "Layout already contains value with name {}", name);

				// SET VARIABLE
		#if _GLSL_STRUCT_HAS_TYPES
			return _set_variable(name, alignmentOffset, type, isTopLevel);
		#else
			return _set_variable(name, alignmentOffset, baseOffset, isTopLevel);
		#endif
		}

		/// @brief sets array variables data
		var_data& _add_array_variable(
		  const std::string_view name, const std::vector<size_t>& alignmentOffsets, const bool isTopLevel,
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
					var_data& varData = _add_variable(get_array_elem_name(name, i), alignmentOffsets[i], false, elemType);
						if (i < alignmentOffsets.size() - 1) {
							varData.set_padding(alignmentOffsets[i + 1] - alignmentOffsets[i] - elemType->get_size());
						}
		#else
					var_data& varData = _add_variable(get_array_elem_name(name, i), alignmentOffsets[i], false, elemBaseOffset);

						if (i < alignmentOffsets.size() - 1) {
							varData.set_padding(alignmentOffsets[i + 1] - alignmentOffsets[i] - elemBaseOffset);
						}
		#endif
				}

		// ADD ARRAY VAR
		#if _GLSL_STRUCT_HAS_TYPES
			return _add_variable(name, alignmentOffsets.front(), isTopLevel, arrayType);
		#else
			return _add_variable(name, alignmentOffsets.front(), isTopLevel, arrayBaseOffset);
		#endif
		}

		#pragma endregion

		#pragma region BEFORE_ADD_EVENTS

		void _before_add() {
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) { traits_type::before_add(_currentOffset, base_struct::_context); }
				else { traits_type::before_add(_currentOffset); }
		}

		void _before_add_scalar() {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_scalar_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_scalar(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_scalar(_currentOffset); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_v<traits_type>) { _before_add(); }
		}

		#pragma endregion

		#pragma region AFTER_ADD_EVENTS

		void _after_add(const size_t baseOffset, const size_t baseAlignment) {
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
					traits_type::after_add(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
				}
				else { traits_type::after_add(_currentOffset, baseOffset, baseAlignment); }
		}

		void _after_add_scalar(const size_t baseOffset, const size_t baseAlignment) {
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_scalar_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_scalar(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_scalar(_currentOffset, baseOffset, baseAlignment); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_v<traits_type>) {
					_after_add(baseOffset, baseAlignment);
				}
		}

		#pragma endregion

		#pragma region GET_ALIGNMENT

		[[nodiscard]] size_t _get_scalar_alignment(const ValueType valueType) {
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
					return traits_type::get_scalar_alignment(valueType, base_struct::_context);
				}
				else { return traits_type::get_scalar_alignment(valueType); }
		}

		#pragma endregion

		#pragma region SPECIALIZED_ADD

		/// @brief adds scalar
		[[nodiscard]] size_t _add_scalar(const std::string_view name, const ValueType valueType) {
			// BEFORE ADD SCALAR
			_before_add_scalar();

			// GET BASE OFFSET
			const size_t baseOffset		 = get_value_type_size(valueType);

			// GET BASE ALIGNMENT
			const size_t baseAlignment		 = _get_scalar_alignment(valueType);

			// GET ALIGNMENT OFFSET
			const size_t alignmentOffset = _add(_currentOffset, baseAlignment, baseOffset);

				// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			_add_variable(name, alignmentOffset, true, std::make_shared<scalar_type>(valueType, baseOffset));
		#else
			_add_variable(name, alignmentOffset, true, baseOffset);
		#endif

			// AFTER ADD SCALAR
			_after_add_scalar(baseOffset, baseAlignment);

			return alignmentOffset;
		}

		/// @brief adds array of scalars
		[[nodiscard]] std::vector<size_t> _add_scalar_array(const std::string_view name, const ValueType valueType,
		  const size_t count) {
				// BEFORE ADD SCALAR ARRAY
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_array_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_array(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_array(_currentOffset); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add(_currentOffset); }
				}

			// GET BASE OFFSET
			const size_t baseOffset	  = get_value_type_size(valueType);

			// GET ARRAY ALIGNMENT
			size_t arrayBaseAlignment = 0;
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
					arrayBaseAlignment =
					  traits_type::get_array_alignment(traits_type::get_scalar_alignment(valueType, base_struct::_context),
						base_struct::_context);
				}
				else { arrayBaseAlignment = traits_type::get_array_alignment(traits_type::get_scalar_alignment(valueType)); }

			// GET ALIGNMENT OFFSETS
			const std::vector<size_t> alignmentOffsets = _add_array(_currentOffset, arrayBaseAlignment, baseOffset, count);

			// GET ARRAY SIZE
			const size_t arraySize					   = alignmentOffsets.back() + baseOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t arrayPadding				   = _calculate_padding(_currentOffset, arrayBaseAlignment);

			// APPLY PADDING TO CURRENT OFFSET
			glsl_struct_assert(_move_current_offset(_currentOffset, alignmentOffsets.back() + baseOffset, arrayPadding),
			  "Data overflow!");

				// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			const auto scalarType = std::make_shared<scalar_type>(valueType, baseOffset);
			const auto arrayType  = std::make_shared<array_type>(scalarType, count, arraySize);

			_add_array_variable(name, alignmentOffsets, true, scalarType, arrayType).set_padding(arrayPadding);
		#else
			_add_array_variable(name, alignmentOffsets, true, baseOffset, arraySize).set_padding(arrayPadding);
		#endif

				// AFTER ADD SCALAR ARRAY
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_array_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_array(_currentOffset, arraySize, arrayBaseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_array(_currentOffset, arraySize, arrayBaseAlignment); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add(_currentOffset, arraySize, arrayBaseAlignment, base_struct::_context);
						}
						else { traits_type::after_add(_currentOffset, arraySize, arrayBaseAlignment); }
				}

			return alignmentOffsets;
		}

		/// @brief adds vec
		[[nodiscard]] size_t _add_vec(const std::string_view name, const size_t length, const ValueType valueType) {
			glsl_struct_assert(length >= 2 && length <= 4, "vec length should be in range <2, 4>!");

				// BEFORE ADD VEC
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_vec_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_vec(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_vec(_currentOffset); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add(_currentOffset); }
				}

			// GET BASE OFFSET
			const size_t baseOffset = get_value_type_size(valueType) * length;

			// GET BASE ALIGNMENT
			size_t baseAlignment	= 0;
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
					baseAlignment = traits_type::get_vec_alignment(valueType, length, base_struct::_context);
				}
				else { baseAlignment = traits_type::get_vec_alignment(valueType, length); }

			// GET ALIGNMENT OFFSET
			const size_t alignmentOffset = _add(_currentOffset, baseAlignment, baseOffset);

				// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			_add_variable(name, alignmentOffset, true, std::make_shared<vec_type>(valueType, length, baseOffset));
		#else
			_add_variable(name, alignmentOffset, true, baseOffset);
		#endif

				// AFTER ADD VEC
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_vec_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_vec(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_vec(_currentOffset, baseOffset, baseAlignment); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
						}
						else { traits_type::after_add(_currentOffset, baseOffset, baseAlignment); }
				}

			return alignmentOffset;
		}

		/// @brief adds array of vecs
		[[nodiscard]] std::vector<size_t> _add_vec_array(const std::string_view name, const size_t length,
		  const ValueType valueType, const size_t count) {
			glsl_struct_assert(length >= 2 && length <= 4, "vec length should be in range <2, 4>!");

				// BEFORE ADD VEC ARRAY
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_array_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_array(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_array(_currentOffset); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add(_currentOffset); }
				}

			// GET BASE OFFSET
			const size_t baseOffset	  = get_value_type_size(valueType) * length;

			// GET ARRAY ALIGNMENT
			size_t arrayBaseAlignment = 0;
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
					arrayBaseAlignment =
					  traits_type::get_array_alignment(traits_type::get_vec_alignment(valueType, length, base_struct::_context),
						base_struct::_context);
				}
				else { arrayBaseAlignment = traits_type::get_array_alignment(traits_type::get_vec_alignment(valueType, length)); }

			// GET ALIGNMENT OFFSETS
			const std::vector<size_t> alignmentOffsets = _add_array(_currentOffset, arrayBaseAlignment, baseOffset, count);

			// GET ARRAY SIZE
			const size_t arraySize					   = alignmentOffsets.back() + baseOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t arrayPadding				   = _calculate_padding(_currentOffset, arrayBaseAlignment);

			// APPLY PADDING TO CURRENT OFFSET
			glsl_struct_assert(_move_current_offset(_currentOffset, alignmentOffsets.back() + baseOffset, arrayPadding),
			  "Data overflow!");

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			const auto vecType	 = std::make_shared<vec_type>(valueType, length, baseOffset);
			const auto arrayType = std::make_shared<array_type>(vecType, count, arraySize);

			_add_array_variable(name, alignmentOffsets, true, vecType, arrayType).set_padding(arrayPadding);
		#else
			_add_array_variable(name, alignmentOffsets, true, baseOffset, arraySize).set_padding(arrayPadding);
		#endif

				// AFTER ADD VEC ARRAY
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_array_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_array(_currentOffset, arraySize, arrayBaseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_array(_currentOffset, arraySize, arrayBaseAlignment); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add(_currentOffset, arraySize, arrayBaseAlignment, base_struct::_context);
						}
						else { traits_type::after_add(_currentOffset, arraySize, arrayBaseAlignment); }
				}

			return alignmentOffsets;
		}

		/// @brief adds mat
		[[nodiscard]] std::vector<size_t> _add_mat(const std::string_view name, const size_t columns, const size_t rows,
		  const ValueType valueType) {
			glsl_struct_assert(columns >= 2 && columns <= 4, "mat columns should be in range <2, 4>!");
			glsl_struct_assert(rows >= 2 && rows <= 4, "mat rows should be in range <2, 4>!");

				// BEFORE ADD MAT
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_mat_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_mat(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_mat(_currentOffset); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add(_currentOffset); }
				}

			// GET BASE OFFSET
			const size_t vecBaseOffset = get_value_type_size(valueType) * rows;

			// GET MAT ALIGNMENT
			size_t matBaseAlignment	   = 0;
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
					matBaseAlignment =
					  traits_type::get_array_alignment(traits_type::get_vec_alignment(valueType, rows, base_struct::_context),
						base_struct::_context);
				}
				else { matBaseAlignment = traits_type::get_array_alignment(traits_type::get_vec_alignment(valueType, rows)); }

			// GET ALIGNMENT OFFSETS
			const std::vector<size_t> alignmentOffsets = _add_array(_currentOffset, matBaseAlignment, vecBaseOffset, columns);

			// GET MAT SIZE
			const size_t matSize					   = alignmentOffsets.back() + vecBaseOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t matPadding					   = _calculate_padding(_currentOffset, matBaseAlignment);

			// APPLY PADDING TO CURRENT OFFSET
			glsl_struct_assert(_move_current_offset(_currentOffset, alignmentOffsets.back() + vecBaseOffset, matPadding),
			  "Data overflow!");

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			const auto vecType = std::make_shared<vec_type>(valueType, rows, vecBaseOffset);
			const auto matType = std::make_shared<mat_type>(valueType, columns, rows, matSize);

			_add_array_variable(name, alignmentOffsets, true, vecType, matType).set_padding(matPadding);
		#else
			_add_array_variable(name, alignmentOffsets, true, vecBaseOffset, matSize).set_padding(matPadding);
		#endif

				// AFTER ADD VEC
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_mat_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_mat(_currentOffset, matSize, matBaseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_mat(_currentOffset, matSize, matBaseAlignment); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add(_currentOffset, matSize, matBaseAlignment, base_struct::_context);
						}
						else { traits_type::after_add(_currentOffset, matSize, matBaseAlignment); }
				}

			return alignmentOffsets;
		}

		/// @brief adds array of mats
		[[nodiscard]] std::vector<std::vector<size_t> > _add_mat_array(const std::string_view name, const size_t columns,
		  const size_t rows, const ValueType valueType, const size_t count) {
			glsl_struct_assert(columns >= 2 && columns <= 4, "mat columns should be in range <2, 4>!");
			glsl_struct_assert(rows >= 2 && rows <= 4, "mat rows should be in range <2, 4>!");

				// BEFORE ADD MAT ARRAY
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_array_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_array(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_array(_currentOffset); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add(_currentOffset); }
				}

			// GET BASE OFFSET
			const size_t vecBaseOffset = get_value_type_size(valueType) * rows;

			// GET ARRAY ALIGNMENT
			size_t matBaseAlignment	   = 0;
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
					matBaseAlignment =
					  traits_type::get_array_alignment(traits_type::get_vec_alignment(valueType, rows, base_struct::_context),
						base_struct::_context);
				}
				else { matBaseAlignment = traits_type::get_array_alignment(traits_type::get_vec_alignment(valueType, rows)); }

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
					glsl_struct_assert(_move_current_offset(_currentOffset, alignmentOffsets.back() + vecBaseOffset, matPadding),
					  "Data overflow!");

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
					var_data& varData =
					  _add_array_variable(get_array_elem_name(name, i), alignmentOffsets, false, vecType, matType);
		#else
					var_data& varData =
					  _add_array_variable(get_array_elem_name(name, i), alignmentOffsets, false, vecBaseOffset, matSize);
		#endif

						if (i < count - 1) { varData.set_padding(matPadding); }

					matsOffsets.push_back(alignmentOffsets.front());

					alignmentOffsetsPerMat.push_back(alignmentOffsets);
				}

			// GET ARRAY SIZE
			const size_t arraySize = matsOffsets.back() + matSize - matsOffsets.front();

			// APPLY PADDING TO CURRENT OFFSET
			glsl_struct_assert(_move_current_offset(_currentOffset, matsOffsets.back() + matSize, matPadding), "Data overflow!");

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			const auto arrayType = std::make_shared<array_type>(matType, count, arraySize);

			_add_variable(name, matsOffsets.front(), true, arrayType).set_padding(matPadding);
		#else
			_add_variable(name, matsOffsets.front(), true, arraySize).set_padding(matPadding);
		#endif

				// AFTER ADD SCALAR ARRAY
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_array_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_array(_currentOffset, arraySize, matBaseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_array(_currentOffset, arraySize, matBaseAlignment); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add(_currentOffset, arraySize, matBaseAlignment, base_struct::_context);
						}
						else { traits_type::after_add(_currentOffset, arraySize, matBaseAlignment); }
				}

			return alignmentOffsetsPerMat;
		}

		/// @brief adds struct
		[[nodiscard]] size_t _add_struct(const std::string_view name, const size_t baseAlignment, const size_t baseOffset,
		  const mstd::ordered_map<std::string, var_data>& values) {
				// BEFORE ADD STRUCT
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_struct_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_struct(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_struct(_currentOffset); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add(_currentOffset); }
				}

			// GET ALIGNMENT OFFSET
			const size_t alignmentOffset = _add(_currentOffset, baseAlignment, baseOffset);

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			_add_variable(name, alignmentOffset, true, std::make_shared<struct_type>(values, baseOffset));
		#else
			_add_variable(name, alignmentOffset, true, baseOffset);
		#endif

			// ADD STRUCTURE VARIABLES DATA
				for (const auto& [value_name, data] : values) {
		#if _GLSL_STRUCT_HAS_TYPES
					_add_variable(get_struct_elem_name(name, value_name), alignmentOffset + data.get_offset(), false,
					  data.get_type())
					  .set_padding(data.get_padding());
		#else
					_add_variable(get_struct_elem_name(name, value_name), alignmentOffset + data.get_offset(), false,
					  data.get_size())
					  .set_padding(data.get_padding());
		#endif
				}

				// AFTER ADD STRUCT
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_struct_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_struct(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_struct(_currentOffset, baseOffset, baseAlignment); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add(_currentOffset, baseOffset, baseAlignment, base_struct::_context);
						}
						else { traits_type::after_add(_currentOffset, baseOffset, baseAlignment); }
				}

			return alignmentOffset;
		}

		/// @brief adds array of structs
		[[nodiscard]] std::vector<size_t> _add_struct_array(const std::string_view name, const size_t baseAlignment,
		  const size_t baseOffset, const mstd::ordered_map<std::string, var_data>& values, const size_t count) {
				// BEFORE ADD SCALAR
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_array_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add_array(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add_array(_currentOffset); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_before_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::before_add(_currentOffset, base_struct::_context);
						}
						else { traits_type::before_add(_currentOffset); }
				}

			// GET ARRAY ALIGNMENT
			size_t arrayBaseAlignment = 0;
				if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
					arrayBaseAlignment = traits_type::get_array_alignment(baseAlignment, base_struct::_context);
				}
				else { arrayBaseAlignment = traits_type::get_array_alignment(baseAlignment); }

			// GET ALIGNMENT OFFSETS
			const std::vector<size_t> alignmentOffsets = _add_array(_currentOffset, arrayBaseAlignment, baseOffset, count);

			// GET ARRAY SIZE
			const size_t arraySize					   = alignmentOffsets.back() + baseOffset - alignmentOffsets.front();

			// GET ARRAY PADDING
			const size_t arrayPadding				   = _calculate_padding(_currentOffset, arrayBaseAlignment);

			// APPLY PADDING TO CURRENT OFFSET
			glsl_struct_assert(_move_current_offset(_currentOffset, _currentOffset, arrayPadding), "Data overflow!");

				for (size_t i = 0; i < count; ++i) {
					const std::string arrayElemName = get_array_elem_name(name, i);

						// ADD STRUCTURE VARIABLES DATA
						for (const auto& [value_name, data] : values) {
		#if _GLSL_STRUCT_HAS_TYPES
							_add_variable(get_struct_elem_name(arrayElemName, value_name),
							  alignmentOffsets[i] + data.get_offset(), false, data.get_type())
							  .set_padding(data.get_padding());
		#else
							_add_variable(get_struct_elem_name(arrayElemName, value_name),
							  alignmentOffsets[i] + data.get_offset(), false, data.get_size())
							  .set_padding(data.get_padding());
		#endif
						}
				}

		// ADD VARIABLE DATA
		#if _GLSL_STRUCT_HAS_TYPES
			const auto structType = std::make_shared<struct_type>(values, baseOffset);
			const auto arrayType  = std::make_shared<array_type>(structType, count, arraySize);

			_add_array_variable(name, alignmentOffsets, true, structType, arrayType).set_padding(arrayPadding);
		#else
			_add_array_variable(name, alignmentOffsets, true, baseOffset, arraySize).set_padding(arrayPadding);
		#endif

				// AFTER ADD SCALAR ARRAY
				if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_array_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add_array(_currentOffset, arraySize, arrayBaseAlignment, base_struct::_context);
						}
						else { traits_type::after_add_array(_currentOffset, arraySize, arrayBaseAlignment); }
				}
				else if _GLSL_STRUCT_CONSTEXPR17 (utils::has_layout_traits_after_add_v<traits_type>) {
						if _GLSL_STRUCT_CONSTEXPR17 (has_context) {
							traits_type::after_add(_currentOffset, arraySize, arrayBaseAlignment, base_struct::_context);
						}
						else { traits_type::after_add(_currentOffset, arraySize, arrayBaseAlignment); }
				}

			return alignmentOffsets;
		}

		/// @brief adds multiple variables
		template<class T, class... Ts, size_t Num, size_t... Nums>
		_GLSL_STRUCT_CONSTEXPR17 void _add_variables(const glsl_variable<T, Num>& var,
		  const glsl_variable<Ts, Nums>&... vars) noexcept {
				if _GLSL_STRUCT_CONSTEXPR17 (var.is_layout) {
						if _GLSL_STRUCT_CONSTEXPR17 (var.is_array) { add(var.varName, var.layout, Num); }
						else { add(var.varName, var.layout); }
				}
				else {
						if _GLSL_STRUCT_CONSTEXPR17 (var.is_array) { add<T>(var.varName, Num); }
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
		base_layout() noexcept _GLSL_STRUCT_REQUIRES(!has_context ||
													 (has_context && std::is_default_constructible_v<base_struct>))
			: base_struct() {
		}

		/// @brief constructor for multiple variables
		#if _GLSL_STRUCT_HAS_CXX20
		template<class... Ts, size_t... Nums>
		#else
		template<class... Ts, size_t... Nums,
		  std::enable_if_t<!has_context || (has_context && std::is_default_constructible_v<base_struct>), bool> = true>
		#endif
		explicit base_layout(
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
		explicit base_layout(const _GLSL_STRUCT_TYPENAME17 base_struct::context_type& ctx) noexcept _GLSL_STRUCT_REQUIRES(
		  has_context
		)
			: base_struct(ctx) {
		}

		/// @brief constructor for multiple variables and with provided context
		#if _GLSL_STRUCT_HAS_CXX20
		template<class... Ts, size_t... Nums>
		#else
		template<class... Ts, size_t... Nums, std::enable_if_t<has_context, bool> = true>
		#endif
		explicit base_layout(const glsl_variable<Ts, Nums>&... vars,
		  const _GLSL_STRUCT_TYPENAME17 base_struct::context_type& ctx) noexcept _GLSL_STRUCT_REQUIRES(has_context)
			: base_struct(ctx) {
			_add_variables(vars...);
		}

		#pragma endregion

		/// @brief default copy constructor
		base_layout(const base_layout& other) noexcept			  = default;

		/// @brief default move constructor
		base_layout(base_layout&& other) noexcept				  = default;

		/// @brief default destructor
		~base_layout() noexcept									  = default;

		/// @brief default copy assign operator
		base_layout& operator=(const base_layout& other) noexcept = default;

		/// @brief default move assign operator
		base_layout& operator=(base_layout&& other) noexcept	  = default;

		/// @brief value indicating error while returning offset
		[[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t bad_offset() noexcept { return std::numeric_limits<size_t>::max(); }

		/// @brief returns true if layout contains variable with given name
		[[nodiscard]] bool contains(const std::string_view name) const noexcept { return _variables.contains(name.data()); }

		/// @brief returns variable data
		[[nodiscard]] const var_data& get(const std::string_view name) const noexcept {
			glsl_struct_assert(contains(name), "The variable name doesn't exists!");
			return _variables.at(name.data());
		}

		/// @breif returns array count (if single value then returns 1 else if value doesn't exist returns 0 else array count)
		[[nodiscard]] size_t get_array_count(const std::string_view name) const noexcept {
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
		[[nodiscard]] std::vector<size_t> get_array_offsets(const std::string_view name) const noexcept {
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
		[[nodiscard]] const base_type_handle& get_type(const std::string_view name) const noexcept {
			return get(name).get_type();
		}

				/// @brief returns type of variable with casting to given type (dynamic casting occurs)
			#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_type Type>
			#else
		template<class Type, std::enable_if_t<utils::is_glsl_type_v<Type>, bool> = true>
			#endif
		[[nodiscard]] std::shared_ptr<Type> get_type(const std::string_view name) const noexcept {
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
		[[nodiscard]] std::vector<std::string> get_names() const noexcept {
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
		void clear() noexcept {
			_currentOffset = 0;
			_variables.clear();
		}

		#pragma region ADD_SCALAR
		/// @brief adds scalar
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar S>
		#else
		template<class S, std::enable_if_t<utils::is_glsl_scalar_v<S>, bool> = true>
		#endif
		size_t add(const std::string_view name) {
			static ValueType valueType = get_scalar_value_type<S>();
			return _add_scalar(name, valueType);
		}

		#pragma endregion

		#pragma region ADD_SCALARS_ARRAY

		/// @brief adds array of scalars
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalar S>
		#else
		template<class S, std::enable_if_t<utils::is_glsl_scalar_v<S>, bool> = true>
		#endif
		std::vector<size_t> add(const std::string_view name, const size_t count) {
			static ValueType valueType = get_scalar_value_type<S>();
			return _add_scalar_array(name, valueType, count);
		}

		/// @brief adds array of scalars
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_array SA>
		#else
		template<class SA, std::enable_if_t<utils::is_glsl_scalars_array_v<SA>, bool> = true>
		#endif
		std::vector<size_t> add(const std::string_view name, const size_t count) {
			using S = utils::array_value_type_t<SA>;
			return add<S>(name, count);
		}

		/// @brief adds array of scalars
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_scalars_static_size_array SA>
		#else
		template<class SA, std::enable_if_t<utils::is_glsl_scalars_static_size_array_v<SA>, bool> = true>
		#endif
		std::vector<size_t> add(const std::string_view name) {
			return add<SA>(name, utils::array_static_size_v<SA>);
		}

		#pragma endregion

		#pragma region ADD_VEC
		/// @brief adds vec
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		size_t add(const std::string_view name) {
			static ValueType valueType = get_vec_value_type<V>();
			static size_t length	   = get_vec_length<V>();
			return _add_vec(name, length, valueType);
		}

		#pragma endregion

		#pragma region ADD_VECS_ARRAY

		/// @brief adds array of vecs
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vec V>
		#else
		template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>
		#endif
		std::vector<size_t> add(const std::string_view name, const size_t count) {
			static ValueType valueType = get_vec_value_type<V>();
			static size_t length	   = get_vec_length<V>();
			return _add_vec_array(name, length, valueType, count);
		}

		/// @brief adds array of vecs
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_array VA>
		#else
		template<class VA, std::enable_if_t<utils::is_glsl_vecs_array_v<VA>, bool> = true>
		#endif
		std::vector<size_t> add(const std::string_view name, const size_t count) {
			using V = utils::array_value_type_t<VA>;
			return add<V>(name, count);
		}

		/// @brief adds array of vecs
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_vecs_static_size_array VA>
		#else
		template<class VA, std::enable_if_t<utils::is_glsl_vecs_static_size_array_v<VA>, bool> = true>
		#endif
		std::vector<size_t> add(const std::string_view name) {
			return add<VA>(name, utils::array_static_size_v<VA>);
		}

		#pragma endregion

		#pragma region ADD_MAT
		/// @brief adds mat
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		std::vector<size_t> add(const std::string_view name) {
			static ValueType valueType = get_mat_value_type<M>();
			static size_t columns	   = get_mat_columns<M>();
			static size_t rows		   = get_mat_rows<M>();
			return _add_mat(name, columns, rows, valueType);
		}

		#pragma endregion

		#pragma region ADD_MATS_ARRAY

		/// @brief adds array of mats
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mat M>
		#else
		template<class M, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>
		#endif
		std::vector<std::vector<size_t> > add(const std::string_view name, const size_t count) {
			static ValueType valueType = get_mat_value_type<M>();
			static size_t columns	   = get_mat_columns<M>();
			static size_t rows		   = get_mat_rows<M>();
			return _add_mat_array(name, columns, rows, valueType, count);
		}

		/// @brief adds array of mats
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_array MA>
		#else
		template<class MA, std::enable_if_t<utils::is_glsl_mats_array_v<MA>, bool> = true>
		#endif
		std::vector<std::vector<size_t> > add(const std::string_view name, const size_t count) {
			using M = utils::array_value_type_t<MA>;
			return add<M>(name, count);
		}

		/// @brief adds array of mats
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_mats_static_size_array MA>
		#else
		template<class MA, std::enable_if_t<utils::is_glsl_mats_static_size_array_v<MA>, bool> = true>
		#endif
		std::vector<std::vector<size_t> > add(const std::string_view name) {
			return add<MA>(name, utils::array_static_size_v<MA>);
		}

		#pragma endregion

		#pragma region ADD_STRUCT

		/// @brief adds struct with given layout
		size_t add(const std::string_view name, const base_layout& layout) {
			return _add_struct(name, layout.base_alignment(), layout._currentOffset, layout._variables);
		}

		#pragma endregion

		#pragma region ADD_STRUCTS_ARRAY

		/// @brief adds array of structs with given layout
		std::vector<size_t> add(const std::string_view name, const base_layout& layout, const size_t count) {
			return _add_struct_array(name, layout.base_alignment(), layout._currentOffset, layout._variables, count);
		}

		#pragma endregion

		/// @brief checks equality of layouts
		[[nodiscard]] bool operator==(const base_layout& other) const noexcept {
			return _currentOffset == other._currentOffset && _variables == other._variables;
		}

		/// @brief default not equal operator
		#if _GLSL_STRUCT_HAS_CXX20
		[[nodiscard]] bool operator!=(const base_layout& other) const noexcept = default;
		#else
		[[nodiscard]] bool operator!=(const base_layout& other) const noexcept { return !(*this == other); }
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
	[[nodiscard]] size_t operator()(const glslstruct::base_layout<Traits>& layout) const noexcept {
		size_t seed = mstd::hash_combine(layout._currentOffset, layout._maxAlignment);
			for (const auto& [name, data] : layout._variables) { mstd::hash_append(seed, name, data); }
		return seed;
	}
};
	#endif
#endif