/**
 * @file BLE.cpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Bluetooth module source file.
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
#include 			"BLE.hpp"
#include			"Main.hpp"

#include 			"nrf.h"
#include 			"app_error.h"

#include 			"nrf_log.h"
#include 			"nrf_log_ctrl.h"
#include 			"nrf_log_default_backends.h"

#include 			"nrf_sdh.h"
#include 			"nrf_sdh_soc.h"
#include 			"nrf_sdh_ble.h"

#include 			"ble.h"
#include 			"ble_hci.h"
#include 			"ble_srv_common.h"
#include 			"ble_advertising.h"
#include 			"ble_advdata.h"
#include 			"ble_conn_params.h"
#include 			"nrf_ble_gatt.h"



// ----- VARIABLES
static int8_t txPower = AppConfig::advTXPower; /**< @brief TX power in dBm. */
static uint8_t advHandle = BLE_GAP_ADV_SET_HANDLE_NOT_SET; /**< @brief Advertisement handle. */
static uint8_t gapAdvDataRaw[BLE_GAP_ADV_SET_DATA_SIZE_MAX]; /**< @brief Raw advertise data. */
static ble_gap_adv_params_t advConfig; /**< @brief Advertise configuration. */
static ble_gap_adv_data_t gapAdvData = /**< @brief Advertise and scan response data. */
{
	.adv_data =
	{
		.p_data = gapAdvDataRaw,
		.len = sizeof(gapAdvDataRaw)
	},
		
	.scan_rsp_data =	
	{
		.p_data = nullptr,
		.len = 0
	}
};
static uint8_t advDone = 0;


// ----- STATIC FUNCTION DECLARATIONS
static Return_t gapInit(void);
static Return_t advInit(void);
static void onBLEEvent(ble_evt_t const* event, void* context);



// ----- FUNCTION DEFINIITONS
/**
 * @brief Bluetooth module namespace.
 * 
 */
namespace BLE
{
	/**
	 * @brief Init Bluetooth module.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	Return_t init(void)
	{
		ret_code_t ret = nrf_sdh_enable_request();
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}
	
		// Configure the BLE stack using the default settings.
		uint32_t ramStart = 0;
		ret = nrf_sdh_ble_default_cfg_set(AppConfig::bleTag, &ramStart);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}
	
		// Enable BLE stack.
		ret = nrf_sdh_ble_enable(&ramStart);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}
	
		// Register a handler for BLE events.
		NRF_SDH_BLE_OBSERVER(m_ble_observer, 3, onBLEEvent, NULL);

		// Init GAP profile
		if (gapInit() != Return_t::OK)
		{
			return Return_t::NOK;
		}	

		// Get MAC address
		ble_gap_addr_t addr;
		ret = sd_ble_gap_addr_get(&addr);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}		
		_PRINTF_INFO("MAC: %02X%02X%02X%02X%02X%02X\n", addr.addr[5], addr.addr[4], addr.addr[3], addr.addr[2], addr.addr[1], addr.addr[0]);

		// Enable DC/DC
		ret = sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}

		// Init advertise
		return advInit();
	}

	Return_t deinit(void)
	{
		ret_code_t ret = nrf_sdh_disable_request();
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}		
		return Return_t::OK;
	}

	/**
	 * @brief Advertise data.
	 * 
	 * @return \c Return_t::NOK on fail.
	 * @return \c Return_t::OK on success.
	 */
	Return_t advertise(const void* data, const uint8_t len)
	{
		// Set custom data
		ble_advdata_manuf_data_t mnfData;
		
		mnfData.company_identifier = AppConfig::bleMnfID;
		mnfData.data.p_data = (uint8_t*)data;
		mnfData.data.size = len;
	
		// Set advertise data
		ble_advdata_t advData;
		memset(&advData, 0, sizeof(advData));
		advData.name_type = BLE_ADVDATA_FULL_NAME;
		advData.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
		advData.p_manuf_specific_data = &mnfData;
		advData.p_tx_power_level = &txPower;
	
		// Encode advertise data
		ret_code_t ret = ble_advdata_encode(&advData, gapAdvData.adv_data.p_data, &gapAdvData.adv_data.len);
		if (ret != NRF_SUCCESS)
		{	
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}

		// Advertise data
		advDone = 0;
		ret = sd_ble_gap_adv_start(advHandle, AppConfig::bleTag);
		if (ret != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(ret);
			return Return_t::NOK;
		}		

		return Return_t::OK;
	}

