#include "Storage.h"

Storage_t::Storage_t(const EEPROM_t &eeprom) : eeprom(eeprom) {
}

void Storage_t::saveCalib() const {
    // need to pass data here
    if (!eeprom.writeByteWithChecksum(calibAddr, calibChecksumAddr, 0)) {
        // failed save data
    }
}