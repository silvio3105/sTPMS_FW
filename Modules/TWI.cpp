/**
 * @file TWI.cpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief TWI module source file.
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
#include			"TWI.hpp"

#include			"nrf.h"
#include			"nrf_gpio.h"
#include 			"nrf_twim.h"
#include			"nrf_nvic.h"
#include			"app_error.h"


/**
 * @addtogroup TWI 
 * 
 * TWI module.
 */

// ----- VARIABLES
static uint8_t error = 0;


// ----- NAMESPACES
/**
 * @brief TWI module namespace.
 * 
 */
namespace TWI
{
	// ----- FUNCTION DEFINITIONS
	Return_t init(void)
	{
		// Configure TWI pins
		/*nrf_gpio_cfg(NRF_GPIO_PIN_MAP(Hardware::ptsSDAPort, Hardware::ptsSDAPin),
		NRF_GPIO_PIN_DIR_OUTPUT,
		NRF_GPIO_PIN_INPUT_DISCONNECT,
		NRF_GPIO_PIN_NOPULL,
		NRF_GPIO_PIN_S0D1,
		NRF_GPIO_PIN_NOSENSE);
		
		nrf_gpio_cfg(NRF_GPIO_PIN_MAP(Hardware::ptsSCLPort, Hardware::ptsSCLPin),
		NRF_GPIO_PIN_DIR_OUTPUT,
		NRF_GPIO_PIN_INPUT_DISCONNECT,
		NRF_GPIO_PIN_NOPULL,
		NRF_GPIO_PIN_S0D1,
		NRF_GPIO_PIN_NOSENSE);*/

		// Enable interrupts
		nrf_twim_int_enable(NRF_TWIM0, NRF_TWIM_INT_ERROR_MASK | NRF_TWIM_INT_LASTTX_MASK | NRF_TWIM_INT_LASTRX_MASK);
		ret_code_t ret = sd_nvic_SetPriority(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn, 3);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}

		ret = sd_nvic_EnableIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}

		// Set TWI speed to 100kHz
		nrf_twim_frequency_set(NRF_TWIM0, NRF_TWIM_FREQ_100K);

		// Enable shortcuts
		nrf_twim_shorts_enable(NRF_TWIM0, NRF_TWIM_SHORT_LASTRX_STOP_MASK);

		// Set TWI pins
		nrf_twim_pins_set(NRF_TWIM0, Hardware::ptsSCLPin, Hardware::ptsSDAPin);

		// Clear TWI errors
		nrf_twim_errorsrc_get_and_clear(NRF_TWIM0);

		// Enable TWI bus
		nrf_twim_enable(NRF_TWIM0);

		return Return_t::OK;
	}

	Return_t deinit(void)
	{
		ret_code_t ret = sd_nvic_DisableIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}

		nrf_twim_int_disable(NRF_TWIM0, NRF_TWIM_INT_ERROR_MASK | NRF_TWIM_INT_LASTTX_MASK | NRF_TWIM_INT_LASTRX_MASK);

		nrf_twim_task_trigger(NRF_TWIM0, NRF_TWIM_TASK_STOP);
		nrf_twim_disable(NRF_TWIM0);

		return Return_t::OK;
	}

	Return_t write(const uint8_t address, const void* data, const uint16_t len)
	{
		nrf_twim_tx_buffer_set(NRF_TWIM0, (const uint8_t*)data, len);
		nrf_twim_address_set(NRF_TWIM0, (address << 1) | 1);
		nrf_twim_task_trigger(NRF_TWIM0, NRF_TWIM_TASK_STARTTX);

		while (NRF_TWIM0->EVENTS_LASTTX == 0)
		{
			if (error)
			{
				error = 0;
				nrf_twim_task_trigger(NRF_TWIM0, NRF_TWIM_TASK_STOP);
				return Return_t::NOK;
			}
		}
		NRF_TWIM0->EVENTS_LASTTX = 0;
		return Return_t::OK;
	}

	Return_t read(const uint8_t address, void* output, const uint16_t len)
	{
		nrf_twim_rx_buffer_set(NRF_TWIM0, (uint8_t*)output, len);
		nrf_twim_address_set(NRF_TWIM0, address << 1);
		nrf_twim_task_trigger(NRF_TWIM0, NRF_TWIM_TASK_STARTRX);	
		
		while (NRF_TWIM0->EVENTS_LASTRX == 0)
		{
			if (error)
			{
				error = 0;
				nrf_twim_task_trigger(NRF_TWIM0, NRF_TWIM_TASK_STOP);
				return Return_t::NOK;
			}
		}
		NRF_TWIM0->EVENTS_LASTRX = 0;		
		return Return_t::OK;
	}

	Return_t isErrorActive(void)
	{
		if (error)
		{
			error = 0;
			return Return_t::OK;
		}

		return Return_t::NOK;
	}
};


extern "C"
{
	void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler(void)
	{
		sd_nvic_ClearPendingIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);

		if (NRF_TWIM0->EVENTS_LASTRX)
		{
			_PRINT("TWI RX done\n");
			NRF_TWIM0->EVENTS_LASTRX = 0;
		}

		else if (NRF_TWIM0->EVENTS_LASTTX)
		{
			_PRINT("TWI TX done\n");
			NRF_TWIM0->EVENTS_LASTTX = 0;
		}
		
		else if (NRF_TWIM0->EVENTS_ERROR)
		{
			error = 1;
			_PRINTF_ERROR("TWI error %u\n", nrf_twim_errorsrc_get_and_clear(NRF_TWIM0));
			NRF_TWIM0->EVENTS_ERROR = 0;
		}		
	}	
};


/** @} */

// END WITH NEW LINE
