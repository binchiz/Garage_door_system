
#ifndef EEPROM_H
#define EEPROM_H
#include <hardware/i2c.h>

class EEPROM {
    public:
        explicit EEPROM(
            i2c_inst_t *i2c = i2c0, uint32_t baudrate = (400 * 1000),
            uint gpio_sda = 16, uint gpio_scl = 17,
            uint8_t device_addr = 0b1010000, uint8_t page_size = 64
            );
        ~EEPROM() = default;
        void init();
        int write(const uint8_t *data, uint16_t mem_addr, uint8_t len);
        int read(uint8_t *buffer, uint16_t mem_addr, uint8_t len);
        bool write_byte_with_checksum(uint16_t mem_addr, uint16_t checksum_addr, uint8_t data);
        bool read_byte_with_checksum(uint16_t mem_addr, uint16_t checksum_addr, uint8_t *data);
    private:
        i2c_inst_t *i2c;
        uint32_t baudrate;
        uint gpio_sda;
        uint gpio_scl;
        uint8_t device_addr;
        uint8_t page_size;
};

#endif //EEPROM_H
