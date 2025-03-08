//
// Created by Sheng Tai on 08/03/2025.
//
#include <iostream>
#include <cstring>
#include "pico/stdlib.h"
#include "hardware/EEPROM.h"

int main() {
    stdio_init_all();

    std::cout << "EEPROM Test" << std::endl;

    EEPROM_t eeprom;
    std::cout << "Initializing EEPROM" << std::endl;

    // 1. Normal write and read

    // write
    uint8_t data[64];
    for (uint8_t i = 0; i < 64; i++) {
        data[i] = i;
        std::cout << (int)data[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Writing data to EEPROM: " << std::endl;
    int ret;
    ret = eeprom.write(data, 0x00, 64);
    std::cout << ret << " bytes written\n" << std::endl;

    while (eeprom.isBusy()) {
        std::cout << "EEPROM is busy" << std::endl;
    }

    // read
    std::cout << "Reading data from EEPROM: " << std::endl;
    uint8_t readBuffer[64];
    std::memset(readBuffer, 0, 64);
    ret = eeprom.read(readBuffer, 0x00, 64);
    std::cout << ret << " bytes read\n" << std::endl;

    // print
    std::cout << "Data read from EEPROM: " << std::endl;
    for (uint8_t i: readBuffer) {
        std::cout << (int)i << " ";
    }
    std::cout << std::endl;


    // 2. Checksum write and read
    bool success;
    // write
    std::cout << "Writing data to EEPROM with checksum: " << std::endl;
    success = eeprom.writeByteWithChecksum(0x00, 0x01, 0x0A);
    std::cout << "Write success: " << success << std::endl;


    // read
    std::cout << "Reading data from EEPROM with checksum: " << std::endl;
    uint8_t readData;
    success = eeprom.readByteWithChecksum(0x00, 0x01, &readData);
    std::cout << "Read success: " << success << std::endl;


    // 3. Checksum write and read with incorrect checksum
    // write
    std::cout << "Writing data to EEPROM with incorrect checksum: " << std::endl;
    success = eeprom.writeByteWithChecksum(0x00, 0x01, 0x0B);
    std::cout << "Write success: " << success << std::endl;
    uint8_t wrongChecksum = 0x0F;
    eeprom.write(&wrongChecksum, 0x01, 1);

    // read
    std::cout << "Reading data from EEPROM with incorrect checksum: " << std::endl;
    success = eeprom.readByteWithChecksum(0x00, 0x01, &readData);
    std::cout << "Read success: " << success << std::endl;


    return 0;
}