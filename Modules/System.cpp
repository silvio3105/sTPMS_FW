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


/**
 * @addtogroup System
 * 
 * System module
 * @{
 */

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

		return Return_t::OK;
	}
};


/** @} */

// END WITH NEW LINE
