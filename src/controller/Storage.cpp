#include "Storage.h"


Storage_t::Storage_t(const EEPROM_t &eeprom, Status_t &status)
        : eeprom(eeprom), status(status) {

}


// saves
void Storage_t::saveCalib() const {
    auto data = static_cast<uint8_t>(status.calibState);
    if (!eeprom.writeByteWithChecksum(ADDR_CALIB, ADDR_CALIB_CSUM, data)) {
        // failed save data, set error and blink led as well?
    }
}

void Storage_t::savePos() const {
    auto dataL = static_cast<uint8_t>(status.currentPosition & 0xFF);
    auto dataH = static_cast<uint8_t>(status.currentPosition >> 8);
    if (!eeprom.writeByteWithChecksum(ADDR_POS_L, ADDR_POS_L_CSUM, dataL)) {
        // failed save data, set error and blink led as well?
    }
    if (!eeprom.writeByteWithChecksum(ADDR_POS_H, ADDR_POS_H_CSUM, dataH)) {
        // failed save data, set error and blink led as well?
    }
}

void Storage_t::saveTotalSteps() const {
    auto dataL = static_cast<uint8_t>(status.totalSteps & 0xFF);
    auto dataH = static_cast<uint8_t>(status.totalSteps >> 8);
    if (!eeprom.writeByteWithChecksum(ADDR_TOTAL_STEPS_L, ADDR_TOTAL_STEPS_L_CSUM, dataL)) {
        // failed save data, set error and blink led as well?
    }
    if (!eeprom.writeByteWithChecksum(ADDR_TOTAL_STEPS_H, ADDR_TOTAL_STEPS_H_CSUM, dataH)) {
        // failed save data, set error and blink led as well?
    }
}

void Storage_t::saveState() const {
    auto data = static_cast<uint8_t>(status.doorState);
    if (!eeprom.writeByteWithChecksum(ADDR_STATE, ADDR_STATE_CSUM, data)) {
        // failed save data, set error and blink led as well?
    }
}

void Storage_t::saveError() const {
    auto data = static_cast<uint8_t>(status.errorState);
    if (!eeprom.writeByteWithChecksum(ADDR_ERROR, ADDR_ERROR_CSUM, data)) {
        // failed save data, set error and blink led as well?
    }
}


// loads
void Storage_t::loadCalib() {
    uint8_t data;
    if (!eeprom.readByteWithChecksum(ADDR_CALIB, ADDR_CALIB_CSUM, &data)) {
        // failed to read data, set error and blink led as well?
    }
    status.calibState = static_cast<calibState_t>(data);
}

void Storage_t::loadPos() {
    uint8_t dataL, dataH;
    if (!eeprom.readByteWithChecksum(ADDR_POS_L, ADDR_POS_L_CSUM, &dataL)) {
        // failed to read data, set error and blink led as well?
    }
    if (!eeprom.readByteWithChecksum(ADDR_POS_H, ADDR_POS_H_CSUM, &dataH)) {
        // failed to read data, set error and blink led as well?
    }
    status.currentPosition = static_cast<int>(dataL) | (static_cast<int>(dataH) << 8);
}

void Storage_t::loadTotalSteps() {
    uint8_t dataL, dataH;
    if (!eeprom.readByteWithChecksum(ADDR_TOTAL_STEPS_L, ADDR_TOTAL_STEPS_L_CSUM, &dataL)) {
        // failed to read data, set error and blink led as well?
    }
    if (!eeprom.readByteWithChecksum(ADDR_TOTAL_STEPS_H, ADDR_TOTAL_STEPS_H_CSUM, &dataH)) {
        // failed to read data, set error and blink led as well?
    }
    status.totalSteps = static_cast<int>(dataL) | (static_cast<int>(dataH) << 8);
}

void Storage_t::loadState() {
    uint8_t data;
    if (!eeprom.readByteWithChecksum(ADDR_STATE, ADDR_STATE_CSUM, &data)) {
        // failed to read data, set error and blink led as well?
    }
    status.doorState = static_cast<GarageDoor::doorState_t>(data);
}

void Storage_t::loadError() {
    uint8_t data;
    if (!eeprom.readByteWithChecksum(ADDR_ERROR, ADDR_ERROR_CSUM, &data)) {
        // failed to read data, set error and blink led as well?
    }
    status.errorState = static_cast<errorState_t>(data);
}