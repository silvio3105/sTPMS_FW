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

// ----- STATIC FUNCTION DECLARATIONS
static Return_t isErrorActive(void);


// ----- VARIABLES
static volatile uint8_t error = 0; /**< @brief TWI bus error flag. */


// ----- NAMESPACES
/**
 * @brief TWI module namespace.
 * 
 * Only for TWIM0.
 */
namespace TWI
{
	// ----- FUNCTION DEFINITIONS
	/**
	 * @brief Init TWI bus.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	Return_t init(void)
	{
		// Enable interrupts
		nrf_twim_int_enable(NRF_TWIM0, NRF_TWIM_INT_ERROR_MASK);
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

		nrf_twim_int_enable(NRF_TWIM0, NRF_TWIM_INT_ERROR_MASK);

		// Set TWI pins
		nrf_twim_pins_set(NRF_TWIM0, Hardware::ptsSCLPin, Hardware::ptsSDAPin);

		// Clear TWI errors
		nrf_twim_errorsrc_get_and_clear(NRF_TWIM0);

		// Enable TWI bus
		nrf_twim_enable(NRF_TWIM0);

		return Return_t::OK;
	}

	/**
	 * @brief Deinit TWI bus.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	Return_t deinit(void)
	{
		ret_code_t ret = sd_nvic_DisableIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}

		nrf_twim_int_disable(NRF_TWIM0, NRF_TWIM_INT_ERROR_MASK);
		nrf_twim_task_trigger(NRF_TWIM0, NRF_TWIM_TASK_STOP);
		nrf_twim_disable(NRF_TWIM0);

		nrf_gpio_cfg_default(NRF_GPIO_PIN_MAP(Hardware::ptsSCLPort, Hardware::ptsSDAPin));
		nrf_gpio_cfg_default(NRF_GPIO_PIN_MAP(Hardware::ptsSDAPort, Hardware::ptsSDAPin));

		return Return_t::OK;
	}

	/**
	 * @brief Write to TWI bus.
	 * 
	 * @param address Address of a slave on TWI bus.
	 * @param data Pointer to data to write.
	 * @param len Length of \c data
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	Return_t write(const uint8_t address, const void* data, const uint16_t len)
	{
		nrf_twim_address_set(NRF_TWIM0, address);
		nrf_twim_tx_buffer_set(NRF_TWIM0, (const uint8_t*)data, len);
		nrf_twim_task_trigger(NRF_TWIM0, NRF_TWIM_TASK_STARTTX);

		while (!nrf_twim_event_check(NRF_TWIM0, NRF_TWIM_EVENT_LASTTX))
		{
			if (isErrorActive() == Return_t::OK)
			{
				nrf_twim_task_trigger(NRF_TWIM0, NRF_TWIM_TASK_STOP);
				return Return_t::NOK;
			}
		}

		nrf_twim_event_clear(NRF_TWIM0, NRF_TWIM_EVENT_LASTTX);		
		return Return_t::OK;
	}

	/**
	 * @brief Read from TWI bus.
	 * 
	 * @param address Address of a slave to read from.
	 * @param output Pointer to output.
	 * @param len Number of bytes to read.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	Return_t read(const uint8_t address, void* output, const uint16_t len)
	{
		nrf_twim_address_set(NRF_TWIM0, address);
		nrf_twim_rx_buffer_set(NRF_TWIM0, (uint8_t*)output, len);
		nrf_twim_shorts_enable(NRF_TWIM0, NRF_TWIM_SHORT_LASTRX_STOP_MASK);
		nrf_twim_task_trigger(NRF_TWIM0, NRF_TWIM_TASK_STARTRX);

		while (!nrf_twim_event_check(NRF_TWIM0, NRF_TWIM_EVENT_STOPPED))
		{
			if (isErrorActive() == Return_t::OK)
			{
				nrf_twim_task_trigger(NRF_TWIM0, NRF_TWIM_TASK_STOP);
				return Return_t::NOK;
			}
		}

		nrf_twim_event_clear(NRF_TWIM0, NRF_TWIM_EVENT_STOPPED);
		return Return_t::OK;
	}
};


// ----- STATIC FUNCTION DEFINITIONS
/**
 * @brief Check is TWI bus error active.
 * 
 * @return \c Return_t::NOK no error on TWI bus.
 * @return \c Return_t::OK error on TWI bus. 
 */
static Return_t isErrorActive(void)
{
	if (error)
	{
		error = 0;
		return Return_t::OK;
	}

	return Return_t::NOK;
}


// ----- INTERRUPTS
extern "C"
{
	/**
	 * @brief TWI0 bus interrupt handler.
	 * 
	 * @return No return value.
	 */
	void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler(void)
	{
		sd_nvic_ClearPendingIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);

		if (nrf_twim_event_check(NRF_TWIM0, NRF_TWIM_EVENT_ERROR))
		{
			error = 1;
			_PRINTF_ERROR("TWI error %u\n", nrf_twim_errorsrc_get_and_clear(NRF_TWIM0));
			nrf_twim_event_clear(NRF_TWIM0, NRF_TWIM_EVENT_ERROR);
			nrf_twim_task_trigger(NRF_TWIM0, NRF_TWIM_TASK_STOP);
		}		
	}	
};


/** @} */

// END WITH NEW LINE
