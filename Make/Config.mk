
######################################
# TOOLCHAIN CONFIG
######################################

# SET TO 1 TO PRINT COMPILER VERSION AFTER BUILD
TC_PRINT_VER = 0

# TOOLCHAIN PREFIX
TC_PREFIX = arm-none-eabi-

ifeq ($(GPP), 1)
TC_CC = $(TC_PREFIX)g++
TC_AS = $(TC_PREFIX)g++
else
TC_CC = $(TC_PREFIX)gcc
TC_AS = $(TC_PREFIX)gcc -x assembler-with-cpp
endif

TC_CP = $(TC_PREFIX)objcopy
TC_SZ = $(TC_PREFIX)size
TC_HEX = $(TC_CP) -O ihex
TC_BIN = $(TC_CP) -O binary -S


######################################
# DIRECTORIES AND FILES
######################################

# DIRECTORY FOR CURRENT BUILD
DIR_BUILD = .builds\$(BUILD_NAME)

# DIRECTORY FOR HARDWARE BUILD
DIR_HARDWARE = Hardware/$(HW_NAME)

# DIRECTORY WITH LINKER SCRIPT
DIR_LINKER = $(DIR_HARDWARE)/Linker

# DIRECTORY WITH STARTUP FILE
DIR_STARTUP = $(DIR_HARDWARE)/Startup

# OUTPUT FILE
DIR_OUTPUT = .outputs\$(BUILD_NAME)

# MAKE FILE LOCATION
MAKEFILE = Builds/$(BUILD_NAME).mk

# J-LINK SCRIPT FILE LOCATIONS
JLINK_FLASH = .jlink\JLink_Flash_$(HW_NAME).jlink
JLINK_ERASE = .jlink\JLink_Erase_$(HW_NAME).jlink
JLINK_ERASE_ALL = .jlink\JLink_EraseAll_$(HW_NAME).jlink
JLINK_RESET = .jlink\JLink_Reset_$(HW_NAME).jlink

# CMSIS CONFIGURATION WIZARD LOCATION
CMSIS_WIZARD = CMSIS/CMSIS_Configuration_Wizard.jar