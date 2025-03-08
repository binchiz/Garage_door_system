
#ifndef EEPROM_H
#define EEPROM_H

#include <hardware/i2c.h>

// AT24C256, 32KB EEPROM, 64-byte page size
class EEPROM_t {
public:
    explicit EEPROM_t(
            i2c_inst_t *i2c = i2c1,
            uint32_t baudrate = (400 * 1000),
            uint gpioSda = 14,
            uint gpioScl = 15,
            uint8_t deviceAddr = 0b1010000
    );
    // when blocking is true, the function will wait until the EEPROM finishes writing, unless a timeout occurs
    int write(const uint8_t *data, uint16_t memAddr, uint8_t len, bool blocking = true) const;
    int read(uint8_t *buffer, uint16_t memAddr, uint8_t len) const;
    [[nodiscard]] bool isBusy() const; // check if the EEPROM is busy writing
    [[nodiscard]] bool writeByteWithChecksum(uint16_t memAddr, uint16_t checksumAddr, uint8_t data) const;
    [[nodiscard]] bool readByteWithChecksum(uint16_t memAddr, uint16_t checksumAddr, uint8_t *data) const;

private:
    i2c_inst_t *i2c;
    const uint32_t baudrate;
    const uint gpioSda;
    const uint gpioScl;
    const uint8_t deviceAddr;
    static constexpr uint8_t PAGE_SIZE = 64;
    static constexpr uint32_t EEPROM_WRITE_TIMEOUT = 1000; // ms
};

#endif //EEPROM_H
