/**
 * @file Data.hpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Device data header file.
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

#ifndef _DATA_HPP_
#define _DATA_HPP_

// ----- INCLUDE FILES
#include			"System.hpp"
#include			"sBuildInfo.h"

#include			<stdint.h>
#include			<string.h>
#include			<stdio.h>

/**
 * @addtogroup Data
 * 
 * Device data
 * @{
 */


// ----- NAMESPACES
/**
 * @brief Device data namespace.
 * 
 */
namespace Data
{
	// ----- ENUMS
	/**
	 * @brief Enum class with value for inited SRAM EEPROM.
	 * 
	 */
	enum class Init_t : uint32_t
	{
		Inited = 0x31053105 /**< @brief Inited SRAM EEPROM flag value. */
	};

	/**
	 * @brief Enum class for errors.
	 * 
	 */
	enum class Error_t : uint8_t
	{
		None = 0, /**< @brief No active error. */
		ADCInit = (1 << 0),
		MeasureFail = (1 << 1),
		MeasureStatus = (1 << 2),
		PartialData = (1 << 3),
	};	


	// ----- STRUCTS
	/**
	 * @brief SRAM EEPROM data struct.
	 * 
	 */
	struct EEPROM_s
	{
		Init_t inited; /**< @brief Inited SRAM EEPROM flag. See \ref Init_t */

		uint16_t lastPressure; /**< @brief Last measured pressure in mbar. */
		int16_t lastTemperature; /**< @brief Last measured temperature in centi degrees celsius. */

		uint16_t lastVoltage; /**< @brief Last measured voltage in mV. */
		uint16_t uptime; /**< @brief Device uptime in hours. */

		uint8_t rstCount; /**< @brief Device reset counter. */
		uint8_t _padding1; // Padding byte
		uint16_t measureCnt; /**< @brief Measure counter used to track device uptime. */

		System::Reset_t rstReason; /**< @brief Reset reason. */
		uint8_t _padding2; // Padding byte
		uint16_t workingSeconds; /**< @brief Working seconds counter. */
	};


	// ----- VARAIBLES	
	static EEPROM_s* eeprom = (EEPROM_s*)MemoryMap::sramEEPROMStart; /**< @brief Reference to EEPROM in SRAM. */


	// ----- CLASSES
	/**
	 * @brief sTPMS data struct.
	 * 
	 */
	class sTPMS
	{
		public:
		// ----- METHOD DEFINITIONS
		/**
		 * @brief sTPMS data object constructor.
		 * 
		 * @return No return value.
		 */
		sTPMS(void)
		{
			memset(this, 0, sizeof(sTPMS));
		}

		/**
		 * @brief Set measured pressure.
		 * 
		 * @param value Measured pressure in mbar. 
		 * 
		 * @return No return value.
		 * 
		 * @note Value will be saved in SRAM EEPROM.
		 */
		inline void setPressure(const uint16_t value)
		{
			pressure = value;
			eeprom->lastPressure = value;
		}

		/**
		 * @brief Set measured temperature.
		 * 
		 * @param value Measured temperature in centi degress celsius. 
		 * 
		 * @return No return value.
		 * 
		 * @note Value will be saved in SRAM EEPROM.
		 */		
		inline void setTemperature(const uint16_t value)
		{
			temperature = value;
			eeprom->lastTemperature = value;
		}	

		/**
		 * @brief Set device uptime
		 * 
		 * @param value Device uptime in hours.
		 * 
		 * @return No return value.
		 */			
		inline void setUptime(const uint16_t value)
		{
			uptime = value;
		}

		/**
		 * @brief Increase device uptime.
		 * 
		 * @return No return value.
		 * 
		 * @note Value will be saved in SRAM EEPROM.
		 */			
		inline void increaseUptime(void)
		{
			uptime++;
			eeprom->uptime = uptime;
		}

		/**
		 * @brief Set measured battery voltage.
		 * 
		 * @param value Measured battery voltage in mV. 
		 * 
		 * @return No return value.
		 * 
		 * @note Value will be saved in SRAM EEPROM.
		 */			
		inline void setVoltage(const uint16_t value)
		{
			if (value)
			{
				voltage = (value / voltageRes) - voltageOffset;
			}
			else
			{
				voltage = 0;
			}
			
			_PRINTF("Voltage set to %ucV(%umV)\n", voltage, value);
			eeprom->lastVoltage = value;
		}

		/**
		 * @brief Set error code.
		 * 
		 * @param code Active error code.
		 * 
		 * @return No return value.
		 */			
		inline void setErrorCode(const Error_t code)
		{
			errorCode = (Error_t)((uint8_t)errorCode | (uint8_t)code);
		}	

