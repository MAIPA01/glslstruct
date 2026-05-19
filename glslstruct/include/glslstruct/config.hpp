/*
 * glslstruct - a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/glslstruct/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _GLSL_STRUCT_CONFIG_HPP_
	#define _GLSL_STRUCT_CONFIG_HPP_

/**
 *	@defgroup glslstruct GLSLStruct
 *	@brief Main group
 */

/**
 * @defgroup utils Utilities
 * @brief group with utilities (not for end user to use)
 * @ingroup glslstruct
 */

/**
 * @namespace glslstruct
 * @brief Main namespace of glslstruct library
 */

/**
 * @namespace glslstruct::uitls
 * @brief Utility namespace of glslstruct library
 */

	#pragma region VERSION
	/**
	 * @brief glslstruct version major number
	 * @ingroup glslstruct
	 */
	#define GLSL_STRUCT_VERSION_MAJOR 1
	/**
	 * @brief glslstruct version minor number
	 * @ingroup glslstruct
	 */
	#define GLSL_STRUCT_VERSION_MINOR 4
	/**
	 * @brief glslstruct version patch number
	 * @ingroup glslstruct
	 */
	#define GLSL_STRUCT_VERSION_PATCH 8

	/**
	 * @brief stringify helper
	 * @ingroup utils
	 */
	#define _GLSL_STRUCT_STRINGIFY_HELPER(x) #x

	/**
	 * @brief converts version numbers to string
	 * @ingroup utils
	 */
	#define _GLSL_STRUCT_VERSION_TO_STRING(major, minor, patch)                                                                \
		_GLSL_STRUCT_STRINGIFY_HELPER(major) "." _GLSL_STRUCT_STRINGIFY_HELPER(minor) "." _GLSL_STRUCT_STRINGIFY_HELPER(patch)
	/**
	 * @brief converts version number to int
	 * @ingroup utils
	 */
	#define _GLSL_STRUCT_VERSION_TO_INT(major, minor, patch) (major * 100 + minor * 10 + patch)

	/**
	 * @brief glslstruct version string
	 * @ingroup glslstruct
	 */
	#define GLSL_STRUCT_VERSION_STRING                                                                                  \
		_GLSL_STRUCT_VERSION_TO_STRING(GLSL_STRUCT_VERSION_MAJOR, GLSL_STRUCT_VERSION_MINOR, GLSL_STRUCT_VERSION_PATCH)
	/**
	 * @brief glslstruct version int
	 * @ingroup glslstruct
	 */
	#define GLSL_STRUCT_VERSION_INT                                                                                  \
		_GLSL_STRUCT_VERSION_TO_INT(GLSL_STRUCT_VERSION_MAJOR, GLSL_STRUCT_VERSION_MINOR, GLSL_STRUCT_VERSION_PATCH)
	/**
	 * @brief glslstruct version string
	 * @ingroup glslstruct
	 */
	#define GLSL_STRUCT_VERSION GLSL_STRUCT_VERSION_STRING
	#pragma endregion

	#pragma region LAST_UPDATE
	/**
	 * @brief glslstruct last update day
	 * @ingroup glslstruct
	 */
	#define GLSL_STRUCT_LAST_UPDATE_DAY 19
	/**
	 * @brief glslstruct last update month
	 * @ingroup glslstruct
	 */
	#define GLSL_STRUCT_LAST_UPDATE_MONTH 05
	/**
	 * @brief glslstruct last update year
	 * @ingroup glslstruct
	 */
	#define GLSL_STRUCT_LAST_UPDATE_YEAR 2026

	/**
	 * @brief converts last update date to string
	 * @ingroup utils
	 */
	#define _GLSL_STRUCT_LAST_UPDATE_DATE_HELPER(day, month, year)                                                          \
		_GLSL_STRUCT_STRINGIFY_HELPER(day) "." _GLSL_STRUCT_STRINGIFY_HELPER(month) "." _GLSL_STRUCT_STRINGIFY_HELPER(year)

	/**
	 * @brief glslstruct last update date string
	 * @ingroup glslstruct
	 */
	#define GLSL_STRUCT_LAST_UPDATE_DATE                                                                 \
		_GLSL_STRUCT_LAST_UPDATE_DATE_HELPER(GLSL_STRUCT_LAST_UPDATE_DAY, GLSL_STRUCT_LAST_UPDATE_MONTH, \
		  GLSL_STRUCT_LAST_UPDATE_YEAR)
	#pragma endregion

	#pragma region CXX_VERSIONS
/**
 * @def _GLSL_STRUCT_HAS_CXX17
 * @brief check if compiler has c++ version greater or equal to c++17
 * @ingroup utils
 */
	#ifndef _HAS_CXX17
	// clang-format off
		#define _GLSL_STRUCT_HAS_CXX17 __cplusplus >= 201703l
	// clang-format on
	#else
		#define _GLSL_STRUCT_HAS_CXX17 _HAS_CXX17
	#endif

	/**
	 * @def _GLSL_STRUCT_HAS_CXX20
	 * @brief check if compiler has c++ version greater or equal to c++20 and if user enabled c++20 features using GLSL_STRUCT_ENABLE_CXX20
	 * @ingroup utils
	 */
	#ifndef GLSL_STRUCT_ENABLE_CXX20
		#define _GLSL_STRUCT_HAS_CXX20 0
	#elif !defined(_HAS_CXX20)
	// clang-format off
		#define _GLSL_STRUCT_HAS_CXX20 __cplusplus >= 202002l && GLSL_STRUCT_ENABLE_CXX20
	// clang-format on
	#else
		#define _GLSL_STRUCT_HAS_CXX20 _HAS_CXX20&& GLSL_STRUCT_ENABLE_CXX20
	#endif
	#pragma endregion

	#pragma region TYPES
