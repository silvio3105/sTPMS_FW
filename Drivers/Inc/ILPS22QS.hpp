/**
 * @file ILPS22QS.hpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief ILPS22QS I2C/3-wire SPI driver header file.
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

#ifndef _ILPS22QS_HPP_
#define _ILPS22QS_HPP_

// ----- INCLUDE FILES
#include 			"sDebug.hpp"


#include			<stdint.h>
#include			<string.h>


/**
 * @addtogroup ILPS22QS_Driver 
 * 
 * Frameworkless I2C/SPI driver for ILPS22QS pressure and temperature sensor.
 * @{
 */


 // ----- DEFINES
#ifndef ILPS22QS_I2C_READ_TIMEOUT
#define ILPS22QS_I2C_READ_TIMEOUT				10 /**< @brief Timeout in ms for I2C read operations. Can be changed during compile. */
#endif // ILPS22QS_I2C_READ_TIMEOUT

#ifndef ILPS22QS_I2C_WRITE_TIMEOUT
#define ILPS22QS_I2C_WRITE_TIMEOUT				10 /**< @brief Timeout in ms for I2C write operations. Can be changed during compile. */
#endif // ILPS22QS_I2C_WRITE_TIMEOUT

#ifndef ILPS22QS_SPI_READ_TIMEOUT
#define ILPS22QS_SPI_READ_TIMEOUT				10 /**< @brief Timeout in ms for SPI read operations. Can be changed during compile. */
#endif // ILPS22QS_SPI_READ_TIMEOUT

#ifndef ILPS22QS_SPI_WRITE_TIMEOUT
#define ILPS22QS_SPI_WRITE_TIMEOUT				10 /**< @brief Timeout in ms for SPI write operations. Can be changed during compile. */
#endif // ILPS22QS_SPI_WRITE_TIMEOUT


// ----- NAMESPACES
/**
 * @brief Namespace for ILPS22QS driver.
 * 
 */
namespace ILPS22QS
{
	// ----- VARIABLES
	static constexpr char version[] = "v1.0rc3"; /**< @brief Driver version string. */


	// ----- DEBUG
	#ifdef DEBUG_ILPS22QS

	#ifdef DEBUG_ILPS22QS_VERBOSE
	DEBUG_ENABLE_VERBOSE(ILPS22QS);
	#else // DEBUG_ILPS22QS_VERBOSE
	DEBUG_DISABLE_VERBOSE();
	#endif // DEBUG_ILPS22QS_VERBOSE

	#ifdef DEBUG_ILPS22QS_INFO
	DEBUG_ENABLE_INFO(ILPS22QS);
	#else // DEBUG_ILPS22QS_INFO
	DEBUG_DISABLE_INFO();
	#endif // DEBUG_ILPS22QS_INFO

	#ifdef DEBUG_ILPS22QS_ERROR
	DEBUG_ENABLE_ERROR(ILPS22QS);
	#else // DEBUG_ILPS22QS_ERROR
	DEBUG_DISABLE_ERROR();
	#endif // DEBUG_ILPS22QS_ERROR

	#else // DEBUG_ILPS22QS

	DEBUG_DISABLE_VERBOSE(ILPS22QS);
	DEBUG_DISABLE_INFO(ILPS22QS);
	DEBUG_DISABLE_ERROR(ILPS22QS);

	#endif // DEBUG_ILPS22QS


	// ----- ENUMS
	/**
	 * @brief Return values for ILPS22QS driver.
	 * 
	 */
	enum class Return_t : uint8_t
	{
		OK = 0, /**< @brief OK return value. */
		NOK, /**< @brief Generic not-OK return value. */
		Timeout, /**< @brief Return value for timeouted operation. */
	};

	/**
	 * @brief Driver process semaphore values.
	 * 
	 */
	enum class Semaphore_t : uint8_t 
	{
		Free = 0, /**< @brief Semaphore value when process is free. */
		Taken = 1 /**< @brief Semaphore value when process is taken. */
	};

	/**
	 * @brief Enable/Disable state enum class.
	 * 
	 */
	enum class State_t : uint8_t
	{
		Disable = 0, /**< @brief Disable feature / set to inactive state etc... */
		Enable = 1 /**< @brief Enable feature / set to active state etc... */
	};

	/**
	 * @brief Interrupt latch values.
	 * 
	 */
	enum class InterruptLatch_t : uint8_t
	{
		NotLatch = 0, /**< @brief Interrupt is not latched. */
		Latch = 1 /**< @brief Interrupt is latched. */
	};

	/**
	 * @brief Temperature unit values.
	 * 
	 */
	enum class TemperatureScale_t : uint8_t
	{
		Celsius = 0, /**< @brief Value for temperature in centideegres celsius. */
		Fahrenheit = 1, /**< @brief Value for temperature in centideegres fahrenheit. */
	};

	/**
	 * @brief Pressure scale values.
	 * 
	 */
	enum class PressureScale_t : uint8_t
	{
		Scale1260hPa = 0, /**< @brief Value for pressure scale up to 1260hPa. */
		Scale4060hPa = 1 /**< @brief Value for pressure scale up to 4060hPa. */
	};

	/**
	 * @brief Enum class with average selection values.
	 * 
	 */
	enum class Average_t : uint8_t
	{
		Average4 = 0b000, 
		Average8 = 0b001,
		Average16 = 0b010,
		Average32 = 0b011,
		Average64 = 0b100,
		Average128 = 0b101,
		Average512 = 0b111
	};

	/**
	 * @brief Enum class with output data rate values.
	 * 
	 */
	enum class OutputDataRate_t : uint8_t
	{
		OneShot = 0b0000, /**< @brief Power-down mode. */
		ODR1Hz = 0b0001, /**< @brief 1Hz data output. */
		ODR4Hz = 0b0010, /**< @brief 4Hz data output. */
		ODR10Hz = 0b0011, /**< @brief 10Hz data output. */
		ODR25Hz = 0b0100, /**< @brief 25Hz data output. */
		ODR50Hz = 0b0101, /**< @brief 50Hz data output. */
		ODR75Hz = 0b0110, /**< @brief 75Hz data output. */
		ODR100Hz = 0b0111, /**< @brief 100Hz data output. */
		ODR200Hz = 0b1000 /**< @brief 200Hz data output. */
	};

	/**
	 * @brief Enum class for low-pass filter config.
	 * 
	 */
	enum class FilterDiscard_t : uint8_t
	{
		Discard1Sample = 0, /**< @brief Configure low-pass filter to discard 1 sample (ODR / 4). */
		Discard6Samples = 1, /**< @brief Configure low-pass filter to discard 6 samples (ODR / 9). */
	};

