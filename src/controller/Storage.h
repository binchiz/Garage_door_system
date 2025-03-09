/*similar as before.
 * information to be stored:
 * 1. current position
 * 2. calibstate
 * 3. errorstate
 * 4. doorstate
 */

#ifndef STORAGE_H
#define STORAGE_H
#include "hardware/EEPROM.h"
#include "controller/DoorController.h"
#include "types.h"

class Storage_t {
    public:
        explicit Storage_t(const EEPROM_t &eeprom, DoorController_t& controller);
        ~Storage_t() = default;
        void saveCalib() const;
        void savePos() const;
        void saveTotalSteps() const;
        void saveState() const;
        void saveError() const;

        void loadCalib();
        void loadPos();
        void loadTotalSteps();
        void loadState();
        void loadError();

    private:
        const EEPROM_t& eeprom;
        DoorController_t& controller;
        const uint16_t ADDR_CALIB = 0x00;
        const uint16_t ADDR_CALIB_CSUM = 0x01;

        const uint16_t ADDR_POS_L = 0x02;
        const uint16_t ADDR_POS_L_CSUM = 0x03;

        const uint16_t ADDR_POS_H = 0x04;
        const uint16_t ADDR_POS_H_CSUM = 0x05;

        const uint16_t ADDR_TOTAL_STEPS_L = 0x06;
        const uint16_t ADDR_TOTAL_STEPS_L_CSUM = 0x07;

        const uint16_t ADDR_TOTAL_STEPS_H = 0x08;
        const uint16_t ADDR_TOTAL_STEPS_H_CSUM = 0x09;

        const uint16_t ADDR_STATE = 0x0A;
        const uint16_t ADDR_STATE_CSUM = 0x0B;

        const uint16_t ADDR_ERROR = 0x0C;
        const uint16_t ADDR_ERROR_CSUM = 0x0D;
};

#endif //STORAGE_H
