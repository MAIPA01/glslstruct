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
_GLSL_STRUCT_ERROR ("This is only available for c++17 and greater!");
#else

#include <glslstruct/utils/assert.hpp>
#include <glslstruct/layout_traits/layout_traits_concept.hpp>
#include <glslstruct/struct_elem/struct_elem_data.hpp>
#include <glslstruct/type_containers/array_type.hpp>
#include <glslstruct/type_containers/mat_type.hpp>
#include <glslstruct/type_containers/scalar_type.hpp>
#include <glslstruct/type_containers/vec_type.hpp>
#include <glslstruct/variable/glsl_variable.hpp>
#include <glslstruct/type_visitors/is_of_type_visitor.hpp>

namespace glslstruct {
#if _GLSL_STRUCT_HAS_CXX20
    template<layout_traits T>
#else
    template<class T, std::enable_if_t<is_layout_traits_v<T>, bool> >

#endif
    class base_layout {
    public:
        using traits_type = T;

    private:
        friend struct std::hash<base_layout<T>>;

        std::unordered_map<std::string, struct_elem_data> _values;
        size_t _currentOffset = 0;
        size_t _maxAlignment = 0;

        [[nodiscard]] static std::string _get_array_elem_name(std::string_view arrayName, size_t elemIdx) {
            return fmt::format("{}[{}]", arrayName, elemIdx);
        }

        [[nodiscard]] static std::string _get_struct_elem_name(std::string_view structName, std::string_view elemName) {
            return fmt::format("{}.{}", structName, elemName);
        }

        [[nodiscard]] static size_t _ceil_to_nearest_multiple(size_t valueToRoundUp, size_t multipleValue) noexcept {
            size_t modulo = valueToRoundUp % multipleValue;
            if (modulo != 0) { valueToRoundUp += multipleValue - modulo; }
            return valueToRoundUp;
        }

#if _GLSL_STRUCT_HAS_TYPES
        void _set_variable(std::string_view name, size_t offset, const base_type_handle &type, size_t padding) {
            _values.emplace(name, struct_elem_data(offset, type, padding));
        }
#else
        void _set_variable(std::string_view name, size_t offset, size_t size, size_t padding) {
            _values.emplace(name, struct_elem_data(offset, size, padding));
        }
#endif

        void _remove_variable(std::string_view name) {
            if (!contains(name)) { return; }

            _values.erase(name.data());
        }

