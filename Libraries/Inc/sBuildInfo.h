/**
 * @file sBuildInfo.h
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Simple Build Info library header file.
 * 
 * @copyright Copyright (c) 2025, silvio3105
 * 
 */

/*
License

Copyright (c) 2025, silvio3105 (www.github.com/silvio3105)

Access and use of this Project and its contents are granted free of charge to any Person.
The Person is allowed to copy, modify and use The Project and its contents only for non-commercial use.
Commercial use of this Project and its contents is prohibited.
Modifying this License and/or sublicensing is prohibited.

THE PROJECT AND ITS CONTENT ARE PROVIDED "AS IS" WITH ALL FAULTS AND WITHOUT EXPRESSED OR IMPLIED WARRANTY.
THE AUTHOR KEEPS ALL RIGHTS TO CHANGE OR REMOVE THE CONTENTS OF THIS PROJECT WITHOUT PREVIOUS NOTICE.
THE AUTHOR IS NOT RESPONSIBLE FOR DAMAGE OF ANY KIND OR LIABILITY CAUSED BY USING THE CONTENTS OF THIS PROJECT.

This License shall be included in all methodal textual files.
*/


#ifndef _SBUILDINFO_H_
#define _SBUILDINFO_H_


/** \addtogroup sBuildInfo 
 * Simple Build Info library
 * 
 * Library can be used only with GCC toolchain.
 * 
 * If \c SBI_NO_FIX is not defined during build, application build info is placed inside \c sBuildInfo section. User must ensure it exists in linker script.
 * 
 * Define \c SBI_NO_FIX in build and build info will not be placed at fixed location in flash memory.
 * 
 * Set maximum length of build info by providing \c SBI_APP_NAME_LEN, \c SBI_APP_VER_LEN, \c SBI_APP_HW_LEN and \c SBI_APP_FLAGS_LEN defines during build. Default length for info is 16 chars(12 for build flags) and 8 chars for application tag and variant.
 * 
 * Build flags example: \c DFT (one character per build flag)
 * - \c D for build with debug prints.
 * - \c F for build with enabled FPU.
 * - \c T for build with trace log system.
 * 
 * @{
*/

// ----- DEFINES
#define SBI_VERSION					"v2.0r2" /**< @brief Library version string. */

#ifndef SBI_APP_NAME_LEN
#define SBI_APP_NAME_LEN			16 /**< @brief Maximum length of application name. */
#endif // SBI_APP_NAME_LEN

#ifndef SBI_APP_VER_LEN
#define SBI_APP_VER_LEN				16 /**< @brief Maximum length of application version. */
#endif // SBI_APP_VER_LEN

#ifndef SBI_APP_HW_LEN
#define SBI_APP_HW_LEN				16 /**< @brief Maximum length of hardware version. */
#endif // SBI_APP_HW_LEN

#ifndef SBI_APP_FLAGS_LEN
#define SBI_APP_FLAGS_LEN			12 /**< @brief Maximum length of build flags. */
#endif // SBI_APP_FLAGS_LEN



// ----- CODE SNIPPETS
#ifndef SBI_NO_FIX
/**
 * @brief Code snippet for creating application build info in flash memory.
 * 
 * Application build info will be placed on fixed location in flash memory with user-defined \c sBuildInfo section within linker script.
 * 
 * @param _name Application name. Max \c SBI_APP_NAME_LEN chars.
 * @param _ver Application version. Max \c SBI_APP_VER_LEN chars.
 * @param _rev Hardware revision. Max \c SBI_APP_HW_LEN chars.
 * @param _flags Build flags. Max \c SBI_APP_FLAGS_LEN chars.
 */
#define __SBI(_name, _ver, _rev, _flags) \
	volatile const BuildInfo_s __sBuildInfo __attribute__((section(".sBuildInfo"))) = \
	{ \
		_name, \
		_ver, \
		_rev, \
		__DATE__, \
		_flags, \
		__TIME__ \
	}

/**
 * @brief Code snippet for creating extended application build info in flash memory.
 * 
 * Application build info will be placed on fixed location in flash memory with user-defined \c sBuildInfo section within linker script.
 * With this snippet, application name is replaced with application tag and variant, both \c SBI_APP_NAME_LEN/2 characters long.
 * 
 * @param _tag Application tag. Max \c SBI_APP_NAME_LEN/2 chars.
 * @param _variant Application variant. Max \c SBI_APP_NAME_LEN/2 chars.
 * @param _ver Application version. Max \c SBI_APP_VER_LEN chars.
 * @param _rev Hardware revision. Max \c SBI_APP_HW_LEN chars.
 * @param _flags Build flags. Max \c SBI_APP_FLAGS_LEN chars.
 * 
 * @warning GCC 11.3+ is required for this snippet.
 */
