/**
 * @file System.hpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief System header file.
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

#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

// ----- INCLUDE FILES
#include			"Main.hpp"

#include			"nrf.h"
#include			"nrf_wdt.h"


// ----- NAMESPACES
namespace System
{
	// ----- ENUMS
	/**
	 * @brief Enum class with reset reasons.
	 * 
	 * \ingroup System
	 */
	enum class Reset_t : uint8_t
	{
		Unknown = 0, /**< @brief Unknown reset reason. */
		Powerup = 1, /**< @brief Device powerup. */
		Pin = 2, /**< @brief Reset via reset pin. */
		Watchdog = 3, /**< @brief Watchdog timeout reset. */
		Software = 4, /**< @brief Software reset. */
		Lockup = 5, /**< @brief CPU lockup detected. */
		SystemOff = 6, /**< @brief Wakeup from system off reset. */

		NMI = 10, /**< @brief Non maskable interrupt reset. */
		HardFault = 11, /**< @brief Hard fault interrupt reset. */
		MM = 12, /**< @brief Memory managment interrupt reset. */
		BusFault = 13, /**< @brief Bus fault interrupt reset. */
		UsageFault = 14, /**< @brief Usage fault interrupt reset. */

		Unexpected = 20, /**< @brief Reset reason for unexpected reset. */
		SystemInit = 21, /**< @brief Reset reason for failed system init. */
		BLEInit = 22, /**< @brief Reset reason for failed BLE init. */
		PTSInit = 23, /**< @brief Reset reason for failed sensor init. */
		TWIInit = 24, /**< @brief Reset reason for failed TWI init. */
		AdvFail = 25, /**< @brief Reset reason for too much BLE advertise fails. */
	};

	
	// ----- FUNCTION DECLARATION
	Return_t init(void);
	void startWakeupTimer(void);
	Return_t isWoken(void);
	void sleep(void);
	Reset_t getResetReason(void);
	void reset(const Reset_t reason);
	

	// ----- FUNCTION DEFINITIONS
	/**
	 * @brief Feed/reload watchdog timer.
	 * 
	 * @return No return value.
	 * 
	 * \ingroup System
	 */
	inline void feedWatchdog(void)
	{
		nrf_wdt_reload_request_set(NRF_WDT_RR0);
	}
};


#endif // _SYSTEM_HPP_

// END WITH NEW LINE
