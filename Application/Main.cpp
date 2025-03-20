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
#include			"System.hpp"
#include			"Data.hpp"

#include 			"nrf_log.h"
#include 			"nrf_log_ctrl.h"
#include 			"nrf_log_default_backends.h"

#include			<stdint.h>
#include 			<stdio.h>
#include			<string.h>


// ----- VARIABLES
Data::sTPMS data = Data::sTPMS(); /**< @brief sTPMS data to advertise. */


// ----- STATIC FUNCTION DECLARATIONS


// ----- FUNCTION DEFINITIONS
/**
 * @brief Application entry point.
 * 
 * @return No return value.
 */
int main(void)
{
	#ifdef DEBUG
	NRF_LOG_INIT(NULL);
	NRF_LOG_DEFAULT_BACKENDS_INIT();
	#endif // DEBUG

	// Init device data
	Data::init(data);

	// Init system
	if (System::init() != Return_t::OK)
	{
		_PRINT_ERROR("System init fail\n");
		// SOON: Add reset
	}

	// Init BLE module
	if (BLE::init() != Return_t::OK)
	{
		_PRINT_ERROR("BLE init fail\n");
	}
	else
	{
		_PRINT_INFO("BLE init OK\n");
	}

	//BLE::advertise(&data, sizeof(data));

	while (1)
	{
		/*if (System::isWoken() == Return_t::OK)
		{
			BLE::advertise(&data, sizeof(data));
		}*/

		//if (BLE::isAdvertiseDone() == Return_t::OK)
		if (1)
		{
			System::startWakeupTimer();
			System::sleep();
		}

		// Feed the dog
		System::feedWatchdog();
	}
}

/**
 * @brief Debug output handler.
 * 
 * @param string Pointer to string to output.
 * @param len Length of \c string.
 * 
 * @return No return value.
 */
void sDebug::out(const char* string, const uint16_t len)
{
	SEGGER_RTT_Write(AppConfig::rttChannel, string, len);
}


// ----- STATIC FUNCTION DEFINITIONS


// END WITH NEW LINE
