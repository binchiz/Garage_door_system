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
        explicit Storage_t(const EEPROM_t& eeprom);
        ~Storage_t() = default;
        void init_storage();
        void saveCalib();
        void savePos();
        void saveState();
        void saveError();
        void loadCalib();
        void loadPos();
        void loadState();
        void loadError();
    private:
        EEPROM_t& eeprom;
};

#endif //STORAGE_H
