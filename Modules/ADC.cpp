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
#include			"nrf_saadc.h"
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
		nrf_saadc_int_enable(NRF_SAADC_INT_END);
		nrf_saadc_resolution_set(NRF_SAADC_RESOLUTION_12BIT);
		nrf_saadc_oversample_set(NRF_SAADC_OVERSAMPLE_16X);
		nrf_saadc_buffer_init((nrf_saadc_value_t*)&adcRaw, 1);

		// Configure ADC channel
		static const nrf_saadc_channel_config_t chConfig =
		{
			.resistor_p = NRF_SAADC_RESISTOR_DISABLED,
			.resistor_n = NRF_SAADC_RESISTOR_DISABLED,
			.gain = NRF_SAADC_GAIN1_6,
			.reference = NRF_SAADC_REFERENCE_INTERNAL,
			.acq_time = NRF_SAADC_ACQTIME_40US,
			.mode = NRF_SAADC_MODE_SINGLE_ENDED,
			.burst = NRF_SAADC_BURST_ENABLED,
			.pin_p = NRF_SAADC_INPUT_VDD,
			.pin_n = NRF_SAADC_INPUT_DISABLED		
		};
		nrf_saadc_channel_init(0, &chConfig);

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

		nrf_saadc_enable();
		nrf_saadc_task_trigger(NRF_SAADC_TASK_START);
		nrf_saadc_task_trigger(NRF_SAADC_TASK_SAMPLE);
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
		_PRINT("ADC IRQ\n");
		sd_nvic_ClearPendingIRQ(SAADC_IRQn);

		if (nrf_saadc_event_check(NRF_SAADC_EVENT_END))
		{
			nrf_saadc_event_clear(NRF_SAADC_EVENT_END);
			nrf_saadc_task_trigger(NRF_SAADC_TASK_STOP);
			nrf_saadc_disable();

			voltage = (600 * ((adcRaw * 1000) / 4096) * 6) / 1000;
			_PRINTF("ADC %u %u\n", adcRaw, voltage);
		}
	}
}


/** @} */

// END WITH NEW LINE