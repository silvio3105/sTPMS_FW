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


// ----- ENUMS
/**
 * @brief Enum class with application states.
 * 
 */
enum class State_t : uint8_t
{
	Sleep = 0, /**< @brief Application sleep state. */
	Measure, /**< @brief Application measure state. */
	Advertise, /**< @brief Application advertise state. */
};


// ----- VARIABLES
static Data::sTPMS data = Data::sTPMS(); /**< @brief sTPMS data to advertise. */
static State_t state = State_t::Advertise; /**< @brief Application state. */
static uint8_t wakeupSet = 0; /**< @brief Wakeup timer flag. If set to \c 1 wakeup timer has started. */


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

	// Init device data and SRAM EEPROM
	Data::init(data);

	// Init system
	if (System::init() != Return_t::OK)
	{
		_PRINT_ERROR("System init fail\n");
		System::reset(System::Reset_t::SystemInit);
	}

	// Init BLE module
	if (BLE::init() != Return_t::OK)
	{
		_PRINT_ERROR("BLE init fail\n");
		System::reset(System::Reset_t::BLEInit);
	}

	while (1)
	{
		switch (state)
		{
			case State_t::Measure:
			{
				// SOON: Add measure stuff
				state = State_t::Advertise;
				break;
			}

			case State_t::Advertise:
			{
				// Advertise sTPMS data
				BLE::advertise(&data, sizeof(data));

				// Wait for advertise to end
				while (BLE::isAdvertiseDone() == Return_t::OK);

				// Feed the dog
				System::feedWatchdog();

				// Go to sleep
				state = State_t::Sleep;
				break;
			}

			default:
			{
				// Start wakeup timer if needed
				if (!wakeupSet)
				{
					wakeupSet = 1;
					System::startWakeupTimer();
				}

				// Put device to sleep
				System::sleep();
				break;
			}
		}

		// Woken by RTC2
		if (System::isWoken() == Return_t::OK)
		{
			wakeupSet = 0;
			state = State_t::Measure;
		}		
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
