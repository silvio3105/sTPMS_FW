/**
 * @file Main.cpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Main source file.
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

// ----- INCLUDE FILES
#include			"Main.hpp"
#include			"SEGGER_RTT.h"
#include			"sDebug.hpp"

#ifdef USING_RTOS
#include			"cmsis_os2.h"
#endif // RTOS

#include			<stdint.h>
#include 			<stdio.h>
#include			<string.h>


// ----- DEBUG
DEBUG_ENABLE_VERBOSE();
DEBUG_ENABLE_INFO();
DEBUG_ENABLE_ERROR();


// ----- DEFINES


// ----- MACRO FUNCTIONS


// ----- ENUMS


// ----- TYPEDEFS


// ----- STRUCTS


// ----- CLASSES	


// ----- VARIABLES


// ----- STATIC FUNCTION DECLARATIONS


// ----- FUNCTION DEFINITIONS
int main(void)
{
	_PRINT("Test\n");

	while (1)
	{
		
	}
}

void sDebug::out(const char* string, const uint16_t len)
{
	SEGGER_RTT_Write(0, string, len);
}


// ----- STATIC FUNCTION DEFINITIONS



// END WITH NEW LINE
