/**
 * @file AppConfig.hpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Application config header file.
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

#ifndef _APPCONFIG_HPP_
#define _APPCONFIG_HPP_

// ----- INCLUDE FILES
#include			<stdint.h>


/**
 * @addtogroup ApplicationConfig 
 * 
 * Application configuration module.
 * @{
 */

// ----- DEFINES
#define APP_NAME			"sTPMS_FW"
#define APP_VERSION			"v1.0.0rc1"
#define APP_HW				"24-0012rev1"




// ----- CONFIGURATION
/**
 * @brief Application configuration namespace.
 * 
 */
namespace AppConfig
{
	// ----- ENUMS
	/**
	 * @brief Enum class for hardware IDs.
	 * 
	 */
	enum class Hardware_t : uint8_t
	{
		sTPMS1 = 1, /**< @brief Hardware ID for sTPMS1. */
	};


	// ----- VARIABLES
	static constexpr char deviceName[] = "sTPMS1"; /**< @brief Bluetooth device name. */
	static constexpr int8_t advTXPower = 4; /**< @brief Bluetooth TX power for advertising role(-40, -20, -16, -12, -8, -4, 0, or 4dBm). */
	static constexpr uint8_t advCount = 1; /**< @brief Number of advertisment events before stopping advertise. */
	static constexpr uint8_t bleTag = 1; /**< @brief Bluetooth connection tag. */
	static constexpr Hardware_t hwID = Hardware_t::sTPMS1; /**< @brief sTPMS hardware ID. */
	static constexpr uint8_t rttChannel = 0; /**< @brief RTT channel ID for debug output. */
	#ifdef DEBUG
	static constexpr uint16_t measurePeriod = 5; /**< @brief Measure period in seconds for debug build. */
	#else
	static constexpr uint16_t measurePeriod = 60; /**< @brief Measure period in seconds for release build. */
	#endif // DEBUG
	static constexpr uint8_t wdtTimeout = measurePeriod + 4; /**< @brief Watchdog timer timeout in seconds. */
	static constexpr uint16_t bleMnfID = 0x3105; /**< @brief Manufacturer ID in BLE advertise packet. */
	static constexpr uint8_t ledBlinkCount = 3; /**< @brief Number of measurments where LED will blink if reset reason is powerup. */
};


/** @} */

#endif // _APPCONFIG_HPP_


// END WITH NEW LINE
