/**
 * @file Main.cpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Main source file.
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
#include			"Main.hpp"
#include			"SEGGER_RTT.h"
#include			"BLE.hpp"
#include			"System.hpp"
#include			"Data.hpp"
#include			"ADC.hpp"
#include			"sBuildInfo.h"
#include			"TWI.hpp"
#include			"PTS.hpp"

#include 			"nrf_log.h"
#include 			"nrf_log_ctrl.h"
#include 			"nrf_log_default_backends.h"
#include			"nrf_gpio.h"

#include			<stdint.h>
#include 			<stdio.h>
#include			<string.h>


// ----- BUILD INFO
__SBI(APP_NAME, APP_VERSION, APP_HW, "");


// ----- ENUMS
/**
 * @brief Enum class with application states.
 * 
 */
enum class State_t : uint8_t
{
	Sleep = 0, /**< @brief Application sleep state. */
	Measure, /**< @brief Application measure state. */
	Advertise, /**< @brief Application advertise state. */
};


// ----- VARIABLES
static Data::sTPMS data = Data::sTPMS(); /**< @brief sTPMS data object. */
static State_t state = State_t::Measure; /**< @brief Application state. */
static uint8_t wakeupSet = 0; /**< @brief Wakeup timer flag. If set to \c 1 wakeup timer has started. */
static uint8_t ledMeasureCount = 0; /**< @brief Measure counter for LED. */
static uint8_t measureBattery = 0; /**< @brief Flag for measure battery with ADC. */


// ----- STATIC FUNCTION DECLARATIONS
static inline void ledInit(void);
static inline void ledOn(void);
static inline void ledOff(void);


// ----- FUNCTION DEFINITIONS
/**
 * @brief Application entry point.
 * 
 * @return No return value.
 */
int main(void)
{
	#ifdef DEBUG
	NRF_LOG_INIT(NULL);
	NRF_LOG_DEFAULT_BACKENDS_INIT();
	#endif // DEBUG

	_PRINTF("\n\n%s %s\n%s\n%s %s\n\n", SBI_APP_NAME, SBI_APP_VER, SBI_APP_HW, SBI_APP_DATE, SBI_APP_TIME);

	// LED init
	ledInit();
	ledOn();

	// Init device data and SRAM EEPROM
	Data::init(data);

	// Init system
	if (System::init() != Return_t::OK)
	{
		_PRINT_ERROR("System init fail\n");
		System::reset(System::Reset_t::SystemInit);
	}	

	// Init BLE module
	if (BLE::init() != Return_t::OK)
	{
		_PRINT_ERROR("BLE init fail\n");
		System::reset(System::Reset_t::BLEInit);
	}

	if (TWI::init() != Return_t::OK)
	{
		_PRINT_ERROR("TWI init fail\n");
		System::reset(System::Reset_t::TWIInit);
	}

	if (PTS::init() != Return_t::OK)
	{
		_PRINT_ERROR("PTS init fail\n");
		System::reset(System::Reset_t::PTSInit);
	}

	if (ADC::init() != Return_t::OK)
	{
		_PRINT_ERROR("ADC init fail\n");
		// SOON: Raise error
	}
	
	ledOff();
	while (1)
	{
		switch (state)
		{
			case State_t::Measure:
			{
				// Measure battery every time in debug buiild
				#ifdef DEBUG
				measureBattery = 1;
				#endif // DEBUG

				// Increase working seconds and uptime if needed
				Data::eeprom->workingSeconds += AppConfig::measurePeriod;
				if (Data::eeprom->workingSeconds >= 3600)
				{
					Data::eeprom->workingSeconds = 0;
					data.increaseUptime();
					measureBattery = 1;
					_PRINT_INFO("Uptime++\n");
				}

				_PRINT_INFO("--- MEASURE\n");		

				// Turn on the LED
				if (System::getResetReason() == System::Reset_t::Powerup)
				{
					if (ledMeasureCount < AppConfig::ledBlinkCount + 1) // + 1 because device will do first measure right after powerup
					{
						ledOn();
					}
				}

				// Start battery measurment
				if (measureBattery)
				{
					ADC::measure();			
				}

				// Measure PTS
				if (PTS::measure() == Return_t::OK)
				{
					data.setPressure(PTS::getPressure());
					data.setTemperature(PTS::getTemperature());
				}
				else
				{
					data.setPressure(0);
					data.setTemperature(0);
				}

				// Check if ADC is done with battery measurment
				if (measureBattery)
				{
					measureBattery = 0;
					while (ADC::isDone() != Return_t::OK);
					data.setVoltage(ADC::getVoltage());	
				}

				// Turn off the LED
				if (System::getResetReason() == System::Reset_t::Powerup)
				{
					if (ledMeasureCount < AppConfig::ledBlinkCount + 1) // + 1 because device will do first measure right after powerup
					{
						for (uint32_t i = 0; i < 0xFFF; i++)
						{
							(void)i;
						}

						ledOff();
						ledMeasureCount++;
					}
				}

				state = State_t::Advertise;
				break;
			}

			case State_t::Advertise:
			{
				_PRINT_INFO("--- ADVERTISE\n");

				// Advertise sTPMS data
				if (BLE::advertise(&data, sizeof(data)) == Return_t::OK)
				{
					while (BLE::isAdvertiseDone() != Return_t::OK);	
				}
				else
				{
					// SOON: Add error
				}

				// Feed the dog
				System::feedWatchdog();

				// Go to sleep
				state = State_t::Sleep;
				break;
			}

			default:
			{
				// Start wakeup timer if needed
				if (!wakeupSet)
				{
					wakeupSet = 1;
					System::startWakeupTimer();
				}

				// Put device to sleep
				System::sleep();

				// Woken by RTC2
				if (System::isWoken() == Return_t::OK)
				{
					wakeupSet = 0;
					state = State_t::Measure;
				}					
				break;
			}
		}	
	}
}

/**
 * @brief Debug output handler.
 * 
 * @param string Pointer to string to output.
 * @param len Length of \c string.
 * 
 * @return No return value.
 */
void sDebug::out(const char* string, const uint16_t len)
{
	SEGGER_RTT_Write(AppConfig::rttChannel, string, len);
}


// ----- STATIC FUNCTION DEFINITIONS
/**
 * @brief Init the LED.
 * 
 * @return No return value.
 */
static inline void ledInit(void)
{
	nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(Hardware::ledPort, Hardware::ledPin));
	ledOff();
}

/**
 * @brief Turn on the LED.
 * 
 * @return No return value.
 */
static inline void ledOn(void)
{
	nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(Hardware::ledPort, Hardware::ledPin), 0);	
}

/**
 * @brief Turn off the LED.
 * 
 * @return No return value.
 */
static inline void ledOff(void)
{
	nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(Hardware::ledPort, Hardware::ledPin), 1);	
}


// END WITH NEW LINE
