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
#include			"nrf.h"

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
		// Test HF crystal only in debug build
		#ifdef DEBUG
		_PRINT("Wait for HFXO\n");
		NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
		NRF_CLOCK->TASKS_HFCLKSTART = 1;
		while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
		NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;	
		_PRINT("HFXO started\n");
		#endif // DEBUG

		return Return_t::OK;
	}
};


/** @} */

// END WITH NEW LINE