	/**
	 * @brief Enum class with values for data update config.
	 * 
	 */
	enum class DataUpdate_t : uint8_t
	{
		Continuous = 0, /**< @brief Update data continuously. */
		BlockUntilRead = 1, /**< @brief Block data update until previously has been read. */
	};


	// ----- STRUCTS
	/**
	 * @brief Interrupt config struct.
	 * 
	 */
	struct InterruptConfig_s
	{
		State_t autoREFP; /**< @brief Enable or disable AutoREFP feature. */
		State_t resetARP; /**< @brief Reset autoREFP feature. */
		State_t autoZero; /**< @brief Enable or disable auto zero feature. */
		State_t resetAZ; /**< @brief Reset auto zero feature. */
		InterruptLatch_t interruptLatch; /**< @brief Configure  */
		State_t pressureLowInterrupt; /**< @brief Generate interrupt on low pressure. */
		State_t pressureHighInterrupt; /**< @brief Generate interrupt on high pressure. */
	};

	/**
	 * @brief Interface config struct.
	 * 
	 */
	struct InterfaceConfig_s
	{
		State_t i2ci3cOff; /**< @brief Turn off I2C/I3C interface. */
		State_t SPIRead; /**< @brief Turn on 3-wire SPI read. */
		State_t sdaPullUp; /**< @brief Enable or disable pull-up on SDA line. */
		State_t ssPullUpOff; /**< @brief Turn off pull-up on SS line. */
	};

	/**
	 * @brief Data output config struct.
	 * 
	 */
	struct DataOutputConfig_s
	{
		OutputDataRate_t dataRate; /**< @brief Output data rate. See \ref OutputDataRate_t */
		Average_t average; /**< @brief Output data average selection. See \ref Average_t */
	};

	/**
	 * @brief Struct for low-pass config.
	 * 
	 */
	struct FilterConfig_s
	{
		FilterDiscard_t discard; /**< @brief Low-pass filter discard config. See \ref FilterDiscard_t */
		State_t filter; /**<@brief Enable or disable low-pass filter. */
	};

	/**
	 * @brief Struct for analog hub/qvar config.
	 * 
	 */
	struct AnalogHubConfig_s
	{
		State_t analogHub; /**< @brief Enable or disable analog hub/Qvar. */
		State_t interleavedMode; /**< @brief Enable or disable interleaved mode. */
		State_t addressIncrement; /**< @brief Enable or disable auto address increment. */
	};

	/**
	 * @brief Struct for interrupt sources.
	 * 
	 */
	struct InterruptSource_s
	{
		uint8_t boot; /**<@brief If set to \c 1 boot phase is ongoing. */
		uint8_t active; /**< @brief If set to \c 1 interrupt is active. */
		uint8_t pressureLow; /**< @brief If set to \c 1 low pressure interrupt is active. */
		uint8_t pressureHigh; /**< @brief If set to \c 1 high pressure interrupt is active. */
	};

	/**
	 * @brief Struct for data status.
	 * 
	 */
	struct DataStatus_s
	{
		uint8_t pressureOverrun; /**< @brief If set to \c 1 pressure data overrun happend. */
		uint8_t pressureAvailable; /**< @brief If set to \c 1 pressure data is available. */
		uint8_t temperatureOverrun; /**< @brief If set to \c 1 temperature data overrun happend. */
		uint8_t temperatureAvailable; /**< @brief If set to \c 1 temperature data is available. */
	};


	// ----- TYPEDEFS
	/**
	 * @brief Typedef for external delay/wait handler.
	 * 
	 * @param period Delay/wait period in ms.
	 * 
	 * @return No return value.
	 */
	typedef void (*Delay_f)(const uint32_t period);

	/**
	 * @brief Typedef for external handler for getting tick.
	 * 
	 * @return Tick count.
	 */
	typedef uint32_t (*Tick_f)(void);

	/**
	 * @brief Typedef for external handler for I2C read/write operations.
	 * 
	 * @param address Slave 7-bit I2C address.
	 * @param data Pointer to IO data.
	 * @param len Length of \c data in bytes.
	 * @param tiemout Read/write timeout in ms.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	typedef Return_t (*I2CRW_f)(const uint8_t address, void* data, const uint8_t len, const uint8_t timeout);

	/**
	 * @brief Typedef for external handler for SPI read/write operations.
	 * 
	 * @param txData Pointer to output data.
	 * @param txLen Length of \c txData in bytes.
	 * @param rxData Pointer to input buffer for incoming data.
	 * @param rxLen Length of \c rxData buffer(number of bytes to receive).
	 * @param timeout Operation timeout in ms.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	typedef Return_t (*SPIRXTX_f)(const void* txData, const uint8_t txLen, void* rxData, const uint8_t rxLen, const uint8_t timeout);

	/**
	 * @brief Typedef for external handler for SPI slave select operations.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	typedef Return_t (*Select_f)(void);

	/**
	 * @brief Typedef for external handler for MSP init and deinit.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	typedef Return_t (*MSP_f)(void);


	// ----- CLASSES
	/**
	 * @brief Class for ILPS22QS driver.
	 * 
	 * @tparam E Interface class for external operations.
	 */
	template<class E>
	class Driver
	{
		public:
		// ----- METHOD DEFINITIONS
		/**
		 * @brief Take semaphore for read and write operations.
		 * 
		 * This is required if async read and write is used(eg., interrupt driven or DMA).
		 * 
		 * @return No return value.
		 */
		inline void takeSemaphore(void)
		{
			semaphore = Semaphore_t::Taken;

			ILPS22QS_PRINTN("ILPS22QS ", 9);
			ILPS22QS_PRINTN("taken\n", 6);
		}

		/**
		 * @brief Free semaphore for read and write operations.
		 * 
		 * This is required if async read and write is used(eg., interrupt driven or DMA).
		 * 
		 * @return No return value.
		 */
		inline void freeSemaphore(void)
		{
			semaphore = Semaphore_t::Free;

			ILPS22QS_PRINTN("ILPS22QS ", 9);
			ILPS22QS_PRINTN("free\n", 5);
		}	