#define __SBI_EXT(_tag, _variant, _ver, _rev, _flags) \
	volatile const BuildInfo_s __sBuildInfo __attribute__((section(".sBuildInfo"))) = \
	{ \
		{ \
			.tag = _tag, \
			_variant \
		}, \
		_ver, \
		_rev, \
		__DATE__, \
		_flags, \
		__TIME__ \
	}
#else // SBI_NO_FIX
/**
 * @brief Code snippet for creating application build info in flash memory.
 * 
 * @param _name Application name. Max \c SBI_APP_NAME_LEN chars.
 * @param _ver Application version. Max \c SBI_APP_VER_LEN chars.
 * @param _rev Hardware revision. Max \c SBI_APP_HW_LEN chars.
 * @param _flags Build flags. Max \c SBI_APP_FLAGS_LEN chars.
 */
#define __SBI(_name, _ver, _rev, _flags) \
	volatile const BuildInfo_s __sBuildInfo = \
	{ \
		_name, \
		_ver, \
		_rev, \
		__DATE__, \
		_flags, \
		__TIME__ \
	}

/**
 * @brief Code snippet for creating extended application build info in flash memory.
 * 
 * With this snippet, application name is replaced with application tag and variant, both \c SBI_APP_NAME_LEN/2 characters long.
 * 
 * @param _tag Application tag. Max \c SBI_APP_NAME_LEN/2 chars.
 * @param _variant Application variant. Max \c SBI_APP_NAME_LEN/2 chars.
 * @param _ver Application version. Max \c SBI_APP_VER_LEN chars.
 * @param _rev Hardware revision. Max \c SBI_APP_HW_LEN chars.
 * @param _flags Build flags. Max \c SBI_APP_FLAGS_LEN chars.
 * 
 * @warning GCC 11.3+ is required for this snippet.
 */
#define __SBI_EXT(_tag, _variant, _ver, _rev, _flags) \
	volatile const BuildInfo_s __sBuildInfo = \
	{ \
		{ \
			.tag = _tag, \
			_variant \
		}, \
		_ver, \
		_rev, \
		__DATE__, \
		_flags, \
		__TIME__ \
	}
#endif // SBI_NO_FIX


#define SBI_APP_NAME				__sBuildInfo.app.name /**< @brief Macro for application name. */
#define SBI_APP_TAG					__sBuildInfo.app.tag /**< @brief Macro for application tag. */
#define SBI_APP_VARIANT				__sBuildInfo.app.variant /**< @brief Macro for application variant. */
#define SBI_APP_VER					__sBuildInfo.appVer /**< @brief Macro for application version. */
#define SBI_APP_HW					__sBuildInfo.hwRev /**< @brief Macro for application hardware revision. */
#define SBI_APP_DATE				__sBuildInfo.buildDate /**< @brief Macro for application build date. Example date: \c Aug \c  8 \c 2019 (day is padded). */
#define SBI_APP_FLAGS				__sBuildInfo.buildFlags /**< @brief Macro for application build flags. Example: \c D flag for debug build. */
#define SBI_APP_TIME				__sBuildInfo.buildTime /**< @brief Macro for application build time. */
#define SBI_APP_USED				(void)(SBI_NAME) /**< @brief Code snippet for preventing compiler from removing build info from flash memory. */


// ----- STRUCTS
/**
 * @brief Build info struct.
 * 
 */
struct BuildInfo_s
{
	/**
	 * @brief Union of application name, tag and variant.
	 * 
	 */
	union
	{
		const char name[SBI_APP_NAME_LEN]; /**< @brief C-string for application name. */
		struct
		{
			const char tag[SBI_APP_NAME_LEN / 2]; /**< @brief C-string for application tag. */
			const char variant[SBI_APP_NAME_LEN / 2]; /**< @brief C-string for application variant name. */
		};
	} app;
	const char appVer[SBI_APP_VER_LEN]; /**< @brief C-string for application version. */
	const char hwRev[SBI_APP_HW_LEN]; /**< @brief C-string for hardware revision. */
	const char buildDate[12]; /**< @brief C-string for build date. */
	const char buildFlags[SBI_APP_FLAGS_LEN]; /**< @brief C-string for build flags. */
	const char buildTime[10]; /**< @brief C-string for build time. */
};


// ----- EXTERNS
extern volatile const BuildInfo_s __sBuildInfo;


/** @} */


#endif // _SBUILDINFO_H_

// END WITH NEW LINE
