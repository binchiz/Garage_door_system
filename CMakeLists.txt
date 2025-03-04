# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Set board type because we are building for PicoW
set(PICO_BOARD pico_w)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(Garage_Door_System C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
)


# Add the src and test directory
add_subdirectory(test)

file(GLOB MQTT_SOURCES "src/handler/lib/paho.mqtt.embedded-c/MQTTPacket/src/*.c")

# Create a library from your source files (excluding main.c)
add_library(${PROJECT_NAME}_lib STATIC
        src/controller/DoorController.cpp
        src/controller/DoorController.h
        src/types.h
        src/hardware/RotaryEncoder.cpp
        src/hardware/RotaryEncoder.h
        src/hardware/StepperMotor.cpp
        src/hardware/StepperMotor.h
        src/handler/ButtonHandler.cpp
        src/handler/ButtonHandler.h
        src/handler/MQTTHandler.cpp
        src/handler/MQTTHandler.h
        src/GarageDoorSytem.cpp
        src/GarageDoorSytem.h
        src/controller/Storage.cpp
        src/controller/Storage.h
        src/hardware/Button.cpp
        src/hardware/Button.h
        src/hardware/EEPROM.cpp
        src/hardware/EEPROM.h

#        ${MQTT_SOURCES}
#        src/handler/lib/mqtt/Countdown.h
#        src/handler/lib/mqtt/Countdown.cpp
#        src/handler/lib/mqtt/IPStack.h
#        src/handler/lib/mqtt/IPStack.cpp
#        src/handler/lib/mqtt/lwipopts.h
#        src/handler/lib/mqtt_tool.h
#        src/handler/lib/mqtt_tool.cpp

)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
        src/handler/lib/mqtt
        src/handler/lib/paho.mqtt.embedded-c/MQTTPacket/src
        src/handler/lib/paho.mqtt.embedded-c/MQTTClient/src
)


# Add the main executable
add_executable(${PROJECT_NAME}
        src/main.cpp
        ${MQTT_SOURCES}
        src/handler/lib/mqtt/Countdown.h
        src/handler/lib/mqtt/Countdown.cpp
        src/handler/lib/mqtt/IPStack.h
        src/handler/lib/mqtt/IPStack.cpp
        src/handler/lib/mqtt/lwipopts.h
        src/handler/lib/mqtt_tool.h
        src/handler/lib/mqtt_tool.cpp

)

# Link the necessary Pico SDK libraries to the static library
target_link_libraries(${PROJECT_NAME}_lib
        pico_stdlib
        hardware_pwm
        hardware_gpio
        hardware_i2c
        hardware_uart
        pico_cyw43_arch_lwip_poll
        hardware_spi
)

# Link the static library to the main executable
target_link_libraries(${PROJECT_NAME}
        ${PROJECT_NAME}_lib
)


# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Disable USB output, enable UART output
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