		/**
		 * @brief Init the sensor.
		 * 
		 * This should be called before interacting with the sensor.
		 * 
		 * @param interfaceCfg Pointer to interface config. Optional.
		 * 
		 * @return \c Return_t::NOK on failed init.
		 * @return \c Return_t::OK on successful init. 
		 */
		Return_t init(const InterfaceConfig_s* interfaceCfg = nullptr)
		{
			ILPS22QS_PRINTF_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTF_INFO(version, sizeof(version) - 1);
			ILPS22QS_PRINTF_INFO("\n", 1);

			// Check interface object
			if (interface.check() != Return_t::OK)
			{
				ILPS22QS_PRINTN_ERROR("ILPS22QS ", 9);
				ILPS22QS_PRINTN_ERROR("Check fail\n", 11);				
				return Return_t::NOK;
			}

			// Call MSP init handler if provided
			if (mspInitHandler)
			{
				if (mspInitHandler() != Return_t::OK)
				{
					ILPS22QS_PRINTN_ERROR("ILPS22QS ", 9);
					ILPS22QS_PRINTN_ERROR("MSP init fail\n", 14);
					return Return_t::NOK;
				}
			}

			// Set interface config
			if (interfaceCfg)
			{
				if (interfaceConfig(interfaceCfg) != Return_t::OK)
				{
					ILPS22QS_PRINTN_ERROR("ILPS22QS ", 9);
					ILPS22QS_PRINTN_ERROR("Cfg fail\n", 9);
					return Return_t::NOK;
				}
			}

			// Check sensor ID
			uint8_t tmp = 0;
			if (whoAmI(tmp) == Return_t::OK)
			{
				if (tmp != chipID)
				{
					ILPS22QS_PRINTN_ERROR("ILPS22QS ", 9);
					ILPS22QS_PRINTF_ERROR("Invalid ID %02X\n", tmp);
					return Return_t::NOK;
				}		
			}
			else
			{
				ILPS22QS_PRINTN_ERROR("ILPS22QS ", 9);
				ILPS22QS_PRINTN_ERROR("WhoAmI fail\n", 12);
				return Return_t::NOK;
			}

			// Cache pressure scale
			return readPressureScale();
		}

		/**
		 * @brief Deinit the sensor.
		 * 
		 * @return \c Return_t::NOK on failed init.
		 * @return \c Return_t::OK on successful init. 
		 */
		Return_t deinit(void) const
		{
			// Call deinit handler if provided
			if (mspDeinitHandler)
			{
				ILPS22QS_PRINTN_ERROR("ILPS22QS ", 9);
				ILPS22QS_PRINTN_ERROR("MSP deinit fail\n", 16);
				return mspDeinitHandler();
			}

			return Return_t::OK;
		}

		/**
		 * @brief Set interrupt config.
		 * 
		 * @param config Reference to interrupt config. See \ref InterruptConfig_s
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t setInterruptConfig(const InterruptConfig_s& config)
		{	
			txBuffer[0] = (uint8_t)Register_t::Interrupt;
			txBuffer[1] = 	((uint8_t)config.autoREFP << (uint8_t)InterruptBitmap_t::AutoREFP) |
							((uint8_t)config.autoZero << (uint8_t)InterruptBitmap_t::AutoZero) |
							((uint8_t)config.interruptLatch << (uint8_t)InterruptBitmap_t::InterruptLatch) |
							((uint8_t)config.pressureHighInterrupt << (uint8_t)InterruptBitmap_t::PressureHighEvent) |
							((uint8_t)config.pressureLowInterrupt << (uint8_t)InterruptBitmap_t::PressureLowEvent) |
							((uint8_t)config.resetARP << (uint8_t)InterruptBitmap_t::ResetAutoREFP) |
							((uint8_t)config.resetAZ << (uint8_t)InterruptBitmap_t::ResetAutoZero);

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTF_INFO("INT cfg set to %02X\n", txBuffer[1]);
			return writeRegister(txBuffer, 2);
		}

		/**
		 * @brief Get interrupt config.
		 * 
		 * @param config Reference to output config.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t getInterruptConfig(InterruptConfig_s& config)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::Interrupt, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			config.autoREFP = (State_t)((tmp >> (uint8_t)InterruptBitmap_t::AutoREFP) & 1);
			config.autoZero = (State_t)((tmp >> (uint8_t)InterruptBitmap_t::AutoZero) & 1);
			config.interruptLatch = (InterruptLatch_t)((tmp >> (uint8_t)InterruptBitmap_t::InterruptLatch) & 1);
			config.pressureHighInterrupt = (State_t)((tmp >> (uint8_t)InterruptBitmap_t::PressureHighEvent) & 1);
			config.pressureLowInterrupt = (State_t)((tmp >> (uint8_t)InterruptBitmap_t::PressureLowEvent) & 1);
			config.resetARP = (State_t)((tmp >> (uint8_t)InterruptBitmap_t::ResetAutoREFP) & 1);
			config.resetAZ = (State_t)((tmp >> (uint8_t)InterruptBitmap_t::ResetAutoZero) & 1);
			return Return_t::OK;
		}

		/**
		 * @brief Set pressure interrupt threshold.
		 * 
		 * @param threshold Pressure threshold in hPa(mbar).
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t setPressureInterruptThreshold(const uint16_t threshold)
		{
			// Calculate raw pressure value and set TX buffer
			const uint16_t rawPressure = threshold * pressureScaleDivider[(uint8_t)getPressureScale()];
			txBuffer[0] = (uint8_t)Register_t::PressureThresholdHigh;
			txBuffer[1] = rawPressure >> 8;

			if (writeRegister(txBuffer, 2) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			txBuffer[0] = (uint8_t)Register_t::PressureThresholdLow;
			txBuffer[1] = rawPressure & 0xFF;

			if (writeRegister(txBuffer, 2) != Return_t::OK)
			{
				return Return_t::NOK;
			}	

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTF_INFO("Interrupt threshold set to %u\n", threshold);
			return Return_t::OK;
		}

		/**
		 * @brief Get pressure threshold for pressure interrupt.
		 * 
		 * @param output Reference for pressure threshold value output.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t getPressureInterruptThreshold(uint16_t& output)
		{
			uint16_t tmpOutput = 0;
			uint8_t tmp = 0;

			// Read high byte
			if (readRegister(Register_t::PressureThresholdHigh, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}
			tmpOutput = tmp << 8;		

			// Read low byte
			if (readRegister(Register_t::PressureThresholdLow, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}
			tmpOutput |= tmp;

			// Convert to output unit
			output = tmpOutput / pressureScaleDivider[(uint8_t)getPressureScale()];
			return Return_t::OK;
		}

		/**
		 * @brief Set pressure scale.
		 * 
		 * @param scale Pressure scale. See \ref PressureScale_t
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t setPressureScale(const PressureScale_t scale)
		{
			// Read control 2 register
			uint8_t tmp = 0;

			if (readRegister(Register_t::Control2, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			txBuffer[0] = (uint8_t)Register_t::Control2;
			txBuffer[1] = tmp & ~(1 << (uint8_t)Control2Bitmap_t::FullScale);
			txBuffer[1] |= ((uint8_t)scale << (uint8_t)Control2Bitmap_t::FullScale);

			if (writeRegister(txBuffer, 2) != Return_t::OK)
			{
				return Return_t::NOK;
			}	

			pressureScale = scale;

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTF_INFO("Pressure scale set to %02X\n", txBuffer[1]);
			return Return_t::OK;
		}

		/**
		 * @brief Get measuing scale for pressure.
		 * 
		 * @return Pressure scale. See \ref PressureScale_t
		 */
		inline PressureScale_t getPressureScale(void) const
		{
			return pressureScale;
		}

