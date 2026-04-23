/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_BASE_TYPE_HPP_
	#define _GLSL_STRUCT_BASE_TYPE_HPP_

	#include <glslstruct/config.hpp>

	#if !_GLSL_STRUCT_HAS_TYPES
_GLSL_STRUCT_ERROR(
  "This is only available for c++17 and greater and when types are not disabled with defined GLSL_STRUCT_DISABLE_TYPES!"
);
	#else

		#include <glslstruct/type/visitors/type_visitor_concept.hpp>

/**
 * @defgroup glsl_types Glsl type containers
 * @ingroup glslstruct
 * @brief type containers are for people that want to know of which type is value stored in struct
 */

namespace glslstruct {
	/**
	 * @brief base class for all glsl type containers
	 * @ingroup glsl_types
	 */
	class _GLSL_STRUCT_EXPORT base_type {
	private:
		friend struct std::hash<base_type>;

		/// @brief size value of type
		size_t _size;

	protected:
		/// @brief accept function for type visitors
		virtual void _accept(mstd::function_view<void(const scalar_type&)> scalarVisit,
		  mstd::function_view<void(const vec_type&)> vecVisit, mstd::function_view<void(const mat_type&)> matVisit,
		  mstd::function_view<void(const struct_type&)> structVisit,
		  mstd::function_view<void(const array_type&)> arrayVisit) const = 0;

	public:
		/// @brief constructor with type size
		explicit base_type(size_t size) noexcept;
		/// @brief default copy constructor
		base_type(const base_type& other) noexcept;
		/// @brief default move constructor
		base_type(base_type&& other) noexcept;
		/// @brief default virtual destructor
		virtual ~base_type() noexcept;

		/// @brief default copy assignment operator
		base_type& operator=(const base_type& other) noexcept;
		/// @brief default move assignment operator
		base_type& operator=(base_type&& other) noexcept;

		/// @brief accept function for type_visitors
		#if _GLSL_STRUCT_HAS_CXX20
		template<type_visitor V>
		#else
		template<class V, std::enable_if_t<is_type_visitor_v<V>, bool> = true>
		#endif
		void accept(V&& visitor) const {
			_accept([&visitor](const scalar_type& s) { visitor(s); }, [&visitor](const vec_type& v) { visitor(v); },
			  [&visitor](const mat_type& m) { visitor(m); }, [&visitor](const struct_type& s) { visitor(s); },
			  [&visitor](const array_type& a) { visitor(a); });
		}

		/// @brief virtual equality operator
		[[nodiscard]] virtual bool operator==(const base_type& other) const noexcept;
		/// @brief default not equal operator
		[[nodiscard]] bool operator!=(const base_type& other) const noexcept;
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_type T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> >
		#endif
		friend _GLSL_STRUCT_CONSTEXPR20 bool operator==(const base_type& lhs, const T& rhs) noexcept;
		#if _GLSL_STRUCT_HAS_CXX20
		template<utils::glsl_type T>
		#else
		template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> >
		#endif
		friend _GLSL_STRUCT_CONSTEXPR20 bool operator!=(const base_type& lhs, const T& rhs) noexcept;

		/// @brief returns size of type
		[[nodiscard]] size_t get_size() const noexcept;

		/// @brief converts type to string
		[[nodiscard]] virtual std::string to_string() const noexcept = 0;
	};

	/**
	 * @brief returns size of given type
	 * @ingroup glsl_types
	 */
	[[nodiscard]] _GLSL_STRUCT_EXPORT size_t sizeof_type(const base_type_handle& type) noexcept;

		/**
		 * @brief returns size of given type
		 * @ingroup glsl_types
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] _GLSL_STRUCT_EXPORT size_t sizeof_type(const std::shared_ptr<T>& type) noexcept {
		return type->get_size();
	}

	/**
	 * @brief converts type to string
	 * @ingroup glsl_types
	 */
	[[nodiscard]] _GLSL_STRUCT_EXPORT std::string to_string(const base_type_handle& type) noexcept;

		/**
		 * @brief converts type to string
		 * @ingroup glsl_types
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] _GLSL_STRUCT_EXPORT std::string to_string(const std::shared_ptr<T>& type) noexcept {
		return type->to_string();
	}

		#pragma region VISIT
		/**
		 * @brief visits type
		 * @ingroup glsl_types
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<type_visitor V>
		#else
	template<class V, std::enable_if_t<is_type_visitor_v<V>, bool> = true>
		#endif
	_GLSL_STRUCT_EXPORT void visit(V&& visitor, const base_type_handle& type) {
		type->accept(visitor);
	}

		/**
		 * @brief visits type
		 * @ingroup glsl_types
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T, type_visitor V>
		#else
	template<class T, class V, std::enable_if_t<utils::is_glsl_type_v<T> && is_type_visitor_v<V>, bool> = true>
		#endif
	_GLSL_STRUCT_EXPORT void visit(V&& visitor, const std::shared_ptr<T>& type) {
		type->accept(visitor);
	}

		/**
		 * @brief visits type
		 * @ingroup glsl_types
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<type_visitor V>
		#else
	template<class V, std::enable_if_t<is_type_visitor_v<V>, bool> = true>
		#endif
	_GLSL_STRUCT_EXPORT void visit(V&& visitor, const base_type& type) {
		type.accept(visitor);
	}

		/**
		 * @brief visits type
		 * @ingroup glsl_types
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T, type_visitor V>
		#else
	template<class T, class V, std::enable_if_t<utils::is_glsl_type_v<T> && is_type_visitor_v<V>, bool> = true>
		#endif
	_GLSL_STRUCT_EXPORT void visit(V&& visitor, const T& type) {
		type.accept(visitor);
	}

		#pragma endregion

		/**
		 * @brief checks if types are equal
		 * @ingroup glsl_types
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] _GLSL_STRUCT_EXPORT _GLSL_STRUCT_CONSTEXPR20 bool operator==(const base_type& lhs, const T& rhs) noexcept {
		return rhs == lhs;
	}

		/**
		 * @brief checks if types are not equal
		 * @ingroup glsl_types
		 */
		#if _GLSL_STRUCT_HAS_CXX20
	template<utils::glsl_type T>
		#else
	template<class T, std::enable_if_t<utils::is_glsl_type_v<T>, bool> = true>
		#endif
	[[nodiscard]] _GLSL_STRUCT_EXPORT _GLSL_STRUCT_CONSTEXPR20 bool operator!=(const base_type& lhs, const T& rhs) noexcept {
		return rhs != lhs;
	}
} // namespace glslstruct

/**
 * @brief std::hash overload for base_type
 * @ingroup glsl_types
 */
template<>
struct _GLSL_STRUCT_EXPORT std::hash<glslstruct::base_type> {
	[[nodiscard]] size_t operator()(const glslstruct::base_type& type) const noexcept;
};
	#endif
#endif