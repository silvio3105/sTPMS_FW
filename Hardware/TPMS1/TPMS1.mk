
######################################
# MCU-RELATED CONFIG
######################################

# COMPILER DEFINES FOR MCU
MCU_DEFINE = NRF52832_XXAA

# MCU CORE
MCU_CORE = cortex-m4

# NAME OF MCU LINKER SCRIPT FILE
MCU_LINKER = nrf52832_xxaa.ld

# NAME OF MCU STARTUP FILE
MCU_STARTUP = startup_nrf52.s

# MCU CORE ARTICHETURE
MCU_ARCH = armv7-m

# SET TO 1 TO USE BIG ENDIAN
MCU_BIG_ENDIAN = 0

# MCU FLOAT ABI (leave empty for no float ABI instruction ; "soft" software floating-point ; "softfp" hardware floating-point but software conversions ; "hard" hardware floating-point)
MCU_FLOAT = hard


######################################
# RTOS CONFIG
######################################

# RTOS SYSTEM FILE NAME
RTOS_SYS = nrf.h

# RTOS IRQ FILE NAME
RTOS_IRQ = irq_armv7m.s

# RTOS CONFIG FILE
RTOS_CFG = RTOS/RTX/Inc/RTX_Config.h


######################################
# J-LINK CONFIG
######################################

# RTOS SYSTEM FILE
JLINK_DEVICE = nRF52832_xxAA

# J-LINK START ADDRESS
JLINK_ADDR_START = 0x26000

# J-LINK END ADDRESS
JLINK_ADDR_END = 0x30000

# J-LINK SWD SPEED IN kHz
JLINK_SPEED = 4000


######################################
# SOFTDEVICE
######################################

# ENABLE SOFTDEVICE
APP_SOFTDEVICE = 1

# J-LINK SCRIPT
JLINK_SOFTDEVICE = .jlink/JLink_Softdevice_$(BUILD_NAME).jlink

# FILE NAME
SOFTDEVICE = s132_nrf52_7.2.0_softdevice.hex


######################################
# HARDWARE-RELATED FILE LIST
######################################

# C++ TRANSLATION FILES
HW_CPP_FILES = \

# C TRANSLATION FILES
HW_C_FILES = \
$(DIR_HARDWARE)/Src/system_nrf52.c \
$(DIR_HARDWARE)/SDK/external/fprintf/nrf_fprintf.c \
$(DIR_HARDWARE)/SDK/external/fprintf/nrf_fprintf_format.c \
$(DIR_HARDWARE)/SDK/components/libraries/experimental_section_vars/nrf_section_iter.c \
$(DIR_HARDWARE)/SDK/components/libraries/log/src/nrf_log_backend_rtt.c \
$(DIR_HARDWARE)/SDK/components/libraries/log/src/nrf_log_frontend.c \
$(DIR_HARDWARE)/SDK/components/libraries/log/src/nrf_log_default_backends.c \
$(DIR_HARDWARE)/SDK/components/libraries/log/src/nrf_log_backend_serial.c \
$(DIR_HARDWARE)/SDK/components/libraries/log/src/nrf_log_str_formatter.c \
$(DIR_HARDWARE)/SDK/components/libraries/strerror/nrf_strerror.c \
$(DIR_HARDWARE)/SDK/components/libraries/util/app_error.c \
$(DIR_HARDWARE)/SDK/components/libraries/util/app_error_handler_gcc.c \
$(DIR_HARDWARE)/SDK/components/libraries/util/app_error_weak.c \
$(DIR_HARDWARE)/SDK/components/libraries/util/app_util_platform.c \
$(DIR_HARDWARE)/SDK/components/libraries/memobj/nrf_memobj.c \
$(DIR_HARDWARE)/SDK/components/libraries/balloc/nrf_balloc.c \
$(DIR_HARDWARE)/SDK/components/libraries/atomic/nrf_atomic.c \
$(DIR_HARDWARE)/SDK/components/libraries/ringbuf/nrf_ringbuf.c \


# ASSEMBLER TRANSLATION FILES
HW_ASM_FILES = \