		/**
		 * @brief Get temperature scale.
		 * 
		 * @return See \ref TemperatureScale_t 
		 */
		inline TemperatureScale_t getTemperatureScale(void) const
		{
			return temperatureScale;
		}

		/**
		 * @brief Set temperature scale.
		 * 
		 * @param scale Output temperature scale.
		 * 
		 * @return No return values.
		 */
		inline void setTemperatureScale(const TemperatureScale_t scale)
		{
			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTF_INFO("Temp scale set to %c\n", scale == TemperatureScale_t::Celsius ? 'C' : 'F');
			temperatureScale = scale;
		}

		/**
		 * @brief Configure data output.
		 * 
		 * @param config Reference to data output config. See \ref DataOutputConfig_s
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t setDataOutputConfig(const DataOutputConfig_s& config)
		{
			txBuffer[0] = (uint8_t)Register_t::Control1;
			txBuffer[1] = 	((uint8_t)config.average << (uint8_t)Control1Bitmap_t::Average) | 
							((uint8_t)config.dataRate << (uint8_t)Control1Bitmap_t::OutputDataRate);

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTF_INFO("DO cfg set to %02X\n", txBuffer[1]);
			return writeRegister(txBuffer, 2);
		}

		/**
		 * @brief Get current data output config.
		 * 
		 * @param config Reference to output config. See \ref DataOutputConfig_s
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t getDataOutputConfig(DataOutputConfig_s& config)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::Control1, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			config.average = (Average_t)(tmp & averageMask);
			config.dataRate = (OutputDataRate_t)(tmp >> (uint8_t)Control1Bitmap_t::OutputDataRate);
			return Return_t::OK;
		}

		/**
		 * @brief Start pressure and temperature measurment when the sensor is in power-down/one-shot mode.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t measure(void)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::Control2, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			txBuffer[0] = (uint8_t)Register_t::Control2;
			txBuffer[1] = tmp | (1 << (uint8_t)Control2Bitmap_t::OneShot);

			ILPS22QS_PRINTN("ILPS22QS ", 9);
			ILPS22QS_PRINTN("Measure started\n", 16);
			return writeRegister(txBuffer, 2);
		}

		/**
		 * @brief Set pressure low-pass filter config.
		 * 
		 * @param config Reference to filter config struct. See \ref FilterConfig_s
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t setFilterConfig(const FilterConfig_s& config)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::Control2, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			txBuffer[0] = (uint8_t)Register_t::Control2;
			tmp &= ~((1 << (uint8_t)Control2Bitmap_t::LowPassFilterConfig) | (1 << (uint8_t)Control2Bitmap_t::LowPassFilterEnable));
			txBuffer[1] = tmp | (((uint8_t)config.filter << (uint8_t)Control2Bitmap_t::LowPassFilterEnable) | ((uint8_t)config.discard << (uint8_t)Control2Bitmap_t::LowPassFilterConfig));

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTF_INFO("Filter cfg set to %02X\n", txBuffer[1]);
			return writeRegister(txBuffer, 2);
		}

		/**
		 * @brief Get pressure low-pass filter config.
		 * 
		 * @param config Reference to output. See \ref FilterConfig_s
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t getFilterConfig(FilterConfig_s& config)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::Control2, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			config.discard = (FilterDiscard_t)((tmp >> (uint8_t)Control2Bitmap_t::LowPassFilterConfig) & 1);
			config.filter = (State_t)((tmp >> (uint8_t)Control2Bitmap_t::LowPassFilterEnable) & 1);
			return Return_t::OK;		
		}

		/**
		 * @brief Set data update config.
		 * 
		 * @param update Data update procedure.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t setDataUpdateConfig(const DataUpdate_t update)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::Control2, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			txBuffer[0] = (uint8_t)Register_t::Control2;
			tmp &= ~(1 << (uint8_t)Control2Bitmap_t::BlockDataUpdate);
			txBuffer[1] = tmp | ((uint8_t)update << (uint8_t)Control2Bitmap_t::BlockDataUpdate);

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTF_INFO("Data update config set to %02X\n", txBuffer[1]);
			return writeRegister(txBuffer, 2);				
		}

		/**
		 * @brief Get data update config.
		 * 
		 * @param update Reference to config output.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t getDataUpdateConfig(DataUpdate_t& update)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::Control2, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			update = (DataUpdate_t)((tmp >> (uint8_t)Control2Bitmap_t::BlockDataUpdate) & 1);
			return Return_t::OK;		
		}

		/**
		 * @brief Trigger software reset.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t reset(void)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::Control2, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			txBuffer[0] = (uint8_t)Register_t::Control2;
			txBuffer[1] = tmp | (1 << (uint8_t)Control2Bitmap_t::Reset);

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTN_INFO("Reset\n", 6);
			return writeRegister(txBuffer, 2);	
		}

		/**
		 * @brief Trigger reboot.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t reboot(void)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::Control2, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			txBuffer[0] = (uint8_t)Register_t::Control2;
			txBuffer[1] = tmp | (1 << (uint8_t)Control2Bitmap_t::Boot);

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTN_INFO("Reboot\n", 7);
			return writeRegister(txBuffer, 2);			
		}

		/**
		 * @brief Disable analog hub and Qvar feature to reduce power consumption.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t disableAnalogHub(void)
		{
			txBuffer[0] = (uint8_t)Register_t::AnalogHubDisable;
			txBuffer[1] = 0;

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTN_INFO("AH disable\n", 11);
			return writeRegister(txBuffer, 2);
		}

		/**
		 * @brief Set analog hub/qvar config.
		 * 
		 * @param config Reference to analog hub config. See \ref AnalogHubConfig_s
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t setAnalogHubConfig(const AnalogHubConfig_s& config)
		{
			txBuffer[0] = (uint8_t)Register_t::Control3;
			txBuffer[1] =	((uint8_t)config.addressIncrement << (uint8_t)Control3Bitmap_t::AddressIncrement) | 
							((uint8_t)config.interleavedMode << (uint8_t)Control3Bitmap_t::AnalogHubInterleaved) |
							((uint8_t)config.analogHub << (uint8_t)Control3Bitmap_t::AnalogHubEnable);

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTF_INFO("AH cfg set to %02X\n", txBuffer[1]);
			return writeRegister(txBuffer, 2);		 
		}

		/**
		 * @brief Get analog hub/qvar config.
		 * 
		 * @param config Reference to config output. See \ref AnalogHubConfig_s
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t getAnalogHubConfig(AnalogHubConfig_s& config)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::Control3, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			config.analogHub = (State_t)((tmp >> (uint8_t)Control3Bitmap_t::AnalogHubEnable) & 1);
			config.addressIncrement = (State_t)((tmp >> (uint8_t)Control3Bitmap_t::AddressIncrement) & 1);
			config.interleavedMode = (State_t)((tmp >> (uint8_t)Control3Bitmap_t::AnalogHubInterleaved) & 1);
			return Return_t::OK;
		}

		/**
		 * @brief Get reference pressure in hPa/mbar.
		 * 
		 * @param output Reference to output variable.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t getReferencePressure(uint16_t& output)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::PressureReferenceHigh, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}		
			output = tmp << 8;

			if (readRegister(Register_t::PressureReferenceLow, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}	
			output |= tmp;

			return Return_t::OK;	
		}

		/**
		 * @brief Set pressure offset measured in one point calibration.
		 * 
		 * @param offset Pressure offset in N/A unit(unclear datasheet).
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t setPressureOffset(const int16_t offset)
		{
			txBuffer[0] = (uint8_t)Register_t::PressureOffsetHigh;
			txBuffer[1] = offset >> 8;
			if (writeRegister(txBuffer, 2) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			txBuffer[0] = (uint8_t)Register_t::PressureOffsetLow;
			txBuffer[1] = offset;
			if (writeRegister(txBuffer, 2) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTF_INFO("Pressure offset set to %d\n", offset);
			return Return_t::OK;		
		}

		/**
		 * @brief Get pressure offset.
		 * 
		 * @param output Reference to output for pressure offset in N/A unit(unclear datasheet).
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t getPressureOffset(int16_t& output)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::PressureOffsetHigh, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}
			output = tmp << 8;

			if (readRegister(Register_t::PressureOffsetLow, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}
			output |= tmp;	

			return Return_t::OK;
		}

		/**
		 * @brief Get interrupt source
		 * 
		 * @param source Reference to output for interrupt source.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t getInterruptSource(InterruptSource_s& source)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::InterruptSource, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			source.active = (tmp >> (uint8_t)InterruptSourceBitmap_t::ActiveInterrupt) & 1;
			source.boot = (tmp >> (uint8_t)InterruptSourceBitmap_t::Boot) & 1;
			source.pressureHigh = (tmp >> (uint8_t)InterruptSourceBitmap_t::PressureHigh) & 1;
			source.pressureLow = (tmp >> (uint8_t)InterruptSourceBitmap_t::PressureLow) & 1;
			return Return_t::OK;
		}

		/**
		 * @brief Get pressure and temperature data status.
		 * 
		 * @param output Reference to output. See \ref DataStatus_s
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t getDataStatus(DataStatus_s& output)
		{
			uint8_t tmp = 0;
			if (readRegister(Register_t::Status, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			output.pressureAvailable = (tmp >> (uint8_t)StatusBitmap_t::PressureAvailable) & 1;
			output.pressureOverrun = (tmp >> (uint8_t)StatusBitmap_t::PressureOverrun) & 1;
			output.temperatureAvailable = (tmp >> (uint8_t)StatusBitmap_t::TemperatureAvailable) & 1;
			output.temperatureOverrun = (tmp >> (uint8_t)StatusBitmap_t::TemperatureOverrun) & 1;
			return Return_t::OK;
		}

		/**
		 * @brief Get measured pressure.
		 * 
		 * @param output Reference to output for pressure in hPa/mbar.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t getPressure(uint16_t& output)
		{
			int32_t tmp = 0;
			uint8_t reg = 0;

			if (readRegister(Register_t::PressureOutHigh, reg) != Return_t::OK)
			{
				return Return_t::NOK;
			}
			tmp = reg << 16;

			if (readRegister(Register_t::PressureOutMid, reg) != Return_t::OK)
			{
				return Return_t::NOK;
			}
			tmp |= reg << 8;	

			if (readRegister(Register_t::PressureOutLow, reg) != Return_t::OK)
			{
				return Return_t::NOK;
			}
			tmp |= reg;

			static constexpr uint16_t scaleDivider[] = { 4096, 2048 };
			output = tmp / scaleDivider[(uint8_t)getPressureScale()];

			ILPS22QS_PRINTN("ILPS22QS ", 9);
			ILPS22QS_PRINTF("Pressure %u\n", output);
			return Return_t::OK;
		}

		/**
		 * @brief Get measured temperature in configured scale and centideegres.
		 * 
		 * @param output Reference to temperature output.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success. 
		 */
		Return_t getTemperature(int16_t& output)
		{
			int16_t tmp = 0;
			uint8_t reg = 0;	

			if (readRegister(Register_t::TemperatureOutHigh, reg) != Return_t::OK)
			{
				return Return_t::NOK;
			}
			tmp = reg << 8;	

			if (readRegister(Register_t::TemperatureOutLow, reg) != Return_t::OK)
			{
				return Return_t::NOK;
			}
			tmp |= reg;	

			output = convertTemperature(tmp);

			ILPS22QS_PRINTN("ILPS22QS ", 9);
			ILPS22QS_PRINTF("Temperature %d\n", output);
			return Return_t::OK;			
		}


