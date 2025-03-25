/**
 * @file PTS.cpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Pressure Temperature Sensor module source file.
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
#include			"PTS.hpp"
#include			"TWI.hpp"
#include 			"ILPS22QS.hpp"

#include			"nrf_gpio.h"

/**
 * @addtogroup PTS 
 * 
 * Pressure Temperature Sensor module.
 * @{
 */

// ----- STATIC FUNCTION DECLARATIONS
ILPS22QS::Return_t read(const uint8_t address, void* data, const uint8_t len, const uint8_t timeout);
ILPS22QS::Return_t write(const uint8_t address, void* data, const uint8_t len, const uint8_t timeout);
ILPS22QS::Return_t msp(void);


// ----- VARIABLES
ILPS22QS::I2C Sensor = ILPS22QS::I2C(read, write, msp, msp); /**< @brief ILPS22QS object. */


// ----- NAMESPACES
/**
 * @brief PTS module namespace.
 * 
 */
namespace PTS
{
	// ----- FUNCTION DEFINITIONS
	Return_t init(void)
	{
		if (Sensor.init() != ILPS22QS::Return_t::OK)
		{
			_PRINT_ERROR("Sensor init fail\n");
			return Return_t::NOK;
		}

		return Return_t::OK;
	}
};


// ----- STATIC FUNCTION DEFINITIONS
ILPS22QS::Return_t read(const uint8_t address, void* data, const uint8_t len, const uint8_t timeout)
{
	(void)timeout;
	
	if (TWI::read(address, data, len) != Return_t::OK)
	{
		return ILPS22QS::Return_t::NOK;
	}

	return ILPS22QS::Return_t::OK;
}

ILPS22QS::Return_t write(const uint8_t address, void* data, const uint8_t len, const uint8_t timeout)
{
	(void)timeout;

	if (TWI::write(address, data, len) != Return_t::OK)
	{
		return ILPS22QS::Return_t::NOK;
	}

	return ILPS22QS::Return_t::OK;	
}

ILPS22QS::Return_t msp(void)
{
	nrf_gpio_cfg(NRF_GPIO_PIN_MAP(Hardware::ptsSelectPort, Hardware::ptsSelectPin),
	NRF_GPIO_PIN_DIR_OUTPUT,
	NRF_GPIO_PIN_INPUT_DISCONNECT,
	NRF_GPIO_PIN_NOPULL,
	NRF_GPIO_PIN_S0S1,
	NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(Hardware::ptsSelectPort, Hardware::ptsSelectPin), 1);
	return ILPS22QS::Return_t::OK;
}


/** @} */

// END WITH NEW LINE