######################################
# HARDWARE-RELATED INCLUDE PATHS
######################################

# INCLUDE DIRECTORIES
HW_INCLUDE_PATHS = \
-I$(DIR_HARDWARE) \
-I$(DIR_HARDWARE)/Inc \
-I$(DIR_HARDWARE)/SDK \
-I$(DIR_HARDWARE)/SDK/modules/nrfx \
-I$(DIR_HARDWARE)/SDK/modules/nrfx/soc \
-I$(DIR_HARDWARE)/SDK/modules/nrfx/hal \
-I$(DIR_HARDWARE)/SDK/external/fprintf \
-I$(DIR_HARDWARE)/SDK/components \
-I$(DIR_HARDWARE)/SDK/components/libraries/util \
-I$(DIR_HARDWARE)/SDK/components/libraries/experimental_section_vars \
-I$(DIR_HARDWARE)/SDK/components/libraries/log \
-I$(DIR_HARDWARE)/SDK/components/libraries/log/src \
-I$(DIR_HARDWARE)/SDK/components/libraries/strerror \
-I$(DIR_HARDWARE)/SDK/components/libraries/memobj \
-I$(DIR_HARDWARE)/SDK/components/libraries/balloc \
-I$(DIR_HARDWARE)/SDK/components/libraries/atomic \
-I$(DIR_HARDWARE)/SDK/components/libraries/ringbuf \
-I$(DIR_HARDWARE)/SDK/components/libraries/DELAY \


######################################
# HARDWARE-RELATED DRFINES
######################################

# INCLUDE DIRECTORIES
HW_DEFINES = \


######################################
# RTOS STUFF
######################################

ifeq ($(APP_RTOS), 1)
HW_CPP_FILES += \

HW_C_FILES += \
RTOS/RTX/RTX_Config.c \
RTOS/RTX/os_systick.c \
RTOS/RTX/rtx_delay.c \
RTOS/RTX/rtx_evflags.c \
RTOS/RTX/rtx_evr.c \
RTOS/RTX/rtx_kernel.c \
RTOS/RTX/rtx_memory.c \
RTOS/RTX/rtx_mempool.c \
RTOS/RTX/rtx_msgqueue.c \
RTOS/RTX/rtx_mutex.c \
RTOS/RTX/rtx_semaphore.c \
RTOS/RTX/rtx_system.c \
RTOS/RTX/rtx_thread.c \
RTOS/RTX/rtx_timer.c \
RTOS/RTX/rtx_lib.c

HW_ASM_FILES += \
RTOS/RTX/IRQ/$(RTOS_IRQ)

HW_INCLUDE_PATHS += \
-IRTOS/RTX/Inc

HW_DEFINES += \
-DCMSIS_device_header=\"$(RTOS_SYS)\"
endif


######################################
# SOFTDEVICE STUFF
######################################

ifeq ($(APP_SOFTDEVICE), 1)
HW_CPP_FILES += \

HW_C_FILES += \
$(DIR_HARDWARE)/Softdevice/Src/nrf_sdh.c \
$(DIR_HARDWARE)/Softdevice/Src/nrf_sdh_ble.c \
$(DIR_HARDWARE)/Softdevice/Src/nrf_sdh_soc.c \
$(DIR_HARDWARE)/SDK/components/ble/common/ble_advdata.c \
$(DIR_HARDWARE)/SDK/components/ble/ble_advertising/ble_advertising.c \
$(DIR_HARDWARE)/SDK/components/ble/nrf_ble_gatt/nrf_ble_gatt.c \

HW_ASM_FILES += \


HW_INCLUDE_PATHS += \
-I$(DIR_HARDWARE)/Softdevice/Inc \
-I$(DIR_HARDWARE)/SDK/components/ble/common \
-I$(DIR_HARDWARE)/SDK/components/ble/ble_advertising \
-I$(DIR_HARDWARE)/SDK/components/ble/nrf_ble_gatt \

HW_DEFINES += \
-DSOFTDEVICE_PRESENT \
-DS132 \
-DNRF_SD_BLE_API_VERSION=7 \
-DNRF_BLE_GATT_ENABLED \

endif