		private:
		// ----- ENUMS
		/**
		 * @brief Enum class with register addresses.
		 * 
		 */
		enum class Register_t : uint8_t
		{
			Interrupt = 0x0B, /**< @brief Interrupt config for pressure acquisition configuration (R/W). */
			PressureThresholdLow = 0x0C, /**< @brief User-defined threshold value for pressure interrupt event (LSB)(R/W). */
			PressureThresholdHigh = 0x0D, /**< @brief User-defined threshold value for pressure interrupt event (MSB)(R/W). */
			Interface = 0x0E, /**< @brief Interface control (R/W). */
			WhoAmI = 0x0F, /**< @brief Device ID (R). */
			Control1 = 0x10, /**< @brief Control 1 (R/W). */
			Control2 = 0x11, /**< @brief Control 2 (R/W). */
			Control3 = 0x12, /**< @brief Control 3 (R/W). */
			ControlFIFO = 0x14, /**< @brief FIFO control (R/W). */
			FIFOWatermark = 0x15, /**< @brief FIFO watermark (R/W). */
			PressureReferenceLow = 0x16, /**< @brief Pressure reference (LSB)(R). */
			PressureReferenceHigh = 0x17, /**< @brief Pressure reference (MSB)(R). */
			I3CControl = 0x19, /**< @brief I3C control (R/W). */
			PressureOffsetLow = 0x1A, /**< @brief Pressure offset (LSB)(R). */
			PressureOffsetHigh = 0x1A, /**< @brief Pressure offset (MSB)(R). */
			InterruptSource = 0x24, /**< @brief Interrupt source for differential pressure (R). */
			FIFOStatus1 = 0x25, /**< @brief FIFO status (R). */
			FIFOStatus2 = 0x26, /**< @brief FIFO status (R). */
			Status = 0x27, /**< @brief Status (R). */
			PressureOutLow = 0x28, /**< @brief Either pressure output or AH/Qvar output value LSB data (R). */
			PressureOutMid = 0x29, /**< @brief Either pressure output or AH/Qvar output value middle data (R). */
			PressureOutHigh = 0x2A, /**< @brief Either pressure output or AH/Qvar output value MSB data (R). */
			TemperatureOutLow = 0x2B, /**< @brief Temperature LSB output (R). */
			TemperatureOutHigh = 0x2C, /**< @brief Temperature MSB output (R). */
			FIFOOutLow = 0x78, /**< @brief Either FIFO pressure or AH/Qvar output LSB data (R). */
			FIFOOutMid = 0x79, /**< @brief Either FIFO pressure or AH/Qvar output middle data (R). */
			FIFOOutHigh = 0x7A, /**< @brief Either FIFO pressure or AH/Qvar output MSB data (R). */