		/**
		 * @brief Clear active errors.
		 * 
		 * @return No return value.
		 */
		inline void clearErrorCode(void)
		{
			errorCode = Error_t::None;
		}
		
		/**
		 * @brief Set device firmware version.
		 * 
		 * @param major Major version number (0-255).
		 * @param minor Minor version number (0-255).
		 * @param build Build version number (0-255).
		 * 
		 * @return No return value.
		 */
		inline void setFirmwareVersion(const uint8_t major, const uint8_t minor, const uint8_t build)
		{
			fwVer[0] = major;
			fwVer[1] = minor;
			fwVer[2] = build;
		}		
		
		/**
		 * @brief Set reset reason and count.
		 * 
		 * @param reason Reset reason. See \ref System::Reset_t
		 * @param count Reset count.
		 * 
		 * @return No return value.
		 */
		inline void setReset(const System::Reset_t reason, const uint8_t count)
		{
			rstReason = reason;
			rstCount = count;
		}
		
		/**
		 * @brief Set device config.
		 * 
		 * @param hwID Hardware ID. See \ref AppConfig::Hardware_t
		 * @param measurePeriod Measure period in seconds.
		 * 
		 * @return No return value.
		 */
		inline void setConfig(const AppConfig::Hardware_t hwID, const uint8_t measurePeriod)
		{
			config = ((uint8_t)hwID << cfgHWIDBit) | ((measurePeriod / cfgPeriodRes) << cfgPeriodBit);
		}


		private:
		// ----- VARIABLES
		static constexpr uint8_t voltageRes = 10; /**< @brief Measured voltage resolution. */	
		static constexpr uint8_t voltageOffset = 200; /**< @brief Measured voltage offset in centi volts. */

		static constexpr uint8_t cfgHWIDMask = 0b111; /**< @brief Hardware ID mask in config. */
		static constexpr uint8_t cfgHWIDBit = 0; /**< @brief Hardware ID bit offset in config. */

		static constexpr uint8_t cfgPeriodMask = 0b11111000; /**< @brief Measure period mask in config. */
		static constexpr uint8_t cfgPeriodBit = 3; /**< @brief Measure bit offset in config. */
		static constexpr uint8_t cfgPeriodRes = 5; /**< @brief Measure period resolution in seconds. */


		uint16_t pressure; /**< @brief Measured pressure in mbar. */
		int16_t temperature; /**< @brief Measured temperature in centi degrees celsius. */

		uint16_t uptime; /**< @brief Device uptime in hours. */
		uint8_t voltage; /**< @brief Battery voltage in centivolts. Offset: 200cV. */
		Error_t errorCode; /**< @brief Last error code. See \ref Error_t. */

		uint8_t fwVer[3]; /**< @brief Firmware version - major, minor, build. */
		System::Reset_t rstReason; /**< @brief Reset reason. See \ref System::Reset_t. */
		uint8_t rstCount; /**< @brief Reset counter. */

		/**
		 * @brief Device config.
		 * 
		 * Bit 0:2 = Hardware ID. See \ref AppConfig::Hardware_t
		 * Bit 3:7 = Advertise period with resolution of \ref cfgPeriodRes.
		 */
		uint8_t config;	
	};


	// ----- FUNCTION DEFINITIONS
	/**
	 * @brief Init SRAM EEPROM.
	 * 
	 * @param data Reference to sTPMS data.
	 * 
	 * @return No return value.
	 */
	inline void init(sTPMS& data)
	{
		// Init EEPROM
		if (eeprom->inited != Init_t::Inited)
		{
			memset(eeprom, 0, sizeof(EEPROM_s));
			eeprom->inited = Init_t::Inited;
			eeprom->rstReason = System::Reset_t::Powerup;
			_PRINT("SRAM EEPROM inited\n");
		}
		else
		{
			eeprom->rstCount++;
			_PRINT("SRAM EEPROM already inited\n");
		}

		// Parse firmware version
		uint8_t major = 0;
		uint8_t minor = 0;
		uint8_t build = 0;

		sscanf((const char*)SBI_APP_VER, "v%hhu.%hhu.%hhu", &major, &minor, &build);

		/*
			Init sTPMS data object
			Reason why sTPMS data are not placed in SRAM EEPROM is in case of data corruption
		*/
		data.setPressure(eeprom->lastPressure);
		data.setTemperature(eeprom->lastTemperature);
		data.setUptime(eeprom->uptime);
		data.setVoltage(eeprom->lastVoltage);
		data.clearErrorCode();
		data.setFirmwareVersion(major, minor, build);
		data.setConfig(AppConfig::hwID, AppConfig::measurePeriod);
	}
};


/** @} */

#endif // _DATA_HPP_

// END WITH NEW LINE
