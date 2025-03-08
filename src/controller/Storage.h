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

class Storage_t {
    public:
        explicit Storage_t(const EEPROM_t &eeprom);
        ~Storage_t() = default;
        void saveCalib() const;
        void savePos();
        void saveState();
        void saveError();
        void saveTotalSteps();

        void loadCalib();
        void loadPos();
        void loadState();
        void loadError();
        void loadTotalSteps();

    private:
        const EEPROM_t& eeprom;
        const uint16_t calibAddr = 0x00;
        const uint16_t calibChecksumAddr = calibAddr + 1;
        const uint16_t posAddr = 0x02;
        const uint16_t posChecksumAddr = posAddr + 1;
        const uint16_t stateAddr = 0x04;
        const uint16_t stateChecksumAddr = stateAddr + 1;
        const uint16_t errorAddr = 0x06;
        const uint16_t errorChecksumAddr = errorAddr + 1;
};

#endif //STORAGE_H
