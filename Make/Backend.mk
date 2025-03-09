
######################################
# FILES, INCLUDES AND DEFINES
######################################

# C++ TRANSLATION FILES
CPP_FILES = \
$(BUILD_CPP_FILES) \
$(HW_CPP_FILES) \
$(APP_CPP_FILES)

# C TRANSLATION FILES
C_FILES = \
$(BUILD_C_FILES) \
$(HW_C_FILES) \
$(APP_C_FILES)

# ASSEMBLER TRANSLATION FILES
ASM_FILES = \
$(BUILD_ASM_FILES) \
$(HW_ASM_FILES) \
$(APP_ASM_FILES) \
$(DIR_STARTUP)/$(MCU_STARTUP) \

# INCLUDE DIRECTORIES
INCLUDE_PATHS = \
$(BUILD_INCLUDE_FILES) \
$(HW_INCLUDE_PATHS) \
$(APP_INCLUDE_PATHS) \
-I$(DIR_HARDWARE)

# DEFINES
DEFINES = \
-DBUILD_$(BUILD_NAME) \
-DHW_$(HW_NAME) \
-D$(MCU_DEFINE) \
$(BUILD_DEFINES) \
$(HW_DEFINES) \
$(APP_DEFINES)


#######################################
# J-LINK
#######################################

# J-LINK RTT DEFINES
ifneq ($(JLINK_RTT_UP), 0)
DEFINES += -DBUFFER_SIZE_UP=$(JLINK_RTT_UP)
endif

ifneq ($(JLINK_RTT_DOWN), 0)
DEFINES += -DBUFFER_SIZE_DOWN=$(JLINK_RTT_DOWN)
endif

# RTOS DEFINE
ifeq ($(APP_RTOS), 1)
DEFINES += -DUSING_RTOS
endif


#######################################
# DEBUG
#######################################

DEFINES += $(DEBUG_ENABLE) $(DEBUG_LEVEL)

ifeq ($(DEBUG_VERBOSE), 1)
DEFINES += -DDEBUG_VERBOSE
endif

ifeq ($(DEBUG_INFO), 1)
DEFINES += -DDEBUG_INFO
endif

ifeq ($(DEBUG_ERROR), 1)
DEFINES += -DDEBUG_ERROR
endif

ifneq ($(DEBUG_BUFFER_SIZE), 0)
DEFINES += -DDEBUG_BUFFER_SIZE=$(DEBUG_BUFFER_SIZE)
endif

ifeq ($(DEBUG_STACK_PRINTF), 1)
DEFINES += -DDEBUG_STACK_PRINTF
endif

# DEBUG BUILD
ifeq ($(DEBUG), 1)

# DEBUG DEFINES
DEFINES += -DDEBUG

# COMPILER FLAGS
OPTIMIZATION = -Og
COMPILER_FLAGS += -ggdb

ifeq ($(USE_G3), 1)
COMPILER_FLAGS += -g3
else
COMPILER_FLAGS += -g
endif

else

# SIZE OPTIMIZATION
ifeq ($(SIZE), 1)
OPTIMIZATION = -Os
else
OPTIMIZATION = -Ofast
endif
ifeq ($(FLTO), 1)
COMPILER_FLAGS += -flto
endif

endif


#######################################
# COMPILER-ASSEMBLER-LINKER FLAGS
#######################################

CAL_FLAGS = -mcpu=$(MCU_CORE) -mthumb -mfloat-abi=$(MCU_FLOAT)

ifeq ($(MCU_BIG_ENDIAN), 1)
CAL_FLAGS += -mbig-endian
endif


#######################################
# COMPILER FLAGS
#######################################

COMPILER_FLAGS += $(CAL_FLAGS) $(DEFINES) $(INCLUDE_PATHS) $(OPTIMIZATION) -MMD -MP -MF"$(@:%.o=%.d)" -Wall -Wdouble-promotion -Wshadow -Wformat=2 -Wformat-overflow -Wformat-truncation -fdata-sections -ffunction-sections

ifeq ($(GPP), 1)
ifeq ($(RTTI), 0)
COMPILER_FLAGS += -fno-rtti
endif
endif

ifeq ($(EXCEPTIONS), 0)
COMPILER_FLAGS += -fno-exceptions
endif

ifeq ($(DEF_LIB), 0)
COMPILER_FLAGS += -nodefaultlibs
endif

ifeq ($(STACK_ANALYSIS), 1)
COMPILER_FLAGS += -fstack-usage
endif

ifneq ($(STACK_OVERFLOW), 0)
COMPILER_FLAGS += -Wstack-usage=$(STACK_OVERFLOW)
endif


#######################################
# LINKER FLAGS
#######################################

LINKER_FLAGS = $(CAL_FLAGS) -specs=nano.specs -T$(DIR_LINKER)/$(MCU_LINKER) -L$(DIR_LINKER) -lc -lm -lnosys -Wl,-Map=$(DIR_BUILD)/$(BUILD_NAME).map,--cref,--gc-sections,--print-memory-usage,--fuse-ld=gold


#######################################
# BUILD
#######################################

all: $(DIR_BUILD)/$(BUILD_NAME).elf $(DIR_BUILD)/$(BUILD_NAME).hex $(DIR_BUILD)/$(BUILD_NAME).bin