			AnalogHubDisable = 0x5F, /**< @brief Non-listed register, controls analog hub (R/W?). Reboot realods this register. */ 
		};

		/**
		 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Interrupt
		 * 
		 */
		enum class InterruptBitmap_t : uint8_t
		{
			PressureHighEvent = 0, /**< @brief Enable interrupt on pressure high event. */
			PressureLowEvent = 1, /**< @brief Enable interrupt on pressure low event. */
			InterruptLatch = 2, /**< @brief Latch interrupt request. */
			ResetAutoZero = 4, /**< @brief Reset autozero function. */
			AutoZero = 5, /**< @brief Enable autozero function. */
			ResetAutoREFP = 6, /**< @brief Reset auto REFP function. */
			AutoREFP = 7 /**< @brief Enable auto REFP function. */
		};

		/**
		 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Interface
		 * 
		 */
		enum class InterfaceBitmap_t : uint8_t
		{
			SSPullUpEnable = 1, /**< @brief Enable pull-up on slave select line. */
			SDAPullUpEnable = 4, /**< @brief Enable pull-up on SDA line. */
			SPIReadEnable = 5, /**< @brief Enable SPI read mode. */
			I2CDisable = 6 /**< @brief Disable I2C and I3C interface. */
		};

		/**
		 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Control1
		 * 
		 */
		enum class Control1Bitmap_t : uint8_t
		{
			Average = 0, /**< @brief Average selection. */
			OutputDataRate = 3 /**< @brief Output data rate. */
		};

		/**
		 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Control2
		 * 
		 */
		enum class Control2Bitmap_t : uint8_t
		{
			OneShot = 0, /**< @brief One shot mode enable. */
			Reset = 2, /**< @brief Software reset. */
			BlockDataUpdate = 3, /**< @brief Block data update until read. */
			LowPassFilterEnable = 4, /**< @brief Enable low pass filter. */
			LowPassFilterConfig = 5, /**< @brief Config low pass filter. */
			FullScale = 6, /**< @brief Enable full scale mode (from 1260hPa to 4060hPa). */
			Boot = 7 /**< @brief Reboot memory content. */
		};

		/**
		 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Control3
		 * 
		 */
		enum class Control3Bitmap_t : uint8_t
		{
			AddressIncrement = 0, /**< @brief Register address automatically incremented during a multiple byte access with a serial interface. */
			AnalogHubInterleaved = 5, /**<@brief Enables AH/Qvar and pressure hardware interleaved mode. */
			AnalogHubEnable = 7 /**< @brief Enable analog uhb/Qvar. */
		};

		/**
		 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::ControlFIFO
		 * 
		 */
		enum class FIFOControlBitmap_t : uint8_t
		{
			Mode = 0, /**< @brief FIFO mode control. */
			//TriggerMode = 2, // Glued with Mode value
			WatermarkStop = 3, /**< @brief Stop on FIFO waftermark. */
			AnalogHubFIFO = 4 /**< @brief Enables AH/Qvar and pressure hardware interleaved mode in FIFO buffer */
		};

		/**
		 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::I3CControl
		 * 
		 */
		enum class I3CControlBitmap_t : uint8_t
		{
			BusPeriod = 0, /**< @brief Bus available period when I3C IBI is used */
			AntiSpikeFilter = 5, /**< @brief Enable anti-spike filter. */
		};

		/**
		 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::InterruptSource
		 * 
		 */
		enum class InterruptSourceBitmap_t : uint8_t
		{
			PressureHigh = 0, /**< @brief Active high pressure interrupt. */
			PressureLow = 1, /**< @brief Active low pressure interrupt. */
			ActiveInterrupt = 2, /**< @brief One or more interrupts are active. */
			Boot = 7, /**< @brief Ongoing reboot. */
		};

		/**
		 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::FIFOStatus2
		 * 
		 */
		enum class FIFOStatus2Bitmap_t : uint8_t
		{
			FIFOFull = 5, /**< @brief FIFO full status. */
			FIFOOverrun = 6, /**< @brief FIFO overrun status. */
			FIFOWatermark = 7 /**< @brief FIFO watermark status. */
		};

		/**
		 * @brief Enum class with bitmap for register \ref ILPS22QS::Register_t::Status
		 * 
		 */
		enum class StatusBitmap_t : uint8_t
		{
			PressureAvailable = 0, /**< @brief Pressure data available. */
			TemperatureAvailable = 1, /**< @brief Temperature data available. */
			PressureOverrun = 4, /**< @brief Pressure overrun. */
			TemperatureOverrun = 5 /**< @brief Temperature overrun. */
		};

		/**
		 * @brief Enum class with FIFO mode values.
		 * 
		 */
		enum class FIFOMode_t : uint8_t
		{
			Bypass = 0b000,
			FIFO = 0b001,
			Continuous = 0b011,
			Bypass2FIFO = 0b101,
			Bypass2Continous = 0b110,
			Continous2FIFO = 0b111
		};
		static constexpr uint8_t FIFOModeMask = 0b111; /**< @brief Bit mask for FIFO mode. */

