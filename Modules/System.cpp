/**
 * @file System.cpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief System source file.
 * 
 * @copyright Copyright (c) 2025, silvio3105
 * 
 */

// ----- INCLUDE FILES
#include			"System.hpp"
#include			"Data.hpp"

#include			"nrf.h"
#include			"nrf_nvic.h"
#include 			"app_error.h"
#include 			"nrf_soc.h"


/**
 * @addtogroup System
 * 
 * System module
 * @{
 */

// ----- VARIABLES
static volatile uint8_t wakeup = 0; /**< @brief Flag for RTC2 wakeup event. */
static System::Reset_t resetReason = System::Reset_t::Unknown; /**< @brief Reset reason. */


// ----- STATIC FUNCTION DECLARATIONS
static void stopWakeupTimer(void);


// ----- NAMESPACES
/**
 * @brief System namespace.
 * 
 */
namespace System
{
	/**
	 * @brief Init system components.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	Return_t init(void)
	{
		// Test crystals only in debug build
		#ifdef DEBUG
		_PRINT("Wait for HFXO\n");
		NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
		NRF_CLOCK->TASKS_HFCLKSTART = 1;
		while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
		NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;	
		_PRINT("HFXO started\n");

		_PRINT("Wait for LFXO\n");
		NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
		NRF_CLOCK->TASKS_LFCLKSTART = 1;
		while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);
		NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;	
		_PRINT("LFXO started\n");		
		#endif // DEBUG

		// Configure power
		NRF_POWER->POFCON = 0;
		NRF_POWER->DCDCEN = 1;

		// Init watchdog
		NRF_WDT->CRV = (AppConfig::wdtTimeout * 32768) - 1;
		#ifdef DEBUG
		NRF_WDT->CONFIG = 1;
		#else
		NRF_WDT->CONFIG = 9;
		#endif // DEBUG
		NRF_WDT->TASKS_START = 1;

		// Read reset reason
		uint32_t tmp = NRF_POWER->RESETREAS;
		NRF_POWER->RESETREAS = 0;

		// Check HW reset reasons if custom one is not set
		if (Data::eeprom->rstReason == System::Reset_t::Unknown)
		{
			if (tmp & (1 << 0))
			{
				resetReason = System::Reset_t::Pin;
			}
			else if (tmp & (1 << 1))
			{
				resetReason = System::Reset_t::Watchdog;
			}			
			else if (tmp & (1 << 2))
			{
				resetReason = System::Reset_t::Software;
			}
			if (tmp & (1 << 3))
			{
				resetReason = System::Reset_t::Lockup;
			}
			if (tmp & (0b1111 << 16))
			{
				resetReason = System::Reset_t::SystemOff;
			}					
		}
		else
		{
			resetReason = Data::eeprom->rstReason;
		}

		// Reset reset reason in SRAM EEPROM
		Data::eeprom->rstReason = System::Reset_t::Unknown;

		// Enable RTC interrupt
		ret_code_t ret = sd_nvic_SetPriority(RTC2_IRQn, 2);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}

		ret = sd_nvic_EnableIRQ(RTC2_IRQn);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}

		// Enable compare0 interrupt
		NRF_RTC2->INTENSET = (1 << 16);

		// Set counter resolution of 125ms
		NRF_RTC2->PRESCALER = 4095;

		startWakeupTimer();
		return Return_t::OK;
	}

	/**
	 * @brief Start RTC2 wakeup timer.
	 * 
	 * @return No return value.
	 */
	void startWakeupTimer(void)
	{
		NRF_RTC2->TASKS_CLEAR = 1;
		NRF_RTC2->CC[0] = (AppConfig::measurePeriod * 1000) / 125;
		NRF_RTC2->TASKS_START = 1;
		_PRINT_INFO("Wakeup timer started\n");
	}

	/**
	 * @brief Is device woken up by RTC2 timer?
	 * 
	 * @return \c Return_t::NOK not woken up by RTC.
	 * @return \c Return_t::OK woken up by RTC.
	 */
	Return_t isWoken(void)
	{
		if (wakeup)
		{
			wakeup = 0;
			return Return_t::OK;
		}

		return Return_t::NOK;
	}

	/**
	 * @brief Put device to sleep.
	 * 
	 * @return No return value.
	 */
	void sleep(void)
	{
		__set_FPSCR(__get_FPSCR()  & ~(0x0000009F));      
		(void) __get_FPSCR();
		sd_nvic_ClearPendingIRQ(FPU_IRQn);		

		sd_power_dcdc_mode_set(NRF_POWER_DCDC_DISABLE);

		_PRINT("Sleep\n");
		sd_app_evt_wait();

		sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
	}

	/**
	 * @brief Get system reset reason.
	 * 
	 * @return See \ref Reset_t for possible return values. 
	 */
	Reset_t getResetReason(void)
	{
		return resetReason;
	}

	/**
	 * @brief Do system reset.
	 * 
	 * @param reason Reset reason. Optional.
	 * 
	 * @return No return value.
	 */
	void reset(const Reset_t reason = Reset_t::Unknown)
	{
		while (1)
		{
			// Set reset reason if needed
			if (reason != Reset_t::Unknown)
			{
				Data::eeprom->rstReason = reason;
			}
			
			sd_nvic_SystemReset();
		}
	}
};


// ----- STATIC FUNCTION DEFINITIONS
static void stopWakeupTimer(void)
{
	NRF_RTC2->TASKS_STOP = 1;
}


// ----- INTERRUPTS
extern "C"
{
	/**
	 * @brief RTC2 interrupt handler.
	 * 
	 * @return No return value.
	 */
	void RTC2_IRQHandler(void)
	{
		sd_nvic_ClearPendingIRQ(RTC2_IRQn);

		if (NRF_RTC2->EVENTS_COMPARE[0])
		{
			NRF_RTC2->EVENTS_COMPARE[0] = 0;

			wakeup = 1;
			stopWakeupTimer();
			
			_PRINT_INFO("Wakeup\n");
			
		}
	}

	void NMI_Handler(void)
	{

	}

	void HardFault_Handler(void)
	{

	}

	void MemoryManagement_Handler(void)
	{

	}

	void BusFault_Handler(void)
	{

	}

	void UsageFault_Handler(void)
	{

	}
}


/** @} */

// END WITH NEW LINE