# LIST OF C OBJECTS
OBJECTS = $(addprefix $(DIR_BUILD)/,$(notdir $(C_FILES:.c=.o)))
vpath %.c $(sort $(dir $(C_FILES)))

# LIST OF C++ OBJECTS
OBJECTS += $(addprefix $(DIR_BUILD)/,$(notdir $(CPP_FILES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_FILES)))

# LIST OF ASM OBJECTS
OBJECTS += $(addprefix $(DIR_BUILD)/,$(notdir $(ASM_FILES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_FILES)))

$(DIR_BUILD)/%.o: %.c $(MAKEFILE) | $(DIR_BUILD) 
	$(TC_CC) -c $(COMPILER_FLAGS) -Wa,-a,-ad,-alms=$(DIR_BUILD)/$(notdir $(<:.c=.lst)) $< -o $@

$(DIR_BUILD)/%.o: %.cpp $(MAKEFILE) | $(DIR_BUILD) 
	$(TC_CC) -c $(COMPILER_FLAGS) -Wa,-a,-ad,-alms=$(DIR_BUILD)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(DIR_BUILD)/%.o: %.s $(MAKEFILE) | $(DIR_BUILD)
	$(TC_AS) -c $(COMPILER_FLAGS) $< -o $@

$(DIR_BUILD)/$(BUILD_NAME).elf: $(OBJECTS) $(MAKEFILE)
	$(TC_CC) $(OBJECTS) $(LINKER_FLAGS) -o $@
	copy /-y /y $(DIR_BUILD)\$(BUILD_NAME).map $(DIR_OUTPUT).map

$(DIR_BUILD)/%.hex: $(DIR_BUILD)/%.elf | $(DIR_BUILD)
	$(TC_HEX) $< $@
	copy /-y /y $(DIR_BUILD)\$(BUILD_NAME).hex $(DIR_OUTPUT).hex
	
$(DIR_BUILD)/%.bin: $(DIR_BUILD)/%.elf | $(DIR_BUILD)
	$(TC_BIN) $< $@
	copy /-y /y $(DIR_BUILD)\$(BUILD_NAME).bin $(DIR_OUTPUT).bin

ifeq ($(TC_PRINT_VER), 1)
	$(TC_CC) --version
endif
	
$(DIR_BUILD):
	mkdir $@


#######################################
# FLASH CHIP
#######################################

flash: all
	if not exist $(JLINK_FLASH) ((echo r & echo h & echo loadbin $(DIR_BUILD)/$(BUILD_NAME).bin,$(JLINK_ADDR_START) & echo verifybin $(DIR_BUILD)/$(BUILD_NAME).bin,$(JLINK_ADDR_START) & echo r & echo q) > $(JLINK_FLASH))
	JLink.exe -device $(JLINK_DEVICE) -if SWD -speed $(JLINK_SPEED) -autoconnect 1 -CommandFile $(JLINK_FLASH)
	

#######################################
# ERASE APPLICATION FROM MEMORY
#######################################		

erase:
	if not exist $(JLINK_ERASE) ((echo r & echo h & echo erase $(JLINK_ADDR_START) $(JLINK_ADDR_END) & echo r & echo q) > $(JLINK_ERASE))
	JLink.exe -device $(JLINK_DEVICE) -if SWD -speed $(JLINK_SPEED) -autoconnect 1 -CommandFile $(JLINK_ERASE)


#######################################
# ERASE MCU MEMORY
#######################################		

erase_all:
	if not exist $(JLINK_ERASE_ALL) ((echo r & echo h & echo erase & echo r & echo q) > $(JLINK_ERASE_ALL))
	JLink.exe -device $(JLINK_DEVICE) -if SWD -speed $(JLINK_SPEED) -autoconnect 1 -CommandFile $(JLINK_ERASE_ALL)


#######################################
# RESET MCU
#######################################		

reset:
	if not exist $(JLINK_RESET) ((echo r & echo q) > $(JLINK_RESET))
	JLink.exe -device $(JLINK_DEVICE) -if SWD -speed $(JLINK_SPEED) -autoconnect 1 -CommandFile $(JLINK_RESET)
	

#######################################
# CLEAN STUFF
#######################################

clean: clean_jlink
	if exist $(DIR_BUILD) (echo Deleting build directory & rmdir /s /q $(DIR_BUILD))
	if exist $(DIR_OUTPUT).bin (echo Deleting bin executable & del $(DIR_OUTPUT).bin)
	if exist $(DIR_OUTPUT).hex (echo Deleting hex executable & del $(DIR_OUTPUT).hex)
	if exist $(DIR_OUTPUT).map (echo Deleting build map file & del $(DIR_OUTPUT).map)

clean_jlink:
	if exist $(JLINK_FLASH) (echo Deleting flash script & del $(JLINK_FLASH))
	if exist $(JLINK_ERASE) (echo Deleting erase script & del $(JLINK_ERASE))
	if exist $(JLINK_ERASE_ALL) (echo Deleting erase ALL script & del $(JLINK_ERASE_ALL))
	if exist $(JLINK_RESET) (echo Deleting reset script & del $(JLINK_RESET))	


#######################################
# OPEN RTOS CONFIG FILE IN CMSIS CONFIG WIZARD
#######################################

rtos_cfg:
	java -jar $(CMSIS_WIZARD) $(RTOS_CFG)


#######################################
# CUSTOM OPTIONS
#######################################

