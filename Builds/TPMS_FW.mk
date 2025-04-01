# BUILD NAME
BUILD_NAME = TPMS_FW

# NAME OF MAKE FILE FOR HARDWARE BUILD
HW_NAME = TPMS1


######################################
# DEBUG CONFIG
######################################

# SET TO 1 TO ENABLE DEBUG BUILD
DEBUG = 0

# SET TO 1 TO USE STACK FOR FORMATTED PRINTS
DEBUG_STACK_PRINTF = 0

# DEBUG FORMATTED PRINT BUFFER SIZE IN BYTES (SET TO 0 TO USE DEFAULT SIZE)	
DEBUG_BUFFER_SIZE = 128

# MODULE DEBUG LEVELS
DEBUG_ENABLE = \

# LOCAL DEBUG LEVELS
DEBUG_LEVEL = \

# GLOBAL DEBUG LEVELS (SET TO 1 TO ENABLE)
DEBUG_VERBOSE = 0
DEBUG_INFO = 0
DEBUG_ERROR = 0


######################################
# BUILD CONFIG
######################################

# STACK SIZE IN BYTES
APP_STACK = 1024

# HEAP SIZE IN BYTES
APP_HEAP = 0

# SET TO 1 TO BUILD WITH RTOS
APP_RTOS = 0

# SET TO 1 TO OPTIMIZE BUILD FOR SIZE
SIZE = 0

# SET TO 1 TO ENABLE LINK TIME OPTIMIZATION (LAST LINE OF DEFENCE IN CASE OF ROM SHORTAGE)
FLTO = 0

# SET TO 1 TO USE -g3 FLAG IN DEBUG BUILD
USE_G3 = 0

# SET TO 1 TO USE G++ INSTEAD OF GCC COMPILER
GPP = 0

# SET TO 1 TO GENERATE STACK ANALYSIS FILE FOR EACH TRANSLATION UNIT
STACK_ANALYSIS = 0

# SET TO 0 TO DISABLE WARNING IF STACKOVERFLOW MIGHT HAPPEND OR SET TO X BYTES FOR MAXIMUM STACK SIZE
STACK_OVERFLOW = 0

# SET TO 1 TO GENERATE RUNTIME TYPE IDENTIFICATION INFORMATION
RTTI = 0

# SET TO 1 TO CATCH EXCEPTIONS
EXCEPTIONS = 0

# SET TO 1 TO USE DEFAULT LIB
DEF_LIB = 0


######################################
# BUILD FILES, INCLUDES OR DEFINES
######################################

# C++ TRANSLATION FILES
BUILD_CPP_FILES = \

# C TRANSLATION FILES
BUILD_C_FILES = \

# ASSEMBLER TRANSLATION FILES
BUILD_ASM_FILES = \

# INCLUDE DIRECTORIES
BUILD_INCLUDE_PATHS = \

# DEFINES
BUILD_DEFINES = \
-DCONFIG_GPIO_AS_PINRESET \


######################################
# J-LINK-RELATED CONFIG
######################################

# J-LINK RTT UP BUFFER SIZE (32-bit aligned, 0 to use default)
JLINK_RTT_UP = 512

# J-LINK RTT UP BUFFER SIZE (32-bit aligned, 0 to use default)
JLINK_RTT_DOWN = 0


######################################
# CONFIGURE APPLICATION OR HARDWARE MAKE
######################################


######################################
# MAKE CONFIG
######################################

include Make/Config.mk


######################################
# APPLICATION MAKE
######################################

include Config/AppConfig.mk


######################################
# HARDWARE MAKE
######################################

include $(DIR_HARDWARE)/$(HW_NAME).mk


######################################
# RECONFIGURE APPLICATION OR HARDWARE MAKE
######################################



############################################################################
# DO NOT ALTER STUFF BELOW
############################################################################

#######################################
# BACKEND MAKE FILE
#######################################

include Make/Backend.mk

