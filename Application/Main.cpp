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
#include			"BLE.hpp"

#include 			"nrf_log.h"
#include 			"nrf_log_ctrl.h"
#include 			"nrf_log_default_backends.h"

#include			<stdint.h>
#include 			<stdio.h>
#include			<string.h>


// ----- FUNCTION DEFINITIONS
int main(void)
{
	#ifdef DEBUG
	NRF_LOG_INIT(NULL);
	NRF_LOG_DEFAULT_BACKENDS_INIT();
	#endif // DEBUG

	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 1;
	_PRINT("Wait\n");
	while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
	_PRINT("Started\n");
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;	

	for (uint32_t i = 0; i < 0xFFFFFF; i++)
	{
		(void)i;
	}

	if (BLE::init() != Return_t::OK)
	{
		_PRINT_ERROR("BLE init fail\n");
	}
	else
	{
		_PRINT_INFO("BLE init OK\n");
	}

	_PRINT("Start\n");

	uint8_t cnt = 0;
	while (1)
	{
		BLE::advertise(++cnt);
		_PRINT("Loop\n");
		for (uint32_t i = 0; i < 0xFFFFFF; i++)
		{
			(void)i;
		}
	}
}

void sDebug::out(const char* string, const uint16_t len)
{
	SEGGER_RTT_Write(0, string, len);
}


// ----- STATIC FUNCTION DEFINITIONS



// END WITH NEW LINE
