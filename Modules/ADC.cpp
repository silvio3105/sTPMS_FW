/**
 * @file ADC.cpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief ADC module source file.
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
#include			"ADC.hpp"

#include			"nrf.h"
#include			"nrf_nvic.h"
#include			"sdk_errors.h"
#include			"app_error.h"


/**
 * @addtogroup ADC 
 * 
 * ADC module.
 * @{
 */

// ----- VARIABLES
static uint16_t adcRaw = 0; /**< @brief Output variable for ADC. */
static uint16_t voltage = 0; /**< @brief Calculated voltage in mV. */


// ----- NAMESPACES
/**
 * @brief ADC module namespace.
 * 
 */
namespace ADC
{
	/**
	 * @brief Init ADC.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success. 
	 */
	Return_t init(void)
	{
		// Enable END event
		NRF_SAADC->INTEN = 1 << 1;

		// Configure CH0 to VDD (Vref_int, 1/6 input gain, burst enabled)
		NRF_SAADC->CH[0].PSELN = 0;
		NRF_SAADC->CH[0].PSELP = 9;
		NRF_SAADC->CH[0].CONFIG = (2 << 16) | (1 << 24);

		// Set 12-bit resolution
		NRF_SAADC->RESOLUTION = 2;

		// Configure oversampling
		NRF_SAADC->OVERSAMPLE = 4;

		// Set output buffer
		NRF_SAADC->RESULT.PTR = (uint32_t)&adcRaw;
		NRF_SAADC->RESULT.MAXCNT = 1;

		// Enable ADC IRQ
		ret_code_t ret = sd_nvic_SetPriority(SAADC_IRQn, 3);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}

		ret = sd_nvic_EnableIRQ(SAADC_IRQn);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}
		
		return Return_t::OK;
	}

	/**
	 * @brief Measure battery voltage.
	 * 
	 * @return No return value.
	 */
	void measure(void)
	{
		voltage = 0;
		adcRaw = 0;
		NRF_SAADC->ENABLE = 1;
		NRF_SAADC->TASKS_START = 1;
		NRF_SAADC->TASKS_SAMPLE = 1;
	}

	/**
	 * @brief Check if ADC is done with measuring.
	 * 
	 * @return \c Return_t::NOK ADC is not done.
	 * @return \c Return_t::OK ADC is done.
	 */
	Return_t isDone(void)
	{
		if (voltage)
		{
			return Return_t::OK;
		}

		return Return_t::NOK;
	}

	/**
	 * @brief Get battery voltage.
	 * 
	 * @return Battery voltage in mV.
	 */
	uint16_t getVoltage(void)
	{
		return voltage;
	}
};


// ----- INTERRUPTS
extern "C"
{
	/**
	 * @brief ADC interrupt handler.
	 * 
	 * @return No return value.
	 */
	void SAADC_IRQHandler(void)
	{
		sd_nvic_ClearPendingIRQ(SAADC_IRQn);

		if (NRF_SAADC->EVENTS_END)
		{
			voltage = (600 * ((adcRaw * 1000) / 4096) * 6) / 1000;
			NRF_SAADC->EVENTS_END = 0;
			NRF_SAADC->TASKS_STOP = 1;
			NRF_SAADC->ENABLE = 0;
			_PRINTF("ADC %u %u\n", adcRaw, voltage);
		}
	}
}


/** @} */

// END WITH NEW LINE