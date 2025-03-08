//
// Created by Sheng Tai on 08/03/2025.
//

#include <pico/stdlib.h>
#include <iostream>
#include "controller/Storage.h"
#include "types.h"

int main() {
    stdio_init_all();

    std::cout << "Storage Test" << std::endl;

    Status_t systemStatus = {
            .doorState = GarageDoor::CLOSING,
            .errorState = STUCK,
            .calibState = CALIBRATED,
            .moving = false,
            .totalSteps = 1000,
            .currentPosition = 100
    };

    EEPROM_t eeprom;
    Storage_t storage(eeprom, systemStatus);

    storage.saveCalib();
    storage.savePos();
    storage.saveTotalSteps();
    storage.saveState();
    storage.saveError();

    // 1. old
    std::cout << "Old Status" << std::endl << std::endl;
    std::cout << "Door State: " << systemStatus.doorState << std::endl;
    std::cout << "Error State: " << systemStatus.errorState << std::endl;
    std::cout << "Calib State: " << systemStatus.calibState << std::endl;
    std::cout << "Total Steps: " << systemStatus.totalSteps << std::endl;
    std::cout << "Current Position: " << systemStatus.currentPosition << std::endl;

    // change the status
    systemStatus.doorState = GarageDoor::OPENED;
    systemStatus.errorState = NORMAL;
    systemStatus.calibState = UNCALIBRATED;
    systemStatus.totalSteps = 2000;
    systemStatus.currentPosition = 200;

    // 2. changed
    std::cout << "Changed Status" << std::endl << std::endl;
    std::cout << "Door State: " << systemStatus.doorState << std::endl;
    std::cout << "Error State: " << systemStatus.errorState << std::endl;
    std::cout << "Calib State: " << systemStatus.calibState << std::endl;
    std::cout << "Total Steps: " << systemStatus.totalSteps << std::endl;
    std::cout << "Current Position: " << systemStatus.currentPosition << std::endl;

    // load the status
    storage.loadCalib();
    storage.loadPos();
    storage.loadTotalSteps();
    storage.loadState();
    storage.loadError();

    // 3. restore
    std::cout << "Restored Status" << std::endl << std::endl;
    std::cout << "Door State: " << systemStatus.doorState << std::endl;
    std::cout << "Error State: " << systemStatus.errorState << std::endl;
    std::cout << "Calib State: " << systemStatus.calibState << std::endl;
    std::cout << "Total Steps: " << systemStatus.totalSteps << std::endl;
    std::cout << "Current Position: " << systemStatus.currentPosition << std::endl;

    return 0;
}