
######################################
# APPLICATION-RELATED FILE LIST
######################################

# APPLICATION C++ TRANSLATION FILES
APP_CPP_FILES = \
Application/Main.cpp \
Libraries/sDebug.cpp \
Modules/BLE.cpp \
Modules/System.cpp \
Modules/ADC.cpp \
Modules/TWI.cpp \
Modules/PTS.cpp \

# APPLICATION C TRANSLATION FILES
APP_C_FILES = \
Libraries/SEGGER_RTT.c \

# APPLICATION ASSEMBLER TRANSLATION FILES
APP_ASM_FILES = \


######################################
# APPLICATION-RELATED INCLUDE PATHS
######################################

APP_INCLUDE_PATHS = \
-IApplication/Inc \
-ICMSIS \
-IConfig \
-IDrivers/Inc \
-ILibraries/Inc \
-IModules/Inc


######################################
# APPLICATION-RELATED DEFINES
######################################

APP_DEFINES = \

