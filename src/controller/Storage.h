/*similar as before.
 * information to be stored:
 * 1. current position
 * 2. calibrated
 * 3. status: CLOSING, OPENING, CALIBRATING, ERROR...
 */

#ifndef STORAGE_H
#define STORAGE_H
#include "../hardware/EEPROM.h"

class Storage {
    public:
        explicit Storage(const EEPROM& eeprom);
        ~Storage() = default;
        void init_storage();
        void saveCalib();
        void savePos();
        void saveState();
        void loadCalib();
        void loadPos();
        void loadState();
    private:
        EEPROM& eeprom;
};

#endif //STORAGE_H