	/**
	 * @brief Check is advertise done.
	 * 
	 * @return \c Return_t::NOK advertise is not done.
	 * @return \c Return_t::OK advertise is done.
	 */
	Return_t isAdvertiseDone(void)
	{
		if (advDone)
		{
			advDone = 0;
			return Return_t::OK;
		}

		return Return_t::NOK;
	}
};


// STATIC FUNCTION DEFINITIONS
/**
 * @brief Init BLE GAP profile.
 * 
 * @return \c Return_t::NOK on fail.
 * @return \c Return_t::OK on success.
 */
static Return_t gapInit(void)
{
	ble_gap_conn_sec_mode_t securtiyMode;
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&securtiyMode);

	ret_code_t ret = sd_ble_gap_device_name_set(&securtiyMode, (const uint8_t*)AppConfig::deviceName, __CONST_STR_LEN(AppConfig::deviceName));
	if (ret != NRF_SUCCESS)
	{
		APP_ERROR_CHECK(ret);
		return Return_t::NOK;
	}

	return Return_t::OK;
}

/**
 * @brief Init BLE advertise.
 * 
 * @return \c Return_t::NOK on fail.
 * @return \c Return_t::OK on success. 
 */
static Return_t advInit(void)
{
	// Set advertise config
	advConfig.primary_phy = BLE_GAP_PHY_AUTO;
	advConfig.duration = 0;
	advConfig.properties.type = BLE_GAP_ADV_TYPE_NONCONNECTABLE_SCANNABLE_UNDIRECTED;
	advConfig.max_adv_evts	= AppConfig::advCount;
	advConfig.p_peer_addr = nullptr;
	advConfig.filter_policy = BLE_GAP_ADV_FP_ANY;
	advConfig.interval = 32; // Does not matter since max advertise event is set to 1 

	ret_code_t ret = sd_ble_gap_adv_set_configure(&advHandle, &gapAdvData, &advConfig);
	if (ret != NRF_SUCCESS)
	{	
		APP_ERROR_CHECK(ret);
		return Return_t::NOK;
	}

	// Set TX power
	ret = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_ADV, advHandle, txPower);
	if (ret != NRF_SUCCESS)
	{	
		APP_ERROR_CHECK(ret);
		return Return_t::NOK;
	}		

	return Return_t::OK;
}

/**
 * @brief BLE stack event handler.
 * 
 * @param event Pointer to event data.
 * @param context Pointer to event context.
 * 
 * @return No return value.
 */
static void onBLEEvent(ble_evt_t const* event, void* context)
{
	_PRINTF_INFO("BLE EVT %u\n", event->header.evt_id);
	ret_code_t ret = NRF_SUCCESS;

	switch (event->header.evt_id)
	{
		case BLE_GAP_EVT_DISCONNECTED:
		{
			_PRINT_INFO("BLE disconnected\n");
			break;
		}

		case BLE_GAP_EVT_CONNECTED:
		{
			_PRINT_INFO("BLE connect\n");
			ret = sd_ble_gap_disconnect(event->evt.gattc_evt.conn_handle,
				BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
			APP_ERROR_CHECK(ret);
			break;
		}

		case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
		{
			_PRINT_INFO("PHY update request\n");
			static const ble_gap_phys_t phys =
			{
				.tx_phys = BLE_GAP_PHY_AUTO,
				.rx_phys = BLE_GAP_PHY_AUTO,
			};
			ret = sd_ble_gap_phy_update(event->evt.gap_evt.conn_handle, &phys);
			APP_ERROR_CHECK(ret);
			break;
		} 

		case BLE_GATTC_EVT_TIMEOUT:
		{
			_PRINT_INFO("GATT Client Timeout\n");
			ret = sd_ble_gap_disconnect(event->evt.gattc_evt.conn_handle,
												BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
			APP_ERROR_CHECK(ret);
			break;
		}

		case BLE_GATTS_EVT_TIMEOUT:
		{
			_PRINT_INFO("GATT Server Timeout\n");
			ret = sd_ble_gap_disconnect(event->evt.gatts_evt.conn_handle,
												BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
			APP_ERROR_CHECK(ret);
			break;
		}

		case BLE_GAP_EVT_ADV_SET_TERMINATED:
		{
			advDone = 1;	
			_PRINT_INFO("Advertise done\n");
			break;
		}

		default: break;
	}
}


// END WITH NEW LINE
