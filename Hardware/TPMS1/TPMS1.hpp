/**
 * @file sTPMS1.hpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief sTPMS1 hardware configuration header file.
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

#ifndef _sTPMS1_HPP_
#define _sTPMS1_HPP_

// ----- INCLUDE FILES

/**
 * @addtogroup sTPMS1_HW
 * 
 * sTPMS1 Hardware-related config.
 * @{
 */

// ----- DEFINES
#define APP_HW				"24-0012rev1"

 
// ----- NAMESPACES
/**
 * @brief Namespace with hardware configuration.
 * 
 */
namespace Hardware
{
	static constexpr uint8_t ptsSCLPort = 0; /**< @brief Pressure and temperature sensor I2C SCL GPIO port. */
	static constexpr uint8_t ptsSCLPin = 7; /**< @brief Pressure and temperature sensor I2C SCL GPIO pin. */
	static constexpr uint8_t ptsSDAPort = 0; /**< @brief Pressure and temperature sensor I2C SDA GPIO port. */
	static constexpr uint8_t ptsSDAPin = 6; /**< @brief Pressure and temperature sensor I2C SDA GPIO pin. */
	static constexpr uint8_t ptsSelectPort = 0; /**< @brief Pressure and temperature sensor select GPIO port. */
	static constexpr uint8_t ptsSelectPin = 5; /**< @brief Pressure and temperature sensor select GPIO pin. */
	static constexpr uint8_t ledPort = 0; /**< @brief LED GPIO port. */
	static constexpr uint8_t ledPin = 2; /**< @brief LED GPIO pin. */
};

/**
 * @brief Namespace with memory configuration.
 * 
 */
namespace MemoryMap
{
	static constexpr uint32_t sramEEPROMStart = 0x2000FC00; /**< @brief Start address of SRAM EEPROM. */
	static constexpr uint16_t sramEEPROMSize = 0x400; /**< @brief Size of SRAM EEPROM in bytes. */
};


/** @} */

#endif // _sTPMS1_HPP_

// END WITH NEW LINE
