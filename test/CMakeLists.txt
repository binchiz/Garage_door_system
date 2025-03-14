include_directories(
        ${PROJECT_SOURCE_DIR}/src
)

set(TEST_SOURCES
        motor_test.cpp
        mqtt_test.cpp
        button_test.cpp
        rotary_encoder_test.cpp
        controller_test.cpp
        eeprom_test.cpp
        storage_test.cpp
        led_test.cpp
)

foreach (TEST_SOURCE ${TEST_SOURCES})
    # get filename without extension
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # add executable
    add_executable(${TEST_NAME}
            ${TEST_SOURCE}
    )

    # link to the library
    target_link_libraries(${TEST_NAME}
            ${PROJECT_NAME}_lib
    )

    # output to UART
    pico_enable_stdio_usb(${TEST_NAME} 0)
    pico_enable_stdio_uart(${TEST_NAME} 1)

    # create map/bin/hex/uf2 files
    pico_add_extra_outputs(${TEST_NAME})
endforeach ()