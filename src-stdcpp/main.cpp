/*
 * Copyright (C) 2023 onwards LabsLand, Inc.
 * All rights reserved.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 */
#include <iostream>
#include "labsland/simulations/watertanksimulation.h"

int main() {

    WatertankSimulation simulation;
    clock_t currentClock = clock();

    int i = 0;
    simulation._initialize();

    while(i < 100) {

        currentClock += 0.1 * CLOCKS_PER_SEC; // Make the simulation advance 100 ms.
        simulation._update(currentClock);
        i++;

        std::cout << "Current water level: " << simulation.mState.level << std::endl;
        std::cout << "Current volume out of total: " << simulation.mState.volume << " l out of " << simulation.mState.totalVolume << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
