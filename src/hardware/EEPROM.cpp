//
// Created by zbinc on 27/02/2025.
//

#include <cstring>
#include "pico/stdlib.h"
#include <hardware/i2c.h>
#include "EEPROM.h"

EEPROM_t::EEPROM_t(i2c_inst *i2c, uint32_t baudrate, uint gpioSda, uint gpioScl, uint8_t deviceAddr)
        : i2c(i2c), baudrate(baudrate), gpioSda(gpioSda), gpioScl(gpioScl), deviceAddr(deviceAddr) {
    i2c_init(i2c, baudrate);
    gpio_set_function(gpioSda, GPIO_FUNC_I2C);
    gpio_set_function(gpioScl, GPIO_FUNC_I2C);
    gpio_pull_up(gpioSda);
    gpio_pull_up(gpioScl);
}

bool EEPROM_t::isBusy() const {
    if (i2c_write_blocking(i2c, deviceAddr, nullptr, 1, false) == PICO_ERROR_GENERIC) {
        return true; // the device is busy or not responding
    } else {
        return false;
    }
}

int EEPROM_t::write(const uint8_t *data, uint16_t memAddr, uint8_t len, bool blocking) const {
    if (i2c_write_blocking(i2c, deviceAddr, nullptr, 1, false) == PICO_ERROR_GENERIC) {
        return PICO_ERROR_TIMEOUT; // the device is busy or not responding
    }

    uint8_t writeBuffer[PAGE_SIZE + 2]; // 2 bytes for memory address
    // make sure the write operation does not cross a page boundary
    uint16_t bytesLeftInPage = PAGE_SIZE - (memAddr & (PAGE_SIZE - 1));
    if (len > bytesLeftInPage) {
        len = bytesLeftInPage;
        assert(false && "Write operation crosses page boundary");
    }

    // write the memory address at the first two bytes
    writeBuffer[0] = static_cast<uint8_t>(memAddr >> 8);
    writeBuffer[1] = static_cast<uint8_t>(memAddr & 0xFF); // & 0xFF is not necessary (but makes the intention clear?)
    // append data, len must be less than PAGE_SIZE after checking for page boundary
    std::memcpy(writeBuffer + 2, data, len);
    uint32_t writeStartTime = time_us_32();
    int ret = i2c_write_blocking(i2c, deviceAddr, writeBuffer, len + 2, false);
    if (ret == len + 2) {
        ret = len; // return the number of bytes written, actually data bytes
    }
    if (blocking) {
        while (isBusy()) {
            if (time_us_32() - writeStartTime > EEPROM_WRITE_TIMEOUT * 1000) {
                return PICO_ERROR_TIMEOUT;
            }
        }
    }
    return ret;
}

int EEPROM_t::read(uint8_t *buffer, uint16_t memAddr, uint8_t len) const{
    if (len > PAGE_SIZE) {
        len = PAGE_SIZE;
        assert(false && "Read should be no more than 64 bytes");
    }

    uint8_t memAddr8[2] = {static_cast<uint8_t>(memAddr >> 8), static_cast<uint8_t>(memAddr & 0xFF)};
    i2c_write_blocking(i2c, deviceAddr, memAddr8, 2, true);
    int ret = i2c_read_blocking(i2c, deviceAddr, buffer, len, false);
    return ret;
}

bool EEPROM_t::writeByteWithChecksum(uint16_t memAddr, uint16_t checksumAddr, uint8_t data) const {
    auto checksum = static_cast<uint8_t>(~data);
    int retData = write(&data, memAddr, 1);
    int retChecksum = write(&checksum, checksumAddr, 1);
    return retData == 1 && retChecksum == 1;
}

bool EEPROM_t::readByteWithChecksum(uint16_t memAddr, uint16_t checksumAddr, uint8_t *data) const{
    uint8_t checksum;
    int retData = read(data, memAddr, 1);
    int retChecksum = read(&checksum, checksumAddr, 1);
    bool isChecksumCorrect = checksum == static_cast<uint8_t>(~(*data));
    return retData == 1 && retChecksum == 1 && isChecksumCorrect;
}