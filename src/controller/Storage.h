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
#include "types.h"

class Storage_t {
    public:
        explicit Storage_t(const EEPROM_t &eeprom, Status_t &status);
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
        Status_t &status;
        const uint16_t ADDR_CALIB = 0x00;
        const uint16_t ADDR_CALIB_CSUM = ADDR_CALIB + 1;

        const uint16_t ADDR_POS_L = 0x02;
        const uint16_t ADDR_POS_L_CSUM = ADDR_POS_L + 1;

        const uint16_t ADDR_POS_H = 0x04;
        const uint16_t ADDR_POS_H_CSUM = ADDR_POS_H + 1;

        const uint16_t ADDR_TOTAL_STEPS_L = 0x06;
        const uint16_t ADDR_TOTAL_STEPS_L_CSUM = ADDR_TOTAL_STEPS_L + 1;

        const uint16_t ADDR_TOTAL_STEPS_H = 0x08;
        const uint16_t ADDR_TOTAL_STEPS_H_CSUM = ADDR_TOTAL_STEPS_H + 1;

        const uint16_t ADDR_STATE = 0x010;
        const uint16_t ADDR_STATE_CSUM = ADDR_STATE + 1;

        const uint16_t ADDR_ERROR = 0x012;
        const uint16_t ADDR_ERROR_CSUM = ADDR_ERROR + 1;
};

#endif //STORAGE_H
