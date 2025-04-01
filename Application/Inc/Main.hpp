/**
 * @file Main.hpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Main header file.
 * 
 * @copyright Copyright (c) 2025, silvio3105
 * 
 */

/*
	Copyright (c) 2025, silvio3105 (www.github.com/silvio3105)

	Access and use of this Project and its contents are granted free of charge to any Person.
	The Person is allowed to copy, modify and use The Project and its contents only for non-commercial use.
	Commercial use of this Project and its contents is prohibited.
	Modifying this License and/or sublicensing is prohibited.

	THE PROJECT AND ITS CONTENT ARE PROVIDED "AS IS" WITH ALL FAULTS AND WITHOUT EXPRESSED OR IMPLIED WARRANTY.
	THE AUTHOR KEEPS ALL RIGHTS TO CHANGE OR REMOVE THE CONTENTS OF THIS PROJECT WITHOUT PREVIOUS NOTICE.
	THE AUTHOR IS NOT RESPONSIBLE FOR DAMAGE OF ANY KIND OR LIABILITY CAUSED BY USING THE CONTENTS OF THIS PROJECT.

	This License shall be included in all functional textual files.
*/

#ifndef _MAIN_HPP_
#define _MAIN_HPP_

// ----- INCLUDE FILES
#include			"AppConfig.hpp"
#include 			"TPMS1.hpp"
#include			"sDebug.hpp"

#include			<stdint.h>


// ----- DEFINES


// ----- MACRO FUNCTIONS
/**
 * @brief Calculate constant string length.
 * 
 * @param _input Input C-string.
 * 
 * @return Length of \c _input C-string.
 */
#define __CONST_STR_LEN(_input) \
	(sizeof(_input) - 1)

/**
 * @brief Calculate array members.
 * 
 * @param _input Input array.
 * 
 * @return Number of members in array.
 */
#define __ARRAY_LEN(_input) \
	(sizeof(_input) / sizeof(_input[0]))

// ----- ENUMS
/**
 * @brief Enum class with return values.
 * 
 */
enum class Return_t : uint8_t
{
	OK = 0, /**< @brief OK return value. */
	NOK, /**< @brief Not OK return value. */
	Timeout /**< @brief Timeout return value. */
};


// ----- TYPEDEFS


// ----- STRUCTS


// ----- CLASSES


// ----- NAMESPACES


// ----- FUNCTION DECLARATIONS


// ----- DEBUG
#ifdef DEBUG_VERBOSE
DEBUG_ENABLE_VERBOSE();
#else // DEBUG_VERBOSE
DEBUG_DISABLE_VERBOSE();
#endif // DEBUG_VERBOSE

#ifdef DEBUG_INFO
DEBUG_ENABLE_INFO();
#else // DEBUG_INFO
DEBUG_DISABLE_INFO();
#endif // DEBUG_INFO

#ifdef DEBUG_ERROR
DEBUG_ENABLE_ERROR();
#else // DEBUG_ERROR
DEBUG_DISABLE_ERROR();
#endif // DEBUG_ERROR



#endif // _MAIN_HPP_

// END WITH NEW LINE
