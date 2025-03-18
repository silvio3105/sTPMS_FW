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

#include 			"nrf_log.h"
#include 			"nrf_log_ctrl.h"
#include 			"nrf_log_default_backends.h"

#include			<stdint.h>
#include 			<stdio.h>
#include			<string.h>


// ----- STRUCTS
/**
 * @brief sTPMS data packet.
 * 
 */
struct sTPMSData_s
{
	uint16_t pressure; /**< @brief Measured pressure in mbar. */
	int16_t temperature; /**< @brief Measured temperature in centi degrees celsius. */
	uint16_t uptime; /**< @brief Device uptime in hours. */
	System::Error_t errorCode; /**< @brief Last error code. See \ref System::Error_t. */
	uint8_t voltage; /**< @brief Battery voltage in centivolts. Offset: 200cV. */
	uint8_t fwVer[3]; /**< @brief sTPMS firmware version - major, minor, build. */
	const AppConfig::Hardware_t hwID = AppConfig::hwID; /**< @brief sTPMS hardware ID. See \ref AppConfig::Hardware_t. */
	System::ResetReason_t rstReason; /**< @brief Reset reason. See \ref System::ResetReason_t. */
	uint8_t rstCount; /**< @brief Reset counter. */
};


// ----- VARIABLES
static sTPMSData_s data; /**< @brief Sensor data to advertise. */


// ----- STATIC FUNCTION DECLARATIONS
static void initTPMSData(void);
static void updateTPMSData(void);


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

	_PRINTF("%d bytes\n", sizeof(data));

	// Init system
	if (System::init() != Return_t::OK)
	{
		_PRINT_ERROR("System init fail\n");
		// SOON: Add reset
	}

	// Init sTPMS data struct
	initTPMSData();

	// Init BLE module
	if (BLE::init() != Return_t::OK)
	{
		_PRINT_ERROR("BLE init fail\n");
	}
	else
	{
		_PRINT_INFO("BLE init OK\n");
	}

	while (1)
	{
		updateTPMSData();
		BLE::advertise(&data, sizeof(data));
		for (uint32_t i = 0; i < 0xFFFFFF; i++)
		{
			(void)i;
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
/**
 * @brief Init advertise data struct.
 * 
 * @return No return value.
 */
static void initTPMSData(void)
{
	// SOON: Set real values

	// Set firmware version
	data.fwVer[0] = 1;
	data.fwVer[1] = 2;
	data.fwVer[2] = 3;

	// Set reset stuff
	data.rstReason = System::ResetReason_t::Powerup;
	data.rstCount = 1;
}

/**
 * @brief Update advertise data struct.
 * 
 * @return No return value.
 */
static void updateTPMSData(void)
{
	// SOON: Set real values

	// Update errors
	data.errorCode = System::Error_t::None;

	// Set measured values
	data.pressure = 1013;
	data.temperature = 2552;
	data.voltage = 120;

	// Set uptime in hours
	data.uptime++;
	//data.uptime = 1;
}


// END WITH NEW LINE