        void _update_max_alignment(size_t baseAlignment) noexcept {
            _maxAlignment = std::max(baseAlignment, _maxAlignment);
        }

#if _GLSL_STRUCT_HAS_TYPES
        [[nodiscard]] size_t _add(std::string_view name, size_t baseAlignment, size_t baseOffset,
                                  const base_type_handle &type) {
#else
            [[nodiscard]] size_t _add(std::string_view name, size_t baseAlignment, size_t baseOffset) {
#endif
            // CHECK VARIABLE
            if (contains(name)) { return bad_offset(); }

            // CALCULATE ALIGNMENT
            const size_t alignmentOffset = _ceil_to_nearest_multiple(_currentOffset, baseAlignment);

            // CHECK IF NOT OVERFLOWED
            if (alignmentOffset < _currentOffset) { return bad_offset(); }

            // UPDATE SIZE
            if (mstd::add_overflow(alignmentOffset, baseOffset, _currentOffset)) { return bad_offset(); }

            // SET VARIABLE
#if _GLSL_STRUCT_HAS_TYPES
            _set_variable(name, alignmentOffset, type, 0);
#else
            _set_variable(name, alignmentOffset, baseOffset, 0);
#endif

            // UPDATE MAX ALIGEMENT
            _update_max_alignment(baseAlignment);

            return alignmentOffset;
        }

#if _GLSL_STRUCT_HAS_TYPES
        [[nodiscard]] std::vector<size_t> _add_array(std::string_view name, size_t count, size_t elemBaseAlignment,
                                                     size_t elemBaseOffset, const base_type_handle &elemType) {
#else
            [[nodiscard]] std::vector<size_t> _add_array(std::string_view name, size_t count, size_t elemBaseAlignment,
                                                         size_t baseOffset) {
#endif
            // CHECK SIZE
            if (count == 0) {
                glsl_struct_assert(false, "Count cannot be 0.");
                return std::vector<size_t>();
            }

            // CHECK VARIABLE
            if (contains(name)) {
                glsl_struct_assert(false, "Layout already contains value with name {}", name);
                return std::vector<size_t>();
            }

            // SET BASE ALIGNMENT
            size_t baseAlignment = traits_type::get_array_alignment(elemBaseAlignment);
            size_t valuePadding = baseAlignment - elemBaseAlignment;

            // CALCULATE ALIGNMENT OFFSET
            size_t alignmentOffset = _ceil_to_nearest_multiple(_currentOffset, baseAlignment);

            // CHECK OVERFLOW
            if (alignmentOffset < _currentOffset) {
                glsl_struct_assert(false, "Data overflow would happen!");
                return std::vector<size_t>();
            }

            // UPDATE SIZE
            size_t arraySize = 0;
            if (mstd::mul_overflow(baseAlignment, count, arraySize) ||
                mstd::add_overflow(alignmentOffset, arraySize, _currentOffset)) {
                glsl_struct_assert(false, "Data overflow would happen!");
                return std::vector<size_t>();
            }

            // SET ARRAY BEGIN POINTER
#if _GLSL_STRUCT_HAS_TYPES
            _set_variable(name, alignmentOffset, std::make_shared<array_type>(elemType, count, arraySize), 0);
#else
            _set_variable(name, alignmentOffset, arraySize, 0);
#endif

            // ADD ARRAY VALUES
            std::vector<size_t> arrayElemOffsets(count);
            for (size_t i = 0; i < count; ++i) {
                // SET ELEMENT VALUE OFFSET
                arrayElemOffsets[i] = alignmentOffset;

                // SET ELEMENT VARIABLE
                _set_variable(_get_array_elem_name(name, i), alignmentOffset, elemType, elemBaseOffset, valuePadding);

                // ADD BASE ALIGNMENT
                alignmentOffset += baseAlignment;
            }

            // UPDATE MAX ALIGEMENT
            _update_max_alignment(baseAlignment);

            return arrayElemOffsets;
        }

        [[nodiscard]] size_t _add_scalar(std::string_view name, ValueType valueType) {
#if _GLSL_STRUCT_HAS_TYPES
            size_t scalarSize = get_value_type_size(valueType);
            return _add(name, traits_type::get_scalar_alignment(valueType), scalarSize,
                        std::make_shared<scalar_type>(valueType, scalarSize));
#else
            return _add(name, traits_type::get_scalar_alignment(valueType), get_value_type_size(valueType));
#endif
        }

        [[nodiscard]] std::vector<size_t> _add_scalar_array(std::string_view name, ValueType valueType, size_t count) {
#if _GLSL_STRUCT_HAS_TYPES
            size_t scalarSize = get_value_type_size(valueType);
            return _add_array(name, count, traits_type::get_scalar_alignment(valueType), scalarSize,
                              std::make_shared<scalar_type>(valueType, scalarSize));
#else
            return _add_array(name, count, traits_type::get_scalar_alignment(valueType),
                              get_value_type_size(valueType));
#endif
        }

        [[nodiscard]] size_t _add_vec(std::string_view name, size_t length, ValueType valueType) {
            if (length < 2 || length > 4) {
                glsl_struct_assert(false, "vec length should be in range <2, 4>!");
                return bad_offset();
            }

#if _GLSL_STRUCT_HAS_TYPES
            const size_t vecSize = length * get_value_type_size(valueType);
            return _add(name, traits_type::get_vec_alignment(valueType, length), vecSize,
                        std::make_shared<vec_type>(valueType, length, vecSize));
#else
            return _add(name, traits_type::get_vec_alignment(valueType, length), get_value_type_size(valueType));
#endif
        }

        [[nodiscard]] std::vector<size_t> _add_vec_array(std::string_view name, size_t length, ValueType valueType,
                                                         size_t count) {
            if (length < 2 || length > 4) {
                glsl_struct_assert(false, "vec length should be in range <2, 4>!");
                return std::vector<size_t>();
            }

#if _GLSL_STRUCT_HAS_TYPES
            const size_t vecSize = length * get_value_type_size(valueType);
            return _add_array(name, count, traits_type::get_vec_alignment(valueType, length), vecSize,
                              std::make_shared<vec_type>(valueType, length, vecSize));
#else
            return _add_array(name, count, traits_type::get_vec_alignment(valueType, length),
                              get_value_type_size(valueType)
#endif
        }

        [[nodiscard]] size_t _add_mat(std::string_view name, size_t columns, size_t rows, ValueType valueType) {
            const size_t arrayCount = columns;
            const size_t vecLength = rows;

            // PRE CHECK IF MAT WILL FIT
            size_t alignment = traits_type::get_mat_alignment(valueType, columns, rows);

            size_t matSize = _ceil_to_nearest_multiple(_currentOffset, alignment);

            if (mstd::mul_overflow(alignment, arrayCount, alignment) ||
                mstd::add_overflow(matSize, alignment, matSize)) {
                glsl_struct_assert(false, "data overflow would happen!");
                return bad_offset();
            }

            // ADD VEC ARRAY
            std::vector<size_t> vecsOffsets = _add_vec_array(name, vecLength, valueType, arrayCount);

            // CHANGE ARRAY TYPE TO MAT TYPE
            struct_elem_data &varData = _values.at(name.data());
#if _GLSL_STRUCT_HAS_TYPES
            varData.set_type(std::make_shared<mat_type>(valueType, columns, rows, matSize));
#endif

            return varData.get_offset();
        }

        [[nodiscard]] std::vector<size_t> _add_mat_array(std::string_view name, size_t columns, size_t rows,
                                                         ValueType valueType,
                                                         size_t count) {
            // CHECK ARRAY SIZE
            if (count == 0) {
                glsl_struct_assert(false, "count cannot be zero!");
                return std::vector<size_t>();
            }

            // CHECK VARIABLE
            if (contains(name)) {
                glsl_struct_assert(false, "variable with given name already exists!");
                return std::vector<size_t>();
            }

            // PRE CHECK IF MATS ARRAY WILL FIT
            const size_t vecArrayCount = columns;
            const size_t vecLength = rows;

            size_t matArrayAlignment = traits_type::get_array_alignment(
                traits_type::get_mat_alignment(valueType, columns, rows));

            size_t matArraySize = _ceil_to_nearest_multiple(_currentOffset, matArrayAlignment);

            size_t alignment = 0;
            if (mstd::mul_overflow(matArrayAlignment, vecArrayCount, matArrayAlignment) ||
                mstd::mul_overflow(matArrayAlignment, count, alignment) ||
                mstd::add_overflow(matArraySize, alignment, matArraySize)) {
                glsl_struct_assert(false, "data overflow would happen!");
                return std::vector<size_t>();
            }

            // ADD ARRAY ELEMENTS
            std::vector<size_t> values(count);
            for (size_t i = 0; i != count; ++i) {
                values[i] = _add_mat(_get_array_elem_name(name, i), columns, rows, valueType);
            }

            // SET ARRAY BEGIN POINTER
#if _GLSL_STRUCT_HAS_TYPES
            _set_variable(name, values[0], std::make_shared<array_type>(valueType, columns, rows, count, matArraySize),
                          alignment,
                          0);
#else
            _set_variable(name, values[0], matArraySize, alignment, 0);
#endif

            // RETURN
            return values;
        }

        [[nodiscard]] size_t _add_struct(std::string_view name, size_t baseAlignment, size_t baseOffset,
                                         const std::unordered_map<std::string, struct_elem_data> &values) {
            size_t alignmentOffset = _add(name, baseAlignment, baseOffset,
                                          std::make_shared<struct_type>(values, baseOffset));

            // CHECK RESULT
            if (alignmentOffset == bad_offset()) {
                glsl_struct_assert(false, "failed to add struct");
                return alignmentOffset;
            }

            for (const auto &[value_name, data]: values) {
#if _GLSL_STRUCT_HAS_TYPES
                _set_variable(_get_struct_elem_name(name, value_name), alignmentOffset + data.get_offset(),
                              data.get_type(),
                              data.get_padding());
#else
                _set_variable(_get_struct_elem_name(name, value_name), alignmentOffset + data.get_offset(),
                              data.get_size(), data.get_padding());
#endif
            }

            // ADD PADDING
            size_t oldCurrentOffset = _currentOffset;
            static _GLSL_STRUCT_CONSTEXPR17 const size_t offset_alignment = 16;
            _currentOffset = _ceil_to_nearest_multiple(_currentOffset, offset_alignment);

            _values.at(name.data()).set_padding(_currentOffset - oldCurrentOffset);

            return alignmentOffset;
        }

        [[nodiscard]] std::vector<size_t> _add_struct_array(std::string_view name, size_t baseAlignment,
                                                                    size_t baseOffset,
                                                                    const std::unordered_map<std::string,
                                                                        struct_elem_data> &values, size_t count) {
            // CHECK ARRAY COUNT
            if (count == 0) {
                glsl_struct_assert(false, "count can't be zero!");
                return std::vector<size_t>();
            }

            // CHECK NAME
            if (contains(name)) { return std::vector<size_t>(); }

            // CHECK IF STRUCTS ARRAY WILL FIT
            size_t alignmentOffset = _ceil_to_nearest_multiple(_currentOffset,
                                                               traits_type::get_array_alignment(baseAlignment));

            size_t structEndOffset = 0;
            if (mstd::add_overflow(alignmentOffset, baseOffset, structEndOffset)) { return std::vector<size_t>(); }

            static _GLSL_STRUCT_CONSTEXPR17 const size_t offset_alignment = 16;
            structEndOffset = _ceil_to_nearest_multiple(structEndOffset, offset_alignment);

            size_t structTotalSize = structEndOffset - alignmentOffset;
            size_t arrayTotalSize = 0;
            if (mstd::mul_overflow(structTotalSize, count, arrayTotalSize) ||
                mstd::add_overflow(alignmentOffset, arrayTotalSize, alignmentOffset)) {
                return std::vector<size_t>();
            }

            std::vector<size_t> valuesOffsets(count);
            for (size_t i = 0; i < count; ++i) {
                valuesOffsets[i] = _add_struct(_get_array_elem_name(name, i), baseAlignment, baseOffset, values);
            }

            // SET ARRAY BEGIN POINTER
            _set_variable(name, valuesOffsets[0], std::make_shared<array_type>(values, baseOffset, count),
                          arrayTotalSize, 0);

            // RETURN
            return valuesOffsets;
        }

        template<class T, class... Ts, size_t num, size_t... nums>
        _GLSL_STRUCT_CONSTEXPR17 void _add_variables(const glsl_variable<T, num> &var,
                                                     const glsl_variable<Ts, nums> &... vars) noexcept {
            if _GLSL_STRUCT_CONSTEXPR17 (var.is_layout) {
                if _GLSL_STRUCT_CONSTEXPR17 (num == 0) { add(var.var_name, var.layout); } else {
                    add(var.var_name, var.layout, num);
                }
            } else {
                if _GLSL_STRUCT_CONSTEXPR17 (num == 0) { add<T>(var.var_name); } else { add<T>(var.var_name, num); }
            }
            if _GLSL_STRUCT_CONSTEXPR17 (sizeof...(Ts) > 0 && sizeof...(nums) > 0) { _add_variables(vars...); }
        }

    public:
        base_layout() noexcept = default;

        template<class... Ts, size_t... nums>
        explicit base_layout(const glsl_variable<Ts, nums> &... vars) noexcept {
            _add_multiple(vars...);
        }

        base_layout(const base_layout &other) noexcept = default;

        base_layout(base_layout &&other) noexcept = default;

        ~base_layout() noexcept = default;

        base_layout &operator=(const base_layout &other) noexcept = default;

        base_layout &operator=(base_layout &&other) noexcept = default;

        [[nodiscard]] static _GLSL_STRUCT_CONSTEXPR17 size_t bad_offset() noexcept { return std::numeric_limits<size_t>::max(); }

        [[nodiscard]] bool contains(std::string_view name) const noexcept {
#if _GLSL_STRUCT_HAS_CXX20
            return _values.contains(name.data());
#else
            return _values.find(name.data()) != _values.end();
#endif
        }

        [[nodiscard]] struct_elem_data get(std::string_view name) const noexcept {
            if (!contains(name)) {
#if _GLSL_STRUCT_HAS_TYPES
                return struct_elem_data(bad_offset(), nullptr, 0);
#else
                return struct_elem_data(bad_offset(), 0, 0);
#endif
            }

            return _values.at(name.data());
        }

        [[nodiscard]] size_t get_offset(std::string_view name) const noexcept { return get(name).get_offset(); }

        [[nodiscard]] std::vector<size_t> get_array_offsets(std::string_view name) const noexcept {
            if (!contains(name)) {
                glsl_struct_assert(false, "There is no array with given name!");
                return std::vector<size_t>();
            }

            const base_type_handle& varType = _values.at(name.data()).get_type();

            size_t arraySize;
            if (is_of_type<mat_type>(varType)) { arraySize = static_type_cast<mat_type>(varType)->get_array_count(); }
            else if (is_of_type<array_type>(varType)) { arraySize = static_type_cast<array_type>(varType)->get_count(); }
            else { return std::vector<size_t>({ _values.at(name.data()).get_offset() }); }

            std::vector<size_t> values(arraySize);
            for (size_t i = 0; i != arraySize; ++i) { values[i] = _values.at(_get_array_elem_name(name, i)).get_offset(); }

            return values;
        }

#if _GLSL_STRUCT_HAS_TYPES
        [[nodiscard]] const base_type_handle &get_type(std::string_view name) const noexcept { return get(name).get_type(); }
#if _GLSL_STRUCT_HAS_CXX20
        template<utils::glsl_type T>
#else
        template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>

#endif
        [[nodiscard]] std::shared_ptr<T> get_type(std::string_view name) const noexcept {
            return dynamic_type_cast<T>(get_type(name));
        }
#endif

        [[nodiscard]] size_t get_total_size(std::string_view name) const noexcept { return get(name).get_total_size(); }

        [[nodiscard]] size_t get_size(std::string_view name) const noexcept { return get(name).get_size(); }

        [[nodiscard]] size_t get_padding(std::string_view name) const noexcept { return get(name).get_padding(); }

        [[nodiscard]] std::vector<std::string> get_names() const noexcept {
            std::vector<std::string> names;
            names.reserve(_values.size());

            for (const auto& [name, data] : _values) { names.push_back(name); }

            return names;
        }

        [[nodiscard]] size_t base_alignment() const noexcept {
            static _GLSL_STRUCT_CONSTEXPR17 const size_t alignment = 16;
            return _ceil_to_nearest_multiple(_maxAlignment, alignment);
        }

        [[nodiscard]] size_t size() const noexcept {
            static _GLSL_STRUCT_CONSTEXPR17 const size_t offset_alignment = 16;
            return _ceil_to_nearest_multiple(_currentOffset, offset_alignment);
        }

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
        [[nodiscard]] size_t add(const std::string_view &name) {
            static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
            static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

            return _add_scalar(name, typeSize, typeSize, valueType);
        }

#if _GLSL_STRUCT_HAS_CXX20
        template<utils::glsl_scalar T>
#else
        template<class T, std::enable_if_t<utils::is_glsl_scalar_v<T>, bool> = true>

#endif
        [[nodiscard]] std::vector<size_t> add(const std::string_view &name, size_t count) {
            static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
            static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

            return _add_scalar_array(name, typeSize, typeSize, valueType, count);
        }

#pragma endregion

#pragma region ADD_VEC
#if _GLSL_STRUCT_HAS_CXX20
        template<utils::glsl_vec V>
#else
        template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>

#endif
        [[nodiscard]] size_t add(const std::string_view &name) {
            using T = glsl_vec_value_type<V>;
            static _GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;

            static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
            static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

            return _add_vec(name, L, typeSize, typeSize, valueType);
        }

#if _GLSL_STRUCT_HAS_CXX20
        template<utils::glsl_vec V>
#else
        template<class V, std::enable_if_t<utils::is_glsl_vec_v<V>, bool> = true>

#endif
        [[nodiscard]] std::vector<size_t> add(const std::string_view &name, size_t count) {
            using T = glsl_vec_value_type<V>;
            static _GLSL_STRUCT_CONSTEXPR17 const size_t L = glsl_vec_length<V>;

            static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
            static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

            return _add_vec_array(name, L, typeSize, typeSize, valueType, count);
        }

#pragma endregion

#pragma region ADD_MAT
#if _GLSL_STRUCT_HAS_CXX20
        template<utils::glsl_mat M, MajorType major = MajorType::Column>
#else
        template<class M, MajorType major = MajorType::Column, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>

#endif
        [[nodiscard]] size_t add(const std::string_view &name) {
            using T = glsl_mat_value_type<M>;
            static _GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
            static _GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

            static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
            static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

            return _add_mat(name, C, R, major, typeSize, typeSize, valueType);
        }

#if _GLSL_STRUCT_HAS_CXX20
        template<utils::glsl_mat M, MajorType major = MajorType::Column>
#else
        template<class M, MajorType major = MajorType::Column, std::enable_if_t<utils::is_glsl_mat_v<M>, bool> = true>

#endif
        [[nodiscard]] std::vector<size_t> add(const std::string_view &name, size_t count) {
            using T = glsl_mat_value_type<M>;
            static _GLSL_STRUCT_CONSTEXPR17 const size_t C = glsl_mat_columns<M>;
            static _GLSL_STRUCT_CONSTEXPR17 const size_t R = glsl_mat_rows<M>;

            static _GLSL_STRUCT_CONSTEXPR17 const ValueType valueType = get_value_type<T>();
            static _GLSL_STRUCT_CONSTEXPR17 const size_t typeSize = get_value_type_size(valueType);

            return _add_mat_array(name, C, R, major, typeSize, typeSize, valueType, count);
        }

#pragma endregion

#pragma region ADD_STRUCT
#if _GLSL_STRUCT_HAS_CXX20
        template<utils::glsl_layout Layout>
#else
        template<class Layout, std::enable_if_t<utils::is_glsl_layout_v<Layout>, bool> = true>

#endif
        [[nodiscard]] size_t add(const std::string_view &name, const Layout &layout) {
            return _add_struct(name, layout.base_alignment(), layout._currentOffset, layout._values);
        }

#if _GLSL_STRUCT_HAS_CXX20
        template<utils::glsl_layout Layout>
#else
        template<class Layout, std::enable_if_t<utils::is_glsl_layout_v<Layout>, bool> = true>

#endif
        [[nodiscard]] std::vector<size_t> add(const std::string_view &name, const Layout &layout, size_t count) {
            return _add_struct_array(name, layout.base_alignment(), layout._currentOffset, layout._values, count);
        }

#pragma endregion

        [[nodiscard]] bool operator==(const base_layout &other) const noexcept {
            return _currentOffset == other._currentOffset && _maxAlignment == other._maxAlignment && _values == other._values;
        }

#if _GLSL_STRUCT_HAS_CXX20
        [[nodiscard]] bool operator!=(const base_layout& other) const noexcept = default;
#else
        [[nodiscard]] bool operator!=(const base_layout& other) const noexcept { return !(*this == other); }
#endif
    };
} // namespace glslstruct

#if _GLSL_STRUCT_HAS_CXX20
template<glslstruct::layout_traits T>
#else
template<class T, std::enable_if_t<glslstruct::is_layout_traits_v<T>, bool> = true>
#endif
struct std::hash<glslstruct::base_layout<T>> {
    [[nodiscard]] size_t operator()(const glslstruct::base_layout<T> &layout) const noexcept {
        size_t seed = mstd::hash_combine(layout._currentOffset, layout._maxAlignment);
        for (const auto& [name, data] : layout._values) { mstd::hash_append(seed, name, data); }
        return seed;
    }
};
#endif
#endif