/**
 * @def _GLSL_STRUCT_HAS_TYPES
 * @brief check if user not disabled type containers using GLSL_STRUCT_DISABLE_TYPES
 * @ingroup utils
 */

	#ifdef GLSL_STRUCT_DISABLE_TYPES
		#define _GLSL_STRUCT_HAS_TYPES 0
	#else
		#define _GLSL_STRUCT_HAS_TYPES _GLSL_STRUCT_HAS_CXX17
	#endif

/**
 * @def _GLSL_STRUCT_HAS_TYPE_CHECKS
 * @brief check if user enabled type checks for struct getters using GLSL_STRUCT_ENABLE_TYPE_CHECKS (remember it only works if GLSL_STRUCT_DISABLE_TYPES was not set)
 * @ingroup utils
 */
	#if defined(GLSL_STRUCT_ENABLE_TYPE_CHECKS) && !defined(GLSL_STRUCT_DISABLE_TYPES)
		#define _GLSL_STRUCT_HAS_TYPE_CHECKS 1
	#else
		#define _GLSL_STRUCT_HAS_TYPE_CHECKS 0
	#endif
	#pragma endregion

/**
 * @def _GLSL_STRUCT_HAS_PARSER
 * @brief check if user want to include parser functionality
 * @ingroup utils
 */

	#ifdef GLSL_STRUCT_DISABLE_PARSER
		#define _GLSL_STRUCT_HAS_PARSER 0
	#else
		#define _GLSL_STRUCT_HAS_PARSER _GLSL_STRUCT_HAS_CXX17
	#endif

	#pragma region VERSION_SPECIFIC_VALUES
/**
 * @def _GLSL_STRUCT_CONSTEXPR17
 * @brief constexpr for c++17 and higher
 * @ingroup utils
 */
	#if _GLSL_STRUCT_HAS_CXX17
		#define _GLSL_STRUCT_CONSTEXPR17 constexpr
	#else
		#define _GLSL_STRUCT_CONSTEXPR17
	#endif

/**
 * @def _GLSL_STRUCT_CONSTEXPR20
 * @brief constexpr keyword for c++20 and higher
 * @ingroup utils
 */

/**
 * @def _GLSL_STRUCT_REQUIRES(condition)
 * @brief requires keyword for c++20 and higher
 * @ingroup utils
 */
	#if _GLSL_STRUCT_HAS_CXX20
		#define _GLSL_STRUCT_CONSTEXPR20		 constexpr
		#define _GLSL_STRUCT_REQUIRES(condition) requires (condition)
		#define _GLSL_STRUCT_INLINE17
		#define _GLSL_STRUCT_TYPENAME17
	#else
		#define _GLSL_STRUCT_CONSTEXPR20
		#define _GLSL_STRUCT_REQUIRES(condition)
		#if _GLSL_STRUCT_HAS_CXX17
			#define _GLSL_STRUCT_INLINE17	inline
			#define _GLSL_STRUCT_TYPENAME17 typename
		#else
			#define _GLSL_STRUCT_INLINE17
			#define _GLSL_STRUCT_TYPENAME17
		#endif
	#endif
	#pragma endregion

/**
 * @def _GLSL_STRUCT_EXPORT
 * @brief This is for exporting symbols in shared library setup
 * @ingroup utils
 */

	#ifdef GLSL_STRUCT_SHARED_LIBS
		#ifdef GLSL_STRUCT_EXPORT
			#ifdef _WIN32
				#define _GLSL_STRUCT_EXPORT __declspec(dllexport)
			#else
				#define _GLSL_STRUCT_EXPORT __attribute__((visibility("default")))
			#endif
		#else
			#ifdef _WIN32
				#define _GLSL_STRUCT_EXPORT __declspec(dllimport)
			#else
				#define _GLSL_STRUCT_EXPORT
			#endif
		#endif
	#else
		#define _GLSL_STRUCT_EXPORT
	#endif

	#include <mstd/mstd.hpp>

	/**
	 * @brief compiler message
	 * @ingroup utils
	 */
	#define _GLSL_STRUCT_MESSAGE(MESSAGE) _MSTD_MESSAGE(MESSAGE)
	/**
	 * @brief compiler warning
	 * @ingroup utils
	 */
	#define _GLSL_STRUCT_WARNING(MESSAGE) _MSTD_WARNING(MESSAGE)
	/**
	 * @brief compiler error
	 * @ingroup utils
	 */
	#define _GLSL_STRUCT_ERROR(MESSAGE) _MSTD_ERROR(MESSAGE)
#endif