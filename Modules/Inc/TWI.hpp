/**
 * @file TWI.hpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief I2C module header file.
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

#ifndef _TWI_HPP_
#define _TWI_HPP_

// ----- INCLUDE FILES
#include			"Main.hpp"


// ----- NAMESPACES
namespace TWI
{
	// ----- FUNCTION DECLARATIONS
	Return_t init(void);
	Return_t deinit(void);
	Return_t send(const uint8_t address, const void* data, const uint16_t len);
	Return_t receive(const uint8_t address, const void* output, const uint16_t len);
};


#endif // _TWI_HPP_

// END WITH NEW LINE