		/**
		 * @brief Enum class with I3C bus periods.
		 * 
		 */
		enum class I3CBusPeriod_t : uint8_t
		{
			Period50us = 0b00,
			Period2us = 0b01,
			Period1ms = 0b10,
			Period25ms = 0b11
		};
		static constexpr uint8_t busPeriodMask = 0b11; /**< @brief Bit mask for bus period. */


		// ----- VARIABLES
		static constexpr uint8_t averageMask = 0b111; /**< @brief Bit mask for average selection. */
		static constexpr uint8_t outputDataRateMask = 0b1111; /**< @brief Bit mask for output data rate. */
		static constexpr uint8_t timeout = 10; /**< @brief Driver R/W timeout in ms. */
		static constexpr uint8_t chipID = 0xB4; /**< @brief Chip ID from register \ref Register_t::WhoAmI. */
		static constexpr uint8_t pressureScaleDivider[2] = { 16, 8 }; /**< @brief Pressure output scale dividers. */

		E& interface = static_cast<E&>(*this);

		MSP_f mspInitHandler = nullptr; /**< @brief Pointer to external function for MSP init. */
		MSP_f mspDeinitHandler = nullptr; /**< @brief Pointer to external function for MSP deinit. */
		Delay_f delayHandler = nullptr; /**< @brief Pointer to external function for wait operations. */
		Tick_f tickHandler = nullptr; /**< @brief Pointer to external function for retrieving tick. */

		uint8_t txBuffer[2]; /**< @brief Buffer for outgoing data. */
		PressureScale_t pressureScale = PressureScale_t::Scale1260hPa; /**< @brief Pressure scale. */
		TemperatureScale_t temperatureScale = TemperatureScale_t::Celsius; /**< @brief Output scale for temperature. */
		Semaphore_t semaphore = Semaphore_t::Free; /**< @brief Bus process semaphore. */


		// ----- METHOD DEFINITIONS
		/**
		 * @brief Read multiple bytes from register.
		 * 
		 * @param reg Register address to read from.
		 * @param output Pointer to output buffer for bytes from \c reg
		 * @param len Number of bytes to read from \c reg
		 * 
		 * @return \c Return_t::OK on success. 
		 */
		Return_t readRegister(const Register_t reg, uint8_t* output, const uint8_t len)
		{
			txBuffer[0] = (uint8_t)reg;
			if (interface.write(txBuffer, 1) != Return_t::OK)
			{
				ILPS22QS_PRINTN_ERROR("ILPS22QS ", 9);
				ILPS22QS_PRINTF_ERROR("Register %02X write fail\n", txBuffer[0]);
				return Return_t::NOK;
			}

			if (interface.read(output, len) != Return_t::OK)
			{
				ILPS22QS_PRINTN_ERROR("ILPS22QS ", 9);
				ILPS22QS_PRINTF_ERROR("Register %02X read fail\n", txBuffer[0]);
				return Return_t::NOK;
			}

			return Return_t::OK;
		}	

		/**
		 * @brief Read single byte from register.
		 * 
		 * @param reg Register address to read from.
		 * @param output Reference to output for value from \c reg
		 * 
		 * @return \c Return_t::OK on success.
		 */
		Return_t readRegister(const Register_t reg, uint8_t& output)
		{
			uint8_t tmp = 0;
			if (readRegister(reg, &tmp, sizeof(tmp)) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			output = tmp;
			return Return_t::OK;
		}

		/**
		 * @brief Write \c len bytes into register.
		 * 
		 * @param input Pointer to input bytes. \c input[0] must contain register address.
		 * @param len Length of \c input
		 * 
		 * @return \c Return_t::OK on success.
		 */
		#ifdef DEBUG_ILPS22QS_ERROR
		Return_t writeRegister(const uint8_t* input, const uint8_t len) const
		#else
		inline Return_t writeRegister(const uint8_t* input, const uint8_t len) const
		#endif // DEBUG_ILPS22QS_ERROR
		{
			#ifdef DEBUG_ILPS22QS_ERROR
			if (interface.write(input, len) != Return_t::OK)
			{
				ILPS22QS_PRINTN_ERROR("ILPS22QS ", 9);
				ILPS22QS_PRINTF_ERROR("Register write %02X fail\n", input[1]);
				return Return_t::NOK;
			}

			return Return_t::OK;
			#else
			return interface.write(input, len);
			#endif // DEBUG_ILPS22QS_ERROR
		}

		/**
		 * @brief Read chip ID.
		 * 
		 * @param output Reference for output ID
		 * 
		 * @return \c Return_t::OK on success.
		 */
		inline Return_t whoAmI(uint8_t& output)
		{
			return readRegister(Register_t::WhoAmI, output);
		}

		/**
		 * @brief Set interface config.
		 * 
		 * @param config Pointer to interface config. See \ref InterfaceConfig_s
		 * 
		 * @return \c Return_t::OK on success.
		 */
		Return_t interfaceConfig(const InterfaceConfig_s* config)
		{
			txBuffer[0] = (uint8_t)Register_t::Interface;
			txBuffer[1] =	((uint8_t)config->i2ci3cOff << (uint8_t)(uint8_t)InterfaceBitmap_t::I2CDisable) |
						((uint8_t)config->sdaPullUp << (uint8_t)(uint8_t)InterfaceBitmap_t::SDAPullUpEnable) |
						((uint8_t)config->SPIRead << (uint8_t)(uint8_t)InterfaceBitmap_t::SPIReadEnable) |
						((uint8_t)config->ssPullUpOff << (uint8_t)(uint8_t)InterfaceBitmap_t::SSPullUpEnable);

			ILPS22QS_PRINTN_INFO("ILPS22QS ", 9);
			ILPS22QS_PRINTF_INFO("Interface cfg set to %02X\n", txBuffer[1]);
			return writeRegister(txBuffer, 2);
		}

		/**
		 * @brief Get pressure scale.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::OK on success.
		 */
		Return_t readPressureScale(void)
		{
			// Read control 2 register
			uint8_t tmp = 0;

			if (readRegister(Register_t::Control2, tmp) != Return_t::OK)
			{
				return Return_t::NOK;
			}

			ILPS22QS_PRINTN_ERROR("ILPS22QS ", 9);
			ILPS22QS_PRINTN_INFO("Scale: ", 7);

			// Get pressure scale
			if (tmp & (1 << (uint8_t)(uint8_t)Control2Bitmap_t::FullScale))
			{
				ILPS22QS_PRINTN_INFO("4060hPa\n", 8);
				pressureScale = PressureScale_t::Scale4060hPa;
			}
			else
			{
				ILPS22QS_PRINTN_INFO("1260hPa\n", 8);
				pressureScale = PressureScale_t::Scale1260hPa;
			}

			return Return_t::OK;
		}

		/**
		 * @brief Convert temperature to configured scale.
		 * 
		 * @param input Input temperature.
		 * 
		 * @return int16_t Output temperature in configured scale.
		 */
		int16_t convertTemperature(const int16_t input) const
		{
			// Convert to fahrenheit if needed
			if (getTemperatureScale() == TemperatureScale_t::Fahrenheit)
			{
				return 3200 + (input * 18 / 10);
			}

			// Just pass celsius
			return input;
		}	


		protected:
		// ----- METHOD DEFINITIONS
		/**
		 * @brief Object constructor.
		 * 
		 * @param mspInit Pointer to external function for MSP init.
		 * @param mspDeinit Pointer to external function for MSP deinit.
		 * @param tempScale Output temperature scale. See \ref TemperatureScale_t
		 * @param waitHandler Pointer to external function for handling wait state.
		 * @param tick Pointer to external function for fetching tick.
		 * 
		 * @return No return value.
		 */
		Driver(const MSP_f mspInit, const MSP_f mspDeinit, const TemperatureScale_t tempScale, const Delay_f waitHandler, const Tick_f tick)
		{
			mspInitHandler = mspInit;
			mspDeinitHandler = mspDeinit;
			delayHandler = waitHandler;
			tickHandler = tick;

			semaphore = Semaphore_t::Free;
			temperatureScale = tempScale;
			memset(txBuffer, 0, sizeof(txBuffer));
		}

		/**
		 * @brief Object deconstructor
		 * 
		 * @return No return value.
		 */
		~Driver(void)
		{

		}

		/**
		 * @brief Wait for free semaphore.
		 * 
		 * @return \c Return_t::Timeout if function timeouted.
		 * @return \c Return_t::OK if semaphore is released in time.
		 */
		Return_t wait(void) const
		{
			// Get system tick if tick handler is provided
			uint32_t tick = 0;
			if (tickHandler)
			{
				tick = tickHandler();
			}

			// Check for process semaphore
			while (isSemaphoreFree() == Return_t::NOK)
			{
				// Call wait handler if provided
				if (delayHandler)
				{
					delayHandler(1);
				}

				// Check for timeout if tick handler is provided
				if (tickHandler)
				{
					if ((tickHandler() - tick) > timeout)
					{
						ILPS22QS_PRINTN_ERROR("ILPS22QS ", 9);
						ILPS22QS_PRINTN_ERROR("Wait timeout\n", 13);
						return Return_t::Timeout;
					}
				}
			}

			return Return_t::OK;
		}

		/**
		 * @brief Check is semaphore free.
		 * 
		 * @return \c Return_t::NOK if semaphore is taken. 
		 * @return \c Return_t::OK if semaphore is free.
		 */
		inline Return_t isSemaphoreFree(void) const
		{
			if (semaphore == Semaphore_t::Free)
			{
				return Return_t::OK;
			}

			return Return_t::NOK;
		}
	};

