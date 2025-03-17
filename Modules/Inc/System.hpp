/**
 * @file System.hpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief System header file.
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

#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

// ----- INCLUDE FILES
#include			"Main.hpp"


// ----- NAMESPACES
namespace System
{
	// ----- ENUMS
	/**
	 * @brief Enum class for errors.
	 * 
	 * \ingroup System
	 */
	enum class Error_t : uint8_t
	{
		None = 0, /**< @brief No active error. */
	};

	/**
	 * @brief Enum class with reset reasons.
	 * 
	 */
	enum class ResetReason_t : uint8_t
	{
		Unknown = 0, /**< @brief Unknown reset reason. */
		Powerup = 1, /**< @brief Device powerup. */
	};

	
	// ----- FUNCTION DECLARATION
	Return_t init(void);
};


#endif // _SYSTEM_HPP_

// END WITH NEW LINE
