# Makefile
# Embedded Firmware Framework
# Copyright (c) 2016 - MIT License

#------------------------------------------------------------------------------
# Target Configuration
#------------------------------------------------------------------------------
TARGET = embedded-firmware
MCU = STM32F407VG
CPU = cortex-m4
FPU = fpv4-sp-d16
FLOAT-ABI = hard

#------------------------------------------------------------------------------
# Toolchain
#------------------------------------------------------------------------------
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
GDB = $(PREFIX)gdb

#------------------------------------------------------------------------------
# Paths
#------------------------------------------------------------------------------
BUILD_DIR = build
SRC_DIR = src
INC_DIR = include
HAL_DIR = hal
DRV_DIR = drivers

#------------------------------------------------------------------------------
# Source Files
#------------------------------------------------------------------------------
C_SOURCES =

CXX_SOURCES = \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/system.cpp \
	$(SRC_DIR)/startup.cpp

ASM_SOURCES =

#------------------------------------------------------------------------------
# Include Paths
#------------------------------------------------------------------------------
C_INCLUDES = \
	-I$(INC_DIR) \
	-I$(HAL_DIR) \
	-I$(DRV_DIR)

#------------------------------------------------------------------------------
# Compiler Flags
#------------------------------------------------------------------------------
MCU_FLAGS = -mcpu=$(CPU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT-ABI)

# C flags
CFLAGS = $(MCU_FLAGS) $(C_INCLUDES)
CFLAGS += -Wall -Wextra -Wpedantic
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -std=c11

# C++ flags
CXXFLAGS = $(MCU_FLAGS) $(C_INCLUDES)
CXXFLAGS += -Wall -Wextra -Wpedantic
CXXFLAGS += -fdata-sections -ffunction-sections
CXXFLAGS += -fno-exceptions -fno-rtti
CXXFLAGS += -std=c++14

# Assembly flags
ASFLAGS = $(MCU_FLAGS)

# Debug/Release configuration
ifeq ($(DEBUG), 1)
CFLAGS += -g3 -Og -DDEBUG
CXXFLAGS += -g3 -Og -DDEBUG
else
CFLAGS += -O2 -DNDEBUG
CXXFLAGS += -O2 -DNDEBUG
endif

#------------------------------------------------------------------------------
# Linker Flags
#------------------------------------------------------------------------------
LDSCRIPT = stm32f407vg.ld

LDFLAGS = $(MCU_FLAGS)
LDFLAGS += -T$(LDSCRIPT)
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref
LDFLAGS += --specs=nano.specs
LDFLAGS += --specs=nosys.specs

# Libraries
LIBS = -lc -lm -lnosys
LIBDIR =

#------------------------------------------------------------------------------
# Build Rules
#------------------------------------------------------------------------------
# List of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CXX_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CXX_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# Default target
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex size

# Create build directory
$(BUILD_DIR):
	mkdir -p $@

# Compile C sources
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo "[CC] $<"
	@$(CC) -c $(CFLAGS) $< -o $@

# Compile C++ sources
$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	@echo "[CXX] $<"
	@$(CXX) -c $(CXXFLAGS) $< -o $@

# Assemble sources
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo "[AS] $<"
	@$(AS) -c $(ASFLAGS) $< -o $@

# Link
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo "[LD] $@"
	@$(CXX) $(OBJECTS) $(LDFLAGS) $(LIBDIR) $(LIBS) -o $@

# Create binary
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	@echo "[BIN] $@"
	@$(CP) -O binary $< $@

# Create hex
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	@echo "[HEX] $@"
	@$(CP) -O ihex $< $@

# Print size
size: $(BUILD_DIR)/$(TARGET).elf
	@echo ""
	@echo "=== Build Size ==="
	@$(SZ) $<

#------------------------------------------------------------------------------
# Flash Targets
#------------------------------------------------------------------------------
flash: $(BUILD_DIR)/$(TARGET).elf
	@echo "Flashing with OpenOCD..."
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
		-c "program $< verify reset exit"

flash-jlink: $(BUILD_DIR)/$(TARGET).bin
	@echo "Flashing with J-Link..."
	JLinkExe -device $(MCU) -if SWD -speed 4000 \
		-autoconnect 1 \
		-CommanderScript flash.jlink

#------------------------------------------------------------------------------
# Debug Target
#------------------------------------------------------------------------------
debug: $(BUILD_DIR)/$(TARGET).elf
	@echo "Starting debug session..."
	$(GDB) $< -ex "target remote :3333"

gdb-server:
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg

#------------------------------------------------------------------------------
# Clean
#------------------------------------------------------------------------------
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

#------------------------------------------------------------------------------
# Help
#------------------------------------------------------------------------------
help:
	@echo ""
	@echo "Embedded Firmware Framework - Build System"
	@echo "==========================================="
	@echo ""
	@echo "Targets:"
	@echo "  all         - Build firmware (default)"
	@echo "  clean       - Remove build artifacts"
	@echo "  flash       - Flash using OpenOCD"
	@echo "  flash-jlink - Flash using J-Link"
	@echo "  debug       - Start GDB debug session"
	@echo "  gdb-server  - Start OpenOCD GDB server"
	@echo "  size        - Print binary size"
	@echo "  help        - Show this help"
	@echo ""
	@echo "Options:"
	@echo "  DEBUG=1     - Build with debug symbols"
	@echo ""

.PHONY: all clean flash flash-jlink debug gdb-server size help