	/**
	 * @brief Class for ILPS22QS I2C operations.
	 * 
	 */
	class I2C : public Driver<I2C>
	{
		public:
		// ----- METHOD DEFINITIONS
		/**
		 * @brief Object constructor.
		 * 
		 * @param i2cRead Pointer to external function for I2C read operations.
		 * @param i2cWrite Pointer to external function for I2C write operations.
		 * @param mspInit Pointer to external function for MSP init. Optional.
		 * @param mspDeinit Pointer to external function for MSP deinit. Optional.
		 * @param tempScale Output temperature scale. Optional. See \ref TemperatureScale_t
		 * @param waitHandler Pointer to external function for handling wait state. Optional.
		 * @param tick Pointer to external function for fetching tick. Optional.
		 * 
		 * @return No return value.
		 */	
		I2C(const I2CRW_f i2cRead, const I2CRW_f i2cWrite,
			const MSP_f mspInit = nullptr, const MSP_f mspDeinit = nullptr,
			const TemperatureScale_t tempScale = TemperatureScale_t::Celsius,
			const Delay_f waitHandler = nullptr, const Tick_f tick = nullptr) :
			Driver<I2C>(mspInit, mspDeinit, tempScale, waitHandler, tick)
		{
			readHandler = i2cRead;
			writeHandler = i2cWrite;
		}

		/**
		 * @brief Object deconstrctor.
		 * 
		 * @return No return value.
		 */
		~I2C(void)
		{

		}	

		/**
		 * @brief Read from sensor.
		 * 
		 * @param data Pointer to output buffer for data from the sensor.
		 * @param len Size of \c data buffer.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::Timeout on timeout.
		 * @return \c Return_t::OK on success.
		 */
		Return_t read(uint8_t* data, const uint8_t len) const
		{
			if (wait() != Return_t::OK)
			{
				return Return_t::Timeout;
			}

			return readHandler(address, data, len, readTimeout);
		}

		/**
		 * @brief Write to sensor.
		 * 
		 * @param data Pointer to data to write to the sensor.
		 * @param len Size of \c data buffer.
		 * 
		 * @return \c Return_t::NOK on fail.
		 * @return \c Return_t::Timeout on timeout.
		 * @return \c Return_t::OK on success.
		 */
		Return_t write(const uint8_t* data, const uint8_t len) const
		{
			if (wait() != Return_t::OK)
			{
				return Return_t::Timeout;
			}

			return writeHandler(address, (void*)data, len, writeTimeout);
		}

		/**
		 * @brief Check I2C interface handlers.
		 * 
		 * @return \c Return_t::NOK I2C interface handlers are not valid.
		 * @return \c Return_t::OK I2C interface handlers are valid.
		 */
		Return_t check(void) const
		{
			if (!readHandler || !writeHandler)
			{
				return Return_t::NOK;
			}

			return Return_t::OK;
		}

		private:
		static constexpr uint8_t address = 0x5C; /**< @brief ILPS22QS' address on I2C bus. */
		static constexpr uint8_t readTimeout = ILPS22QS_I2C_READ_TIMEOUT; /**< @brief Timeout in ms for read operation. */
		static constexpr uint8_t writeTimeout = ILPS22QS_I2C_WRITE_TIMEOUT; /**< @brief Timeout in ms for write operation. */

		I2CRW_f readHandler = nullptr; /**< @brief Pointer to external function for I2C read. */
		I2CRW_f writeHandler = nullptr; /**< @brief Pointer to external function for I2C write. */
	};

	/**
	 * @brief Class for ILPS22QS 3-wire SPI operations.
	 * 
	 */
	class SPI : protected Driver<SPI>
	{
		public:

		private:
	};

};


/** @} */


#endif // _ILPS22QS_HPP_


// END WITH NEW LINE
