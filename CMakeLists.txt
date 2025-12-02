# CMakeLists.txt
# Embedded Firmware Framework
# Copyright (c) 2016 - MIT License

cmake_minimum_required(VERSION 3.5)

# Project configuration
project(embedded-firmware 
    VERSION 1.0.0
    LANGUAGES C CXX ASM
)

# C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Target MCU configuration
set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F407xx)
set(CPU_CORE cortex-m4)
set(FPU fpv4-sp-d16)
set(FLOAT_ABI hard)

# Compiler flags
set(MCU_FLAGS "-mcpu=${CPU_CORE} -mthumb -mfpu=${FPU} -mfloat-abi=${FLOAT_ABI}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MCU_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MCU_FLAGS} -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${MCU_FLAGS}")

# Optimization flags
set(CMAKE_C_FLAGS_DEBUG "-Og -g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# Common warning flags
set(WARN_FLAGS "-Wall -Wextra -Wpedantic -Wshadow -Wdouble-promotion")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARN_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARN_FLAGS}")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/hal
    ${CMAKE_SOURCE_DIR}/drivers
)

# Source files
set(SOURCES
    src/main.cpp
    src/system.cpp
    src/startup.cpp
)

set(HAL_SOURCES
    # Add HAL source files here when implemented
    # hal/gpio.cpp
    # hal/uart.cpp
    # hal/spi.cpp
    # hal/i2c.cpp
)

set(DRIVER_SOURCES
    # Add driver source files here
    # drivers/led_driver.cpp
    # drivers/sensor_driver.cpp
)

# Linker script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32f407vg.ld)

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${LINKER_SCRIPT}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MCU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_NAME}.map")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nano.specs")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nosys.specs")

# Create executable
add_executable(${PROJECT_NAME}.elf
    ${SOURCES}
    ${HAL_SOURCES}
    ${DRIVER_SOURCES}
)

# Print size after build
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
)

# Generate binary file
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Generating binary file"
)

# Generate hex file
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Generating hex file"
)

# Flash target (OpenOCD)
add_custom_target(flash
    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg 
            -c "program ${PROJECT_NAME}.elf verify reset exit"
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Flashing target device"
)

# Flash target (J-Link)
add_custom_target(flash-jlink
    COMMAND JLinkExe -device ${MCU_MODEL} -if SWD -speed 4000
            -CommanderScript flash.jlink
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Flashing with J-Link"
)

# Debug target
add_custom_target(debug
    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg &
    COMMAND ${CMAKE_GDB} ${PROJECT_NAME}.elf -ex "target remote :3333"
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Starting debug session"
)

# Clean extended
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_NAME}.elf
    COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_NAME}.map
    COMMENT "Cleaning all build artifacts"
)

# Print build info
message(STATUS "")
message(STATUS "=== Embedded Firmware Build Configuration ===")
message(STATUS "Target MCU:     ${MCU_MODEL}")
message(STATUS "CPU Core:       ${CPU_CORE}")
message(STATUS "FPU:            ${FPU}")
message(STATUS "C++ Standard:   ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "")
